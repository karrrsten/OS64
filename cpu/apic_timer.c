#include "apic_timer.h"

#include "apic.h"
#include "idt.h"

#include "util/panic.h"

#include <cpuid.h>
#include <stdint.h>

static void (*timer_handler)(struct interrupt_frame *frame);

static void _timer_handler(struct interrupt_frame *frame) {
	timer_handler(frame);
	apic_eoi();
}

enum apic_timer_divide {
	APIC_TIMER_DIVIDE_2 = 0b0000,
	APIC_TIMER_DIVIDE_4 = 0b0001,
	APIC_TIMER_DIVIDE_8 = 0b0010,
	APIC_TIMER_DIVIDE_16 = 0b0011,
	APIC_TIMER_DIVIDE_32 = 0b1000,
	APIC_TIMER_DIVIDE_64 = 0b1001,
	APIC_TIMER_DIVIDE_128 = 0b1010,
	APIC_TIMER_DIVIDE_1 = 0b1011
};

/**
 * @brief Set the Local APIC timer.
 * @param time The time in nanoseconds.
 * @param handler An interrupt handler, to be registered for the timer.
 * @param mode The mode in which the timer is running. Can be
APIC_TIMER_ONE_SHOT or APIC_TIMER_PERIODIC.
*/
void apic_set_timer(uint64_t time, interrupt_handler handler,
	enum apic_timer_mode mode) {
	static bool did_init = false;
	static uint8_t vector;
	static uint32_t hz_frequency;
	if (!did_init) {
		// TODO: find a portable way to determine frequency
		uint32_t eax = 0x15, ebx, ecx, edx;
		__cpuid(0, eax, ebx, ecx, edx);
		if (ecx) {
			hz_frequency = ecx;
		} else {
			eax = 0x16;
			__cpuid(0, eax, ebx, ecx, edx);
			if (!ecx) {
				panic("Could not determine APIC timer frequenzy.");
			}
			hz_frequency = ecx * 1000000; /* scale ecx from mHz to Hz */
		}

		vector = idt_alloc_vector();
		/* Disable the timer, enable the LVT entry */
		lapic_write(APIC_TIMER_INIT, 0);
		lapic_write(APIC_LVT_TIMER, (mode << 17) | vector);

		idt_register(vector, _timer_handler);
		did_init = true;
	}

	timer_handler = handler;

	/* Start the timer */
	uint64_t count = time * hz_frequency / 1000000000;

	/* Determine the correct divide value to use by counting finding out how far
	 * count needs to be shifted left to fit in 32 bits */
	int bits = __builtin_clzll(count);
	uint32_t divide;
	if (bits < 32) {
		int shift = 32 - bits;
		count = count >> shift;
		switch (shift) {
		case 1: divide = APIC_TIMER_DIVIDE_2; break;
		case 2: divide = APIC_TIMER_DIVIDE_4; break;
		case 3: divide = APIC_TIMER_DIVIDE_8; break;
		case 4: divide = APIC_TIMER_DIVIDE_16; break;
		case 5: divide = APIC_TIMER_DIVIDE_32; break;
		case 6: divide = APIC_TIMER_DIVIDE_64; break;
		case 7: divide = APIC_TIMER_DIVIDE_128; break;
		default: divide = APIC_TIMER_DIVIDE_1;
		}
	} else {
		divide = APIC_TIMER_DIVIDE_1;
	}

	lapic_write(APIC_TIMER_DIVIDE, divide);
	lapic_write(APIC_TIMER_INIT, count);
}

/**
 * @brief Stop the APIC timer.
 */
void apic_stop_timer(void) {
	lapic_write(APIC_TIMER_INIT, 0);
}

static uint64_t timer_current;

/**
 * @brief Pause the APIC timer.
 */
void apic_pause_timer(void) {
	timer_current = lapic_read(APIC_TIMER_CURRENT);
	lapic_write(APIC_TIMER_INIT, 0);
}

/**
 * @brief Resume the APIC timer.
 */
void apic_resume_timer(void) {
	lapic_write(APIC_TIMER_INIT, timer_current);
}
