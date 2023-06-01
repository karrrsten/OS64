#pragma once

#include <stdint.h>

union [[gnu::packed]] pci_config_space {
	char raw_space[4096];

	struct [[gnu::packed]] {
		uint16_t vendor_id;
		uint16_t device_id;

		struct [[gnu::packed]] {
			uint16_t io_space                 : 1;
			uint16_t mem_space                : 1;
			uint16_t bus_master               : 1;
			uint16_t special_cycle_enable     : 1; /* PCIe: 0 */
			uint16_t mem_write_inval_enable   : 1; /* PCIe: 0 */
			uint16_t vga_palette_snoop        : 1; /* PCIe: 0 */
			uint16_t parity_error_response    : 1;
			uint16_t                          : 1;
			uint16_t serr_enable              : 1;
			uint16_t fast_back_to_back_enable : 1; /* PCIe: 0 */
			uint16_t interrupt_disable        : 1;
			uint16_t                          : 5;
		} command;

		struct [[gnu::packed]] {
			uint16_t                           : 3;
			uint16_t interrupt_status          : 1;
			uint16_t capabilities_list         : 1; /* PCIe: 1 */
			uint16_t mhz66_capable             : 1; /* PCIe: 0 */
			uint16_t                           : 1;
			uint16_t fast_back_to_back_capable : 1; /* PCIe: 0 */
			uint16_t master_data_parity_error  : 1;
			uint16_t devsel_timing             : 2; /* PCIe: 0 */
			uint16_t signaled_target_abort     : 1;
			uint16_t received_target_abort     : 1;
			uint16_t received_master_abort     : 1;
			uint16_t signaled_system_error     : 1;
			uint16_t detected_parity_error     : 1;

		} status;

		uint8_t revision_id;
		uint8_t prog_if;
		uint8_t subclass;
		uint8_t class;
		uint8_t cache_line_size; /* PCIe: legacy only */
		uint8_t master_latency_timer; /* PCIe: 0 */

		struct [[gnu::packed]] {
			uint8_t type          : 7;
			uint8_t multifunction : 1;
		} header_type;

		struct [[gnu::packed]] {
			uint8_t completion_code : 4;
			uint8_t                 : 2;
			uint8_t start_bist      : 1;
			uint8_t bist_capable    : 1;
		} bist;

		uint32_t bar0;
		uint32_t bar1;
		uint32_t bar2;
		uint32_t bar3;
		uint32_t bar4;
		uint32_t bar5;
		uint32_t cardbus_cis_ptr;
		uint16_t subsystem_vendor_id;
		uint16_t subsystem_id;
		uint32_t expansion_rom_base;
		uint32_t capabilities_ptr : 8;
		uint32_t                  : 24;
		uint32_t                  : 32;
		uint8_t interrupt_line;
		uint8_t interrupt_pin;
		uint8_t min_grant; /* PCIe: 0 */
		uint8_t max_latency; /* PCIe: 0 */
	};
};
