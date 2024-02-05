#pragma once

#include <stddef.h>

/**
 * @var kernel_end End of the kernel in virtual memory
 */
extern void *kernel_end;

/**
 * @var mem_max The size of physical memory
 */
extern size_t mem_max;

void mem_init(void);

void *early_alloc_page(void);

void *alloc_page(void);
void *alloc_pages(size_t size);

void mark_page_used(const void *page);
void mark_pages_used(const void *pages, size_t size);

void free_page(void *page);
void free_pages(void *pages, size_t size);
