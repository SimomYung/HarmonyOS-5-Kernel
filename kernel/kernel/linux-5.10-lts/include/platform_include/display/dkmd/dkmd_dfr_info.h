/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
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

#ifndef DKMD_DFR_INFO_H
#define DKMD_DFR_INFO_H

#include <linux/types.h>
#include "dkmd_dpu.h"
#include "dkmd_mipi_panel_info.h"

enum panel_oled_type {
	PANEL_OLED_LTPS,
	PANEL_OLED_LTPO,
};

enum panel_ltpo_version {
	PANEL_LTPO_V1,
	PANEL_LTPO_V2,
};

enum panel_ltpo_dsi_cmd_type {
	PANEL_LTPO_DSI_CMD_TE_90HZ,
	PANEL_LTPO_DSI_CMD_TE_120HZ,
	PANEL_LTPO_DSI_CMD_REFRESH,
	PANEL_LTPO_DSI_CMD_REFRESH_1HZ,      /* not used any more */
	PANEL_LTPO_DSI_CMD_BL,
	PANEL_LTPO_DSI_CMD_TE_360HZ,
	PANEL_LTPO_DSI_CMD_TE_432HZ,
	PANEL_LTPO_DSI_CMD_REFRESH_PPU_FULL, /* full screen refresh cmds */
	PANEL_LTPO_DSI_CMD_REFRESH_PPU_HIGH, /* high region refresh cmds */
	PANEL_LTPO_DSI_CMD_REFRESH_EXT,
	PANEL_LTPO_DSI_CMD_TE_144HZ,
	PANEL_LTPO_DSI_CMD_MAX
};

enum ddic_type {
	DDIC_TYPE_C08,
	DDIC_TYPE_H01,
	DDIC_TYPE_F01,
	DDIC_TYPE_INVALID
};
enum dimming_type {
	DIMMING_NORMAL,
	DIMMING_90HZ,
	DIMMING_TYPE_MAX
};

enum dimming_mode {
	DIMMING_MODE_PLATFORM,
	DIMMING_MODE_DDIC,
	DIMMING_MODE_INVALID
};

enum dfr_present_timing {
	DFR_PRESENT_VALID_TIMING = 0,
	DFR_PRESENT_INVALID_TIMING = 1,
	DFR_PRESENT_TIMING_UNSUPPORT = 2,
};

#define DSI_CMDS_NUM_MAX 20
struct dsi_cmds {
	uint32_t cmd_num;
	struct dsi_cmd_desc cmds[DSI_CMDS_NUM_MAX];
};

struct safe_frm_rates {
	uint32_t safe_frm_rate;
	struct dsi_cmds dsi_cmds;
};

struct dimming_node {
	uint32_t frm_rate;
	uint32_t repeat_num;
};

struct dimming_gear_config {
	uint32_t frm_rate;
	uint32_t dimming_gear1;
	uint32_t dimming_gear2;
	uint32_t dimming_end;
};

#define DIMMING_SEQ_LEN_MAX 22
#define DIMMING_GEAR_CONFIG_MAX 20
#define DIMMING_GEAR_INFO_NUM 4
struct dimming_sequence {
	enum dimming_type type;
	uint32_t dimming_seq_num;
	struct dimming_node dimming_seq_list[DIMMING_SEQ_LEN_MAX];
};

#define TE_FREQ_NUM_MAX 2
#define TE_FREQ_NUM_MAX_LTPS 3
#define SAFE_FRM_RATE_MAX_NUM 12
#define SAFE_FRM_RATE_MAX_NUM_LTPS 7

struct panel_ltpo_info {
	/* version only used to print */
	enum panel_ltpo_version ver;

	uint32_t te_freq_num;
	uint32_t te_freqs[TE_FREQ_NUM_MAX];

	uint32_t te_mask_num;
	uint32_t te_masks[TE_FREQ_NUM_MAX];

	/* ctrl te2 freq that fixed 120hz or followed by refresh */
	uint32_t te2_enable;
	bool dimming_enable;
	enum dimming_mode dimming_mode;
	struct dimming_sequence dimming_sequence[DIMMING_TYPE_MAX];
	bool dimming_gear_enable;
	uint32_t dimming_gear_len;
	struct dimming_gear_config dimming_gear_config[DIMMING_GEAR_CONFIG_MAX];

	struct dsi_cmds dsi_cmds[PANEL_LTPO_DSI_CMD_MAX];

	uint32_t safe_frm_rates_num;
	struct safe_frm_rates safe_frm_rates[SAFE_FRM_RATE_MAX_NUM];
};

struct panel_ltps_info {
	uint32_t te_freq_num; // for harden dfr
	uint32_t te_freqs[TE_FREQ_NUM_MAX_LTPS];
	uint32_t safe_frm_rates_num;
	struct safe_frm_rates safe_frm_rates[SAFE_FRM_RATE_MAX_NUM_LTPS];
};

struct panel_oled_info {
	enum panel_oled_type oled_type;
	uint32_t fps_sup_num;
	uint32_t fps_sup_seq[FPS_LEVEL_MAX];
	uint32_t fps_dvfs_seq[FPS_LEVEL_MAX];
	union {
		struct panel_ltpo_info ltpo_info;
		struct panel_ltps_info ltps_info;
	};
};

struct dfr_info {
	enum dfr_mode dfr_mode;
	struct panel_oled_info oled_info;
	enum ddic_type ddic_type;
};

#endif
