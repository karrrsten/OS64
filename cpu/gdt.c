#include "gdt.h"

#include "x86.h"

#include "kernel/limine_reqs.h"
#include "util/print.h"

#include <stdint.h>

#define CS_LONG_MODE ((uint64_t)1 << 53)

#define DS_WRITABLE ((uint64_t)1 << 41)

#define SYS_SEG_GRANULARITY ((__uint128_t)1 << 55)
#define SYS_SEG_TSS         ((__uint128_t)0b1001 << 40)

#define SYS_SEG_SPLIT_BASE(base)             \
	((((__uint128_t)base & 0xFF'FFFF) << 16) \
		| ((((__uint128_t)base & 0xFFFF'FFFF'FFF0'0000) >> 24) << 56))
#define SYS_SEG_SPLIT_LIMIT(limit) \
	(((__uint128_t)limit & 0xFFFF) \
		| ((((__uint128_t)limit & 0xF'0000) >> 16) << 48))

struct [[gnu::packed]] tss {
	uint32_t : 32;
	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;
	uint64_t : 64;
	uint64_t ist1;
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;
	uint64_t : 64;
	uint16_t : 16;
	uint16_t io_pb_base;
};

static volatile struct tss tss;

static volatile void *isr_stack;

alignas(8) static volatile char gdt[56];

/**
 * @brief Initialize the GDT with a kernel and a user code segment, a data
 * segment for both an an entry for a tss.
 */
void gdt_init(void) {
	kprint("Preparing GDT...\n");

	*(uint64_t *)&gdt[GDT_NULL] = 0;

	*(uint64_t *)&gdt[GDT_KERNEL_CS]
		= SEG_CODE | SEG_PRESENT | SEG_DPL_0 | CS_LONG_MODE;

	*(uint64_t *)&gdt[GDT_KERNEL_DS]
		= SEG_DATA | SEG_PRESENT | SEG_DPL_0 | DS_WRITABLE;

	*(uint64_t *)&gdt[GDT_USER_CS]
		= SEG_CODE | SEG_PRESENT | SEG_DPL_3 | CS_LONG_MODE;

	// TODO: might be able to delete this
	*(uint64_t *)&gdt[GDT_USER_DS]
		= SEG_DATA | SEG_PRESENT | SEG_DPL_3 | DS_WRITABLE;

	uint64_t tss_phys
		= (uint64_t)&tss - limine_kernel_address_response->virtual_base
	    + limine_kernel_address_response->physical_base;
	*(__uint128_t *)&gdt[GDT_TSS]
		= SYS_SEG_TSS | SEG_PRESENT | SEG_DPL_3 | SYS_SEG_SPLIT_BASE(tss_phys)
	    | SYS_SEG_SPLIT_LIMIT(sizeof(tss));

	kprint("Loading GDT...\n");
	lgdt(sizeof(gdt) - 1, (uint64_t)gdt);
	kprint("Loading GDT: Success\n");

	kprint("Loading TSS...\n");
	tss.rsp0 = (uint64_t)isr_stack;
	ltr(GDT_TSS);
	kprint("Loading TSS: Success\n");

	kprint("Loading segment registers...\n");
	/* load %ds, %es, %fs, %gs, %ss */
	/* Note: apparently ds,es,ss contents are ignored entirely, NULL selector
	 * should also work...a full GDT is still needed for syscall/sysret though
	 */
	asm volatile(
		"movw $0x10, %%ax\n"
		"movw %%ax, %%ds\n"
		"movw %%ax, %%es\n"
		"movw %%ax, %%fs\n"
		"movw %%ax, %%gs\n"
		"movw %%ax, %%ss\n"
		:
		:
		: "ax");
	/* load %cs with a far return, -mno-red-zone is required */
	asm volatile goto(
		"pushq $0x8\n"
		"pushq $%l0\n"
		"lretq\n"
		:
		:
		:
		: ret);
ret:
	kprint("Loading segment registers: Success\n");
}
