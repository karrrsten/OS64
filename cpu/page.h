#pragma once

void pg_init(void);

void *mmap(void *phys);
void *mmap_virt(void *phys, void *virt);

void mfree(void *virt);
