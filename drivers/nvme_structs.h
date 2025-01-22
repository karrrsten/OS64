#pragma once

#include <stdint.h>

struct [[gnu::packed]] nvme_regs {
	struct CAP {
		uint32_t MQES   : 16;
		uint32_t CQR    : 1;
		uint32_t AMS    : 2;
		uint32_t        : 5;
		uint32_t TO     : 8;
		uint32_t DSTRD  : 4;
		uint32_t NSSRS  : 1;
		uint32_t CSS    : 8;
		uint32_t BPS    : 1;
		uint32_t CPS    : 2;
		uint32_t MPSMIN : 4;
		uint32_t MPSMAX : 4;
		uint32_t PMRS   : 1;
		uint32_t CMBS   : 1;
		uint32_t NSSS   : 1;
		uint32_t CRMS   : 2;
		uint32_t        : 3;
	} CAP;

	struct VS {
		uint32_t TER : 8;
		uint32_t MNR : 8;
		uint32_t MJR : 16;
	} VS;

	uint32_t INTMS;

	uint32_t INTMC;

	struct CC {
		uint32_t EN     : 1;
		uint32_t        : 3;
		uint32_t CSS    : 3;
		uint32_t MPS    : 4;
		uint32_t AMS    : 3;
		uint32_t SHN    : 2;
		uint32_t IOSQES : 4;
		uint32_t IOCQES : 4;
		uint32_t CRIME  : 1;
		uint32_t        : 7;
	} CC;

	uint32_t : 32;

	struct CSTS {
		uint32_t RDY   : 1;
		uint32_t CFS   : 1;
		uint32_t SHST  : 2;
		uint32_t NNSRO : 1;
		uint32_t PP    : 1;
		uint32_t ST    : 1;
	} CSTS;

	uint32_t NSSR;

	struct AQA {
		uint32_t ASQS : 12;
		uint32_t      : 4;
		uint32_t ACQS : 12;
		uint32_t      : 4;
	} AQA;

	uint64_t ASQ; /* Lower 12 bits masked off */

	uint64_t ACQ; /* Lower 12 bits masked off */
};

struct [[gnu::packed]] nvme_cmd {
	struct CDW0 {
		uint32_t OPC  : 8;
		uint32_t FUSE : 2;
		uint32_t      : 4;
		uint32_t PSDT : 2;
		uint32_t CID  : 16;
	} CDW0;

	uint32_t NSID;
	uint32_t CDW2;
	uint32_t CDW3;
	uint64_t MPTR;
	__uint128_t DPTR;
	uint32_t CDW10;
	uint32_t CDW11;
	uint32_t CDW12;
	uint32_t CDW13;
	uint32_t CDW14;
	uint32_t CDW15;
};

struct [[gnu::packed]] nvme_cq_entry {
	uint32_t DW0;
	uint32_t DW1;
	uint32_t SQHD : 16;
	uint32_t SQID : 16;
	uint32_t CID  : 16;
	uint32_t P    : 1;

	union {
		uint32_t Status;

		struct {
			uint32_t Status_SC  : 8;
			uint32_t Status_SCT : 3;
			uint32_t Status_CRD : 2;
			uint32_t Status_M   : 1;
			uint32_t Status_DNR : 1;
		};
	};
};
