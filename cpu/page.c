#include "page.h"

#include "mem.h"
#include "x86.h"

#include "kernel/limine_reqs.h"
#include "kernel/vmem.h"
#include "util/log.h"
#include "util/print.h"

#include <stdalign.h>
#include <stdint.h>

#define ADDR_MASK_4K (0xF'FFFF'FFFF'F000)
#define ADDR_MASK_2M (0xF'FFFF'FFF0'0000)
#define ADDR_MASK_1G (0xF'FFFF'E000'0000)

#define PML4_INDEX(addr) ((addr >> 39) & 0x1FF)
#define PDP_INDEX(addr)  ((virt >> 30) & 0x1FF)
#define PD_INDEX(addr)   ((virt >> 21) & 0x1FF)
#define PT_INDEX(addr)   ((virt >> 12) & 0x1FF)

#define PAGE_TABLE_ALIGN (4096)

alignas(PAGE_TABLE_ALIGN) volatile uint64_t pml4[512];

static void early_mmap(void *phys_addr, void *virt_addr, uint64_t flags);

/**
 * @brief Initialize the kernel's page tables.
 */
void pg_init(void) {
	log("Initializing paging...");
	uint64_t cr3
		= ((uint64_t)&pml4 - KERNEL_BASE
			  + limine_kernel_address_response->physical_base)
	    & ADDR_MASK_4K;

	uint64_t *pdp_entry_zero = early_alloc_page();

	pml4[PML4_INDEX(HIGHER_HALF_BASE)]
		= ((uint64_t)pdp_entry_zero & ADDR_MASK_4K) | PAGE_PRESENT | PAGE_WRITE;

	/* Map all of memory into the higher half (max 512 GB) */
	for (size_t i = 0; i <= mem_max / 0x4000'0000 /* 1GB */ && i < 512; ++i) {
		pdp_entry_zero[i]
			= (i * 0x4000'0000 & ADDR_MASK_1G) | PAGE_SIZE | PAGE_PRESENT
		    | PAGE_WRITE | PAGE_GLOBAL;
	}

	/* Map the kernel into the upper 2GB */
	for (uint64_t phys_addr = limine_kernel_address_response->physical_base,
				  virt_addr = KERNEL_BASE;
		 virt_addr < (uint64_t)kernel_end;
		 phys_addr += 4096, virt_addr += 4096) {
		// TODO: this could actually be mmap
		early_mmap((void *)phys_addr, (void *)virt_addr,
			PAGE_PRESENT | PAGE_WRITE | PAGE_GLOBAL);
	}

	log("Initializing paging: Success");

	log("Loading CR3...");
	wcr3(cr3);
	log("Loading CR3: Success");
}

static void early_mmap(void *phys_addr, void *virt_addr, uint64_t flags) {
	uint64_t phys = (uint64_t)phys_addr;
	uint64_t virt = (uint64_t)virt_addr;
	unsigned pml4_index = PML4_INDEX(virt);
	unsigned pdp_index = PDP_INDEX(virt);
	unsigned pd_index = PD_INDEX(virt);
	unsigned pt_index = PT_INDEX(virt);

	if (!pml4[pml4_index]) {
		pml4[pml4_index]
			= ((uint64_t)early_alloc_page() & ADDR_MASK_4K) | flags;
	} else {
		pml4[pml4_index] |= flags;
	}

	uint64_t *pdp = (uint64_t *)(pml4[pml4_index] & ADDR_MASK_4K);
	if (!pdp[pdp_index]) {
		pdp[pdp_index] = ((uint64_t)early_alloc_page() & ADDR_MASK_4K) | flags;
	} else {
		pdp[pdp_index] |= flags;
	}

	uint64_t *pd = (uint64_t *)(pdp[pdp_index] & ADDR_MASK_4K);
	if (!pd[pd_index]) {
		pd[pd_index] = ((uint64_t)early_alloc_page() & ADDR_MASK_4K) | flags;
	} else {
		pd[pd_index] |= flags;
	}

	uint64_t *pt = (uint64_t *)(pd[pd_index] & ADDR_MASK_4K);
	pt[pt_index] = ((uint64_t)phys & ADDR_MASK_4K) | flags;
}

static void map_single_page(uint64_t phys, uint64_t virt, uint64_t flags) {
	unsigned pml4_index = PML4_INDEX(virt);
	unsigned pdp_index = PDP_INDEX(virt);
	unsigned pd_index = PD_INDEX(virt);
	unsigned pt_index = PT_INDEX(virt);

	if (!pml4[pml4_index]) {
		pml4[pml4_index] = ((uint64_t)alloc_page() & ADDR_MASK_4K) | flags;
	} else {
		pml4[pml4_index] |= flags;
	}

	uint64_t *pdp = P2V((uint64_t *)(pml4[pml4_index] & ADDR_MASK_4K));
	if (!pdp[pdp_index]) {
		pdp[pdp_index] = ((uint64_t)alloc_page() & ADDR_MASK_4K) | flags;
	} else {
		pdp[pdp_index] |= flags;
	}

	uint64_t *pd = P2V((uint64_t *)(pdp[pdp_index] & ADDR_MASK_4K));
	if (!pd[pd_index]) {
		pd[pd_index] = ((uint64_t)alloc_page() & ADDR_MASK_4K) | flags;
	} else {
		pd[pd_index] |= flags;
	}

	uint64_t *pt = P2V((uint64_t *)(pd[pd_index] & ADDR_MASK_4K));
	pt[pt_index] = ((uint64_t)phys & ADDR_MASK_4K) | flags;
	invlpg((void *)virt);
}

/**
 * @brief Map physical memory to virtual memory.
 * @param phys_addr The physical address.
 * @param virt_addr The virtual address. If it is nullptr, virtual memory is
 * allocated.
 * @param size The size of the region to map.
 * @param flags The flags to be used for mapping. Must contain PAGE_PRESENT.
 * @return The virtual address that was mapped.
 */
void *mmap(void *phys_addr, void *virt_addr, size_t size, uint64_t flags) {
	if (virt_addr == nullptr) {
		virt_addr = vmem_alloc(size);
	}

	uint64_t phys = (uint64_t)phys_addr;
	uint64_t virt = (uint64_t)virt_addr;

	for (; virt <= virt + size; phys += 4096, virt += 4096) {
		map_single_page(phys, virt, flags);
	}
	return virt_addr;
}

static void unmap_single_page(uint64_t virt) {
	unsigned pml4_index = PML4_INDEX(virt);
	unsigned pdp_index = PDP_INDEX(virt);
	unsigned pd_index = PD_INDEX(virt);
	unsigned pt_index = PT_INDEX(virt);

	uint64_t *pdp = P2V((uint64_t *)(pml4[pml4_index] & ADDR_MASK_4K));
	uint64_t *pd = P2V((uint64_t *)(pdp[pdp_index] & ADDR_MASK_4K));
	uint64_t *pt = P2V((uint64_t *)(pd[pd_index] & ADDR_MASK_4K));
	pt[pt_index] = 0;
	invlpg((void *)virt);
}

/**
 * @brief Unmap physical memory.
 * @param virt_addr The virtual address of the mapping.
 * @param size The size of the mapping.
 */
void munmap(void *virt_addr, size_t size) {
	uint64_t virt = (uint64_t)virt_addr;
	for (; virt <= virt + size; virt += 4096) {
		unmap_single_page(virt);
	}
}
