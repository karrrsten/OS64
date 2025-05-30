#include "isr.h"

#include "idt.h"
#include "x86.h"

#include "util/panic.h"

static const char *exception_strings[] = {"Divide-by-Zero-Error Exception",
	"Debug Exception", "Non-Maskable-Interrupt Exception", "Breakpoint",
	"Overflow", "Bound-Range", "Invalid-Opcode", "Device-Not-Available",
	"Double-Fault", "Reserved (9)", "Invalid-TSS", "Segment-Not-Present",
	"Stack", "General-Protection", "Page-Fault", "Reserved (15)",
	"x87 Floating-Point Exception-Pending", "Alignment-Check", "Machine-Check",
	"SIMD Floating-Point", "Control-Protection Exception", "Reserved (21)",
	"Reserved (22)", "Reserved (23)", "Reserved (24)", "Reserved (25)",
	"Reserved (26)", "Reserved (27)", "Hypervisor Injection Exception",
	"VMM Communication Exception", "Security Exception", "Reserved"};

[[noreturn]] static void exception_handler(struct interrupt_frame *frame) {
	panic_frame(frame, "An Error occured: \n Error: %s\n",
		exception_strings[frame->vector], frame->error_code);
}

[[noreturn]] static void page_fault(struct interrupt_frame *frame) {
	panic_frame(frame,
		"An Error occured:\nError: Page-Fault Exception\nPage fault linear "
		"address: 0x%w64X\n",
		frame->error_code, rcr2());
}

/**
 * @brief Initialize the exception handlers.
 */
void isr_init(void) {
	for (int i = 0; i < 32; ++i) {
		if (i == 14) {
			idt_register(14, page_fault);
		} else {
			idt_register(i, exception_handler);
		}
	}
}
