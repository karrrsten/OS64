#pragma once

#include <stdint.h>

#define GDT_RPL_0 (0)
#define GDT_RPL_3 (3)

#define GDT_NULL      (0x0)
#define GDT_KERNEL_CS (0x8)
#define GDT_ANY_DS    (0x10)
#define GDT_USER_CS   (0x18)
#define GDT_TSS       (0x20)

void gdt_init(void);

void idt_init(void);

int idt_alloc_vector(void);

/**
 * @def GATE_TYPE_INT
 * @brief An interrupt gate of type INT, disables further
 * interrupts.
 */
#define GATE_TYPE_INT (0xE)

/**
 * @def GATE_TYPE_INT
 * @brief An interrupt gate of type TRAP, does not disable further
 * interrupts.
 */
#define GATE_TYPE_TRAP (0xF)

/**
 * @struct interrupt_frame
 * @brief The interrupt frame pushed by the cpu onto the stack when an interrupt
 * happens. Can be used in [[gnu::interrupt]] handler functions.
 */
struct [[gnu::packed]] interrupt_frame {
	uint64_t rip;
	uint64_t cs : 16;
	uint64_t    : 48;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss : 16;
	uint64_t    : 48;
};

void idt_register(uint8_t vector, void *handler, uint64_t type);
