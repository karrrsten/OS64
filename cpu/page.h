#pragma once

#include "kernel/limine_reqs.h"

#include <limine.h>
#include <stddef.h>
#include <stdint.h>

#define KERNEL_BASE      (limine_kernel_address_response->virtual_base)
#define HIGHER_HALF_BASE (limine_hhdm_response->offset)
/* All of physical memory is mapped at the higher half base */
#define P2V(addr) ((typeof(addr))((uint64_t)addr + HIGHER_HALF_BASE))

#define PAGE_PRESENT (1 << 0)
#define PAGE_WRITE   (1 << 1)
#define PAGE_USER    (1 << 2)
#define PAGE_PWT     (1 << 3)
#define PAGE_PCD     (1 << 4)
#define PAGE_SIZE    (1 << 7)
#define PAGE_GLOBAL  (1 << 8)
#define PAGE_NX      (1 << 63)

extern volatile uint64_t *pg_pml4;

void pg_init(void);

void *get_physical_address(const void *virt_addr);
void set_pml4(volatile uint64_t *pml4);

void *kmap(void *phys_addr, void *virt_addr, size_t size, uint64_t flags);

void kunmap(void *virt_addr, size_t size);

volatile uint64_t *alloc_pml4(void);
void free_pml4(volatile uint64_t *pml4);
