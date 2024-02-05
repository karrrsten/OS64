#pragma once

#include "idt.h"

#include <stdint.h>

/**
 * @enum apic_timer_mode
 * @brief Modes for the APIC timer.
 */
enum apic_timer_mode {
	APIC_TIMER_ONE_SHOT = 0b00,
	APIC_TIMER_PERIODIC = 0b01
};

void apic_set_timer(uint32_t time, interrupt_handler handler,
	enum apic_timer_mode mode);
void apic_mask_timer(void);
void apic_unmask_timer(void);
