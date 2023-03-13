#pragma once

#include "idt.h"

#include <stdint.h>

enum mode {
	APIC_TIMER_ONE_SHOT = 0b00,
	APIC_TIMER_PERIODIC = 0b01
};

void apic_set_timer(uint32_t time, interrupt_handler handler, enum mode mode);
void apic_mask_timer(void);
void apic_unmask_timer(void);
