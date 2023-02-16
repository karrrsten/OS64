#include "log.h"

#include "cpu/x86.h"
#include "print.h"
#include <stdarg.h>

void log(const char *format, ...) {
	kprint("[LOG]: ");
	va_list arg;
	va_start(arg);
	kvprintf(format, arg);
	kprintf("\n");
	va_end(arg);
}

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
