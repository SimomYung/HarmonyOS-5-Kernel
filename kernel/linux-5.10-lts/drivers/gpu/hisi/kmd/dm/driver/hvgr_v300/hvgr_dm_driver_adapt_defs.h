/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DM_DRIVER_ADAPT_DEFS_H
#define HVGR_DM_DRIVER_ADAPT_DEFS_H

#include <linux/types.h>

#define HEBC_FAULT 0x7DU
#define HEBC_FAULT_THRESHOLD 10u

struct hvgr_dm_fault_msg {
	uint32_t fault_code;
	char *fault_name;
};

enum hvgr_irqs_type {
	JOB_IRQ,
	MMU_IRQ,
	GPU_IRQ,
	FCP_IRQ,
	CQ_IRQ,
	CC_IRQ,
	GPU_IRQ_CNT
};

/* jasid1_mapped_valid 28 */
#define GET_JASID1_MAPPED_VALID(x) (((x) & 0x10000000) >> 28)
/* jasid1_mapped_ssid 27:16 */
#define GET_JASID1_MAPPED_SSID(x)  (((x) & 0xFFF0000) >> 16)
/* jasid0_mapped_valid 12 */
#define GET_JASID0_MAPPED_VALID(x) (((x) & 0x1000) >> 12)
/* jasid0_mapped_ssid 11:0 */
#define GET_JASID0_MAPPED_SSID(x)  (((x) & 0xFFF))

/* JOB gpu_faultstatus[13:11] */
#define GET_JOB_JASID(x)       ((uint32_t)((x) & 0x3800) >> 11)
/* FCP_CTRL gpu_faultdata0[18:16] */
#define GET_FCP_CTRL_JASID(x)  ((uint32_t)((x) & 0x70000) >> 16)
/* JOB gpu_faultstatus[16:14] */
#define GET_JOB_GPCID(x)       ((uint32_t)((x) & 0x1c000) >> 14)
/* MISC CTRL gpu_faultdata2[19:14] - Core_id[2:0] */
#define GET_MISC_CTRL_GPCID(x) ((uint32_t)((x) & 0x1c000) >> 14)
#endif
