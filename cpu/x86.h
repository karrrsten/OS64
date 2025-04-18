#pragma once

#include "cpu/idt.h"

#include <stdint.h>

struct [[gnu::packed]] regs {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t rip;
	uint64_t cs;
	uint64_t rsp;
};

#define RFLAGS_IF (1 << 9)

static inline void cli() {
	asm volatile("cli");
}

static inline void sti() {
	asm volatile("sti");
}

[[gnu::no_caller_saved_registers]] static inline void hlt() {
	asm volatile("hlt");
}

static inline uint8_t inb(uint16_t port) {
	irq_disable();
	uint8_t result;
	asm volatile("inb %%dx, %%al"
		: "=a"(result)
		: "d"(port));
	irq_enable();
	return result;
}

static inline uint16_t inw(uint16_t port) {
	irq_disable();
	uint16_t result;
	asm volatile("inw %%dx, %%ax"
		: "=a"(result)
		: "d"(port));
	irq_enable();
	return result;
}

static inline uint32_t inl(uint16_t port) {
	irq_disable();
	uint32_t result;
	asm volatile("inl %%dx, %%eax"
		: "=a"(result)
		: "d"(port));
	irq_enable();
	return result;
}

static inline void outb(uint16_t port, uint8_t data) {
	irq_disable();
	asm volatile("outb %%al, %%dx"
		:
		: "a"(data), "d"(port));
	irq_enable();
}

static inline void outw(uint16_t port, uint16_t data) {
	irq_disable();
	asm volatile("outw %%ax, %%dx"
		:
		: "a"(data), "d"(port));
	irq_enable();
}

static inline void outl(uint16_t port, uint32_t data) {
	irq_disable();
	asm volatile("outl %%eax, %%dx"
		:
		: "a"(data), "d"(port));
	irq_enable();
}

static inline void __wrmsr(uint32_t msr, uint32_t low, uint32_t high) {
	asm volatile("wrmsr"
		:
		: "c"(msr), "a"(low), "d"(high));
}

#define MSR_IA32_EFER (0xC000'0080)
#define IA32_EFER_NXE (1LL << 11)
#define IA32_EFER_LMA (1LL << 10)
#define IA32_EFER_LME (1LL << 8)
#define IA32_EFER_SCE (1LL << 0)

static inline void wrmsr(uint32_t msr, uint64_t val) {
	uint32_t low = (uint32_t)(val & 0xFFFF'FFFF);
	uint32_t high = (uint32_t)(val >> 32);
	__wrmsr(msr, low, high);
}

static inline void __rdmsr(uint32_t msr, uint32_t *low, uint32_t *high) {
	asm volatile("rdmsr"
		: "=a"(*low), "=d"(*high)
		: "c"(msr));
}

static inline uint64_t rdmsr(uint32_t msr) {
	uint32_t low, high;
	__rdmsr(msr, &low, &high);
	return ((uint64_t)high << 32) | low;
}

#define CR0_PG (1LL << 31)
#define CR0_WP (1LL << 16)
#define CR0_NE (1LL << 5)
#define CR0_ET (1LL << 4)
#define CR0_PE (1LL << 0)

static inline void wcr0(uint64_t value) {
	asm volatile("movq %0, %%cr0"
		:
		: "r"(value));
}

static inline uint64_t rcr0(void) {
	uint64_t cr0;
	asm volatile("movq %%cr0, %0"
		: "=r"(cr0));
	return cr0;
}

static inline uint64_t rcr2(void) {
	uint64_t cr2;
	asm volatile("movq %%cr2, %0"
		: "=r"(cr2));
	return cr2;
}

#define CR4_PGE (1LL << 7)
#define CR4_PAE (1LL << 5)

static inline void wcr3(uint64_t value) {
	asm volatile("movq %0, %%cr3"
		:
		: "r"(value));
}

static inline uint64_t rcr3(void) {
	uint64_t cr3;
	asm volatile("movq %%cr3, %0"
		: "=r"(cr3));
	return cr3;
}

static inline void wcr4(uint64_t value) {
	asm volatile("movq %0, %%cr4"
		:
		: "r"(value));
}

static inline uint64_t rcr4(void) {
	uint64_t cr4;
	asm volatile("movq %%cr4, %0"
		: "=r"(cr4));
	return cr4;
}

static inline void invlpg(volatile void *addr) {
	asm volatile("invlpg (%0)"
		:
		: "r"((uint64_t)addr));
}

static inline void ltr(uint16_t selector) {
	asm volatile("ltr %0"
		:
		: "r"(selector));
}

static inline void lgdt(uint16_t size, uint64_t offset) {
	struct [[gnu::packed]] gdt_descriptor {
		uint16_t size;
		uint64_t offset;
	};

	struct gdt_descriptor gdt_descr;
	gdt_descr.size = size;
	gdt_descr.offset = offset;

	asm volatile("lgdt (%0)"
		:
		: "r"(&gdt_descr));
}

static inline void lidt(uint16_t size, uint64_t offset) {
	struct [[gnu::packed]] idt_descriptor {
		uint16_t size;
		uint64_t offset;
	};

	struct idt_descriptor idt_descr;
	idt_descr.size = size;
	idt_descr.offset = offset;

	asm volatile("lidt (%0)"
		:
		: "r"(&idt_descr));
}
