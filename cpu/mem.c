#include "mem.h"

#include "kernel/limine_reqs.h"
#include "util/string.h"
#include <limine.h>
#include <stddef.h>

extern char _kernel_end; /* end of the kernel, defined in linker.ld*/

char *memmap; /* map of all of physical memory */
size_t memmap_size;

void mem_init(void) {
	memmap = &_kernel_end;
	struct limine_memmap_entry **memmap_entries
		= limine_memmap_response->entries;

	/* find the highest physical address to save space in memmap */
	for (size_t i = 0; i < limine_memmap_response->entry_count; ++i) {
		if (memmap_entries[i]->type != LIMINE_MEMMAP_RESERVED) {
			memmap_size = memmap_entries[i]->base + memmap_entries[i]->length;
		}
	}

	/* mark all pages as used */
	memset_volatile(memmap, 0xFF, memmap_size);

	// TODO: bootloader reclaimable can probably reclaimed too, smp is not needed
	/* mark available pages as unused */
	for (size_t i = 0; i < limine_memmap_response->entry_count; ++i) {
		if (memmap_entries[i] == LIMINE_MEMMAP_USABLE) {
			for (size_t page = memmap_entries[i]->base;
				 page < memmap_entries[i]->base + memmap_entries[i]->length;
				 page += 4096) {
				size_t index = page / 4096 / 8;
				int bit = page / 4096 % 8;
				memmap[index] ^= 1 << bit;
			}
		}
	}

	/* mark memmap itself as used */
	for (size_t page = (size_t)memmap; page < (size_t)memmap + memmap_size;
		 page += 4096) {
		size_t index = page / 4096 / 8;
		int bit = page / 4096 % 8;
		memmap[index] |= 1 << bit;
	}
}

void *alloc_page(void) {
	for (size_t index = 0; index < memmap_size; ++index) {
		if (~memmap[index]) {
			for (int bit = 0; bit < 1; ++bit) {
				if (~memmap[index] & 1 << bit) {
					void *page = (void *)(index * 4096 * 8 + bit * 4096);
					size_t index = (size_t)page / 4096 / 8;
					int bit = (size_t)page / 4096 % 8;
					memmap[index] |= 1 << bit;
					return page;
				}
			}
		}
	}

	return nullptr;
}

// TODO
void *alloc_pages(size_t size);

void free_page(void *page) {
	size_t index = (size_t)page / 4096 / 8;
	int bit = (size_t)page / 4096 % 8;
	memmap[index] &= ~(1 << bit);
}
// TODO
void free_pages(void *pages, size_t size);
