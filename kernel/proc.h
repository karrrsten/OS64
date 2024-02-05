#pragma once

#include "cpu/idt.h"
#include "cpu/x86.h"
#include "util/list.h"

#include <stddef.h>
#include <stdint.h>

struct proc;

struct thread {
	struct list_head thread_list;
	uint64_t id;
	void *kernel_stack;
	struct proc *proc;
	struct interrupt_frame *regs;
	struct list_head siblings;
};

struct proc {
	struct list_head proc_list;
	uint64_t id;
	struct proc *parent;
	volatile uint64_t *pml4;
	struct list_head threads;
};

void kthread_new(void *func, void *data);
uint64_t get_current_thread_id(void);

void proc_init(void);
void sched_start(void);
void sched_pause(void);
void sched_resume(void);

void proc_new(void *func, void *data);

void thread_new(struct proc *p, void *func, void *data);
