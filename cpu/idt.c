#include "idt.h"

#include "gdt.h"
#include "interrupt.h"
#include "isr.h"
#include "x86.h"

#include "util/log.h"

#include <stdalign.h>

volatile alignas(16) __uint128_t idt[256];
static interrupt_handler handlers[265];

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
		log("An interrupt (vector 0x%hX) was received, but no handler was "
			"registered. Ignoring the interrupt.",
			frame->vector);
	}
}

/**
 * @brief Initialize and load the IDT.
 */
void idt_init(void) {
	log("Initializing and loading IDT...");

	idt_register_stubs();
	isr_init();

	lidt(sizeof(idt) - 1, (uint64_t)idt);
	log("Initializing and loadingIDT: Success");
}
