#include "panic.h"

#include "print.h"

#include "cpu/idt.h"
#include "cpu/x86.h"

#include <stdarg.h>

/**
 * @brief Halt, displaying a panic message
 * @param format A C-style format string to display (note the limitations of
 * kprintf).
 * @see kprintf()
 */
[[noreturn]] void panic(const char *format, ...) {
	kprint("\n\n[PANIC!]: ");
	va_list arg;
	va_start(arg);
	kvprintf(format, arg);
	va_end(arg);
	kprintf("Halting...");
	for (;;) {
		hlt();
	}
}

/**
 * @brief Halt, displaying a panic message
 * @param frame An interrupt frame.
 * @param format A C-style format string to display (note the limitations of
 * kprintf).
 * @see kprintf()
 */
[[noreturn]] void panic_frame(struct interrupt_frame *frame, const char *format,
	...) {
	kprint("\n\n[PANIC!]: ");
	va_list arg;
	va_start(arg);
	kvprintf(format, arg);
	va_end(arg);
	dump_frame(frame);
	kprintf("Halting...");
	for (;;) {
		hlt();
	}
}
