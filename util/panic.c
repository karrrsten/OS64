#include "panic.h"

#include "print.h"

#include "cpu/x86.h"

#include <stdarg.h>

/**
 * @brief Halt, displaying a panic message
 * @param format A C-style format string to display (note the limitations of
 * kprintf).
 * @see kprintf()
 */
[[noreturn]] void panic(const char *format, ...) {
	kprint("[PANIC!]: ");
	va_list arg;
	va_start(arg);
	kvprintf(format, arg);
	kprintf("Halting...");
	va_end(arg);
	for (;;) {
		hlt();
	};
}
