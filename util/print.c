#include "print.h"

#include "cpu/x86.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define COM1 (0x3F8)

/**
 * @brief Output a single char to the serial console (i.e. COM1).
 * @param c The char to output.
 */
static inline void serial_putchar(char c) {
	if (c == '\n') {
		outb(COM1, '\r');
	}
	outb(COM1, c);
}

/**
 * @brief Output a single char to the serial console.
 * @param c The char to output.
 */
void kputchar(char c) {
	serial_putchar(c);
}

/**
 * @brief Output a string to the serial console.
 * @param str A pointer to the string to output.
 */
void kprint(const char *str) {
	for (; *str != '\0'; ++str) {
		serial_putchar(*str);
	}
}

/**
 * @brief Output to the serial console
 * @param format A C-style format string to output. Note that Format flags;
 * field width; precision; wN, wfN, L, H, D, DD length modifiers; f/F, e/E, g/G,
 * a/A format specifier; wide strings (length modifier l + format specifier c or
 * s) are not allowed.
 */
int kprintf(const char *format, ...) {
	va_list arg;
	va_start(arg);
	int ret = kvprintf(format, arg);
	va_end(arg);
	return ret;
}

#define FLAG_MINUS (1 << 0)
#define FLAG_PLUS  (1 << 1)
#define FLAG_SPACE (1 << 2)
#define FLAG_HASH  (1 << 3)
#define FLAG_ZERO  (1 << 4)

enum LENGTH_MODIFIER {
	NONE,
	CHAR,
	SHORT,
	LONG,
	LONG_LONG,
	INTMAX_T,
	SIZE_T,
	PTRDIFF_T,
	W8,
	W16,
	W32,
	W64
};

static int print_signed(intmax_t val);
static int print_unsigned(uintmax_t val, char format);

/**
 * @see kprintf()
 */
