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
	wcr0(CR0_PG | CR0_WP | CR0_NE | CR0_ET | CR0_PE);
	wcr4(CR4_PGE | CR4_PAE);
	wrmsr(MSR_IA32_EFER,
		IA32_EFER_NXE | IA32_EFER_LMA | IA32_EFER_LME | IA32_EFER_SCE);

	serial_init();
	kprintf("Initializing kernel...");

	gdt_init();
	idt_init();
	mem_init();
	pg_init();
	heap_init(kernel_end, 0x4000);
	vmem_init();
	apic_init();

	pci_init();
	nvme_init(pci_get_dev(1, 8, 2));
	sti();

	kprintf("Initializing kernel: Success");
	for (;;)
		;
}
