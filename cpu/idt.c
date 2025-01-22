#include "idt.h"

#include "interrupt.h"
#include "isr.h"
#include "x86.h"

#include "util/print.h"

#include <stdint.h>

alignas(16) volatile __uint128_t idt[256];
static interrupt_handler handlers[265];

static uint32_t irq_disable_count = 0;

void irq_enable(void) {
	if (!--irq_disable_count) {
		sti();
	}
}

void irq_disable(void) {
	cli();
	++irq_disable_count;
}

/**
 * @brief Allocate a vector in the idt.
 * @return The allocated vector or -1 if no vector was free
 */
int idt_alloc_vector(void) {
	for (int i = 0; i < 256; ++i) {
		if (!handlers[i]) {
			return i;
		}
	}

	return -1;
}

/**
 * @brief Register an interrupt handler.
 * @param vector The vector for which the handler should be registered.
 * @param handler The handler to be registerd.
 */
void idt_register(uint8_t vector, interrupt_handler handler) {
	handlers[vector] = handler;
}

void interrupt_stub(struct interrupt_frame *frame) {
	if (handlers[frame->vector] != 0) {
		handlers[frame->vector](frame);
	} else {
		kprintf("An interrupt (vector 0x%w64X) was received, but no handler "
				"was registered. Ignoring the interrupt.",
			frame->vector);
	}
}

/**
 * @brief Initialize and load the IDT.
 */
void idt_init(void) {
	kprintf("Initializing and loading IDT...");

	idt_register_stubs();
	isr_init();

	lidt(sizeof(idt) - 1, (uint64_t)idt);
	kprintf("Initializing and loadingIDT: Success");
}
