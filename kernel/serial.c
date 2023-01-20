#include "serial.h"

#include "cpu/x86.h"

#define COM1 (0x3F8)

[[gnu::no_caller_saved_registers]] void serial_write(const char *str) {
	for (; *str != '\0'; ++str) outb(COM1, *str);
}
