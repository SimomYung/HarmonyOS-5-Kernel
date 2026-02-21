/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: Implement of soft rest config.
 * Create: 2025-08-13
 */

#ifndef __INCLUDE_HJPG370_SOFTRESET_H__
#define __INCLUDE_HJPG370_SOFTRESET_H__

struct sr_reg_sets {
	u32 base;
	u32 offset;
	u32 bit_flag;
	u32 value;
	u32 op_flag;
};

enum SR_OP_FLAG {
	SR_OP_WRITE_ALL,
	SR_OP_READ_ALL,
	SR_OP_WRITE_BIT,
	SR_OP_READ_BIT,
};

#define SR_JPEG_TOP_BASE 0xE1104000
#define SR_CVDR_AXI_BASE 0xE1106000

/* for cs */
struct sr_reg_sets sr_cfg_reg_sets[] = {
	// step 1: config reset handshake signal
	{ SR_CVDR_AXI_BASE, 0x58, 0x02000000, 0x1, SR_OP_WRITE_BIT },
	{ SR_CVDR_AXI_BASE, 0x8C0, 0x02000000, 0x1, SR_OP_WRITE_BIT },

	// step 2: wait handshake state ok
	{ SR_CVDR_AXI_BASE, 0x58, 0x01000000, 0x1, SR_OP_READ_BIT },
	{ SR_CVDR_AXI_BASE, 0x8C0, 0x01000000, 0x1, SR_OP_READ_BIT },

	// step 3: start soft reset
	{ SR_JPEG_TOP_BASE, 0x104, 0x00000001, 0x1, SR_OP_WRITE_BIT },

	// step 4: cancel soft reset
	{ SR_CVDR_AXI_BASE, 0x58, 0x02000000, 0x0, SR_OP_WRITE_BIT },
	{ SR_CVDR_AXI_BASE, 0x8C0, 0x02000000, 0x0, SR_OP_WRITE_BIT },
	{ SR_JPEG_TOP_BASE, 0x104, 0x00000001, 0x0, SR_OP_WRITE_BIT },
};

#endif /* __INCLUDE_HJPG370_SOFTRESET_H__ */