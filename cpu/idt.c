#include "idt.h"

#include "interrupt.h"
#include "isr.h"
#include "x86.h"

#include "util/print.h"

#include <stdint.h>

alignas(16) volatile __uint128_t idt[256];
static interrupt_handler handlers[265];

static uint64_t irq_disable_count = 0;

/**
 * @brief Enable irqs. Can stack such that irqs are only enabled again
 * once for every call to irq_disable irq_enable has been called.
 */
void irq_enable(void) {
	if (irq_disable_count > 1) {
		--irq_disable_count;
	} else {
		--irq_disable_count;
		sti();
	}
}

/**
 * @brief Disable irqs. Can stack such that irqs are only enabled again
 * once for every call to irq_disable irq_enable has been called.
 */
void irq_disable(void) {
	cli();
	++irq_disable_count;
}

void dump_frame(struct interrupt_frame *frame) {
	kprintf(
		"Frame dump:\nvector: 0x%w64X\nerror_code: 0x%w64X\nrflags: "
		"0x%w64X\ncs: 0x%w64X\nrip: 0x%w64X\nss: 0x%w64X\nrsp: 0x%w64X\nrbp: "
		"0x%w64X\nrax: 0x%w64X\nrbx: 0x%w64X\nrcx: 0x%w64X\nrdx: 0x%w64X\nrsi: "
		"0x%w64X\nrdi: 0x%w64X\nr8: 0x%w64X\nr9: 0x%w64X\nr10: 0x%w64X\nr11: "
		"0x%w64X\nr12: 0x%w64X\nr13: 0x%w64X\nr14: 0x%w64X\nr15: 0x%w64X\n\n",
		frame->vector, frame->error_code, frame->rflags, frame->cs, frame->rip,
		frame->ss, frame->rsp, frame->rbp, frame->rax, frame->rbp, frame->rcx,
		frame->rdx, frame->rsi, frame->rdi, frame->r8, frame->r9, frame->r10,
		frame->r11, frame->r12, frame->r13, frame->r14, frame->r15);
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
				"was registered. Ignoring the interrupt\n",
			frame->vector);
	}
}

/**
 * @brief Initialize and load the IDT.
 */
void idt_init(void) {
	kprint("Initializing and loading IDT...\n");

	idt_register_stubs();
	isr_init();

	lidt(sizeof(idt) - 1, (uint64_t)idt);
	kprint("Initializing and loadingIDT: Success\n");
}
