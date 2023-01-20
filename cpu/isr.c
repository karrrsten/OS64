#include "isr.h"

#include "descr_tbl.h"
#include "kernel/serial.h"
#include "x86.h"

#define DEFAULT_HANDLER_WITH_CODE(vec, err_str)         \
	[[gnu::interrupt, noreturn]] void isr##vec(         \
		[[maybe_unused]] struct interrupt_frame *frame, \
		[[maybe_unused]] uint64_t err_code) {           \
		serial_write("An Error occured:\nError: ");     \
		serial_write(err_str);                          \
		serial_write("\nCode: 0x");                     \
		/* serial_write(err_code); */                   \
		for (;;) {                                      \
			hlt();                                      \
		}                                               \
	}

#define DEFAULT_HANDLER_NO_CODE(vec, err_str)             \
	[[gnu::interrupt, noreturn]] void isr##vec(           \
		[[maybe_unused]] struct interrupt_frame *frame) { \
		serial_write("An Error occured:\nError: ");       \
		serial_write(err_str);                            \
		serial_write("\nNo Error Code");                  \
		for (;;) {                                        \
			hlt();                                        \
		}                                                 \
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

[[gnu::interrupt, noreturn]] void isr14(
	[[maybe_unused]] struct interrupt_frame *frame,
	[[maybe_unused]] uint64_t err_code) {
	serial_write("An Error occured:\nError: Page-Fault Exception\nCode: 0x");
	/* serial_write(err_code); */
	/* serial_write("Page fault linear address: 0x")*/
	/* serial_write(rcr2()); */
	for (;;) {
		hlt();
	}
}

void isr_init(void) {
	idt_register(0, (void *)&isr0, DESCR_TYPE_INT);
	idt_register(1, (void *)&isr1, DESCR_TYPE_INT);
	idt_register(2, (void *)&isr2, DESCR_TYPE_INT);
	idt_register(3, (void *)&isr3, DESCR_TYPE_INT);
	idt_register(4, (void *)&isr4, DESCR_TYPE_INT);
	idt_register(5, (void *)&isr5, DESCR_TYPE_INT);
	idt_register(6, (void *)&isr6, DESCR_TYPE_INT);
	idt_register(7, (void *)&isr7, DESCR_TYPE_INT);
	idt_register(8, (void *)&isr8, DESCR_TYPE_INT);
	idt_register(9, nullptr, DESCR_TYPE_INT); /* reserved Vector */
	idt_register(10, (void *)&isr10, DESCR_TYPE_INT);
	idt_register(11, (void *)&isr11, DESCR_TYPE_INT);
	idt_register(12, (void *)&isr12, DESCR_TYPE_INT);
	idt_register(13, (void *)&isr13, DESCR_TYPE_INT);
	idt_register(14, (void *)&isr14, DESCR_TYPE_INT);
	idt_register(15, nullptr, DESCR_TYPE_INT); /* reserved Vector */
	idt_register(16, (void *)&isr16, DESCR_TYPE_INT);
	idt_register(17, (void *)&isr17, DESCR_TYPE_INT);
	idt_register(18, (void *)&isr18, DESCR_TYPE_INT);
	idt_register(19, (void *)&isr19, DESCR_TYPE_INT);
	idt_register(20, nullptr, DESCR_TYPE_INT); /* reserved Vector */
	idt_register(21, (void *)&isr21, DESCR_TYPE_INT);
	idt_register(22, nullptr, DESCR_TYPE_INT); /* reserved Vector */
	idt_register(23, nullptr, DESCR_TYPE_INT); /* reserved Vector */
	idt_register(24, nullptr, DESCR_TYPE_INT); /* reserved Vector */
	idt_register(25, nullptr, DESCR_TYPE_INT); /* reserved Vector */
	idt_register(26, nullptr, DESCR_TYPE_INT); /* reserved Vector */
	idt_register(27, nullptr, DESCR_TYPE_INT); /* reserved Vector */
	idt_register(28, (void *)&isr28, DESCR_TYPE_INT);
	idt_register(29, (void *)&isr29, DESCR_TYPE_INT);
	idt_register(30, (void *)&isr30, DESCR_TYPE_INT);
	idt_register(31, nullptr, DESCR_TYPE_INT); /* reserved Vector */
}
