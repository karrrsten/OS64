#pragma once

#include <stdint.h>

#define GDT_RPL_0 (0)
#define GDT_RPL_3 (3)

#define GDT_NULL      (0x0)
#define GDT_KERNEL_CS (0x8)
#define GDT_ANY_DS    (0x10)
#define GDT_USER_CS   (0x18)
#define GDT_TSS       (0x20)

#define SEG_DPL_0   ((uint64_t)0 << 45)
#define SEG_DPL_3   ((uint64_t)3 << 45)
#define SEG_PRESENT ((uint64_t)1 << 47)

#define SEG_CODE ((uint64_t)0b11 << 43)
#define SEG_DATA ((uint64_t)0b10 << 43)

void gdt_init(void);
