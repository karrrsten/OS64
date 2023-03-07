#include "acpi.h"

#include "acpi_tables.h"
#include "limine_reqs.h"

#include "cpu/page.h"
#include "util/log.h"
#include "util/string.h"

#include <stddef.h>

static bool check_table(struct sdt_header *sdt_header) {
	uint8_t *ptr = (uint8_t *)sdt_header;
	uint8_t sum = 0;
	for (size_t i = 0; i < sdt_header->Length; ++i) {
		/* sum can and will wrap around */
		sum += ptr[i];
	}

	/* the entire table must add up to *zero* for the table to be valid */
	return !sum;
}

void *acpi_get_table(char *signature) {
	static struct RSDP *rsdp = nullptr;
	if (!rsdp) {
		rsdp = limine_rsdp_response->address;
		if (rsdp->Revision == 0) {
			panic("ACPI version 1.0 not supported!");
		}
	}

	static struct XSDT *xsdt = nullptr;
	if (!xsdt) {
		xsdt = P2V((struct XSDT *)rsdp->XsdtAddress);
	}

	size_t xsdt_num_entries = (xsdt->Length - 36) / 8;


	static struct sdt_header **xsdt_entries = nullptr;
	xsdt_entries = (struct sdt_header **)xsdt->Entry;
	for (size_t i = 0; i < xsdt_num_entries; ++i) {
		struct sdt_header *table = P2V(xsdt_entries[i]);
		log("%s", table->Signature);
		if (!strncmp(table->Signature, signature, 4)) {
			if (!check_table(table)) {
				continue;
			}
			return table;
		}
	}
	return nullptr;
}
