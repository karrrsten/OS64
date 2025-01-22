#include "malloc.h"
#include "proc.h"
#include "vmem.h"

#include "cpu/apic.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/mem.h"
#include "cpu/page.h"
#include "cpu/x86.h"
#include "drivers/nvme.h"
#include "drivers/pci.h"
#include "kernel/limine_reqs.h"
#include "util/print.h"

#include <cpuid.h>
#include <limine.h>

/**
 * @brief The main kernel function.
 */
[[noreturn]] void kmain(void) {
	serial_init();
	kprintf("Initializing kernel...");

	gdt_init();
	idt_init();
	mem_init();
	pg_init();
	heap_init(kernel_end + (~(uint64_t)kernel_end & 0xFFF) + 1, 0x4000);
	vmem_init();
	apic_init();

	pci_init();
	nvme_init(pci_get_dev(1, 8, 2));
	sti();

	kprintf("Initializing kernel: Success");
	for (;;)
		;
}
