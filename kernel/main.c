#include "malloc.h"

#include "cpu/descr_tbl.h"
#include "cpu/mem.h"
#include "cpu/page.h"
#include "util/log.h"
#include "util/print.h"

#define HEAP_SIZE (16384)

[[noreturn]] void kmain(void) {
	kprint("\x1B[2J"); /* clear the serial console */
	log("Initializing kernel...");
	gdt_init();
	idt_init();
	mem_init();
	pg_init();
	heap_init(kernel_end, HEAP_SIZE);

	log("Initializing kernel: Success");
	for (;;)
		;
}
