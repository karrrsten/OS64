#pragma once

#include <stddef.h>

void heap_init(void *heap_start, size_t heap_size);
void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);
