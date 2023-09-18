#pragma once

#include <stdint.h>

void idt_init(void);

int idt_alloc_vector(void);

/**
 * @struct interrupt_frame
 * @brief The interrupt frame provided by the interrupt stub to the registered
 * handler.
 */
struct [[gnu::packed]] interrupt_frame {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t vector;
	uint64_t error_code;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
};

typedef void (*interrupt_handler)(struct interrupt_frame *);

extern volatile __uint128_t idt[256];

void idt_register(uint8_t vector, interrupt_handler handler);
