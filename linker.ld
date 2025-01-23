ENTRY(_start)

SECTIONS
{
	/* Load the kernel at the base of the higher half (as per limine protocol) */
	. = 0xFFFFFFFF80000000;

	.text ALIGN(4K) :
	{
		*(.text .text.*)
	}

	.rodata ALIGN(4K) :
	{
		*(.rodata .rodata.*)
	}

	.data ALIGN(4K) :
	{
		*(.data .data.*)
	}

	.bss ALIGN(4K) :
	{
		*(.bss .bss.* .lbss)
		*(COMMON)
	}

	_kernel_end = ALIGN(4K);
}
