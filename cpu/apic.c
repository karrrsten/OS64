#include "apic.h"

#include "idt.h"
#include "ioapic.h"
#include "page.h"
#include "x86.h"

#include "kernel/acpi.h"
#include "util/print.h"

#include <stdint.h>

#define IA32_APIC_BASE_MSR (0x1B)
#define APIC_MSR_ENABLE    (1 << 11)

#define APIC_SOFTWARE_ENABLE (1 << 8)

uint8_t lapic_id;
static volatile uint32_t *lapic;

/**
 * @brief Write to a register of the local APIC.
 * @param reg The register to write to.
 * @param val The value to write.
 */
void lapic_write(enum apic_reg reg, uint32_t val) {
	/* reg is an absolut offset, lapic is a pointer to 4-byte values */
	*(lapic + reg / 4) = val;
}

/**
 * @brief Read a register of the local APIC.
 * @param reg The register to read.
 * @return The value read from the register.
 */
uint32_t lapic_read(enum apic_reg reg) {
	/* reg is an absolut offset, lapic is a pointer to 4-byte values */
	return *(lapic + reg / 4);
}

/**
 * @brief Send an EOI to the Local APIC.
 */
void apic_eoi(void) {
	lapic_write(APIC_EOI, 0);
}

static void apic_spurious_handler(struct interrupt_frame *) {
	return;
}

/**
 * @brief Initialize the Processor Local APIC and IOAPIC(s).
 */

void apic_init(void) {
	kprintf("Initializing APIC...");

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
		PAGE_PRESENT | PAGE_PCD | PAGE_WRITE | PAGE_GLOBAL);

	lapic_id = lapic_read(APIC_ID) << 24;
	kprintf("Processor Local APIC ID: %w8X", lapic_id);

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
		if (madt_entry[0] != 4 /* Local APIC NMI */) {
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
	kprintf("Initializing APIC: Success");
}
