#include "page.h"

#include "mem.h"
#include "x86.h"

#include "kernel/limine_reqs.h"
#include "kernel/vmem.h"
#include "util/print.h"
#include "util/string.h"

#include <stdint.h>

#define ADDR_MASK_4K (0xF'FFFF'FFFF'F000lu)
#define ADDR_MASK_2M (0xF'FFFF'FFF0'0000lu)
#define ADDR_MASK_1G (0xF'FFFF'E000'0000lu)

#define PML4_INDEX(addr) ((addr >> 39) & 0x1FF)
#define PDP_INDEX(addr)  ((virt >> 30) & 0x1FF)
#define PD_INDEX(addr)   ((virt >> 21) & 0x1FF)
#define PT_INDEX(addr)   ((virt >> 12) & 0x1FF)

#define PAGE_TABLE_ALIGN (4'096)

alignas(PAGE_TABLE_ALIGN) static volatile uint64_t pml4_kernel[512];
volatile uint64_t *pg_pml4 = pml4_kernel;

static void map_single_page(uint64_t phys, uint64_t virt, uint64_t flags);

/**
 * @brief Initialize the kernel's page tables.
 */
void pg_init(void) {
	kprint("Initializing paging...\n");
	uint64_t cr3
		= ((uint64_t)&pml4_kernel - KERNEL_BASE
			  + limine_kernel_address_response->physical_base)
	    & ADDR_MASK_4K;

	/* Prepare kernel pdp entries in the pml4 that are always mapped */
	for (int i = PML4_INDEX(HIGHER_HALF_BASE); i < 512; ++i) {
		void *pdp = alloc_page();
		memset_volatile(P2V(pdp), 0, 4'096);

		pml4_kernel[i] = (uint64_t)pdp | PAGE_PRESENT | PAGE_WRITE;
	}

	/* Map all of memory into the higher half (max 512 GB) */
	uint64_t *pdp_entry = P2V(
		(uint64_t *)(pml4_kernel[PML4_INDEX(HIGHER_HALF_BASE)] & ADDR_MASK_4K));
	for (size_t i = 0; i <= mem_max / 0x4000'0000 /* 1GB */ && i < 512; ++i) {
		// Does not work in simics
		/* pdp_entry[i]
		    = i * 0x4000'0000 | PAGE_SIZE | PAGE_PRESENT | PAGE_WRITE
		    | PAGE_GLOBAL; */

		uint64_t *pt_entry = alloc_page();
		pdp_entry[i]
			= (uint64_t)pt_entry | PAGE_PRESENT | PAGE_WRITE | PAGE_GLOBAL;

		for (size_t j = 0; j < 512; ++j) {
			P2V(pt_entry)
			[j] = j * 0x20'0000 + i * 0x4000'0000 | PAGE_SIZE | PAGE_PRESENT
			    | PAGE_WRITE | PAGE_GLOBAL;
		}
	}

	/* Map the kernel into the upper 2GB */
	for (uint64_t phys_addr = limine_kernel_address_response->physical_base,
				  virt_addr = limine_kernel_address_response->virtual_base;
		virt_addr < (uint64_t)kernel_end;
		phys_addr += 4'096, virt_addr += 4'096) {
		map_single_page(phys_addr, virt_addr,
			PAGE_PRESENT | PAGE_WRITE | PAGE_GLOBAL);
	}

	kprint("Initializing paging: Success\n");

	kprint("Loading CR3...\n");
	wcr3(cr3);
	kprint("Loading CR3: Success\n");
}

/**
 * @brief Get the physical address corresponding to a virtual address.
 * @param virt_addr The virtual address to look up.
 * @return The correpsonding physical address or nullptr if the address is
 * currently not mapped.*/
void *get_physical_address(const void *virt_addr) {
	uint64_t virt = (uint64_t)virt_addr;
	unsigned pml4_index = PML4_INDEX(virt);
	unsigned pdp_index = PDP_INDEX(virt);
	unsigned pd_index = PD_INDEX(virt);
	unsigned pt_index = PT_INDEX(virt);

	if (!pg_pml4[pml4_index]) {
		return nullptr;
	}

	uint64_t *pdp = P2V((uint64_t *)(pg_pml4[pml4_index] & ADDR_MASK_4K));
	if (!pdp[pdp_index]) {
		return nullptr;
	} else if (pdp[pdp_index] & PAGE_SIZE) {
		/* Also account for the offset into the page */
		return (void *)((pdp[pdp_index] & ADDR_MASK_1G) + (virt & 0x1FFF'FFFF));
	}

	uint64_t *pd = P2V((uint64_t *)(pdp[pdp_index] & ADDR_MASK_4K));
	if (!pd[pd_index]) {
		return nullptr;
	} else if (pd[pd_index] & PAGE_SIZE) {
		/* Also account for the offset into the page */
		return (void *)((pd[pd_index] & ADDR_MASK_2M) + (virt & 0xF'FFFF));
	}

	uint64_t *pt = P2V((uint64_t *)(pd[pd_index] & ADDR_MASK_4K));
	/* Also account for the offset into the page */
	return (void *)(pt[pt_index] & ADDR_MASK_4K) + (virt & 0xFFF);
}

void set_pml4(volatile uint64_t *pml4) {
	pg_pml4 = pml4;
	wcr3((uint64_t)get_physical_address((void *)pml4));
}

static void map_single_page(uint64_t phys, uint64_t virt, uint64_t flags) {
	unsigned pml4_index = PML4_INDEX(virt);
	unsigned pdp_index = PDP_INDEX(virt);
	unsigned pd_index = PD_INDEX(virt);
	unsigned pt_index = PT_INDEX(virt);

	if (!pg_pml4[pml4_index]) {
		pg_pml4[pml4_index] = ((uint64_t)alloc_page() & ADDR_MASK_4K) | flags;
	} else {
		pg_pml4[pml4_index] |= flags;
	}

	uint64_t *pdp = P2V((uint64_t *)(pg_pml4[pml4_index] & ADDR_MASK_4K));
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
void *kmap(void *phys_addr, void *virt_addr, size_t size, uint64_t flags) {
	if (virt_addr == nullptr) {
		virt_addr = vmem_alloc(size);
	}

	uint64_t phys = (uint64_t)phys_addr;
	uint64_t virt = (uint64_t)virt_addr;

	for (; virt < (uint64_t)virt_addr + size; phys += 4'096, virt += 4'096) {
		map_single_page(phys, virt, flags);
	}
	return virt_addr;
}

static void unmap_single_page(uint64_t virt) {
	unsigned pml4_index = PML4_INDEX(virt);
	unsigned pdp_index = PDP_INDEX(virt);
	unsigned pd_index = PD_INDEX(virt);
	unsigned pt_index = PT_INDEX(virt);

	uint64_t *pdp = P2V((uint64_t *)(pg_pml4[pml4_index] & ADDR_MASK_4K));
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
void kunmap(void *virt_addr, size_t size) {
	uint64_t virt = (uint64_t)virt_addr;
	for (; virt <= virt + size; virt += 4'096) {
		unmap_single_page(virt);
	}
}

volatile uint64_t *alloc_pml4(void) {
	volatile uint64_t *pml4 = P2V(alloc_page());

	memset_volatile(pml4, 0, 4'096);
	memcpy_volatile(&pml4[256], &pg_pml4[256], 2'048);

	return pml4;
}

void free_pml4(volatile uint64_t *pml4) {
	free_pages((void *)pml4, 4'096);
}
