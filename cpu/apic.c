#include "apic.h"

#include "idt.h"
#include "mem.h"
#include "x86.h"

#include "cpu/page.h"
#include "kernel/acpi.h"
#include "kernel/acpi_tables.h"
#include "util/log.h"

#include <stdint.h>

#define IA32_APIC_BASE_MSR (0x1B)
#define APIC_MSR_ENABLE    (1 << 11)

#define APIC_SOFTWARE_ENABLE (1 << 8)

#define APIC_LVT_MASK (1 << 16)

enum apic_reg : uint32_t {
	APIC_ID = 0x20,
	APIC_VERSION = 0x30,
	APIC_TPR = 0x80,
	APIC_APR = 0x90,
	APIC_PPR = 0xA0,
	APIC_EOI = 0xB0,
	APIC_RRD = 0xC0,
	APIC_LOGICAL_DESTINATION = 0xD0,
	APIC_DESTINATION_FORMAT = 0xE0,
	APIC_SPURIOUS_INT = 0xF0,
	/* skipping the In-Service, Trigger Mode and Interrupt Request Registers */
	APIC_ERROR_STATUS = 0x280,
	APIC_LVT_CMCI = 0x2F0,
	APIC_ICR_LOW = 0x300,
	APIC_ICR_HIGH = 0x310,
	APIC_LVT_TIMER = 0x320,
	APIC_LVT_THERMAL = 0x330,
	APIC_LVT_PERFORMANCE = 0x340,
	APIC_LVT_LINT0 = 0x350,
	APIC_LVT_LINT1 = 0x360,
	APIC_LVT_ERROR = 0x370,
	APIC_TIMER_INIT = 0x380,
	APIC_TIMER_CURRENT = 0x390,
	APIC_TIMER_DIVIDE = 0x3E0
};

static volatile uint32_t *lapic;
static uint8_t lapic_id;

static void lapic_write(enum apic_reg reg, uint32_t val) {
	/* reg is an absolut offset, lapic is a pointer to 4-byte values */
	*(lapic + reg / 4) = val;
}

static uint32_t lapic_read(enum apic_reg reg) {
	/* reg is an absolut offset, lapic is a pointer to 4-byte values */
	return *(lapic + reg / 4);
}

void apic_eoi(void) {
	lapic_write(APIC_EOI, 0);
}

[[gnu::interrupt]] void apic_spurious_handler(struct interrupt_frame *) {
	return;
}

// TODO
void ioapic_init(void) {
	return;
}

void apic_init(void) {
	log("Initializing APIC...");

	struct MADT *madt = acpi_get_table(ACPI_MADT);

	if (madt->PCAT_COMPAT) {
		/* Disable the 8259 compatible PIC */
		outb(0xA1, 0xFF);
		outb(0x21, 0xFF);
	}

	/* Get the phyiscal base of the APIC from the MADT */
	uint64_t lapic_phys_base
		= (uint64_t)madt->Local_Interrupt_Controller_Address;

	char *madt_entry = madt->Interrupt_Controller_Structure;
	for (; (void *)madt_entry < (void *)madt + madt->Length;
		 madt_entry += *(madt_entry + 1)) {
		/* Local APIC Address Override */
		if (madt_entry[0] != 5) {
			continue;
		}

		lapic_phys_base = *(uint64_t *)(madt_entry + 4);
	}

	/* Map the APIC into virtual memory */
	lapic = kmap((void *)lapic_phys_base, nullptr, 4096,
		PAGE_PRESENT | PAGE_PCD | PAGE_WRITE);

	lapic_id = lapic_read(APIC_ID) << 24;
	log("Processor Local APIC ID: %hX", lapic_id);

	/* Disable all LVT entries */
	lapic_write(APIC_LVT_CMCI, APIC_LVT_MASK);
	lapic_write(APIC_LVT_TIMER, APIC_LVT_MASK);
	lapic_write(APIC_LVT_THERMAL, APIC_LVT_MASK);
	lapic_write(APIC_LVT_PERFORMANCE, APIC_LVT_MASK);
	lapic_write(APIC_LVT_LINT0, APIC_LVT_MASK);
	lapic_write(APIC_LVT_LINT1, APIC_LVT_MASK);
	lapic_write(APIC_LVT_ERROR, APIC_LVT_MASK);

	/* Configure LINT0/LINT1 according to the MADT */
	madt_entry = madt->Interrupt_Controller_Structure;
	for (; (void *)madt_entry < (void *)madt + madt->Length;
		 madt_entry += *(madt_entry + 1)) {
		/* Local APIC NMI */
		if (madt_entry[0] != 4) {
			continue;
		}

		/* Note: technically one would need to consider the ACPI Processor UID,
		 * this Local APIC NMI Structure is associated with, but since this
		 * kernel is designed for a uniprocessor system and will most likely
		 * only ever run on Qemu, this Problem will be ignored for now. */

		enum apic_reg reg;
		if (madt_entry[5] == 0) {
			reg = APIC_LVT_LINT0;
		} else if (madt_entry[5] == 1) {
			reg = APIC_LVT_LINT1;
		} else {
			continue;
		}

		/* See the ACPI spec for how these flags work */
		uint8_t polarity = (madt_entry[3] & 0b11) == 0b01 ? 0 : 1;
		uint8_t trigger_mode = ((madt_entry[3] >> 2) & 0b11) == 0b01 ? 0 : 1;

		/* Register the LINT to the NMI vector (2) */
		lapic_write(reg, polarity | trigger_mode | (0b100 << 8) | 2);
	}
	ioapic_init();

	idt_register(0xFF, apic_spurious_handler);
	lapic_write(APIC_SPURIOUS_INT, APIC_SOFTWARE_ENABLE | 0xFF);
	wrmsr(IA32_APIC_BASE_MSR, rdmsr(IA32_APIC_BASE_MSR) | APIC_MSR_ENABLE);
	log("Initializing APIC: Success");
}
