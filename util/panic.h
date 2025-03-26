#pragma once
#include "cpu/idt.h"

[[noreturn]] void panic(const char *format, ...);
[[noreturn]] void panic_frame(struct interrupt_frame *frame, const char *format,
	...);
