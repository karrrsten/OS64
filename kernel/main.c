#include "malloc.h"

#include "cpu/apic.h"
#include "cpu/apic_timer.h"
#include "cpu/gdt.h"
#include "cpu/mem.h"
#include "cpu/page.h"
#include "kernel/vmem.h"
#include "util/log.h"
#include "util/print.h"

#define HEAP_SIZE (16384)

/**
 * @brief The main kernel function.
 */
[[noreturn]] void kmain(void) {
	kprint("\x1B[2J"); /* clear the serial console */
	log("Initializing kernel...");
	gdt_init();
	idt_init();
	mem_init();
	pg_init();
	heap_init(kernel_end, HEAP_SIZE);
	vmem_init();
	apic_init();

	log("Initializing kernel: Success");
	for (;;)
		;
}
