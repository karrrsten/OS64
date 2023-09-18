#pragma once

#include <stdarg.h>

#define PRId8 "%hhd"
#define PRIi8 "%hhi"
#define PRIu8 "%hhu"
#define PRIb8 "%hhb"
#define PRIo8 "%hho"
#define PRIx8 "%hhx"
#define PRIX8 "%hhX"

#define PRId16 "%hd"
#define PRIi16 "%hi"
#define PRIu16 "%hu"
#define PRIb16 "%hb"
#define PRIo16 "%ho"
#define PRIx16 "%hx"
#define PRIX16 "%hX"

#define PRId32 "%d"
#define PRIi32 "%i"
#define PRIu32 "%u"
#define PRIb32 "%b"
#define PRIo32 "%o"
#define PRIx32 "%x"
#define PRIX32 "%X"

#define PRId64 "%ld"
#define PRIi64 "%li"
#define PRIu64 "%lu"
#define PRIb64 "%lb"
#define PRIo64 "%lo"
#define PRIx64 "%lx"
#define PRIX64 "%lX"

#define PRIdMAX "%ld"
#define PRIiMAX "%li"
#define PRIuMAX "%lu"
#define PRIbMAX "%lb"
#define PRIoMAX "%lo"
#define PRIxMAX "%lx"
#define PRIXMAX "%lX"

#define PRIdPTR "%ld"
#define PRIiPTR "%li"
#define PRIuPTR "%lu"
#define PRIbPTR "%lb"
#define PRIoPTR "%lo"
#define PRIxPTR "%lx"
#define PRIXPTR "%lX"

void kputchar(char c);
void kprint(const char *str);

int kprintf(const char *format, ...);

int kvprintf(const char *format, va_list arg);
