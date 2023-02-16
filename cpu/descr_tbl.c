#include "descr_tbl.h"

#include "isr.h"
#include "x86.h"

#include "kernel/limine_reqs.h"
#include "util/log.h"

#include <stdalign.h>
#include <stdint.h>

#define SEG_DPL_0   ((uint64_t)0 << 45)
#define SEG_DPL_3   ((uint64_t)3 << 45)
#define SEG_PRESENT ((uint64_t)1 << 47)

#define SEG_CODE ((uint64_t)0b11 << 43)
#define SEG_DATA ((uint64_t)0b10 << 43)

#define CS_CONFORMING      ((uint64_t)1 << 42)
#define CS_LONG_MODE       ((uint64_t)1 << 53)
#define CS_DEFAULT_OP_SIZE ((uint64_t)1 << 54)

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

volatile struct tss tss;

volatile void *isr_stack;

volatile char alignas(8) gdt[48];

void gdt_init(void) {
	log("Preparing GDT...");

	*(uint64_t *)&gdt[GDT_NULL] = 0;

	*(uint64_t *)&gdt[GDT_KERNEL_CS]
		= SEG_CODE | SEG_PRESENT | SEG_DPL_0 | CS_LONG_MODE;

	*(uint64_t *)&gdt[GDT_ANY_DS] = SEG_DATA | SEG_PRESENT | DS_WRITABLE;

	*(uint64_t *)&gdt[GDT_USER_CS]
		= SEG_CODE | SEG_PRESENT | SEG_DPL_3 | CS_LONG_MODE;

	uint64_t tss_phys
		= (uint64_t)&tss - limine_kernel_address_response->virtual_base
	    + limine_kernel_address_response->physical_base;
	*(__uint128_t *)&gdt[GDT_TSS]
		= SYS_SEG_TSS | SEG_PRESENT | SEG_DPL_3 | SYS_SEG_SPLIT_BASE(tss_phys)
	    | SYS_SEG_SPLIT_LIMIT(sizeof(tss));

	log("Loading GDT...");
	lgdt(sizeof(gdt) - 1, (uint64_t)gdt);
	log("Loading GDT: Success");

	log("Loading TSS...");
	tss.rsp0 = (uint64_t)isr_stack;
	ltr(GDT_TSS);
	log("Loading TSS: Success");

	log("Loading segment registers...");
	/* load %ds, %es, %fs, %gs, %ss */
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
	log("Loading segment registers: Success");
}

#define GATE_SPLIT_OFFSET(handler)   \
	(((__uint128_t)handler & 0xFFFF) \
		| (((__uint128_t)handler & 0xFFFF'FFFF'FFFF'0000) >> 16) << 48)
#define GATE_DESCR(descr) ((__uint128_t)descr << 16)
#define GATE_TYPE(type)   ((__uint128_t)type << 40)

volatile alignas(16) __uint128_t idt[256];

void idt_init(void) {
	log("Initializing IDT...");
	isr_init();
	log("Initializing IDT: Success");
	log("Loading IDT...");
	lidt(sizeof(idt) - 1, (uint64_t)idt);
	log("Loading IDT: Success");
}

void idt_register(uint8_t vector, void *handler, uint64_t type) {
	idt[vector]
		= GATE_SPLIT_OFFSET(handler) | GATE_DESCR(GDT_KERNEL_CS) | SEG_PRESENT
	    | SEG_DPL_0 | GATE_TYPE(type);
}
