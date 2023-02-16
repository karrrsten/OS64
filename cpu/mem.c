#include "mem.h"

#include "kernel/limine_reqs.h"
#include "util/log.h"
#include "util/print.h"
#include "util/string.h"

#include <limine.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

extern void _kernel_end; /* end of the kernel, defined in linker.ld */
void *kernel_end;

size_t mem_max;

uint8_t *memmap; /* map of all of physical memory */
uint8_t *memmap_phys; /* Physical address of memmap */
size_t memmap_size;

void mem_init(void) {
	log("Initializing physical memory allocator...");
	// TODO: this could overrun into used memory
	memmap = &_kernel_end;
	struct limine_memmap_entry **memmap_entries
		= limine_memmap_response->entries;

	/* find the highest useable physical address to save space in memmap */
	for (size_t i = 0; i < limine_memmap_response->entry_count; ++i) {
		if (memmap_entries[i]->type != LIMINE_MEMMAP_RESERVED) {
			mem_max = memmap_entries[i]->base + memmap_entries[i]->length;
		}
	}
	log("Total amount of memory available: %llu =  0x%llX", mem_max, mem_max);
	memmap_size = mem_max / 4096 / 8;
	kernel_end = (void *)memmap + memmap_size;

	memmap_phys
		= memmap - limine_kernel_address_response->virtual_base
	    + limine_kernel_address_response->physical_base;

	/* mark all pages as used */
	memset(memmap_phys, INT_MAX, memmap_size);

	/* mark available pages as unused */
	for (size_t i = 0; i < limine_memmap_response->entry_count; ++i) {
		if (memmap_entries[i]->type == LIMINE_MEMMAP_USABLE) {
			for (uint64_t page = memmap_entries[i]->base;
				 page < memmap_entries[i]->base + memmap_entries[i]->length;
				 page += 4096) {
				size_t index = page / 4096 / 8;
				int bit = page / 4096 % 8;
				memmap_phys[index] &= ~(1 << bit);
			}
		}
	}

	/* mark memmap itself as used */
	for (uint64_t page
		 = ((uint64_t)memmap_phys % 4096) != 0
	         ? (uint64_t)memmap_phys - (uint64_t)memmap_phys % 4096
	         : (uint64_t)memmap_phys;
		 page < (uint64_t)memmap_phys + memmap_size; page += 4096) {
		size_t index = page / 4096 / 8;
		int bit = page / 4096 % 8;
		memmap_phys[index] |= 1 << bit;
	}
	log("Initializing physical memory allocator: Success");
}

void *early_alloc_page(void) {
	for (size_t index = 0; index < mem_max; ++index) {
		if (~memmap_phys[index]) {
			for (int bit = 0; bit < 1; ++bit) {
				if (~memmap_phys[index] & 1 << bit) {
					void *page = (void *)(index * 4096 * 8 + bit * 4096);
					size_t index = (size_t)page / 4096 / 8;
					int bit = (size_t)page / 4096 % 8;
					memmap_phys[index] |= 1 << bit;
					return page;
				}
			}
		}
	}

	return nullptr;
}

void *alloc_page(void) {
	for (size_t index = 0; index < mem_max; ++index) {
		if (~memmap[index]) {
			for (int bit = 0; bit < 1; ++bit) {
				if (~memmap[index] & 1 << bit) {
					void *page = (void *)(index * 4096 * 8 + bit * 4096);
					mark_page_used(page);
					return page;
				}
			}
		}
	}

	return nullptr;
}

// TODO
void *alloc_pages(size_t size);

void mark_page_used(void *page) {
	size_t index = (size_t)page / 4096 / 8;
	int bit = (size_t)page / 4096 % 8;
	memmap[index] |= 1 << bit;
}

// TODO
void mark_pages_used(void *pages);

void free_page(void *page) {
	size_t index = (size_t)page / 4096 / 8;
	int bit = (size_t)page / 4096 % 8;
	memmap[index] &= ~(1 << bit);
}

// TODO
void free_pages(void *pages, size_t size);
