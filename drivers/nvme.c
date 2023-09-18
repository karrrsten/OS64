#include "nvme.h"

#include "nvme_structs.h"
#include "pci_config_space.h"

#include "cpu/mem.h"
#include "cpu/page.h"
#include "kernel/malloc.h"
#include "util/panic.h"
#include "util/print.h"
#include "util/string.h"

#include <stdint.h>

#define CC_IOCQES (4)
#define CC_IOSQES (6)

#define AQA_ACQS (4096 / (1 << CC_IOCQES))
#define AQA_ASQS (4096 / (1 << CC_IOSQES))

struct nvme_cq {
	volatile void *cq;
	volatile uint32_t *cq_doorbell;
	uint32_t cq_head;
	uint16_t cq_size;
};

struct nvme_sq {
	volatile void *sq;
	volatile uint32_t *sq_doorbell;
	uint32_t sq_tail;
	uint16_t sq_size;

	struct nvme_cq *cq;
};

struct nvme_drive {
	struct pci_func *pci_func;
	volatile struct nvme_regs *regs;
	void *identify_controller_data_structure;
	uint16_t CNTLID;
	uint32_t NSID;
	uint64_t NSZE;
	uint64_t logical_block_size;

	struct nvme_sq admin_q;
	struct nvme_sq io_q;
};

static struct nvme_drive drive;

static void nvme_send_command(struct nvme_sq *queue,
	const struct nvme_cmd *cmd) {
	volatile struct nvme_cq_entry *cq_entry = (volatile struct nvme_cq_entry
			*)(queue->cq->cq + queue->cq->cq_head * (1 << CC_IOCQES));

	/* Save the current state of the Phase Flag to know about new entries */
	uint8_t cq_p_flag = cq_entry->P;

	/* Submit the command */
	memcpy_volatile(queue->sq + queue->sq_tail * (1 << CC_IOSQES), (void *)cmd,
		64);

	/* Increment the Submission Queue Tail Pointer Doorbell*/
	if (queue->sq_tail >= drive.admin_q.cq->cq_size) {
		queue->sq_tail = 0;
	} else {
		++queue->sq_tail;
	}
	*queue->sq_doorbell = queue->sq_tail;

	/* Wait until a new Completion Queue Entry is available */
	while (cq_entry->P == cq_p_flag)
		;

	/* Increment the Completion Queue Head Pointer Doorbell */
	if (queue->cq->cq_head >= drive.admin_q.sq_size) {
		queue->cq->cq_head = 0;
	} else {
		++queue->cq->cq_head;
	}
	*queue->cq->cq_doorbell = queue->cq->cq_head;

	if (cq_entry->Status != 0) {
		panic(
			"NVMe Command failed:\n\tCDW0: OPC: 0x" PRIX8 ", FUSE: 0b" PRIb8
			", PSDT: "
			"0b" PRIb8 ", CID: 0x" PRIX16 "\n\tNSID: 0x" PRIX32
			"\n\tCDW2: 0x" PRIX32 "\n\tCW3: "
			"0x" PRIX32 "\n\tMPTR: 0x" PRIX64 "\n\tDPTR: 0x" PRIX64 PRIX64
			"\n\tCDW10: "
			"0x" PRIX32 "\n\tCDW11: 0x" PRIX32 "\n\tCDW12: 0x" PRIX32
			"\n\tCDW13: 0x" PRIX32 "\n\tCDW14: "
			"0x" PRIX32 "\n\tCDW15: 0x" PRIX32
			"\n\nCompletion Queue Entry:\n\tDW0: "
			"0x" PRIX32 "\n\tDW1: 0x" PRIX32 "\n\tSQHD: 0x" PRIX32
			"\n\tSQID: 0x" PRIX32 "\n\tCID: "
			"0x" PRIX32 "\n\tStatus: 0b" PRIb32 "\n\t",
			cmd->CDW0.OPC, cmd->CDW0.FUSE, cmd->CDW0.PSDT, cmd->CDW0.CID,
			cmd->NSID, cmd->CDW2, cmd->CDW3, cmd->MPTR,
			(uint64_t)(cmd->DPTR >> 64), (uint64_t)(cmd->DPTR & UINT64_MAX),
			cmd->CDW10, cmd->CDW11, cmd->CDW12, cmd->CDW13, cmd->CDW14,
			cmd->CDW15, cq_entry->DW0, cq_entry->DW1, cq_entry->SQHD,
			cq_entry->SQID, cq_entry->CID, cq_entry->Status);
	}
}

