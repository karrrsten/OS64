#include "malloc.h"

#include "cpu/mem.h"
#include "cpu/page.h"
#include "util/log.h"
#include "util/string.h"

#include <stddef.h>
#include <stdint.h>

struct heap_header {
	size_t size;
	struct heap_header *next;
	char data[];
};

static void *heap; /* Pointer to the start of the heap */
static void *heap_end; /* Pointer to the end of the heap */
static struct heap_header *heap_head; /* Pointer to the first allocated block */

/**
 * @brief Initialize the memory manager for calls to malloc() and friends.
 * @param heap_start The linear start address of the heap.
 * @param size The size of the heap.
 */
void heap_init(void *heap_start, size_t size) {
	log("Initilizing heap...");
	heap = heap_start;
	heap_end = heap_start + size;

	/* map the heap */
	uint64_t temp = (uint64_t)heap_start;
	temp += (temp % 4096 != 0) ? (4096 - temp % 4096) : 0;
	void *ptr = (void *)temp;

	for (; ptr <= heap_end; ptr += 4096) {
		(void)kmap(alloc_page(), ptr, 4096, PAGE_PRESENT | PAGE_WRITE);
	}
	memset(heap, 0, size);

	/* initialize a first block of size 0 on the heap */
	heap_head = (struct heap_header *)heap_start;
	heap_head->size = 0;
	heap_head->next = nullptr;

	log("Initializing heap: Success");
}

void *malloc(size_t size) {
	if (size > (size_t)(heap_end - heap)) {
		return nullptr;
	}
	struct heap_header *current = heap_head;
	/* first use of malloc, nothing has been allocated yet */
	if (current->size == 0) {
		current->size = size + sizeof(struct heap_header);
		current->next = nullptr;
		return (void *)current->data;
	}
	/* is the block between heap and the first allocated block big enough? */
	if ((size_t)((void *)heap_head - heap)
		>= size + sizeof(struct heap_header)) {
		current = (struct heap_header *)heap;
		current->size = size;
		current->next = heap_head;
		heap_head = current;
		return (void *)current->data;
	}
	do {
		/* have we reached the end of the heap? */
		if (current->next == nullptr) {
			/* the heap is not big enough */
			if ((void *)current->next + size > heap_end) {
				return nullptr;
			}
			current->next
				= (struct heap_header *)((void *)current + current->size);
			current->next->next = nullptr;
			current->next->size = size + sizeof(struct heap_header);
			return (void *)current->next->data;
		}
		current = current->next;
	} while (current->next == nullptr
			 || ((ptrdiff_t)current->next - ((ptrdiff_t)current + current->size)
				 <= size + sizeof(struct heap_header)));

	/* there is enough space between current and current->next */
	struct heap_header *temp = current->next;
	current->next = (struct heap_header *)((void *)current + current->size);
	current->next->size = size + sizeof(struct heap_header);
	current->next->next = temp;
	return (void *)current->data;
}

void free(void *ptr) {
	if (ptr == nullptr) {
		return;
	}
	/* the pointer points to the data of the block which is preceded by a
	 * heap_header */
	ptr -= sizeof(struct heap_header);
	struct heap_header *current = heap_head;

	/* should we free the first block? */
	if (heap_head == ptr) {
		heap_head = heap_head->next;
		return;
	}
	while (current->next != nullptr) {
		if ((void *)current->next == ptr) {
			current->next = current->next->next;
			return;
		}
		current = current->next;
	}
}

void *realloc(void *ptr, size_t size) {
	if (!ptr) {
		return malloc(size);
	}

	struct heap_header *header = ptr - sizeof(*header);
	if ((header->next == nullptr
			&& ((uint64_t)header + size) < (uint64_t)heap_end)
		|| ((uint64_t)header + size) < (uint64_t)header->next) {
		header->size = size;
		return ptr;
	}

	void *new_ptr = malloc(size);
	if (new_ptr) {
		memcpy(new_ptr, ptr, header->size);
		free(ptr);
	}
	return new_ptr;
}
