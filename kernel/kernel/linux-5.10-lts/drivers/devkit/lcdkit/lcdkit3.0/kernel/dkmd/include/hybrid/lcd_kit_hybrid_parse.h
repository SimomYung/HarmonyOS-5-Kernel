/* SPDX-License-Identifier: GPL-2.0 */
/*
 * lcd_kit_hybrid_parse.h
 *
 * parse dts head file for hybrid control
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __LCD_KIT_HYBRID_PARSE_H_
#define __LCD_KIT_HYBRID_PARSE_H_
#include "linux/of.h"
#include "lcd_kit_common.h"

#define MAX_COMP_LEN 128
#define MAX_DDICMAP_SIZE 20
#define MAX_DDIC_PROP_LEN 64

struct DDICMap {
	uint32_t ddicID;
	u8 *compatible;
};

struct lcd_kit_hybrid_info {
	struct lcd_kit_dsi_panel_cmds te_off_cmds;
	struct lcd_kit_dsi_panel_cmds te_on_cmds;
	/* ap aod cmd */
	struct lcd_kit_dsi_panel_cmds alpm_on_cmds;
	struct lcd_kit_dsi_panel_cmds alpm_on_hl_cmds;
	struct lcd_kit_dsi_panel_cmds alpm_on_ll_cmds;
	struct lcd_kit_dsi_panel_cmds alpm_on_ml_cmds;
	struct lcd_kit_dsi_panel_cmds alpm_on_hbml_cmds;
	struct lcd_kit_dsi_panel_cmds alpm_on_nl_cmds;
	/* ap aod exit cmd */
	struct lcd_kit_dsi_panel_cmds alpm_exit_cmds;

	struct lcd_kit_dsi_panel_cmds status_cmds;
	struct lcd_kit_dsi_panel_cmds status_cmds_52;
	struct lcd_kit_dsi_panel_cmds status_cmds_54;
	struct lcd_kit_dsi_panel_cmds status_cmds_0d;
	struct lcd_kit_dsi_panel_cmds status_cmds_0e;
	struct lcd_kit_dsi_panel_cmds status_cmds_0f;

	/* brightness status */
	struct lcd_kit_dsi_panel_cmds normal_brightness_cmds;
	struct lcd_kit_dsi_panel_cmds hbm_brightness_cmds;
	struct lcd_kit_dsi_panel_cmds hbm_mode_cmds;

	struct lcd_kit_dsi_panel_cmds ddic_id_cmds;
	struct lcd_kit_dsi_panel_cmds dirty_region_cmds;
	struct lcd_kit_dsi_panel_cmds ddic_deep_sleep_cmd;
	bool esd_enabled;
	u8 vxn_id;
};

void hybrid_parse_dt(struct lcd_kit_hybrid_info *hybrid_info);
void hybrid_parse_ddic_id(struct DDICMap ddic_map[]);

#endif
