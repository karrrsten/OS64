#include "isr.h"

#include "descr_tbl.h"
#include "x86.h"

#include "util/log.h"

#include <stdint.h>

#define serial_write(...)

#define DEFAULT_HANDLER_WITH_CODE(vec, err_str)                              \
	[[gnu::interrupt, noreturn]] static void isr##vec(                       \
		[[maybe_unused]] struct interrupt_frame *frame, uint64_t err_code) { \
		handler_with_code(err_str, err_code);                                \
	}

#define DEFAULT_HANDLER_NO_CODE(vec, err_str)             \
	[[gnu::interrupt, noreturn]] static void isr##vec(    \
		[[maybe_unused]] struct interrupt_frame *frame) { \
		handler_no_code(err_str);                         \
	}

[[noreturn, gnu::no_caller_saved_registers]] static void handler_with_code(
	const char *err_str, uint64_t err_code) {
	panic("An Error occured: \n Error: %s\n Code: 0x%p", err_str, err_code);
}

[[noreturn, gnu::no_caller_saved_registers]] static void handler_no_code(
	const char *err_str) {
	panic("An Error occured: \n Error: %s\n No error code!", err_str);
}

DEFAULT_HANDLER_NO_CODE(0, "Divide-by-Zero-Error Exception")
DEFAULT_HANDLER_NO_CODE(1, "Debug Exception")
DEFAULT_HANDLER_NO_CODE(2, "Non-Maskable-Interrupt Exception")
DEFAULT_HANDLER_NO_CODE(3, "Breakpoint")
DEFAULT_HANDLER_NO_CODE(4, "Overflow")
DEFAULT_HANDLER_NO_CODE(5, "Bound-Range")
DEFAULT_HANDLER_NO_CODE(6, "Invalid-Opcode")
DEFAULT_HANDLER_NO_CODE(7, "Device-Not-Available")
DEFAULT_HANDLER_WITH_CODE(8, "Double-Fault")
/* Vector 9 is reserved */
DEFAULT_HANDLER_WITH_CODE(10, "Invalid-TSS")
DEFAULT_HANDLER_WITH_CODE(11, "Segment-Not-Present")
DEFAULT_HANDLER_WITH_CODE(12, "Stack")
DEFAULT_HANDLER_WITH_CODE(13, "General-Protection")
/* Vector 14 is handled seperately */
/* Vector 15 is reserved */
DEFAULT_HANDLER_NO_CODE(16, "x87 Floating-Point Exception-Pending")
DEFAULT_HANDLER_WITH_CODE(17, "Alignment-Check")
DEFAULT_HANDLER_NO_CODE(18, "Machine-Check")
DEFAULT_HANDLER_NO_CODE(19, "SIMD Floating-Point")
/* Vector 20 is reserved */
DEFAULT_HANDLER_WITH_CODE(21, "Control-Protection Exception")
/* Vector 22 is reserved */
/* Vector 23 is reserved */
/* Vector 24 is reserved */
/* Vector 25 is reserved */
/* Vector 26 is reserved */
/* Vector 27 is reserved */
DEFAULT_HANDLER_NO_CODE(28, "Hypervisor Injection Exception")
DEFAULT_HANDLER_WITH_CODE(29, "VMM Communication Exception")
DEFAULT_HANDLER_WITH_CODE(30, "Security Exception")

/* Vector 31 is reserved */

/* Vectors 32-255 are user defined, i.e. generated externally and handlers are
 * installed when the corresponding device is configured */

[[gnu::no_caller_saved_registers]] static void print_page_fault(
	uint64_t err_code) {
	panic("An Error occured:\n Error: Page-Fault Exception\nCode: 0x%p\nPage "
		  "fault linear address: 0x%p",
		err_code, rcr2());
}

[[gnu::interrupt, noreturn]] static void isr14(
	[[maybe_unused]] struct interrupt_frame *frame,
	[[maybe_unused]] uint64_t err_code) {
	print_page_fault(err_code);
	for (;;) {
		hlt();
	}
}

/**
 * @brief Initialize the interrupt/exception handlers of the kernel, i.e.
 * vectors 0-31.
 */
void isr_init(void) {
	idt_register(0, (void *)&isr0, GATE_TYPE_INT);
	idt_register(1, (void *)&isr1, GATE_TYPE_INT);
	idt_register(2, (void *)&isr2, GATE_TYPE_INT);
	idt_register(3, (void *)&isr3, GATE_TYPE_INT);
	idt_register(4, (void *)&isr4, GATE_TYPE_INT);
	idt_register(5, (void *)&isr5, GATE_TYPE_INT);
	idt_register(6, (void *)&isr6, GATE_TYPE_INT);
	idt_register(7, (void *)&isr7, GATE_TYPE_INT);
	idt_register(8, (void *)&isr8, GATE_TYPE_INT);
	idt_register(9, nullptr, GATE_TYPE_INT); /* reserved Vector */
	idt_register(10, (void *)&isr10, GATE_TYPE_INT);
	idt_register(11, (void *)&isr11, GATE_TYPE_INT);
	idt_register(12, (void *)&isr12, GATE_TYPE_INT);
	idt_register(13, (void *)&isr13, GATE_TYPE_INT);
	idt_register(14, (void *)&isr14, GATE_TYPE_INT);
	idt_register(15, nullptr, GATE_TYPE_INT); /* reserved Vector */
	idt_register(16, (void *)&isr16, GATE_TYPE_INT);
	idt_register(17, (void *)&isr17, GATE_TYPE_INT);
	idt_register(18, (void *)&isr18, GATE_TYPE_INT);
	idt_register(19, (void *)&isr19, GATE_TYPE_INT);
	idt_register(20, nullptr, GATE_TYPE_INT); /* reserved Vector */
	idt_register(21, (void *)&isr21, GATE_TYPE_INT);
	idt_register(22, nullptr, GATE_TYPE_INT); /* reserved Vector */
	idt_register(23, nullptr, GATE_TYPE_INT); /* reserved Vector */
	idt_register(24, nullptr, GATE_TYPE_INT); /* reserved Vector */
	idt_register(25, nullptr, GATE_TYPE_INT); /* reserved Vector */
	idt_register(26, nullptr, GATE_TYPE_INT); /* reserved Vector */
	idt_register(27, nullptr, GATE_TYPE_INT); /* reserved Vector */
	idt_register(28, (void *)&isr28, GATE_TYPE_INT);
	idt_register(29, (void *)&isr29, GATE_TYPE_INT);
	idt_register(30, (void *)&isr30, GATE_TYPE_INT);
	idt_register(31, nullptr, GATE_TYPE_INT); /* reserved Vector */
}
