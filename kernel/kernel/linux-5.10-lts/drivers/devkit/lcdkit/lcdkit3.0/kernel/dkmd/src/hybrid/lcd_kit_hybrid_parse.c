// SPDX-License-Identifier: GPL-2.0
/*
 * lcd_kit_hybrid_parse.c
 *
 * dts source file for hybrid control
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
#include "lcd_kit_hybrid_parse.h"
#include "lcd_kit_parse.h"
#include "lcd_kit_disp.h"

static void hybrid_parse_aod_dt(struct device_node *np, struct lcd_kit_hybrid_info *hybrid_info)
{
	/* aod cmd */
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,alpm-on-hl-cmds", "lcd-kit,alpm-on-hl-cmds-state",
			       &hybrid_info->alpm_on_hl_cmds);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,alpm-on-ll-cmds", "lcd-kit,alpm-on-ll-cmds-state",
			       &hybrid_info->alpm_on_ll_cmds);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,alpm-on-ml-cmds", "lcd-kit,alpm-on-ml-cmds-state",
			       &hybrid_info->alpm_on_ml_cmds);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,alpm-on-hbml-cmds", "lcd-kit,alpm-on-hbml-cmds-state",
			       &hybrid_info->alpm_on_hbml_cmds);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,alpm-on-nl-cmds", "lcd-kit,alpm-on-nl-cmds-state",
			       &hybrid_info->alpm_on_nl_cmds);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,alpm-on-cmds", "lcd-kit,alpm-on-cmds-state",
			       &hybrid_info->alpm_on_cmds);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,alpm-exit-cmds", "lcd-kit,alpm-exit-cmds-state",
			       &hybrid_info->alpm_exit_cmds);
}

static void hybrid_parse_status_dt(struct device_node *np, struct lcd_kit_hybrid_info *hybrid_info)
{
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,status-cmds", "lcd-kit,status-cmds-state",
			       &hybrid_info->status_cmds);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,status-cmds-52", "lcd-kit,status-cmds-state",
			       &hybrid_info->status_cmds_52);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,status-cmds-54", "lcd-kit,status-cmds-state",
			       &hybrid_info->status_cmds_54);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,status-cmds-0d", "lcd-kit,status-cmds-state",
			       &hybrid_info->status_cmds_0d);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,status-cmds-0e", "lcd-kit,status-cmds-state",
			       &hybrid_info->status_cmds_0e);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,status-cmds-0f", "lcd-kit,status-cmds-state",
			       &hybrid_info->status_cmds_0f);
}

void hybrid_parse_dt(struct lcd_kit_hybrid_info *hybrid_info)
{
	struct device_node *np = of_find_compatible_node(NULL, NULL, DTS_COMP_LCD_KIT_PANEL_TYPE);

	if (!np || !hybrid_info) {
		LCD_KIT_ERR("param is invalid\n");
		return;
	}

	LCD_KIT_ERR("of_property_read begin\n");

	lcd_kit_parse_dcs_cmds(np, "lcd-kit,te-off-cmds", "lcd-kit,te-off-cmds-state",
			       &hybrid_info->te_off_cmds);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,te-on-cmds", "lcd-kit,te-on-cmds-state",
			       &hybrid_info->te_on_cmds);

	/* hybrid brightness */
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,normal-brightness-cmds",
			       "lcd-kit,normal-brightness-cmds-state",
			       &hybrid_info->normal_brightness_cmds);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,hbm-brightness-cmds",
			       "lcd-kit,hbm-brightness-cmds-state",
			       &hybrid_info->hbm_brightness_cmds);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,hbm-mode-cmds", "lcd-kit,hbm-mode-cmds-state",
			       &hybrid_info->hbm_mode_cmds);

	hybrid_parse_aod_dt(np, hybrid_info);

	hybrid_parse_status_dt(np, hybrid_info);

	lcd_kit_parse_dcs_cmds(np, "lcd-kit,ddic-id-cmds", "lcd-kit,ddic-id-cmds-state",
			       &hybrid_info->ddic_id_cmds);

	lcd_kit_parse_dcs_cmds(np, "lcd-kit,dirty-region-cmds", "lcd-kit,dirty-region-cmds-state",
			       &hybrid_info->dirty_region_cmds);
	lcd_kit_parse_dcs_cmds(np, "lcd-kit,panel-off-deepsleep-cmds", "lcd-kit,panel-off-deepsleep-cmds-state",
			       &hybrid_info->ddic_deep_sleep_cmd);

	of_property_read_u8(np, "lcd-kit,vxn-ddic-id", &hybrid_info->vxn_id);
}

void hybrid_parse_ddic_id(struct DDICMap ddic_map[])
{
	int ret = LCD_KIT_OK;
	int i = 0;
	struct lcd_kit_array_data ddic_ids;
	char panel_comp_prop[MAX_DDIC_PROP_LEN] = {0};
	struct device_node *np = of_find_compatible_node(NULL, NULL, DTS_COMP_LCD_KIT_PANEL_TYPE);

	if (!np || !ddic_map) {
		LCD_KIT_ERR("param is invalid\n");
		return;
	}

	ret = lcd_kit_parse_array_data(np, "lcd-kit,ddic-ids", &ddic_ids);
	if (ret != LCD_KIT_OK) {
		LCD_KIT_ERR("fail to parse ddic ids\n");
		return;
	}

	if (ddic_ids.cnt > MAX_DDICMAP_SIZE) {
		LCD_KIT_ERR("over max ddic count: %d\n", ddic_ids.cnt);
		return;
	}

	for (i = 0; i < ddic_ids.cnt; i++) {
		ddic_map[i].ddicID = ddic_ids.buf[i];
		ddic_map[i].compatible = NULL;
		ret = snprintf_s(panel_comp_prop, MAX_DDIC_PROP_LEN,
			MAX_DDIC_PROP_LEN - 1, "lcd-kit,lcd%d-compatible", i);
		if (ret < 0) {
			LCD_KIT_ERR("snprintf fail for panel compatible prop, ret: %d\n", ret);
			continue;
		}

		char *panel_comp = NULL;

		ret = lcd_kit_parse_string(np, panel_comp_prop, (const char **)&panel_comp);
		if (ret != LCD_KIT_OK) {
			LCD_KIT_ERR("fail to parse panel compatible, ret: %d\n", ret);
			ddic_map[i].compatible = NULL;
			continue;
		}
		ddic_map[i].compatible = panel_comp;
	}
}
