/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __DPU_DP_DBG_V800_H__
#define __DPU_DP_DBG_V800_H__

#include "dp_ctrl_dev.h"

struct dp_debug_params {
	int dp_lane_num_debug;
	int dp_lane_rate_debug;
	int dp_mst_enable_debug;
	int dp_ssc_enable_debug;
	int dp_fec_enable_debug;
	int dp_dsc_enable_debug;
	int dp_efm_enable_debug;
	int dp_vactive_pixels_debug;
	int dp_hactive_pixels_debug;
	uint dp_aux_ctrl;

	uint32_t dp_rbr_pll1_rate0_coef[4];
	uint32_t dp_hbr_pll1_rate1_coef[4];
	uint32_t dp_hbr2_pll1_rate2_coef[4];

	int dp_same_source_debug;
	uint32_t dp_preemphasis_swing[DPTX_PREE_SWING_MAX][DPTX_COMBOPHY_PARAM_NUM];
	int dp_pe_sw_length;
	uint32_t dp_pe_sw_attach_length;
	int clk_hblank_code_enable;

	uint64_t pixel_clock_debug;
	uint32_t video_code_debug;
	uint16_t hblank_debug;

	int port_id_sync_debug; // dual-tx synchronization scenarios, port_id_sync and port_id must be synchronous in v740.
	int dpc_index_selected_debug; // 0/1/2
	bool is_double_tx_sync_debug; // the control flag that double edp or dp sync function in v740
	uint32_t pmcc_pll;
	uint32_t dp_mode;
};

extern struct dp_debug_params g_dp_params[2];

#endif
