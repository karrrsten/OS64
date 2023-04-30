#include "pci.h"

#include "cpu/page.h"
#include "kernel/acpi.h"
#include "kernel/acpi_tables.h"
#include "kernel/malloc.h"
#include "util/log.h"

#include <stddef.h>
#include <stdint.h>

struct pci_func {
	void *base_address; /* Base address in virtual memory */
	uint16_t segment_group;
	uint8_t bus_number;
	uint8_t device;
	uint8_t function;
	uint8_t class;
	uint8_t subclass;
	uint8_t prog_if;
	void *phys_address;
};

struct pci_func *pci_functions; /* Linked list of all PCI functions */
size_t num_pci_functions = 0;

static void register_function(void *function_base, uint16_t segment_group,
	uint8_t bus_number, uint8_t device, uint8_t function) {
	pci_functions = realloc(pci_functions,
		(++num_pci_functions) * sizeof(*pci_functions));
	if (!pci_functions) {
		panic("Failed to allocate memory!");
	}

	uint8_t class = *(uint8_t *)(function_base + 11);
	uint8_t subclass = *(uint8_t *)(function_base + 10);
	uint8_t prog_if = *(uint8_t *)(function_base + 9);
	log("Function %hX:%hhX:%hhX:%hhX of type %hhX:%hhX:%hhX", segment_group,
		bus_number, device, function, class, subclass, prog_if);

	pci_functions[num_pci_functions - 1].base_address = function_base;
	pci_functions[num_pci_functions - 1].segment_group = segment_group;
	pci_functions[num_pci_functions - 1].bus_number = bus_number;
	pci_functions[num_pci_functions - 1].device = device;
	pci_functions[num_pci_functions - 1].function = function;
	pci_functions[num_pci_functions - 1].class = class;
	pci_functions[num_pci_functions - 1].subclass = subclass;
	pci_functions[num_pci_functions - 1].prog_if = prog_if;
	pci_functions[num_pci_functions - 1].phys_address
		= get_physical_address(function_base);
}

static void check_device(void *base_address, uint16_t segment_group,
	uint8_t bus_number, uint8_t device) {
	uint16_t vendor_id = *(uint16_t *)(base_address);

	/* Device does not exist */
	if (vendor_id == 0xFFFF) {
		return;
	}

	uint8_t header_type = *(uint8_t *)(base_address + 14);

	if (!(header_type & 0x8 /* Multifunction device */)) {
		log("Multifunction PCI device at %hX:%hhX:%hhX with vendor id "
			"0x%hX",
			segment_group, bus_number, device, vendor_id);

		for (uint8_t function = 0; function < 8; ++function) {
			void *function_base
				= (void *)((uint64_t)base_address + (function << 12));

			uint16_t vendor_id = *(uint16_t *)(function_base);

			/* Function does not exist */
			if (vendor_id == 0xFFFF) {
				continue;
			}
			register_function(function_base, segment_group, bus_number, device,
				function);
		}

	} else {
		log("Singlefunction PCI device at %hX:%hhX:%hhX with vendor id 0x%hX",
			segment_group, bus_number, device, vendor_id);
		register_function(base_address, segment_group, bus_number, device, 0);
	}
}

static void check_segment_group(void *base_address, uint16_t segment_group,
	uint8_t pci_bus_start, uint8_t pci_bus_end) {
	for (uint16_t bus = pci_bus_start; bus <= pci_bus_end; ++bus) {
		for (uint8_t device = 0; device < 32; ++device) {
			void *device_base = (void
					*)((uint64_t)base_address
					   + (((uint8_t)bus - pci_bus_start) << 20 | device << 15));
			check_device(device_base, segment_group, bus, device);
		}
	}
}

void pci_init(void) {
	log("Enumerating PCI devices...");
	struct MCFG *mcfg = acpi_get_table(ACPI_MCFG);
	if (!mcfg) {
		panic("Unable to locate ACPI MCFG table!");
	}

	size_t num_entries = (mcfg->Length - 44) / 16;
	if (num_entries == 0) {
		log("No MCFG entries present.");
	}

	struct [[gnu::packed]] config_space_entry {
		uint64_t base_address;
		uint16_t pci_segment_group;
		uint8_t pci_bus_start;
		uint8_t pci_bus_end;
		uint32_t : 32;
	};

	struct config_space_entry *entries
		= (struct config_space_entry *)mcfg->entries;

	for (size_t i = 0; i < num_entries; ++i) {
		void *phys_address = (void *)entries[i].base_address;
		uint16_t segment_group = entries[i].pci_segment_group;
		uint8_t pci_bus_start = entries[i].pci_bus_start;
		uint8_t pci_bus_end = entries[i].pci_bus_end;

		void *base_address = kmap(phys_address, nullptr,
			(pci_bus_end - pci_bus_start + 1) * 0xF'FFFF /* 1 MiB */,
			PAGE_PRESENT | PAGE_WRITE | PAGE_PCD);

		check_segment_group(base_address, segment_group, pci_bus_start,
			pci_bus_end);
	}
	log("Enumerating PCI devices: Success");
}
