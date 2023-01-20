#pragma once

#include <stddef.h>

void mem_init(void);

void *alloc_page(void);
void *alloc_pages(size_t size);

void free_page(void *page);
void free_pages(void *pages, size_t size);