#include "descr_tbl.h"

#include "cpu/x86.h"
#include <stdalign.h>
#include <stdint.h>

struct [[gnu::packed]] null_segment {
	uint64_t : 64;
};

struct [[gnu::packed]] code_segment {
	uint64_t                 : 42;
	uint64_t conforming      : 1;
	uint64_t fill            : 2; /* must be 0b11 */
	uint64_t dpl             : 2;
	uint64_t present         : 1;
	uint64_t                 : 5;
	uint64_t long_mode       : 1;
	uint64_t default_op_size : 1;
	uint64_t                 : 9;
};

struct [[gnu::packed]] data_segment {
	uint64_t          : 41;
	uint64_t writable : 1; /* required for loading %ss, #GP otherwise*/
	uint64_t          : 2;
	uint64_t fill     : 1; /* must be 0b1 */
	uint64_t          : 2;
	uint64_t present  : 1;
	uint64_t          : 16;
};

struct [[gnu::packed]] system_segment {
	uint64_t limit_low   : 16;
	uint64_t base_low    : 24;
	uint64_t type        : 4;
	uint64_t             : 1;
	uint64_t dpl         : 2;
	uint64_t present     : 1;
	uint64_t limit_high  : 4;
	uint64_t avl         : 1;
	uint64_t             : 2;
	uint64_t granularity : 1;
	uint64_t base_mid    : 8;

	uint64_t base_high : 32;

	uint64_t : 32;
};

struct [[gnu::packed]] gate {
	uint64_t offset_low : 16;
	uint64_t selector   : 16;

	uint64_t ist        : 3;
	uint64_t            : 5;
	uint64_t type       : 4;
	uint64_t            : 1;
	uint64_t dpl        : 2;
	uint64_t present    : 1;
	uint64_t offset_mid : 16;

	uint64_t offset_high : 32;

	uint64_t : 32;
};

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
	*(struct null_segment *)&gdt[0] = (struct null_segment) {};

	*(struct code_segment *)&gdt[KERNEL_CS] = (struct code_segment) {
		.conforming = 0,
		.fill = 0b11,
		.dpl = 0,
		.present = 1,
		.long_mode = 1,
		.default_op_size = 0};

	*(struct data_segment *)&gdt[ANY_DS]
		= (struct data_segment) {.fill = 0b1, .writable = 1, .present = 1};

	*(struct code_segment *)&gdt[USER_CS] = (struct code_segment) {
		.conforming = 0,
		.fill = 0b11,
		.dpl = 3,
		.present = 1,
		.long_mode = 1,
		.default_op_size = 0};

	*(struct system_segment *)&gdt[GDT_TSS] = (struct system_segment) {
		.limit_low = sizeof(tss) & 0xFFFF,
		.base_low = (uint64_t)&tss & 0xFFFF,
		.base_mid = ((uint64_t)&tss >> 24) & 0xFF,
		.type = DESCR_TYPE_LDT,
		.dpl = 0,
		.present = 1,
		.limit_high = (sizeof(tss) >> 16) & 0xFFFF,
		.avl = 1,
		.granularity = 0,
		.base_high = ((uint64_t)&tss >> 32) & 0xFFFFFFFF};

	lgdt(sizeof(gdt) - 1, (uint64_t)gdt);

	tss.rsp0 = (uint64_t)isr_stack;
	/* ltr(GDT_TSS); */

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
}

volatile struct gate alignas(8) idt[256];

void idt_init(void) {
	lidt(sizeof(idt) - 1, (uint64_t)idt);
}

void idt_register(uint8_t vector, void *handler, enum descriptor_type type) {
	idt[vector].offset_low = (uint32_t)((uint64_t)handler & 0xFFFF);
	idt[vector].selector = KERNEL_CS;
	idt[vector].ist = 0;
	idt[vector].type = type;
	idt[vector].dpl = 0;
	idt[vector].present = 1;
	idt[vector].offset_mid = (uint16_t)((uint64_t)handler >> 16 & 0xFFFF);
	idt[vector].offset_high = (uint32_t)((uint64_t)handler >> 32 & 0xFFFFFFFF);
}
