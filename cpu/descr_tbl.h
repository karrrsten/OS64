#pragma once

#include <stdint.h>

#define RPL_0 (0)
#define RPL_3 (3)

#define KERNEL_CS (0x8 | RPL_0)
#define USER_CS   (0x18 | RPL_3)
#define ANY_DS    (0x10)
#define GDT_TSS   (0x20)

void gdt_init(void);

void idt_init(void);

/* only TYPE_INT and TYPE_TRAP are relevant for interrupt handlers */
enum descriptor_type {
	DESCR_TYPE_LDT = 0x2,
	DESCR_TYPE_TSS_AVL = 0x9,
	DESCR_TYPE_TSS_BUSY = 0xB,
	DESCR_TYPE_CALL = 0xC,
	DESCR_TYPE_INT = 0xE,
	DESCR_TYPE_TRAP = 0xF
};

struct [[gnu::packed]] interrupt_frame {
	uint64_t rip;
	uint64_t cs : 16;
	uint64_t    : 48;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss : 16;
};

void idt_register(uint8_t vector, void *handler, enum descriptor_type type);
