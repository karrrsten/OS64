#pragma once

#include <stddef.h>

void vmem_init(void);
void *vmem_alloc(size_t size);
void vmem_free(void *addr);
