.extern interrupt_stub
common:
	//TODO: load ss if cpl changes (is set to NULL otherwise)
	/* Note: if the order is modified, idt.h needs to be modified accordingly */
	/* stack: ss, rsp, rflags, cs, rip, error_code, vector */
	push %rax
	push %rbx
	push %rcx
	push %rdx
	push %rbp
	push %rsi
	push %rdi
	push %r8
	push %r9
	push %r10
	push %r11
	push %r12
	push %r13
	push %r14
	push %r15

	/* The C handler expects a pointer to the interrupt frame */
	mov %rsp, %rdi
	call interrupt_stub

	pop %r15
	pop %r14
	pop %r13
	pop %r12
	pop %r11
	pop %r10
	pop %r9
	pop %r8
	pop %rdi
	pop %rsi
	pop %rbp
	pop %rdx
	pop %rcx
	pop %rbx
	pop %rax

	/* Remove the error code and vector number from the stack */
	addq $16, %rsp
	iretq

#define true 1
#define false 0

.macro isr vector, dummy_error_code /* do we need to push an error code? */
	.global isr\vector
	isr\vector:
	.if \dummy_error_code
		pushq %rax
		movq $0, (%rsp)
	.endif
		pushq %rax
		movq $\vector, (%rsp)
		jmp common
.endm

