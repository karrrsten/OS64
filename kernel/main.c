#include "cpu/descr_tbl.h"
#include "cpu/isr.h"
#include "cpu/mem.h"
#include "cpu/page.h"
#include "serial.h"

[[noreturn]] void kmain() {
	gdt_init();
	idt_init();
	isr_init();
	// pg_init(); // Currently not working
	// mem_init();

	serial_write("hello world");
	for (;;)
		;
}
