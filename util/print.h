#pragma once

#include <stdarg.h>
#include <stddef.h>

void kputchar(char c);
void kprint(const char *str);

int kprintf(const char *restrict format, ...);

int kvprintf(const char *restrict format, va_list arg);
