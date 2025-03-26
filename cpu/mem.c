#include "mem.h"

#include "page.h"

#include "kernel/limine_reqs.h"
#include "util/panic.h"
#include "util/print.h"
#include "util/string.h"

#include <limine.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

extern void _kernel_end; /* End of the kernel, defined in linker.ld */
void *kernel_end = &_kernel_end;

size_t mem_max;

static uint8_t *memmap; /* Map of all of physical memory */
static size_t memmap_size;

// TODO: reclaim bootloader reclaimable at the end of kmain

/**
 * @brief Initialize the physical memory manager.
 */
void mem_init(void) {
	kprintf("Initializing physical memory allocator...");
	struct limine_memmap_entry **memmap_entries
		= limine_memmap_response->entries;

	/* find the highest useable physical address to save space in memmap */
	for (size_t i = 0; i < limine_memmap_response->entry_count; ++i) {
		if (memmap_entries[i]->type != LIMINE_MEMMAP_RESERVED) {
			mem_max = memmap_entries[i]->base + memmap_entries[i]->length;
		}
	}

	kprintf("Total amount of memory available: %zu = 0x%zX", mem_max, mem_max);
	memmap_size = mem_max / 4'096 / 8;

	for (size_t i = 0; i < limine_memmap_response->entry_count; ++i) {
		if (memmap_entries[i]->type == LIMINE_MEMMAP_USABLE) {
			if (memmap_entries[i]->length >= memmap_size) {
				memmap = (uint8_t *)P2V(memmap_entries[i]->base);
				break;
			}
		}
	}

	if (!memmap) {
		panic("Failed to find a memory region large enough (memmap).");
	}

	/* mark all pages as used */
	memset(memmap, INT_MAX, memmap_size);

	/* mark available pages as unused */
	for (size_t i = 0; i < limine_memmap_response->entry_count; ++i) {
		if (memmap_entries[i]->type == LIMINE_MEMMAP_USABLE) {
			for (uint64_t page = memmap_entries[i]->base;
				page < memmap_entries[i]->base + memmap_entries[i]->length;
				page += 4'096) {
				size_t index = page / 4'096 / 8;
				int bit = page / 4'096 % 8;
				memmap[index] &= ~(1 << bit);
			}
		}
	}

	/* Mark the first MB as used */
	for (uint64_t page = 0; page < 0x10'0000; page += 4'096) {
		size_t index = page / 4'096 / 8;
		int bit = page / 4'096 % 8;
		memmap[index] |= 1 << bit;
	}

	/* mark memmap itself as used */
	for (uint64_t page = (uint64_t)memmap - HIGHER_HALF_BASE;
		page < (uint64_t)memmap - HIGHER_HALF_BASE + memmap_size;
		page += 4'096) {
		size_t index = page / 4'096 / 8;
		int bit = page / 4'096 % 8;
		memmap[index] |= 1 << bit;
	}
	kprintf("Initializing physical memory allocator: Success");
}

/**
 * @brief Allocate a single page of physical memory.
 * @return The address of a free page in memory.
 */
void *alloc_page(void) {
	for (size_t index = 0; index < memmap_size; ++index) {
		if (~memmap[index] & 0xFF) {
			for (int bit = 0; bit < 8; ++bit) {
				if (~memmap[index] & (1 << bit)) {
					void *page = (void *)(index * 4'096 * 8 + bit * 4'096);
					mark_page_used(page);
					return page;
				}
			}
		}
	}

	return nullptr;
}

/**
 * @brief Allocate a contigious range of physical memory.
 * @param size The size of the range to allocate.
 * @return The address of the allocated range.
 */
void *alloc_pages(size_t size) {
	size_t num_pages = size / 4'096;
	size_t found_free = 0;
	size_t first_free_idx;
	for (size_t i = 0; i < memmap_size; ++i) {
		if (memmap[i] == 0) {
			++found_free;
			first_free_idx = found_free ? first_free_idx : i;
		} else {
			found_free = 0;
			first_free_idx = 0;
		}

		if (found_free >= num_pages / 8) {
			mark_pages_used((void *)(i * 8 * 4'096), size);
			return (void *)(i * 8 * 4'096);
		}
	}
	return nullptr;
}

/**
 * @brief Mark a single page of physical memory as used.
 * @param page The page to be marked as used.
 */
void mark_page_used(const void *page) {
	size_t index = (size_t)page / 4'096 / 8;
	int bit = (size_t)page / 4'096 % 8;
	memmap[index] |= 1 << bit;
}

/**
 * @brief Mark a range of pages of physical memory as used.
 * @param pages The first page to be marked as used.
 * @param size The size of physical memory to be marked as used.
 */
void mark_pages_used(const void *pages, size_t size) {
	size_t index = (size_t)pages / 4'096 / 8;
	size_t start_bit = (size_t)pages / 4'096 % 8;

	/* Fill bits that do not fill an entire byte */
	if (start_bit != 0) {
		for (int bit = start_bit; bit <= 8; ++bit) {
			memmap[index] |= 1 << bit;
			--size;
		}
		++index;
	}

	/* Fill entire bytes */
	while (size > 8) {
		memmap[index] = UINT8_MAX;
		++index;
		size -= 8;
	}

	/* Fill remaining bits */
	if (size != 0) {
		for (int bit = 0; bit < (int)size; ++bit) {
			memmap[index] |= 1 << bit;
			--size;
		}
		++index;
	}
}

/**
 * @brief Free a previously allocated page of physical memory.
 * @param page The page to be freed.
 */
void free_page(void *page) {
	size_t index = (size_t)page / 4'096 / 8;
	int bit = (size_t)page / 4'096 % 8;
	memmap[index] &= ~(1 << bit);
}

/**
 * @brief Free a previously allocated range of pages of physical memory.
 * @param pages The first page to be freed.
 * @param size The size of the range to be freed.
 */
void free_pages(void *pages, size_t size) {
	size_t index = (size_t)pages / 4'096 / 8;
	size_t start_bit = (size_t)pages / 4'096 % 8;

	/* Fill bits that do not fill an entire byte */
	if (start_bit != 0) {
		for (int bit = start_bit; bit <= 8; ++bit) {
			memmap[index] &= ~(1 << bit);
			--size;
		}
		++index;
	}

	/* Fill entire bytes */
	while (size > 8) {
		memmap[index] = 0;
		++index;
		size -= 8;
	}

	/* Fill remaining bits */
	if (size != 0) {
		for (int bit = 0; bit < (int)size; ++bit) {
			memmap[index] &= ~(1 << bit);
			--size;
		}
		++index;
	}
}
