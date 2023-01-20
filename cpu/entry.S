#define STACK_SIZE (0x100000)

.section .bss
.align 16
.lcomm stack, STACK_SIZE

/******************************************************************************/

.section .text
.extern kmain
.extern
.global _start
_start:
	movq $(stack + STACK_SIZE), %rbp
	movq %rbp, %rsp
	jmp kmain