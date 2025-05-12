#include "vmem.h"

#include "malloc.h"

#include "cpu/mem.h"
#include "cpu/page.h"
#include "util/print.h"

#include <stddef.h>

struct vheap_header {
	void *addr;
	size_t size;
	struct vheap_header *next;
};

/* pointer to the first allocated block of pages */
static struct vheap_header *vheap_head = NULL;
static void *vheap_start;
static void *vheap_end;

/**
 * @brief Initialize the heap of virtual memory/pages.
 */
void vmem_init(void) {
	kprint("Initializing virtual heap...\n");

	vheap_start
		= (void *)HIGHER_HALF_BASE + mem_max
	    + ((mem_max % 0x4000'0000 /* 1GB */ != 0)
				? (0x4000'0000 - mem_max % 0x4000'0000)
				: 0);
	vheap_end = (void *)KERNEL_BASE;

	vheap_head = malloc(sizeof(*vheap_head));

	vheap_head->addr = (void *)vheap_start;
	vheap_head->size = 0;
	vheap_head->next = nullptr;
	kprint("Initializing virtual heap: Success\n");
}

/**
 * @brief Allocate a range of pages in the higher half of virtual memory.
 * @param size The size of the range to allocate.
 * @return The address of the allocated range.
 */
void *vmem_alloc(size_t size) {
	/* make size a multiple of 4096 for ease of use */
	size += (size % 4'096 != 0) ? (4'096 - size % 4'096) : 0;
	struct vheap_header *current = vheap_head;

	if (current->size == 0) {
		current->size = size;
		return current->addr;
	}

	/* Is there enough space at the start of the virtual heap? */
	if ((size_t)(vheap_head->addr - vheap_start) >= size) {
		vheap_head = malloc(sizeof(*vheap_head));
		vheap_head->addr = vheap_start;
		vheap_head->size = size;
		vheap_head->next = current;
		return vheap_head->addr;
	}

	while (current->next != nullptr) {
		if ((size_t)current->next->addr - ((size_t)current->addr + current->size)
			>= size) {
			struct vheap_header *temp = current->next;

			current->next = malloc(sizeof(*current));
			current->next->addr = current->addr + current->size;
			current->next->size = size;
			current->next->next = temp;

			return current->next->addr;
		}
		current = current->next;
	}

	/* current->next == nullptr */
	if ((size_t)vheap_end - ((size_t)current->addr + current->size) >= size) {
		current->next = malloc(sizeof(*current));
		current->next->addr = current->addr + current->size;
		current->next->size = size;
		current->next->next = nullptr;

		return current->next->addr;
	}

	return nullptr;
}

/**
 * @brief Free a range of pages in the higher half of virtual memory.
 * @param addr The address of the range to allocate.
 */
void vmem_free(void *addr) {
	if (addr == nullptr) {
		return;
	}

	struct vheap_header *temp;

	/* should we free the first block? */
	if (vheap_head->addr == addr) {
		temp = vheap_head;
		vheap_head = vheap_head->next;
		free(temp);
		return;
	}

	struct vheap_header *current = vheap_head;
	while (current->next != nullptr) {
		if ((void *)current->next->addr == addr) {
			temp = current->next;
			current->next = current->next->next;
			free(temp);
			return;
		}
		current = current->next;
	}
}
