#pragma once

#include <stdint.h>

void ioapic_init(void);

#define POLARITY_HIGH (0b0)
#define POLARITY_LOW  (0b1)

#define TRIGGER_EDGE  (0b0)
#define TRIGGER_LEVEL (0b1)

#define DELMOD_FIX    (0b000)
#define DELMOD_SMI    (0b010)
#define DELMOD_NMI    (0b100)
#define DELMOD_INIT   (0b101)
#define DELMOD_ExtINT (0b111)

void ioapic_register_interrupt(uint8_t vector, int64_t gsi, uint8_t isa,
	uint8_t delmod, int polarity, int trigger_mode);

void ioapic_mask(int64_t gsi, uint8_t isa);
void ioapic_unmask(int64_t gsi, uint8_t isa);
