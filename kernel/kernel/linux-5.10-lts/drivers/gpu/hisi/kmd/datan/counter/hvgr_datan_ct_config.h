/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2023-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DATAN_CT_CONFIG_H
#define HVGR_DATAN_CT_CONFIG_H

#include <linux/types.h>

#define CT_OFFSET_32BIT                         32
#define GPU_COMMAND_WAIT_MAX_TIME               1000000 /* max wait 1s. */
#define DISABLE_WAIT_MAX_TIME_WHEN_DUMPING      2000 /* max wait 2000ms */

struct hvgr_datan_ct_inner_config {
	u32 dump_buffer_base_low;
	u32 dump_buffer_base_high;
	u32 fcm_enable;
	u32 gpc_enable;
	u32 mmu_enable;
	u32 btc_enable;
	u32 bvh_enable;
	u32 dump_mode;
	u32 set_id;
};

struct hvgr_ct_setup {
	u64 dump_buffer;
	u32 fcm_bm;
	u32 mmu_l2_bm;
	u32 btc_bm;
	u32 bvh_bm;
	u32 gpc_bm;
	u32 set_id;
};

#endif /* end of the HVGR_DATAN_CT_CONFIG_H */