int kvprintf(const char *format, va_list arg) {
	int ret = 0;
	for (int n = 0; format[n] != '\0'; ++n) {
		if (format[n] == '\n') {
			serial_putchar('\n');
			++ret;
			continue;
		} else if (format[n] != '%') {
			serial_putchar(format[n]);
			++ret;
			continue;
		} else if (format[n + 1] == '%') {
			serial_putchar('%');
			++ret;
			n += 2;
			continue;
		}
		++n; /* format[n] is a format specifier (%...) */

		int length_modifier = NONE;
		switch (format[n]) {
		case 'h':
			if (format[n + 1] == 'h') {
				length_modifier = CHAR;
				++n;
			} else {
				length_modifier = SHORT;
			}
			++n;
			break;
		case 'l':
			if (format[n + 1] == 'l') {
				length_modifier = LONG_LONG;
				++n;
			} else {
				length_modifier = LONG;
			}
			++n;
			break;
		case 'j':
			length_modifier = INTMAX_T;
			++n;
			break;
		case 'z':
			length_modifier = SIZE_T;
			++n;
			break;
		case 't':
			length_modifier = PTRDIFF_T;
			++n;
			break;
		case 'w':
			++n;
			switch (format[n]) {
			case '8' /* 8 */:
				length_modifier = W8;
				++n;
				break;
			case '1' /* 16 */:
				length_modifier = W16;
				n += 2;
				break;
			case '3' /* 32 */:
				length_modifier = W32;
				n += 2;
				break;
			case '6' /* 64 */:
				length_modifier = W64;
				n += 2;
				break;
			}
			break;
		default: length_modifier = NONE; break;
		}

		/* conversion specifier */
		switch (format[n]) {
		case 'd':
		case 'i':
			intmax_t val;
			switch (length_modifier) {
				/* note: char/uint8_t and short/uint16_t undergo integer
				 * promotion on a function call, thus they are passed as ints */
			case CHAR: val = (intmax_t)va_arg(arg, int); break;
			case SHORT: val = (intmax_t)va_arg(arg, int); break;
			case LONG: val = (intmax_t)va_arg(arg, long); break;
			case LONG_LONG: val = (intmax_t)va_arg(arg, long long); break;
			case INTMAX_T: val = (intmax_t)va_arg(arg, intmax_t); break;
			case SIZE_T: val = (intmax_t)va_arg(arg, size_t); break;
			case PTRDIFF_T: val = (intmax_t)va_arg(arg, ptrdiff_t); break;
			case W8: val = (intmax_t)va_arg(arg, int); break;
			case W16: val = (intmax_t)va_arg(arg, int); break;
			case W32: val = (intmax_t)va_arg(arg, int32_t); break;
			case W64: val = (intmax_t)va_arg(arg, int64_t); break;
			case NONE: val = (intmax_t)va_arg(arg, int); break;
			}
			ret += print_signed(val);
			break;
		case 'b': /* unsigned int, binary */
		case 'o': /* unsigned int, octal */
		case 'u': /* unsigned int, decimal*/
		case 'x': /* unsigned int, lower case hex */
		case 'X': /* unsigned int, upper case hex*/
			uintmax_t uval;
			switch (length_modifier) {
				/* note: char/uint8_t and short/uint16_t undergo integer
				 * promotion on a function call, thus they are passed as ints */
			case CHAR: uval = (uintmax_t)va_arg(arg, unsigned int); break;
			case SHORT: uval = (uintmax_t)va_arg(arg, unsigned int); break;
			case LONG: uval = (uintmax_t)va_arg(arg, unsigned long); break;
			case LONG_LONG:
				uval = (uintmax_t)va_arg(arg, unsigned long long);
				break;
			case INTMAX_T: uval = (uintmax_t)va_arg(arg, uintmax_t); break;
			case SIZE_T: uval = (uintmax_t)va_arg(arg, size_t); break;
			case PTRDIFF_T: uval = (uintmax_t)va_arg(arg, ptrdiff_t); break;
			case W8: uval = (uintmax_t)va_arg(arg, unsigned int); break;
			case W16: uval = (uintmax_t)va_arg(arg, unsigned int); break;
			case W32: uval = (uintmax_t)va_arg(arg, uint32_t); break;
			case W64: uval = (uintmax_t)va_arg(arg, uint64_t); break;
			case NONE: uval = (uintmax_t)va_arg(arg, unsigned int); break;
			}
			ret += print_unsigned(uval, format[n]);
			break;
		case 'c':
			int c = va_arg(arg, int);
			serial_putchar((unsigned char)c);
			++ret;
			break;
		case 's':
			char *str = va_arg(arg, char *);
			do {
				serial_putchar(*str);
				++ret;
				++str;
			} while (*str != '\0');
			break;
		case 'p': /* pointer, implementation defined */
			uint64_t pval = (uint64_t)va_arg(arg, void *);
			print_unsigned(pval, 'X');
			break;
		case 'n': /* store current ret value in argument */
			int *ret_val = va_arg(arg, int *);
			*ret_val = ret;
			break;
		default: return -1;
		}
	}
	serial_putchar('\n');
	return ret;
}

static int print_signed(intmax_t val) {
	if (val == 0) {
		serial_putchar('0');
		return 1;
	}
	char digits[] = "0123456789";
	char buf[20] = {}; /* intmax_t is 64 bits -> max 19 digits + sign */
	int idx = 0;
	bool negative = val < 0 ? true : false;
	val = negative ? -val : val; /* make val positive */

	do {
		buf[idx++] = digits[val % 10];
		val /= 10;
	} while (val > 0);

	if (negative) {
		buf[idx++] = '-';
	}

	for (int i = idx - 1; i >= 0; --i) {
		serial_putchar(buf[i]);
	}

	return idx - 1;
}

static int print_unsigned(uintmax_t val, char format) {
	if (val == 0) {
		serial_putchar('0');
		return 1;
	}
	char *digits = "0123456789abcdef";
	char buf[64] = {}; /* intmax_t is 64 bits -> max 64 digits in binary */
	int idx = 0;
	int base;
	switch (format) {
	case 'b': base = 2; break;
	case 'o': base = 8; break;
	case 'u': base = 10; break;
	case 'x': base = 16; break;
	case 'X':
		base = 16;
		digits = "0123456789ABCDEF";
		break;
	}

	do {
		buf[idx++] = digits[val % base];
		val /= base;
	} while (val > 0);

	for (int i = idx - 1; i >= 0; --i) {
		serial_putchar(buf[i]);
	}

	return idx - 1;
}
