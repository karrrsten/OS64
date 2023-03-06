#include "limine_reqs.h"

struct limine_hhdm_request hhdm_request = {
	.id = LIMINE_HHDM_REQUEST,
	.revision = 0,
};

struct limine_memmap_request memmap_request = {
	.id = LIMINE_MEMMAP_REQUEST,
	.revision = 0,
};

struct limine_rsdp_request rsdp_request = {
	.id = LIMINE_RSDP_REQUEST,
	.revision = 0,
};

struct limine_kernel_address_request kernel_address_request = {
	.id = LIMINE_KERNEL_ADDRESS_REQUEST,
	.revision = 0,
};

[[maybe_unused, gnu::section(".limine_reqs")]] static void *volatile requests[]
	= {(void *)&hhdm_request, (void *)&memmap_request, (void *)&rsdp_request,
		(void *)&kernel_address_request};
