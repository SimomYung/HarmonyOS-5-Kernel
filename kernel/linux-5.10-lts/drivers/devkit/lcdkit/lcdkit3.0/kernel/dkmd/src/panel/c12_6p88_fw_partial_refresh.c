/*
 * c12_6p88_fw_partial_refresh.c
 *
 * lcdkit utils function for lcd driver
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

#include "lcd_kit_partial_refresh.h"
#include "lcd_kit_disp.h"

#define REGION_MIN_HEIGHT 64
#define REGION_BOUNDARY_BUFFER 48
#define PANEL_HEIGHT (DPU_PINFO->yres)
#define minus_one(x) ((x) - 1)
#define plus_one(x) ((x) + 1)

#define IP1_OFFSET_PARAM (common_info->part_refresh.ip1_offset_param)
#define IP2_OFFSET_PARAM (common_info->part_refresh.ip2_offset_param)

static int rd_roi_ys = 0;
static int rd_roi_ye = 0;

struct rd_cal_param {
	int dic_ys_cr3;
	int dic_ye_cr3;
	int dic_ys_cr3_2;
	int dic_ye_cr3_2;
	int adjust_a;
	int adjust_b;
	int adjust_c;
	int adjust_d;
	int adjust_e;
	int adjust_f;
	int adjust_g;
	int case_a;
	int case_b;
	int case_c;
	int case_d;
	int case_e;
	int case_f;
};

static struct rd_region_info g_rd_region_info;
static struct rd_cal_param g_rd_cal_param;

static void lcd_kit_rd_cal_corner(int panel_id)
{
	g_rd_cal_param.dic_ys_cr3 = (rd_roi_ys - REGION_BOUNDARY_BUFFER > 0) ? rd_roi_ys - REGION_BOUNDARY_BUFFER : 0;
	g_rd_cal_param.dic_ye_cr3 = (rd_roi_ye + REGION_BOUNDARY_BUFFER < minus_one(PANEL_HEIGHT)) ?
		rd_roi_ye + REGION_BOUNDARY_BUFFER : minus_one(PANEL_HEIGHT);

	if (g_rd_cal_param.dic_ys_cr3 <= minus_one(REGION_MIN_HEIGHT))
		g_rd_cal_param.dic_ys_cr3_2 = 0;
	 else if (g_rd_cal_param.dic_ys_cr3 >= PANEL_HEIGHT - REGION_MIN_HEIGHT)
		g_rd_cal_param.dic_ys_cr3_2 = PANEL_HEIGHT - REGION_MIN_HEIGHT;
	 else
		g_rd_cal_param.dic_ys_cr3_2 = g_rd_cal_param.dic_ys_cr3;

	if (g_rd_cal_param.dic_ye_cr3 <= minus_one(REGION_MIN_HEIGHT))
		g_rd_cal_param.dic_ye_cr3_2 = minus_one(REGION_MIN_HEIGHT);
	 else if (g_rd_cal_param.dic_ye_cr3 >= PANEL_HEIGHT - REGION_MIN_HEIGHT)
		g_rd_cal_param.dic_ye_cr3_2 = minus_one(PANEL_HEIGHT);
	 else
		g_rd_cal_param.dic_ye_cr3_2 = g_rd_cal_param.dic_ye_cr3;
}

static void lcd_kit_rd_cal_case(int panel_id)
{
	lcd_kit_rd_cal_corner(panel_id);
	g_rd_cal_param.adjust_a = (g_rd_cal_param.dic_ys_cr3_2 <= minus_one(REGION_MIN_HEIGHT)) ? 1 : 0;
	g_rd_cal_param.adjust_b = (g_rd_cal_param.dic_ys_cr3_2 > minus_one(REGION_MIN_HEIGHT)) ? 1 : 0;
	g_rd_cal_param.adjust_c = (g_rd_cal_param.dic_ys_cr3_2 <= minus_one(REGION_MIN_HEIGHT * CMDS_TWO)) ? 1 : 0;
	g_rd_cal_param.adjust_d = (g_rd_cal_param.dic_ye_cr3_2 <= minus_one(REGION_MIN_HEIGHT)) ? 1 : 0;
	g_rd_cal_param.adjust_e = (g_rd_cal_param.dic_ye_cr3_2 > minus_one(REGION_MIN_HEIGHT)) ? 1 : 0;
	g_rd_cal_param.adjust_f = (g_rd_cal_param.dic_ye_cr3_2 >= PANEL_HEIGHT - REGION_MIN_HEIGHT) ? 1 : 0;
	g_rd_cal_param.adjust_g = (g_rd_cal_param.dic_ye_cr3_2 < PANEL_HEIGHT - REGION_MIN_HEIGHT) ? 1 : 0;

	g_rd_cal_param.case_a = g_rd_cal_param.adjust_a & g_rd_cal_param.adjust_d;
	g_rd_cal_param.case_b = g_rd_cal_param.adjust_a & (g_rd_cal_param.adjust_f == 0) & g_rd_cal_param.adjust_e;
	g_rd_cal_param.case_c = g_rd_cal_param.adjust_a & g_rd_cal_param.adjust_f;
	g_rd_cal_param.case_d = g_rd_cal_param.adjust_b & g_rd_cal_param.adjust_g;
	g_rd_cal_param.case_e = g_rd_cal_param.adjust_b & (g_rd_cal_param.adjust_c == 0) & g_rd_cal_param.adjust_f;
	g_rd_cal_param.case_f = g_rd_cal_param.adjust_b & g_rd_cal_param.adjust_c & g_rd_cal_param.adjust_f;
	return;
}

static void lcd_kit_rd_cal_region(int panel_id)
{
	if (g_rd_cal_param.case_a) {
		g_rd_region_info.region_b_start = REGION_MIN_HEIGHT;
		g_rd_region_info.region_c_start = REGION_MIN_HEIGHT * CMDS_TWO;
	} else if (g_rd_cal_param.case_b) {
		g_rd_region_info.region_b_start = plus_one(g_rd_cal_param.dic_ye_cr3_2);
		g_rd_region_info.region_c_start = g_rd_cal_param.dic_ye_cr3_2 + plus_one(REGION_MIN_HEIGHT);
	} else if (g_rd_cal_param.case_c) {
		g_rd_region_info.region_b_start = PANEL_HEIGHT - REGION_MIN_HEIGHT * CMDS_TWO;
		g_rd_region_info.region_c_start = PANEL_HEIGHT - REGION_MIN_HEIGHT;
	} else if (g_rd_cal_param.case_d) {
		g_rd_region_info.region_b_start = g_rd_cal_param.dic_ys_cr3_2;
		g_rd_region_info.region_c_start = plus_one(g_rd_cal_param.dic_ye_cr3_2);
	} else if (g_rd_cal_param.case_e) {
		g_rd_region_info.region_b_start = g_rd_cal_param.dic_ys_cr3_2 - REGION_MIN_HEIGHT;
		g_rd_region_info.region_c_start = g_rd_cal_param.dic_ys_cr3_2;
	} else if (g_rd_cal_param.case_f) {
		g_rd_region_info.region_b_start = g_rd_cal_param.dic_ys_cr3_2;
		g_rd_region_info.region_c_start = PANEL_HEIGHT - REGION_MIN_HEIGHT;
	}

	g_rd_region_info.ip1_offset_b = (g_rd_region_info.region_b_start - CMDS_SIXTEEN) / CMDS_TWO * IP1_OFFSET_PARAM;
	g_rd_region_info.ip1_offset_c = (g_rd_region_info.region_c_start - CMDS_SIXTEEN) / CMDS_TWO * IP1_OFFSET_PARAM;
	g_rd_region_info.ip2_offset_b = (g_rd_region_info.region_b_start - CMDS_SIXTEEN) / CMDS_SIXTEEN * IP2_OFFSET_PARAM;
	g_rd_region_info.ip2_offset_c = (g_rd_region_info.region_c_start - CMDS_SIXTEEN) / CMDS_SIXTEEN * IP2_OFFSET_PARAM;
	return;
}

static void lcd_kit_get_rd_region_info(int panel_id)
{
	if (common_info->part_refresh.multi_region.is_in) {
		rd_roi_ys = common_info->part_refresh.multi_region.ys;
		rd_roi_ye = minus_one(common_info->part_refresh.multi_region.ye);
	} else {
		rd_roi_ys = common_info->part_refresh.ys;
		rd_roi_ye = minus_one(common_info->part_refresh.ye);
	}

	lcd_kit_rd_cal_case(panel_id);
	lcd_kit_rd_cal_region(panel_id);
	LCD_KIT_INFO("rd regions[%d %d %d %d %d %d]\n", g_rd_region_info.region_b_start,
		g_rd_region_info.region_c_start, g_rd_region_info.ip1_offset_b, g_rd_region_info.ip1_offset_c,
		g_rd_region_info.ip2_offset_b, g_rd_region_info.ip2_offset_c);
	return;
}

static void lcd_kit_get_local_coordinate_cmds(struct mipi_dsi_tx_params *params,
	int val, int num, int start, bool order)
{
	int i;
	int temp = 0;

	if (params == NULL) {
		LCD_KIT_ERR("params is null, or cmd_cnt <= 0!\n");
		return;
	}

	if (!order)
		temp = num;

	for (i = start; i <= start + num && temp >= 0; i++) {
		params->write_cmds.cmds[i].payload[PLYLOAD_ONE] =
			(val >> (temp * EIGHT_BIT)) & 0xff;
		if (order && temp < num) {
			temp++;
		} else if (temp > -1) {
			temp--;
		}
	}
}

static void lcd_kit_rd_get_part_refresh_cmds(int panel_id, struct mipi_dsi_tx_params *params)
{
	int i = CMDS_TWO;

	if (!params || !params->write_cmds.cmds) {
		LCD_KIT_ERR("params is null, or write_cmds is null!\n");
		return;
	}

	LCD_KIT_INFO("begin\n");
	params->write_cmds.cmds[i].payload[PLYLOAD_ONE] =
		(common_info->part_refresh.ys >> EIGHT_BIT) & 0xff;
	params->write_cmds.cmds[i].payload[PLYLOAD_TWO] =
		common_info->part_refresh.ys & 0xff;
	params->write_cmds.cmds[i].payload[PLYLOAD_THREE] =
		((common_info->part_refresh.ye - 1) >> EIGHT_BIT) & 0xff;
	params->write_cmds.cmds[i].payload[PLYLOAD_FOUR] =
		(common_info->part_refresh.ye - 1) & 0xff;
}

static void lcd_kit_rd_get_refresh_cmds_with_ip(int panel_id, struct mipi_dsi_tx_params *params)
{
	int i = CMDS_TWO;

	if (!params) {
		LCD_KIT_ERR("params is null, or cmd_cnt is worng!\n");
		return;
	}
	LCD_KIT_INFO("begin\n");
	lcd_kit_get_rd_region_info(panel_id);

	params->write_cmds.cmds[i].payload[PLYLOAD_ONE] =
		(common_info->part_refresh.ys >> EIGHT_BIT) & 0xff;
	params->write_cmds.cmds[i].payload[PLYLOAD_TWO] =
		common_info->part_refresh.ys & 0xff;
	params->write_cmds.cmds[i].payload[PLYLOAD_THREE] =
		((common_info->part_refresh.ye - 1) >> EIGHT_BIT) & 0xff;
	params->write_cmds.cmds[i].payload[PLYLOAD_FOUR] =
		(common_info->part_refresh.ye - 1) & 0xff;

	lcd_kit_get_local_coordinate_cmds(params, g_rd_region_info.region_b_start,
		EIGHT_BIT_ONE, CMDS_FIVE, true);
	lcd_kit_get_local_coordinate_cmds(params, g_rd_region_info.region_c_start,
		EIGHT_BIT_ONE, CMDS_SEVEN, true);
	lcd_kit_get_local_coordinate_cmds(params, g_rd_region_info.ip1_offset_b,
		EIGHT_BIT_TWO, CMDS_TEN, false);
	lcd_kit_get_local_coordinate_cmds(params, g_rd_region_info.ip1_offset_c,
		EIGHT_BIT_TWO, CMDS_THIRTEEN, false);
	lcd_kit_get_local_coordinate_cmds(params, g_rd_region_info.ip2_offset_b,
		EIGHT_BIT_THREE, CMDS_SEVENTEEN, false);
	lcd_kit_get_local_coordinate_cmds(params, g_rd_region_info.ip2_offset_c,
		EIGHT_BIT_THREE, CMDS_TWENTYONE, false);
}

void lcd_kit_rd_get_refresh_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params)
{
	LCD_KIT_INFO("get_rd_refresh_cmds begin\n");
	if (common_info->part_refresh.multi_region.is_in && !common_info->part_refresh.multi_region.cal_ip) {
		lcd_kit_mipi_tx_params_convert(panel_id,
			&common_info->part_refresh.bureau_brush_cmd, params, cmds_info);
		lcd_kit_rd_get_part_refresh_cmds(panel_id, params);
	} else {
		lcd_kit_mipi_tx_params_convert(panel_id,
			&common_info->part_refresh.part_refresh_cmd, params, cmds_info);
		lcd_kit_rd_get_refresh_cmds_with_ip(panel_id, params);
		common_info->part_refresh.multi_region.cal_ip = false;
	}
	return;
}

void lcd_kit_get_rd_region_refresh_info(const struct dkmd_cmds_info *cmds_info)
{
	if (!cmds_info || !cmds_info->data) {
		LCD_KIT_ERR("input params error\n");
		return;
	}

	int panel_id = get_panel_id();

	/* data[0] = x, data[1] = y, data[2] = w, data[3] = h */
	common_info->dirty_region.cmds.cmds[CMDS_ZERO].payload[PLYLOAD_ONE] =
		((cmds_info->data[COORADINATE_X]) >> EIGHT_BIT) & 0xFF;
	common_info->dirty_region.cmds.cmds[CMDS_ZERO].payload[PLYLOAD_TWO] =
		(cmds_info->data[COORADINATE_X]) & 0xff;
	common_info->dirty_region.cmds.cmds[CMDS_ZERO].payload[PLYLOAD_THREE] =
		((cmds_info->data[COORADINATE_X] + cmds_info->data[COORADINATE_W] - 1) >> EIGHT_BIT) & 0xff;
	common_info->dirty_region.cmds.cmds[CMDS_ZERO].payload[PLYLOAD_FOUR] =
		(cmds_info->data[COORADINATE_X] + cmds_info->data[COORADINATE_W] - 1) & 0xff;
	common_info->dirty_region.cmds.cmds[CMDS_ONE].payload[PLYLOAD_ONE] =
		((cmds_info->data[COORADINATE_Y]) >> EIGHT_BIT) & 0xff;
	common_info->dirty_region.cmds.cmds[CMDS_ONE].payload[PLYLOAD_TWO] =
		(cmds_info->data[COORADINATE_Y]) & 0xff;
	common_info->dirty_region.cmds.cmds[CMDS_ONE].payload[PLYLOAD_THREE] =
		((cmds_info->data[COORADINATE_Y] + cmds_info->data[COORADINATE_H] - 1) >> EIGHT_BIT) & 0xff;
	common_info->dirty_region.cmds.cmds[CMDS_ONE].payload[PLYLOAD_FOUR] =
		(cmds_info->data[COORADINATE_Y] + cmds_info->data[COORADINATE_H] - 1) & 0xff;
}
