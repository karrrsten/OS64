#include "ioapic.h"

#include "cpu/apic.h"
#include "cpu/page.h"
#include "kernel/acpi.h"
#include "kernel/malloc.h"

#include <stdint.h>

static uint64_t ioapic_phys = 0;
static volatile uint8_t *ioregsel;
static volatile uint32_t *iowin;

static uint32_t *isa_to_gsi;
static uint32_t gsi_max;

#define IOAPICID    (0x00)
#define IOAPICVER   (0x01)
#define IOAPICARB   (0x02)
#define IOREDTBL(n) (2 * n + 0x10)

static void ioapic_write(uint8_t reg, uint32_t val) {
	*ioregsel = reg;
	*iowin = val;
}

static uint32_t ioapic_read(uint8_t reg) {
	*ioregsel = reg;
	return *iowin;
}

/**
 * @brief Initialize the IOAPIC. Only the IOAPIC covering the lowest Global
 * System Interrupts will be used.
 */
void ioapic_init(void) {
	struct MADT *madt = acpi_get_table(ACPI_MADT);

	char *madt_entry = madt->Interrupt_Controller_Structure;

	for (; (void *)madt_entry < (void *)madt + madt->Length;
		 madt_entry += *(madt_entry + 1)) {
		if (madt_entry[0] == 1 /* I/O APIC */
			&& *(uint32_t *)(madt_entry + 8) == 0 /* GSI Base */) {
			ioapic_phys = *(uint32_t *)(madt_entry + 4);

		} else if (madt_entry[0] == 6 /* I/O SAPIC */
				   && *(uint32_t *)(madt_entry + 4) == 0 /* GSI Basr */) {
			ioapic_phys = *(uint64_t *)(madt_entry + 8);
		}
	}

	uint64_t page_offset = ioapic_phys & 0x1FFF;
	ioregsel
		= kmap((void *)(ioapic_phys - page_offset), nullptr, 4096,
			  PAGE_PRESENT | PAGE_WRITE | PAGE_PCD | PAGE_GLOBAL)
	    + page_offset;
	iowin = (void *)ioregsel + 0x10;


	/* Handle Interrupt Source Overrides */
	gsi_max = ioapic_read(IOAPICVER) >> 16;
	isa_to_gsi = malloc(gsi_max * sizeof(*isa_to_gsi));

	for (uint32_t i = 0; i < gsi_max; ++i) {
		isa_to_gsi[i] = i;
	}

	madt_entry = madt->Interrupt_Controller_Structure;
	for (; (void *)madt_entry < (void *)madt + madt->Length;
		 madt_entry += *(madt_entry + 1)) {
		if (madt_entry[0] == 2 /* Interrupt Source Overrride */) {
			uint8_t n = madt_entry[3];
			isa_to_gsi[n] = *(uint32_t *)(madt_entry + 4);
		}
	}

	/* Handle NMI sources */
	madt_entry = madt->Interrupt_Controller_Structure;
	for (; (void *)madt_entry < (void *)madt + madt->Length;
		 madt_entry += *(madt_entry + 1)) {
		if (madt_entry[0] == 3 /* NMI Source */) {
			ioapic_register_interrupt(2, *(uint32_t *)(madt_entry + 4), 0,
				DELMOD_NMI, *(uint16_t *)(madt_entry + 2) >> 1,
				*(uint16_t *)(madt_entry + 2) >> 3);
		}
	}
}

/**
 * @brief Connect an IOAPIC interrupt pin to a vector on the CPU.
 * @param vector The vector on the CPU to connect to.
 * @param gsi The Global System Interrupt. If negative, the GSI is determined
from the isa.
 * @param isa The ISA interrupt if the GSI is negative/not known.
 * @param delmod The delivery mode of the interrupt. Can be DELMOD_FIX,
DELMOD_SMI, DELMOD_NMI, DELMOD_INIT or DELMOD_ExtINT.
 * @param polarty The polarity of the interrupt pin. Can be POLARITY_HIGH or
POLARITY_LOW.
 * @param trigger_mode The trigger mode of the interrupt pin. Can be
TRIGGER_EDGE or TRIGGER_LEVEL.
 */
void ioapic_register_interrupt(uint8_t vector, int64_t gsi, uint8_t isa,
	uint8_t delmod, int polarity, int trigger_mode) {
	if (gsi < 0) {
		gsi = isa_to_gsi[isa];
	}

	if (delmod != DELMOD_FIX) {
		trigger_mode = TRIGGER_EDGE;
	}

	uint64_t value
		= (uint64_t)lapic_id << 56 | trigger_mode << 15 | polarity << 13
	    | delmod << 8 | vector;
	ioapic_write(IOREDTBL(gsi), value & 0xFFFF'FFFF);
	ioapic_write(IOREDTBL(gsi) + 1, (value >> 32) & 0xFFFF'FFFF);
}

/**
 * @brief Mask an IOAPIC interrupt pin.
 * @param gsi The Global System Interrupt. If negative, the GSI is determined
from the isa.
 * @param isa The ISA interrupt if the GSI is negative/not known..
 */
void ioapic_mask(int64_t gsi, uint8_t isa) {
	if (gsi < 0) {
		gsi = isa_to_gsi[isa];
	}
	ioapic_write(IOREDTBL(gsi), 1 << 16);
}

/**
 * @brief Unmask an IOAPIC interrupt pin.
 * @param gsi The Global System Interrupt. If negative, the GSI is determined
from the isa.
 * @param isa The ISA interrupt if the GSI is negative/not known.
 */
void ioapic_unmask(int64_t gsi, uint8_t isa) {
	if (gsi < 0) {
		gsi = isa_to_gsi[isa];
	}
	ioapic_write(IOREDTBL(gsi), ioapic_read(IOREDTBL(gsi)) & ~(1 << 16));
}
