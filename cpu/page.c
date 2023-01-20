#include "page.h"

#include "kernel/limine_reqs.h"
#include "page_tables.h"
#include "x86.h"
#include <stdalign.h>
#include <stdint.h>

#define KERNEL_BASE            (0xFFFF'FFFF'8000'0000)
#define KERNEL_PAGE_PML4_INDEX ((KERNEL_BASE >> 39) & 0x1FF)
#define KERNEL_PAGE_PDP_INDEX  ((KERNEL_BASE >> 30) & 0x1FF)

alignas(PAGE_TABLE_ALIGN) volatile union pml4_entry pml4[512];
alignas(PAGE_TABLE_ALIGN) volatile union pdp_entry pdp_kernel[512];

uint64_t kernel_offset;

// TODO: map upper 2GB not only 1GB
void pg_init(void) {
	kernel_offset
		= limine_kernel_address_response->virtual_base
	    - limine_kernel_address_response->physical_base;

	union cr3 cr3 = {
		.addr = (uint64_t)&pml4 - KERNEL_BASE
	          + limine_kernel_address_response->physical_base};
	cr3.pwt = 0;
	cr3.pcd = 0;

	/* identity map the low 4GB of physical memory and map it to the kernel */
	pml4[0].addr = (uint64_t)&pdp_kernel - kernel_offset;
	pml4[0].present = 1;
	pml4[0].read_write = 1;
	pml4[0].user_supervisor = 0;
	pml4[0].pwt = 0;
	pml4[0].pcd = 0;

	pml4[KERNEL_PAGE_PML4_INDEX].addr = (uint64_t)&pdp_kernel - kernel_offset;
	pml4[KERNEL_PAGE_PML4_INDEX].present = 1;
	pml4[KERNEL_PAGE_PML4_INDEX].read_write = 1;
	pml4[KERNEL_PAGE_PML4_INDEX].user_supervisor = 0;
	pml4[KERNEL_PAGE_PML4_INDEX].pwt = 0;
	pml4[KERNEL_PAGE_PML4_INDEX].pcd = 0;


	pdp_kernel[0].addr = 0;
	pdp_kernel[0].present = 1;
	pdp_kernel[0].read_write = 1;
	pdp_kernel[0].user_supervisor = 0;
	pdp_kernel[0].pwt = 0;
	pdp_kernel[0].pcd = 0;
	pdp_kernel[0].page_size = 1;


	pdp_kernel[KERNEL_PAGE_PDP_INDEX].addr = 0;
	pdp_kernel[KERNEL_PAGE_PDP_INDEX].present = 1;
	pdp_kernel[KERNEL_PAGE_PDP_INDEX].read_write = 1;
	pdp_kernel[KERNEL_PAGE_PDP_INDEX].user_supervisor = 0;
	pdp_kernel[KERNEL_PAGE_PDP_INDEX].pwt = 0;
	pdp_kernel[KERNEL_PAGE_PDP_INDEX].pcd = 0;
	pdp_kernel[KERNEL_PAGE_PDP_INDEX].page_size = 1;

	// TODO: kernel is not loaded 1GB aligned which would be required
	wcr3(cr3.value);
}

// TODO
void *mmap(void *phys);
// TODO
void *mmap_virt(void *phys, void *virt);
// TODO
void mfree(void *virt);