isr vector=0, dummy_error_code=true
isr vector=1, dummy_error_code=true
isr vector=2, dummy_error_code=true
isr vector=3, dummy_error_code=true
isr vector=4, dummy_error_code=true
isr vector=5, dummy_error_code=true
isr vector=6, dummy_error_code=true
isr vector=7, dummy_error_code=true
isr vector=8, dummy_error_code=false
isr vector=9, dummy_error_code=true
isr vector=10, dummy_error_code=false
isr vector=11, dummy_error_code=false
isr vector=12, dummy_error_code=false
isr vector=13, dummy_error_code=false
isr vector=14, dummy_error_code=false
isr vector=15, dummy_error_code=true
isr vector=16, dummy_error_code=true
isr vector=17, dummy_error_code=false
isr vector=18, dummy_error_code=true
isr vector=19, dummy_error_code=true
isr vector=20, dummy_error_code=true
isr vector=21, dummy_error_code=false
isr vector=22, dummy_error_code=true
isr vector=23, dummy_error_code=true
isr vector=24, dummy_error_code=true
isr vector=25, dummy_error_code=true
isr vector=26, dummy_error_code=true
isr vector=27, dummy_error_code=true
isr vector=28, dummy_error_code=true
isr vector=29, dummy_error_code=false
isr vector=30, dummy_error_code=false
isr vector=31, dummy_error_code=true
isr vector=32, dummy_error_code=true
isr vector=33, dummy_error_code=true
isr vector=34, dummy_error_code=true
isr vector=35, dummy_error_code=true
isr vector=36, dummy_error_code=true
isr vector=37, dummy_error_code=true
isr vector=38, dummy_error_code=true
isr vector=39, dummy_error_code=true
isr vector=40, dummy_error_code=true
isr vector=41, dummy_error_code=true
isr vector=42, dummy_error_code=true
isr vector=43, dummy_error_code=true
isr vector=44, dummy_error_code=true
isr vector=45, dummy_error_code=true
isr vector=46, dummy_error_code=true
isr vector=47, dummy_error_code=true
isr vector=48, dummy_error_code=true
isr vector=49, dummy_error_code=true
isr vector=50, dummy_error_code=true
isr vector=51, dummy_error_code=true
isr vector=52, dummy_error_code=true
isr vector=53, dummy_error_code=true
isr vector=54, dummy_error_code=true
isr vector=55, dummy_error_code=true
isr vector=56, dummy_error_code=true
isr vector=57, dummy_error_code=true
isr vector=58, dummy_error_code=true
isr vector=59, dummy_error_code=true
isr vector=60, dummy_error_code=true
isr vector=61, dummy_error_code=true
isr vector=62, dummy_error_code=true
isr vector=63, dummy_error_code=true
isr vector=64, dummy_error_code=true
isr vector=65, dummy_error_code=true
isr vector=66, dummy_error_code=true
isr vector=67, dummy_error_code=true
isr vector=68, dummy_error_code=true
isr vector=69, dummy_error_code=true
isr vector=70, dummy_error_code=true
isr vector=71, dummy_error_code=true
isr vector=72, dummy_error_code=true
isr vector=73, dummy_error_code=true
isr vector=74, dummy_error_code=true
isr vector=75, dummy_error_code=true
isr vector=76, dummy_error_code=true
isr vector=77, dummy_error_code=true
isr vector=78, dummy_error_code=true
isr vector=79, dummy_error_code=true
isr vector=80, dummy_error_code=true
isr vector=81, dummy_error_code=true
isr vector=82, dummy_error_code=true
isr vector=83, dummy_error_code=true
isr vector=84, dummy_error_code=true
isr vector=85, dummy_error_code=true
isr vector=86, dummy_error_code=true
isr vector=87, dummy_error_code=true
isr vector=88, dummy_error_code=true
isr vector=89, dummy_error_code=true
isr vector=90, dummy_error_code=true
isr vector=91, dummy_error_code=true
isr vector=92, dummy_error_code=true
isr vector=93, dummy_error_code=true
isr vector=94, dummy_error_code=true
isr vector=95, dummy_error_code=true
isr vector=96, dummy_error_code=true
isr vector=97, dummy_error_code=true
isr vector=98, dummy_error_code=true
isr vector=99, dummy_error_code=true
isr vector=100, dummy_error_code=true
isr vector=101, dummy_error_code=true
isr vector=102, dummy_error_code=true
isr vector=103, dummy_error_code=true
isr vector=104, dummy_error_code=true
isr vector=105, dummy_error_code=true
isr vector=106, dummy_error_code=true
isr vector=107, dummy_error_code=true
isr vector=108, dummy_error_code=true
isr vector=109, dummy_error_code=true
isr vector=110, dummy_error_code=true
isr vector=111, dummy_error_code=true
isr vector=112, dummy_error_code=true
isr vector=113, dummy_error_code=true
isr vector=114, dummy_error_code=true
isr vector=115, dummy_error_code=true
isr vector=116, dummy_error_code=true
isr vector=117, dummy_error_code=true
isr vector=118, dummy_error_code=true
isr vector=119, dummy_error_code=true
isr vector=120, dummy_error_code=true
isr vector=121, dummy_error_code=true
isr vector=122, dummy_error_code=true
isr vector=123, dummy_error_code=true
isr vector=124, dummy_error_code=true
isr vector=125, dummy_error_code=true
isr vector=126, dummy_error_code=true
isr vector=127, dummy_error_code=true
isr vector=128, dummy_error_code=true
isr vector=129, dummy_error_code=true
isr vector=130, dummy_error_code=true
isr vector=131, dummy_error_code=true
isr vector=132, dummy_error_code=true
isr vector=133, dummy_error_code=true
isr vector=134, dummy_error_code=true
isr vector=135, dummy_error_code=true
isr vector=136, dummy_error_code=true
isr vector=137, dummy_error_code=true
isr vector=138, dummy_error_code=true
isr vector=139, dummy_error_code=true
isr vector=140, dummy_error_code=true
isr vector=141, dummy_error_code=true
isr vector=142, dummy_error_code=true
isr vector=143, dummy_error_code=true
isr vector=144, dummy_error_code=true
isr vector=145, dummy_error_code=true
isr vector=146, dummy_error_code=true
isr vector=147, dummy_error_code=true
isr vector=148, dummy_error_code=true
isr vector=149, dummy_error_code=true
isr vector=150, dummy_error_code=true
isr vector=151, dummy_error_code=true
isr vector=152, dummy_error_code=true
isr vector=153, dummy_error_code=true
isr vector=154, dummy_error_code=true
isr vector=155, dummy_error_code=true
isr vector=156, dummy_error_code=true
isr vector=157, dummy_error_code=true
isr vector=158, dummy_error_code=true
isr vector=159, dummy_error_code=true
isr vector=160, dummy_error_code=true
isr vector=161, dummy_error_code=true
isr vector=162, dummy_error_code=true
isr vector=163, dummy_error_code=true
isr vector=164, dummy_error_code=true
isr vector=165, dummy_error_code=true
isr vector=166, dummy_error_code=true
isr vector=167, dummy_error_code=true
isr vector=168, dummy_error_code=true
isr vector=169, dummy_error_code=true
isr vector=170, dummy_error_code=true
isr vector=171, dummy_error_code=true
isr vector=172, dummy_error_code=true
isr vector=173, dummy_error_code=true
isr vector=174, dummy_error_code=true
isr vector=175, dummy_error_code=true
isr vector=176, dummy_error_code=true
isr vector=177, dummy_error_code=true
isr vector=178, dummy_error_code=true
isr vector=179, dummy_error_code=true
isr vector=180, dummy_error_code=true
isr vector=181, dummy_error_code=true
isr vector=182, dummy_error_code=true
isr vector=183, dummy_error_code=true
isr vector=184, dummy_error_code=true
isr vector=185, dummy_error_code=true
isr vector=186, dummy_error_code=true
isr vector=187, dummy_error_code=true
isr vector=188, dummy_error_code=true
isr vector=189, dummy_error_code=true
isr vector=190, dummy_error_code=true
isr vector=191, dummy_error_code=true
isr vector=192, dummy_error_code=true
isr vector=193, dummy_error_code=true
isr vector=194, dummy_error_code=true
isr vector=195, dummy_error_code=true
isr vector=196, dummy_error_code=true
isr vector=197, dummy_error_code=true
isr vector=198, dummy_error_code=true
isr vector=199, dummy_error_code=true
isr vector=200, dummy_error_code=true
isr vector=201, dummy_error_code=true
isr vector=202, dummy_error_code=true
isr vector=203, dummy_error_code=true
isr vector=204, dummy_error_code=true
isr vector=205, dummy_error_code=true
isr vector=206, dummy_error_code=true
isr vector=207, dummy_error_code=true
isr vector=208, dummy_error_code=true
isr vector=209, dummy_error_code=true
isr vector=210, dummy_error_code=true
isr vector=211, dummy_error_code=true
isr vector=212, dummy_error_code=true
isr vector=213, dummy_error_code=true
isr vector=214, dummy_error_code=true
isr vector=215, dummy_error_code=true
isr vector=216, dummy_error_code=true
isr vector=217, dummy_error_code=true
isr vector=218, dummy_error_code=true
isr vector=219, dummy_error_code=true
isr vector=220, dummy_error_code=true
isr vector=221, dummy_error_code=true
isr vector=222, dummy_error_code=true
isr vector=223, dummy_error_code=true
isr vector=224, dummy_error_code=true
isr vector=225, dummy_error_code=true
isr vector=226, dummy_error_code=true
isr vector=227, dummy_error_code=true
isr vector=228, dummy_error_code=true
isr vector=229, dummy_error_code=true
isr vector=230, dummy_error_code=true
isr vector=231, dummy_error_code=true
isr vector=232, dummy_error_code=true
isr vector=233, dummy_error_code=true
isr vector=234, dummy_error_code=true
isr vector=235, dummy_error_code=true
isr vector=236, dummy_error_code=true
isr vector=237, dummy_error_code=true
isr vector=238, dummy_error_code=true
isr vector=239, dummy_error_code=true
isr vector=240, dummy_error_code=true
isr vector=241, dummy_error_code=true
isr vector=242, dummy_error_code=true
isr vector=243, dummy_error_code=true
isr vector=244, dummy_error_code=true
isr vector=245, dummy_error_code=true
isr vector=246, dummy_error_code=true
isr vector=247, dummy_error_code=true
isr vector=248, dummy_error_code=true
isr vector=249, dummy_error_code=true
isr vector=250, dummy_error_code=true
isr vector=251, dummy_error_code=true
isr vector=252, dummy_error_code=true
isr vector=253, dummy_error_code=true
isr vector=254, dummy_error_code=true
isr vector=255, dummy_error_code=true
