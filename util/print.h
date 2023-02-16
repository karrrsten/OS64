#pragma once

#include <stdarg.h>
#include <stddef.h>

void kputchar(char c);
void kprint(const char *str);

/* C-style formatted ouput to COM1.
 * Format flags, field width, precision; wN, wfN, L, H, D, DD length
 * modifiers; f, F, e, E, g, G, a, A format specifier; wide strings (length
 * modifier l + format specifier c or s) are not allowed
 */
int kprintf(const char *restrict format, ...);

/* C-style formatted ouput to COM1.
 * Format flags, field width, precision; wN, wfN, L, H, D, DD length modifiers;
 * f, F, e, E, g, G, a, A format specifier; wide strings (length modifier l +
 * format specifier c or s) are not allowed
 */
int kvprintf(const char *restrict format, va_list arg);
