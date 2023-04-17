#pragma once

#include <stdint.h>

struct [[gnu::packed]] RSDP {
	char Signature[8];
	uint8_t Checksum;
	char OEMID[6];
	uint8_t Revision;
	uint32_t RsdtAddress;
	uint32_t Length;
	uint64_t XsdtAddress;
	uint8_t Extended_Checksum;
	uint8_t  : 8;
	uint32_t : 32;
};

struct [[gnu::packed]] sdt_header {
	char Signature[4];
	uint32_t Length;
	uint8_t Revision;
	uint8_t Checksum;
	char OEMID[6];
	char OEM_Table_ID[8];
	uint32_t OEM_Revision;
	uint32_t Creator_ID;
	uint32_t Creator_Revision;
};

#define ACPI_RSDT          \
	(char[4]) {            \
		'R', 'S', 'D', 'T' \
	}

struct [[gnu::packed]] RSDT {
	struct sdt_header;
	uint32_t Entry[];
};

#define ACPI_XSDT          \
	(char[4]) {            \
		'X', 'S', 'D', 'T' \
	}

struct [[gnu::packed]] XSDT {
	struct sdt_header;
	uint64_t Entry[];
};

#define ACPI_MADT          \
	(char[4]) {            \
		'A', 'P', 'I', 'C' \
	}

struct [[gnu::packed]] MADT {
	struct sdt_header;
	uint32_t Local_Interrupt_Controller_Address;
	uint32_t PCAT_COMPAT : 1;
	uint32_t             : 31;
	char Interrupt_Controller_Structure[];
};

#define ACPI_MCFG          \
	(char[4]) {            \
		'M', 'C', 'F', 'G' \
	}

struct [[gnu::packed]] MCFG {
	struct sdt_header;
	uint64_t : 64;
	char entries[];
};