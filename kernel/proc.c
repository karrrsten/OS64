#include "proc.h"

#include "malloc.h"

#include "cpu/apic_timer.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/mem.h"
#include "cpu/page.h"
#include "cpu/x86.h"
#include "util/list.h"
#include "util/print.h"

#include <stdint.h>

#define KSTACK_SIZE (4 * 4'096)
#define THREAD_TIME (1'000'000)

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
	if (current_thread) {
		*(current_thread->regs) = *frame;
	}

	current_thread = next_thread(current_thread);
	current_proc = current_thread->proc;

	set_pml4(current_thread->proc->pml4);
	*frame = *(current_thread->regs);

	apic_set_timer(THREAD_TIME, switch_context, APIC_TIMER_ONE_SHOT);
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
	apic_set_timer(THREAD_TIME, switch_context, APIC_TIMER_ONE_SHOT);
}

/**
 * @brief Create a new thread.
 * @param p The Process for which to create the thread.
 * @param func The function to be executed by the thread. Must not return.
 * @param data This pointer is passed to the executed function.
 */
void thread_new(struct proc *p, void *func, void *data) {
	irq_disable();

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

	irq_enable();
}

/**
 * @brief Create a new process with a single thread.
 * @param func The function to be executed by the thread. Must not return.
 * @param data This pointer is passed to the executed function.
 */
void proc_new(void *func, void *data) {
	irq_disable();

	struct proc *p = malloc(sizeof(struct proc));

	p->id = ++last_thread_id;
	p->parent = current_proc;
	p->pml4 = alloc_pml4();

	thread_new(p, func, data);

	list_add(&p->proc_list, &proc_list);

	irq_enable();
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
	for (;;);
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
