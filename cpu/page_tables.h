#pragma once

#include <stdint.h>

#define PAGE_TABLE_ALIGN (4096)

/* Note: the lower bits of all addresses referencing page tables are assumed to
 * be zero as the tables are required to be aligned by the architecture. This
 * makes space for attribute bits. In this implementation, the upper bits (i.e.
 * the sign extension bits) of all addresses referencing page tables are also
 * required to be zero since some of them are reserved by the architecture,
 * must-be-zero or have another purpose that is unused in this implementation.
 * This eases the handling of them the addresses as the table entries can be
 * treated as regular addresses, with the lower bits masked off
 *
 * Also: Page-Attribute Tabel is not used */

/* Each page table entry has its own union, containing a struct of bitfiels with
 * the attributes and the address, the entry is pointing to. Note that the lower
 * bits of the address have to be masked off correctly */
union [[gnu::packed]] cr3 {
	struct [[gnu::packed]] {
		uint64_t     : 3;
		uint64_t pwt : 1;
		uint64_t pcd : 1;
		uint64_t     : 7;
		uint64_t     : 52;
	};

	uint64_t addr;

	uint64_t value;
};

union [[gnu::packed]] pml4_entry {
	struct [[gnu::packed]] {
		uint64_t present         : 1;
		uint64_t read_write      : 1;
		uint64_t user_supervisor : 1;
		uint64_t pwt             : 1;
		uint64_t pcd             : 1;
		uint64_t accessed        : 1;
		uint64_t                 : 6;
		uint64_t                 : 52;
	};

	uint64_t addr; /* Remember to mask off the lower bits! */
};

union [[gnu::packed]] pdp_entry {
	struct [[gnu::packed]] {
		uint64_t present         : 1;
		uint64_t read_write      : 1;
		uint64_t user_supervisor : 1;
		uint64_t pwt             : 1;
		uint64_t pcd             : 1;
		uint64_t accessed        : 1;
		uint64_t dirty           : 1; /* only when page_size = 1 */
		uint64_t page_size       : 1;
		uint64_t global          : 1; /* only when page_size = 1 */
		uint64_t                 : 3;
		uint64_t                 : 52;
	};

	uint64_t addr; /* Remember to mask off the lower bits! */
};

union [[gnu::packed]] pd_entry {
	struct [[gnu::packed]] {
		uint64_t present         : 1;
		uint64_t read_write      : 1;
		uint64_t user_supervisor : 1;
		uint64_t pwt             : 1;
		uint64_t pcd             : 1;
		uint64_t accessed        : 1;
		uint64_t dirty           : 1; /* only when page_size = 1*/
		uint64_t page_size       : 1;
		uint64_t global          : 1; /* only when page_size = 1*/
		uint64_t                 : 6;
		uint64_t                 : 52;
	};

	uint64_t addr; /* Remember to mask off the lower bits! */
};

union [[gnu::packed]] pt_entry {
	struct [[gnu::packed]] {
		uint64_t present         : 1;
		uint64_t read_write      : 1;
		uint64_t user_supervisor : 1;
		uint64_t pwt             : 1;
		uint64_t pcd             : 1;
		uint64_t accessed        : 1;
		uint64_t dirty           : 1;
		uint64_t                 : 1;
		uint64_t global          : 1;
		uint64_t                 : 3;
		uint64_t                 : 52;
	};

	uint64_t addr; /* Remember to mask off the lower bits! */
};