void nvme_init(struct pci_func *pci_func) {
	kprintf("Initializing NVMe drive...");

	drive.pci_func = pci_func;
	drive.pci_func->config_space->command.mem_space = 1;

	volatile uint64_t *bar0_long = &drive.pci_func->config_space->bar0_long;
	volatile void *bar0_phys = (void *)(*bar0_long & 0xFFFF'FFFF'FFFF'FFF0);
	uint64_t bar0_size;
	PCI_BAR_SIZE(bar0_long, bar0_size);
	drive.regs = kmap((void *)bar0_phys, nullptr, bar0_size,
		PAGE_PRESENT | PAGE_PCD | PAGE_WRITE | PAGE_GLOBAL);

	/* Initialize internal data structure around submission/completion queues */
	drive.admin_q.sq = malloc(sizeof(struct nvme_sq));
	drive.admin_q.sq = kmap(alloc_page(), nullptr, 4096,
		PAGE_PRESENT | PAGE_PCD | PAGE_WRITE | PAGE_GLOBAL);
	drive.admin_q.sq_doorbell = (void *)drive.regs + 0x1000;
	drive.admin_q.sq_tail = 0;
	drive.admin_q.sq_size = AQA_ASQS;

	drive.admin_q.cq = malloc(sizeof(struct nvme_cq));
	drive.admin_q.cq->cq = kmap(alloc_page(), nullptr, 4096,
		PAGE_PRESENT | PAGE_PCD | PAGE_WRITE | PAGE_GLOBAL);
	drive.admin_q.cq->cq_doorbell
		= (void *)drive.regs + 0x1000 + (1 * (4 << drive.regs->CAP.DSTRD));
	drive.admin_q.cq->cq_head = 0;
	drive.admin_q.cq->cq_size = AQA_ACQS;

	/* Reset the NVMe controller */
	drive.regs->CC.EN = 0;

	// TODO: access to NVMe regs with a 2-byte read, undefined behavior,
	// -fstrict-volatile-bitfields would help if it was supported by clang
	/* Wait for drive ready */
	while (drive.regs->CSTS.RDY != 0)
		;

	/* Configure the Admin Queue */
	drive.regs->AQA.ACQS = AQA_ACQS;
	drive.regs->AQA.ASQS = AQA_ASQS;
	drive.regs->ACQ
		= (uint64_t)get_physical_address((void *)drive.admin_q.cq->cq);
	drive.regs->ASQ = (uint64_t)get_physical_address((void *)drive.admin_q.sq);

	/* Set the I/O Queue Entry Sizes */
	drive.regs->CC.IOCQES = CC_IOCQES;
	drive.regs->CC.IOSQES = CC_IOSQES;

	/* Configure command sets */
	// QEMU reports faulty values
	drive.regs->CC.CSS = 0b000;
#if 0
	if (drive.regs->CAP.CSS >> 7) {
		kprintf("NVMe Controller does not support the NVM Command Set");
		return;
	} else if (drive.regs->CAP.CSS >> 6) {
		drive.regs->CC.CSS = 0b110;
	} else {
		drive.regs->CC.CSS = 0b000;
	}
#endif

	/* Enable the NVMe controller */
	drive.regs->CC.EN = 1;

	/* Wait for drive ready */
	while (drive.regs->CSTS.RDY != 1)
		;

	/* Identify (Identify Controller Data Structure) */
	void *identify_command_buffer = kmap(alloc_page(), nullptr, 4096,
		PAGE_PRESENT | PAGE_PCD | PAGE_GLOBAL);

	nvme_send_command(&drive.admin_q,
		&(struct nvme_cmd) {.CDW0.OPC = 0x6,
			.DPTR = (uint64_t)get_physical_address(identify_command_buffer),
			.CDW10 = 0x1});

	drive.CNTLID = *(uint16_t *)(identify_command_buffer + 78);

	/* The NVMe Base Specification, Section 3.5.1 (Controller Initialization)
	 * suggests issuing several Identify commands. This is not done here, since
	 * the information is not used at this time */

	/* Configure MSI-X */
	// TODO: NVMe MSI-X

	/* Create I/O Completion Queue */
	drive.io_q.cq = malloc(sizeof(struct nvme_cq));
	drive.io_q.cq->cq = kmap(alloc_page(), nullptr, 4096,
		PAGE_PRESENT | PAGE_PCD | PAGE_WRITE | PAGE_GLOBAL);
	drive.io_q.cq->cq_doorbell
		= (void *)drive.regs + 0x1000 + (2 * (4 << drive.regs->CAP.DSTRD));
	drive.io_q.cq->cq_head = 0;
	if (drive.regs->CAP.MQES + 1 < (4096 / (1 << 4))) {
		drive.io_q.cq->cq_size = drive.regs->CAP.MQES + 1;
	} else {
		drive.io_q.cq->cq_size = (4096 / (1 << 4));
	}

	nvme_send_command(&drive.admin_q,
		&(struct nvme_cmd) {.CDW0.OPC = 0x5,
			.DPTR = (uint64_t)get_physical_address((void *)drive.io_q.cq->cq),
			.CDW10 = ((drive.io_q.cq->cq_size - 1) << 16) | 0x1,
			.CDW11 = /* (vector << 16) | (1 << 1) | */ 1});


	/* Create I/O Submission Queue */
	drive.io_q.sq = malloc(sizeof(struct nvme_sq));
	drive.io_q.sq = kmap(alloc_page(), nullptr, 4096,
		PAGE_PRESENT | PAGE_PCD | PAGE_WRITE | PAGE_GLOBAL);
	drive.io_q.sq_doorbell
		= (void *)drive.regs + 0x1000 + (3 * (4 << drive.regs->CAP.DSTRD));
	drive.io_q.sq_tail = 0;
	if (drive.regs->CAP.MQES + 1 < (4096 / (1 << 6))) {
		drive.io_q.sq_size = drive.regs->CAP.MQES + 1;
	} else {
		drive.io_q.sq_size = (4096 / (1 << 6));
	}
	nvme_send_command(&drive.admin_q,
		&(struct nvme_cmd) {.CDW0.OPC = 0x1,
			.DPTR = (uint64_t)get_physical_address((void *)drive.io_q.sq),
			.CDW10 = ((drive.io_q.sq_size - 1) << 16) | 0x1,
			.CDW11 = (1 << 16) | 1});


	/* Identify (Active Namespace ID list) */
	uint32_t *identity_namepsace_list = kmap(alloc_page(), nullptr, 4096,
		PAGE_PRESENT | PAGE_PCD | PAGE_GLOBAL);

	nvme_send_command(&drive.admin_q,
		&(struct nvme_cmd) {.CDW0.OPC = 0x6,
			.NSID = 0,
			.DPTR = (uint64_t)get_physical_address(identity_namepsace_list),
			.CDW10 = 0x2});

	/* Note: only the first reported namespace is currently used */
	drive.NSID = identity_namepsace_list[0];


	/* Identify (Identify Namespace Data Structure, NVM Command Set) */
	void *identify_namespace_buffer = kmap(alloc_page(), nullptr, 4096,
		PAGE_PRESENT | PAGE_PCD | PAGE_GLOBAL);
	;
	nvme_send_command(&drive.admin_q,
		&(struct nvme_cmd) {.CDW0.OPC = 0x6,
			.NSID = drive.NSID,
			.DPTR = (uint64_t)get_physical_address(identify_namespace_buffer),
			.CDW10 = 0x0,
			.CDW11 = 0});

	drive.NSZE = *(uint64_t *)(identify_namespace_buffer + 0);

	uint8_t format_index
		= ((*(uint8_t *)(identify_namespace_buffer + 26)) & 0xF)
	    | (*(uint8_t *)(identify_namespace_buffer + 25) > 16
				? (((*(uint8_t *)(identify_namespace_buffer + 26)) & 0x30) >> 5)
					  << 4
				: 0);

	drive.logical_block_size
		= 1
	   << ((*(uint32_t *)(identify_namespace_buffer + 128 + format_index * 4)
			   >> 16)
			  & 0xFFFF);

	kprintf("Initializing NVMe drive: Success");
}
