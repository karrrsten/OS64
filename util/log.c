#include "log.h"

#include "cpu/x86.h"
#include "print.h"
#include <stdarg.h>

/**
 * @brief Log to the serial console
 * @param format A C-style format string to log (note the limitations of
 * kprintf).
 * @see kprintf()
 */
void log(const char *format, ...) {
	kprint("[LOG]: ");
	va_list arg;
	va_start(arg);
	kvprintf(format, arg);
	kprintf("\n");
	va_end(arg);
}

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
	kprintf("\nHalting...\n");
	va_end(arg);
	for (;;) {
		hlt();
	};
}
