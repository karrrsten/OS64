#pragma once

#include "pci_config_space.h"

#include <stdint.h>

struct pci_func {
	volatile struct pci_config_space *config_space;
	volatile void *phys_address; /* Physical address of config space */
	struct pci_func *next;
	struct pci_dev *parent;
	uint8_t function_number;
	uint8_t class;
	uint8_t subclass;
	uint8_t prog_if;
};

#define PCI_BAR_SIZE(_bar_ptr, _bar_size_buf) \
	{                                         \
		uint64_t _bar_prev = *_bar_ptr;       \
		*_bar_ptr = (typeof(*_bar_ptr))-1;    \
		_bar_size_buf = ~(*_bar_ptr) + 1;     \
		*_bar_ptr = _bar_prev;                \
	}

void pci_init(void);

struct pci_func *pci_get_dev(uint8_t class, uint8_t subclass, uint8_t prog_if);
