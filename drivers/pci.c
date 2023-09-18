#include "pci.h"

#include "pci_config_space.h"

#include "cpu/page.h"
#include "kernel/acpi.h"
#include "kernel/acpi_tables.h"
#include "kernel/malloc.h"
#include "util/panic.h"
#include "util/print.h"

#include <stddef.h>
#include <stdint.h>

// TODO: MSI-X

struct pci_dev {
	struct pci_func *functions;
	struct pci_dev *next;
	struct pci_bus *parent;
	uint8_t device_number;
};

struct pci_bus {
	struct pci_dev *devices;
	struct pci_bus *next;
	struct pci_group *parent;
	uint8_t bus_number;
};

struct pci_group {
	struct pci_bus *busses;
	struct pci_group *next;
	uint16_t group_number;
};

struct pci_group *pci_tree = nullptr;

static void register_function(struct pci_config_space *config_space,
	uint16_t group_number, uint8_t bus_number, uint8_t device_number,
	uint8_t function_number) {
	struct pci_group *group;
	struct pci_bus *bus;
	struct pci_dev *dev;
	struct pci_func *func;

	uint8_t class = config_space->class;
	uint8_t subclass = config_space->subclass;
	uint8_t prog_if = config_space->prog_if;
	kprintf("Function " PRIX16 ":" PRIX8 ":" PRIX8 ":" PRIX8 " of type " PRIX8
			":" PRIX8 ":" PRIX8 "",
		group_number, bus_number, device_number, function_number, class,
		subclass, prog_if);

	for (group = pci_tree; group != nullptr; group = group->next) {
		if (group->group_number == group_number) {
			break;
		}
	}
	if (!group) {
		struct pci_group *temp = pci_tree;
		pci_tree = malloc(sizeof(struct pci_group));
		group = pci_tree;
		group->busses = nullptr;
		group->next = temp;
		group->group_number = group_number;
	}

	for (bus = group->busses; bus != nullptr; bus = bus->next) {
		if (bus->bus_number == bus_number) {
			break;
		}
	}
	if (!bus) {
		struct pci_bus *temp = group->busses;
		group->busses = malloc(sizeof(struct pci_bus));
		bus = group->busses;
		bus->devices = nullptr;
		bus->next = temp;
		bus->parent = group;
		bus->bus_number = bus_number;
	}

	for (dev = bus->devices; dev != nullptr; dev = dev->next) {
		if (dev->device_number == device_number) {
			break;
		}
	}
	if (!dev) {
		struct pci_dev *temp = bus->devices;
		bus->devices = malloc(sizeof(struct pci_dev));
		dev = bus->devices;
		dev->functions = nullptr;
		dev->next = temp;
		dev->parent = bus;
		dev->device_number = device_number;
	}

	struct pci_func *temp = dev->functions;
	dev->functions = malloc(sizeof(struct pci_func));
	func = dev->functions;

	func->next = temp;
	func->parent = dev;
	func->function_number = function_number;
	func->config_space = config_space;
	func->phys_address = get_physical_address((void *)config_space);
	func->class = class;
	func->subclass = subclass;
	func->prog_if = prog_if;
}

static void check_device(struct pci_config_space *config_space,
	uint16_t segment_group, uint8_t bus_number, uint8_t device) {
	uint16_t vendor_id = config_space->vendor_id;

	/* Device does not exist */
	if (vendor_id == 0xFFFF) {
		return;
	}

	struct pci_header_type header_type = config_space->header_type;

	if (header_type.multifunction == 0 /* Multifunction device */) {
		kprintf("Multifunction PCI device at " PRIX16 ":" PRIX8 ":" PRIX8
				" with vendor id "
				"0x" PRIX16 "",
			segment_group, bus_number, device, vendor_id);

		for (uint8_t function = 0; function < 8; ++function) {
			struct pci_config_space *function_config_space
				= (void *)((uint64_t)config_space + (function << 12));

			uint16_t vendor_id = function_config_space->vendor_id;

			/* Function does not exist */
			if (vendor_id == 0xFFFF) {
				continue;
			}
			register_function(function_config_space, segment_group, bus_number,
				device, function);
		}
	} else {
		kprintf("Singlefunction PCI device at " PRIX16 ":" PRIX8 ":" PRIX8
				" with vendor id "
				"0x" PRIX16 "",
			segment_group, bus_number, device, vendor_id);
		register_function(config_space, segment_group, bus_number, device, 0);
	}
}

static void check_segment_group(void *base_address, uint16_t segment_group,
	uint8_t pci_bus_start, uint8_t pci_bus_end) {
	for (uint16_t bus = pci_bus_start; bus <= pci_bus_end; ++bus) {
		for (uint8_t device = 0; device < 32; ++device) {
			struct pci_config_space *config_space = (void
					*)((uint64_t)base_address
					   + (((uint8_t)bus - pci_bus_start) << 20 | device << 15));
			check_device(config_space, segment_group, bus, device);
		}
	}
}

void pci_init(void) {
	kprintf("Enumerating PCI devices...");
	struct MCFG *mcfg = acpi_get_table(ACPI_MCFG);
	if (!mcfg) {
		panic("Unable to locate ACPI MCFG table!");
	}

	size_t num_entries = (mcfg->Length - 44) / 16;
	if (num_entries == 0) {
		kprintf("No MCFG entries present.");
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
			PAGE_PRESENT | PAGE_WRITE | PAGE_PCD | PAGE_GLOBAL);

		check_segment_group(base_address, segment_group, pci_bus_start,
			pci_bus_end);
	}
	kprintf("Enumerating PCI devices: Success");
}

struct pci_func *pci_get_dev(uint8_t class, uint8_t subclass, uint8_t prog_if) {
	struct pci_group *group = pci_tree;
	while (pci_tree) {
		struct pci_bus *bus = group->busses;
		while (bus) {
			struct pci_dev *dev = bus->devices;
			while (dev) {
				struct pci_func *func = dev->functions;
				while (func) {
					if (func->class == class && func->subclass == subclass
						&& func->prog_if == prog_if) {
						return func;
					}
					func = func->next;
				}
				dev = dev->next;
			}
			bus = bus->next;
		}
		group = group->next;
	}
	return nullptr;
}
