#pragma once

#include <stddef.h>

/* End of the kernel in virtual memory
 * Do not modify, cannot be const because has to be modfied by some internal
 * code*/
extern void *kernel_end;
extern size_t mem_max; // Do not modify, cannot be const because has to be
                       // modfied by some internal code

void mem_init(void);

/* To be used while paging is still in the state the bootloader left it in */
// TODO: replace with bool init???
void *early_alloc_page(void);

void *alloc_page(void);
void *alloc_pages(size_t size);

void mark_page_used(void *page);
void mark_pages_used(void *pages);

void free_page(void *page);
void free_pages(void *pages, size_t size);
