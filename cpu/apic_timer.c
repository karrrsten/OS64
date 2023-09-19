#include "apic_timer.h"

#include "apic.h"
#include "idt.h"

#include <cpuid.h>

/**
 * @brief Set the Local APIC timer.
 * @param time The time in milliseconds.
 * @param handler An interrupt handler, to be registered for the timer. Note:
the handler will need to call apic_eoi().
 * @param mode The mode in which the timer is running. Can be
APIC_TIMER_ONE_SHOT or APIC_TIMER_PERIODIC.
*/
void apic_set_timer(uint32_t time, interrupt_handler handler, enum mode mode) {
	static bool did_init = false;
	static uint8_t vector;
	static uint64_t mHz_frequency;
	if (!did_init) {
		unsigned int eax, ebx, ecx, edx;
		__get_cpuid(0x15, &eax, &ebx, &ecx, &edx);

		mHz_frequency = ecx / 1000;

		lapic_write(APIC_TIMER_DIVIDE, 0b1011);

		vector = idt_alloc_vector();
		/* Disable the timer, enable the LVT entry */
		lapic_write(APIC_TIMER_INIT, 0);
		lapic_write(APIC_LVT_TIMER, (mode << 17) | vector);
	}

	idt_register(vector, handler);

	/* Start the timer */
	uint64_t count = time * mHz_frequency;
	lapic_write(APIC_TIMER_INIT, count);
}

/**
 * @brief Mask and disable the APIC timer.
 */
void apic_mask_timer(void) {
	lapic_write(APIC_LVT_TIMER, lapic_read(APIC_LVT_TIMER) | APIC_LVT_MASK);
}

/**
 * @brief Unmask and enable the APIC timer.
 */
void apic_unmask_timer(void) {
	lapic_write(APIC_LVT_TIMER, lapic_read(APIC_LVT_TIMER) & ~APIC_LVT_MASK);
}
