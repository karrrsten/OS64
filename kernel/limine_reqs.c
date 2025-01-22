#include "limine_reqs.h"

#include <limine.h>

LIMINE_BASE_REVISION(1)

[[gnu::aligned(8)]] struct limine_hhdm_request hhdm_request
	= {.id = LIMINE_HHDM_REQUEST, .revision = 1};

[[gnu::aligned(8)]] struct limine_framebuffer_request framebuffer_request
	= {.id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 1};

[[gnu::aligned(8)]] struct limine_memmap_request memmap_request
	= {.id = LIMINE_MEMMAP_REQUEST, .revision = 1};

[[gnu::aligned(8)]] struct limine_rsdp_request rsdp_request
	= {.id = LIMINE_RSDP_REQUEST, .revision = 1};

[[gnu::aligned(8)]] struct limine_kernel_address_request kernel_address_request
	= {.id = LIMINE_KERNEL_ADDRESS_REQUEST, .revision = 1};
