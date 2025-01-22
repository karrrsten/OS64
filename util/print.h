#pragma once

#include <stdarg.h>

#define dump_struct(s) __builtin_dump_struct(s, kprintf)

void serial_init(void);

void kputchar(char c);
void kprint(const char *str);

int kprintf(const char *format, ...);

int kvprintf(const char *format, va_list arg);
