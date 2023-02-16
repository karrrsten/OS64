#pragma once

#include <limine.h>

#define limine_hhdm_response           (hhdm_request.response)
#define limine_memmap_response         (memmap_request.response)
#define limine_rsdp_response           (rsdp_request.response)
#define limine_kernel_address_response (kernel_address_request.response)

extern struct limine_hhdm_request hhdm_request;
extern struct limine_memmap_request memmap_request;
extern struct limine_rsdp_request rsdp_request;
extern struct limine_kernel_address_request kernel_address_request;
