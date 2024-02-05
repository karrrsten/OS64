#include "proc.h"

#include "malloc.h"

#include "cpu/apic.h"
#include "cpu/apic_timer.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/mem.h"
#include "cpu/page.h"
#include "cpu/x86.h"
#include "kernel/vmem.h"
#include "util/list.h"
#include "util/panic.h"
#include "util/print.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define KSTACK_SIZE (4 * 4096)

static struct list_head proc_list = LIST_HEAD_INIT(proc_list);
static struct list_head thread_list = LIST_HEAD_INIT(thread_list);

static struct proc *current_proc;
static struct thread *current_thread;

static struct proc *kproc;

static uint64_t last_thread_id = 0;

static struct thread *next_thread(struct thread *current) {
	return current ? list_next_circular(current, thread_list, &thread_list)
	               : list_entry(thread_list.next, struct thread, thread_list);
}

static void switch_context(struct interrupt_frame *frame) {
	kprint("Switch");
	if (current_thread && frame) {
		*(current_thread->regs) = *frame;
	}

	current_thread = next_thread(current_thread);
	current_proc = current_thread->proc;

	set_pml4(current_thread->proc->pml4);
	*frame = *(current_thread->regs);
}

/**
 * @brief Initialize the process tree.
 */
void proc_init(void) {
	kproc = malloc(sizeof(struct proc));

	kproc->id = 0;
	kproc->parent = nullptr;
	init_list_head(&kproc->threads);
	kproc->pml4 = pg_pml4;

	list_add(&kproc->proc_list, &proc_list);

	current_proc = kproc;
}

/**
 * @brief Start the scheduler and execute created threads. The startup thread
 * will cease to exist shortly after.
 */
void sched_start(void) {
	apic_set_timer(100000, switch_context, APIC_TIMER_PERIODIC);
}

static int proc_pause_count = 0;

/**
 * @brief Pause scheduling of threads and prevent context switches. Can be
 * called multiple times, @ref sched_resume needs to be called once for every
 * call afterwards.
 */
void sched_pause(void) {
	if (!proc_pause_count) {
		apic_mask_timer();
	}
	proc_pause_count++;
}

/**
 * @brief Resume scheduling of threads and reenable context switches. Will only
 * actually resume if this is the outer most call.
 */
void sched_resume(void) {
	if (!--proc_pause_count) {
		apic_unmask_timer();
	}
}

/**
 * @brief Create a new thread.
 * @param p The Process for which to create the thread.
 * @param func The function to be executed by the thread. Must not return.
 * @param data This pointer is passed to the executed function.
 */
void thread_new(struct proc *p, void *func, void *data) {
	sched_pause();

	struct thread *t = malloc(sizeof(struct thread));

	t->id = ++last_thread_id;
	t->proc = p;

	t->kernel_stack = kmap(alloc_pages(KSTACK_SIZE), nullptr, KSTACK_SIZE,
		PAGE_PRESENT | PAGE_WRITE);
	uint64_t stack_top = (uint64_t)t->kernel_stack + KSTACK_SIZE - 16;

	t->regs = malloc(sizeof(struct interrupt_frame));
	*(t->regs) = (struct interrupt_frame) {.cs = GDT_KERNEL_CS | GDT_RPL_0,
		.ss = GDT_KERNEL_DS | GDT_RPL_0,
		.rsp = (uint64_t)stack_top,
		.rbp = (uint64_t)stack_top,
		.rflags = RFLAGS_IF,
		.rip = (uint64_t)func,
		.rdi = (uint64_t)data};

	init_list_head(&p->threads);
	list_add(&t->siblings, &p->threads);

	list_add(&t->thread_list, &thread_list);

	sched_resume();
}

/**
 * @brief Create a new process with a single thread.
 * @param func The function to be executed by the thread. Must not return.
 * @param data This pointer is passed to the executed function.
 */
void proc_new(void *func, void *data) {
	sched_pause();

	struct proc *p = malloc(sizeof(struct proc));

	p->id = ++last_thread_id;
	p->parent = current_proc;
	p->pml4 = alloc_pml4();

	thread_new(p, func, data);

	list_add(&p->proc_list, &proc_list);

	sched_resume();
}

struct kthread_wrapper_data {
	void *func;
	void *data;
};

[[noreturn]] static void kthread_wrapper(struct kthread_wrapper_data *d) {
	void *func = d->func;
	void *data = d->data;
	free(d);
	((void (*)(void *))func)(data);
	for (;;)
		;
}

/**
 * @brief Create a new kthread.
 * @param func The function to be executed by the thread. Can return.
 * @param data This pointer is passed to the executed function.
 */
void kthread_new(void *func, void *data) {
	struct kthread_wrapper_data *d
		= malloc(sizeof(struct kthread_wrapper_data));
	d->func = func;
	d->data = data;
	thread_new(kproc, kthread_wrapper, d);
}
