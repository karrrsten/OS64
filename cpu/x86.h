#pragma once

#include <stdint.h>

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
	uint8_t result;
	asm volatile(
		"inb %%dx, %%al"
		: "=a"(result)
		: "d"(port));
	return result;
}

static inline uint16_t inw(uint16_t port) {
	uint16_t result;
	asm volatile(
		"inw %%dx, %%ax"
		: "=a"(result)
		: "d"(port));
	return result;
}

static inline uint32_t inl(uint16_t port) {
	uint32_t result;
	asm volatile(
		"inl %%dx, %%eax"
		: "=a"(result)
		: "d"(port));
	return result;
}

static inline void outb(uint16_t port, uint8_t data) {
	asm volatile(
		"outb %%al, %%dx"
		:
		: "a"(data), "d"(port));
}

static inline void outw(uint16_t port, uint16_t data) {
	asm volatile(
		"outw %%ax, %%dx"
		:
		: "a"(data), "d"(port));
}

static inline void outl(uint16_t port, uint32_t data) {
	asm volatile(
		"outl %%eax, %%dx"
		:
		: "a"(data), "d"(port));
}

static inline void __wrmsr(uint32_t msr, uint32_t low, uint32_t high) {
	asm volatile(
		"wrmsr"
		:
		: "c"(msr), "a"(low), "d"(high));
}

static inline void wrmsr(uint32_t msr, uint64_t val) {
	uint32_t low = (uint32_t)(val & 0xFFFF'FFFF);
	uint32_t high = (uint32_t)(val >> 32);
	__wrmsr(msr, low, high);
}

static inline void __rdmsr(uint32_t msr, uint32_t *low, uint32_t *high) {
	asm volatile(
		"rdmsr"
		: "=a"(*low), "=d"(*high)
		: "c"(msr));
}

static inline uint64_t rdmsr(uint32_t msr) {
	uint32_t low, high;
	__rdmsr(msr, &low, &high);
	return ((uint64_t)high << 32) | low;
}

static inline uint64_t rcr2() {
	uint64_t cr2;
	asm volatile("movq %%cr2, %0"
				 : "=r"(cr2));
	return cr2;
}

static inline void wcr3(uint64_t value) {
	asm volatile(
		"movq %0, %%cr3"
		:
		: "r"(value));
}

static inline void invlpg(volatile void *addr) {
	asm volatile(
		"invlpg (%0)"
		:
		: "r"((uint64_t)addr));
}

static inline void ltr(uint16_t selector) {
	asm volatile(
		"ltr %0"
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

	asm volatile(
		"lgdt (%0)"
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

	asm volatile(
		"lidt (%0)"
		:
		: "r"(&idt_descr));
}
