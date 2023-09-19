#pragma once

#include <stdint.h>

extern uint8_t lapic_id;

void apic_init(void);

void apic_eoi(void);

/**
 * @enum apic_reg
 * @brief Local APIC registers. The In-Service, Trigger Mode and Interrupt
 * Request Registers are omitted.
 */
enum apic_reg {
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

#define APIC_LVT_MASK (1 << 16)

/**
 * @brief Write a value to one of the Local APIC's registers.
 * @param reg The register to write to.
 * @param val The value to write.
 */
void lapic_write(enum apic_reg reg, uint32_t val);

/**
 * @brief Read a value from one of the Local APIC's registers.
 * @param reg The regiser to read from.
 * @return The read value.
 */
uint32_t lapic_read(enum apic_reg reg);
