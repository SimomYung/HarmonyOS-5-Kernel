/*
 * lcd_kit_dynamic_panel.c
 *
 * lcd kit dpu dynamic panel info function head file for lcd driver
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

#include "lcd_kit_utils.h"
#include "lcd_kit_disp.h"
#include "lcd_kit_parse.h"

#include <linux/sort.h>
#include <securec.h>

static void lcd_kit_parse_dynamic_pinfo_mipi(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo, struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	unsigned int pxl_clk_rate = 0;

	lcd_kit_parse_u32(np, "lcd-kit,dynamic-mipi-h-sync-area", &dynamic_pinfo->mipi.hsa, 0);
	lcd_kit_parse_u32(np, "lcd-kit,dynamic-mipi-h-back-porch", &dynamic_pinfo->mipi.hbp, 0);
	lcd_kit_parse_u32(np, "lcd-kit,dynamic-mipi-h-line-time", &dynamic_pinfo->mipi.hline_time, 0);
	lcd_kit_parse_u32(np, "lcd-kit,dynamic-mipi-dpi-h-size", &dynamic_pinfo->mipi.dpi_hsize, 0);
	lcd_kit_parse_u32(np, "lcd-kit,dynamic-mipi-v-sync-area", &dynamic_pinfo->mipi.vsa, 0);
	lcd_kit_parse_u32(np, "lcd-kit,dynamic-mipi-v-back-porch", &dynamic_pinfo->mipi.vbp, 0);
	lcd_kit_parse_u32(np, "lcd-kit,dynamic-mipi-v-front-porch", &dynamic_pinfo->mipi.vfp, 0);
	dynamic_pinfo->mipi.phy_mode = pinfo->mipi.phy_mode;
	dynamic_pinfo->mipi.lane_nums = pinfo->mipi.lane_nums;
	dynamic_pinfo->mipi.vc = pinfo->mipi.vc;
	lcd_kit_parse_u32(np, "lcd-kit,dynamic-mipi-dsi-bit-clk", &dynamic_pinfo->mipi.dsi_bit_clk, 0);
	lcd_kit_parse_u32(np, "lcd-kit,dynamic-panel-pxl-clk", &pxl_clk_rate, 0);
	dynamic_pinfo->mipi.pxl_clk_rate_div = pinfo->mipi.pxl_clk_rate_div;
	dynamic_pinfo->mipi.dsi_bit_clk_upt_support = pinfo->mipi.dsi_bit_clk_upt_support;
	dynamic_pinfo->mipi.non_continue_en = pinfo->mipi.non_continue_en;
	dynamic_pinfo->mipi.clk_post_adjust = pinfo->mipi.clk_post_adjust;
	dynamic_pinfo->mipi.clk_pre_adjust = pinfo->mipi.clk_pre_adjust;
	dynamic_pinfo->mipi.clk_t_hs_prepare_adjust = pinfo->mipi.clk_t_hs_prepare_adjust;
	dynamic_pinfo->mipi.clk_t_lpx_adjust = pinfo->mipi.clk_t_lpx_adjust;
	dynamic_pinfo->mipi.data_t_lpx_adjust = pinfo->mipi.data_t_lpx_adjust;
	dynamic_pinfo->mipi.clk_t_hs_trial_adjust = pinfo->mipi.clk_t_hs_trial_adjust;
	dynamic_pinfo->mipi.clk_t_hs_exit_adjust = pinfo->mipi.clk_t_hs_exit_adjust;
	dynamic_pinfo->mipi.clk_t_hs_zero_adjust = pinfo->mipi.clk_t_hs_zero_adjust;
	dynamic_pinfo->mipi.data_t_hs_zero_adjust = pinfo->mipi.data_t_hs_zero_adjust;
	dynamic_pinfo->mipi.data_t_hs_trial_adjust = pinfo->mipi.data_t_hs_trial_adjust;
	dynamic_pinfo->mipi.rg_vrefsel_eq_adjust = pinfo->mipi.rg_vrefsel_eq_adjust;
	dynamic_pinfo->mipi.rg_vrefsel_vcm_adjust = pinfo->mipi.rg_vrefsel_vcm_adjust;
	dynamic_pinfo->mipi.data_lane_lp2hs_time_adjust = pinfo->mipi.data_lane_lp2hs_time_adjust;
	dynamic_pinfo->mipi.data_t_hs_prepare_adjust = pinfo->mipi.data_t_hs_prepare_adjust;
	lcd_kit_parse_u8(np, "lcd-kit,dynamic-mipi-color-mode", &dynamic_pinfo->mipi.color_mode, 0);
	dynamic_pinfo->mipi.dsi_version= pinfo->mipi.dsi_version;
	dynamic_pinfo->mipi.max_tx_esc_clk= pinfo->mipi.max_tx_esc_clk;
	dynamic_pinfo->mipi.burst_mode= pinfo->mipi.burst_mode;

	dynamic_pinfo->mipi.pxl_clk_rate = (unsigned long long)pxl_clk_rate;
	// change MHz to Hz
	dynamic_pinfo->mipi.pxl_clk_rate = dynamic_pinfo->mipi.pxl_clk_rate * 1000000UL;

	dynamic_pinfo->mipi.dsi_bit_clk_upt = dynamic_pinfo->mipi.dsi_bit_clk;
	dynamic_pinfo->mipi.dsi_bit_clk_default = dynamic_pinfo->mipi.dsi_bit_clk;
}

static void lcd_kit_parse_dynamic_pinfo_dsc_param(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo, struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	dynamic_pinfo->input_dsc_info.dsc_version = 0;;
	dynamic_pinfo->input_dsc_info.format = 0;
	dynamic_pinfo->input_dsc_info.dsc_bpc = 0;
	dynamic_pinfo->input_dsc_info.dsc_bpp = 0;
	dynamic_pinfo->input_dsc_info.slice_height = 0;
	dynamic_pinfo->input_dsc_info.slice_width = 0;
	dynamic_pinfo->input_dsc_info.block_pred_enable = 0;
	dynamic_pinfo->input_dsc_info.linebuf_depth = 0;
	dynamic_pinfo->input_dsc_info.gen_rc_params = 0;
	dynamic_pinfo->input_dsc_info.pic_width = pinfo->input_dsc_info.pic_width;
	dynamic_pinfo->input_dsc_info.pic_height = pinfo->input_dsc_info.pic_height;
	return;
}

static int lcd_cmp_u32(const void *a, const void *b)
{
	if (*(const uint32_t *)a < *(const uint32_t *)b)
		return 1;
	else if (*(const uint32_t *)a > *(const uint32_t *)b)
		return -1;
	else
		return 0;
}

static void sort_dynamic_panel_info_fps_list(struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	unsigned int i;

	if (dynamic_pinfo->dfr_info.dfr_mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU)
		return;

	sort(dynamic_pinfo->dfr_info.oled_info.fps_sup_seq, dynamic_pinfo->dfr_info.oled_info.fps_sup_num,
		sizeof(uint32_t), lcd_cmp_u32, NULL);
	for (i = 0; i < dynamic_pinfo->dfr_info.oled_info.fps_sup_num; i++)
		LCD_KIT_INFO("dynamic_pinfo->dfr_info.oled_info.fps_sup_seq[%d] is %d\n",
			i, dynamic_pinfo->dfr_info.oled_info.fps_sup_seq[i]);
}

static int parse_dynamic_pinfo_ltpo_te_info(struct device_node *np,
	struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	struct lcd_kit_array_data te_freq = {0};
	struct lcd_kit_array_data te_mask = {0};
	int i;
	int ret;

	ret = lcd_kit_parse_array_data(np, "lcd-kit,dynamic-te-freq-info", &te_freq);
	if (ret != LCD_KIT_OK)
		 return LCD_KIT_FAIL;

	ret = lcd_kit_parse_array_data(np, "lcd-kit,dynamic-te-mask-info", &te_mask);
	if (ret != LCD_KIT_OK) {
		kfree(te_freq.buf);
		return LCD_KIT_FAIL;
	}
	LCD_KIT_INFO("te_freq.cnt is %d, te_mask.cnt is %d!\n", te_freq.cnt, te_mask.cnt);

	// te_freq.cnt and te_mask.cnt must be equal
	if (te_freq.cnt > TE_FREQ_NUM_MAX || te_mask.cnt > TE_FREQ_NUM_MAX || te_freq.cnt != te_mask.cnt) {
		LCD_KIT_ERR("te cnt para config error!\n");
		kfree(te_freq.buf);
		kfree(te_mask.buf);
		return LCD_KIT_FAIL;
	}

	dynamic_pinfo->dfr_info.oled_info.ltpo_info.te_freq_num = (uint32_t)te_freq.cnt;
	dynamic_pinfo->dfr_info.oled_info.ltpo_info.te_mask_num = (uint32_t)te_mask.cnt;
	for (i = 0; i < te_freq.cnt; i++) {
		dynamic_pinfo->dfr_info.oled_info.ltpo_info.te_freqs[i] = te_freq.buf[i];
		dynamic_pinfo->dfr_info.oled_info.ltpo_info.te_masks[i] = te_mask.buf[i];
		LCD_KIT_INFO("te_freq.buf[%d] is %d!\n", i, te_freq.buf[i]);
		LCD_KIT_INFO("te_mask.buf[%d] is %d!\n", i, te_mask.buf[i]);
	}
	kfree(te_freq.buf);
	kfree(te_mask.buf);
	return LCD_KIT_OK;
}

/* LTPO 120hz dimming seq */
static void parse_dynamic_pinfo_ltpo_120hz_dimming_seq(int panel_id,
	struct device_node *np, struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	struct lcd_kit_array_data dimming_seq = {0};
	struct lcd_kit_array_data repeat_num = {0};
	struct lcd_kit_array_data dimming_gear_seq = {0};
	unsigned int dimming_gear_len = 0;
	unsigned int dimming_gear_enable = 0;
	int i;
	int j;

	if ((lcd_kit_parse_array_data(np, "lcd-kit,dynamic-dimming-seq", &dimming_seq) == LCD_KIT_OK) &&
		(lcd_kit_parse_array_data(np, "lcd-kit,dynamic-repeat-num", &repeat_num) == LCD_KIT_OK)) {
		dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_sequence[DIMMING_SEQ_120HZ_INDEX].dimming_seq_num =
			(uint32_t)dimming_seq.cnt;
		for (i = 0; i < dimming_seq.cnt && dimming_seq.buf != NULL; i++) {
			dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_sequence[DIMMING_SEQ_120HZ_INDEX]
				.dimming_seq_list[i].frm_rate = dimming_seq.buf[i];
			dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_sequence[DIMMING_SEQ_120HZ_INDEX]
				.dimming_seq_list[i].repeat_num = repeat_num.buf[i];
			LCD_KIT_INFO("120hz frm_rate[%d] is %d!\n", i, dimming_seq.buf[i]);
			LCD_KIT_INFO("120hz repeat_num[%d] is %d!\n", i, repeat_num.buf[i]);
		}
		dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_sequence[DIMMING_SEQ_120HZ_INDEX].type =
			DIMMING_SEQ_120HZ_INDEX;
		kfree(dimming_seq.buf);
		kfree(repeat_num.buf);
	}

	lcd_kit_parse_u32(np, "lcd-kit,dynamic-dimming-gear-enable", &dimming_gear_enable, 0);
	dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_gear_enable = dimming_gear_enable;
    LCD_KIT_INFO("dimming-gear-enable:%u", dimming_gear_enable);

	lcd_kit_parse_u32(np, "lcd-kit,dynamic-dimming-gear-len", &dimming_gear_len, 0);
	dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_gear_len = dimming_gear_len;
	if ((lcd_kit_parse_array_data(np, "lcd-kit,dynamic-dimming-gear-seq", &dimming_gear_seq) == LCD_KIT_OK)) {
		for (i = 0, j = 0; i < (int)dimming_gear_len &&
			j <= dimming_gear_seq.cnt - DIMMING_GEAR_INFO_NUM; i++, j += DIMMING_GEAR_INFO_NUM) {
			dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_gear_config[i].frm_rate = dimming_gear_seq.buf[j];
			dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_gear_config[i].dimming_gear1 =
				dimming_gear_seq.buf[j + DIMMING_GEAR_INFO_NUM - 3];
			dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_gear_config[i].dimming_gear2 =
				dimming_gear_seq.buf[j + DIMMING_GEAR_INFO_NUM - 2];
			dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_gear_config[i].dimming_end =
				dimming_gear_seq.buf[j + DIMMING_GEAR_INFO_NUM - 1];
		}
		kfree(dimming_gear_seq.buf);
	}
}

static void parse_dynamic_pinfo_dynamic_pinfo_ltpo_base_cmd(int panel_id,
	struct device_node *np, struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	int i;
	int j;
	int cmd_num;
	struct lcd_kit_dsi_panel_cmds ltpo_base_cmd[] = {
		disp_info->fps.fps_to_90_cmds,
		disp_info->fps.fps_to_120_cmds,
		disp_info->fps.refresh_cmds,
		disp_info->fps.fps_to_1_cmds,
		disp_info->fps.bl_cmds,
		disp_info->fps.fps_to_360_cmds,
		disp_info->fps.fps_to_432_cmds,
		disp_info->fps.refresh_full_cmds,
		disp_info->fps.refresh_part_cmds,
	};

	cmd_num = sizeof(ltpo_base_cmd) / sizeof(ltpo_base_cmd[0]);
	for (i = 0; i < cmd_num; i++) {
		dynamic_pinfo->dfr_info.oled_info.ltpo_info.dsi_cmds[i].cmd_num = (uint32_t)ltpo_base_cmd[i].cmd_cnt;
		for (j = 0; j < ltpo_base_cmd[i].cmd_cnt; j++)
			lcd_kit_cmds_to_dsi_cmds(&ltpo_base_cmd[i].cmds[j],
				&dynamic_pinfo->dfr_info.oled_info.ltpo_info.dsi_cmds[i].cmds[j], true);
	}
}

static void dynamic_pinfo_safe_fps_cmd_to_dsi_cmds(int panel_id,
	struct dpu_dynamic_panel_info *dynamic_pinfo, unsigned int frame_rate, int num)
{
	int i;
	int j;
	int cmd_num;
	struct fps_cmd_table fps_safe_cmd[] = {
		{ FPS_1, &disp_info->fps.fps_to_1_cmds },
		{ FPS_10, &disp_info->fps.fps_to_10_cmds },
		{ FPS_15, &disp_info->fps.fps_to_15_cmds },
		{ FPS_18, &disp_info->fps.fps_to_18_cmds },
		{ FPS_20, &disp_info->fps.fps_to_20_cmds },
		{ FPS_24, &disp_info->fps.fps_to_24_cmds },
		{ FPS_30, &disp_info->fps.fps_to_30_cmds },
		{ FPS_40, &disp_info->fps.fps_to_40_cmds },
		{ FPS_45, &disp_info->fps.fps_to_45_cmds },
		{ FPS_48, &disp_info->fps.fps_to_48_cmds },
		{ FPS_60, &disp_info->fps.fps_to_60_cmds },
		{ FPS_72, &disp_info->fps.fps_to_72_cmds },
		{ FPS_90, &disp_info->fps.fps_to_90_cmds },
		{ FPS_120, &disp_info->fps.fps_to_120_cmds },
		{ FPS_144, &disp_info->fps.fps_to_144_cmds },
	};

	cmd_num = sizeof(fps_safe_cmd) / sizeof(fps_safe_cmd[0]);
	for (i = 0; i < cmd_num; i++) {
		if (frame_rate == (unsigned int)fps_safe_cmd[i].val) {
			dynamic_pinfo->dfr_info.oled_info.ltpo_info.safe_frm_rates[num].dsi_cmds.cmd_num =
			(uint32_t)fps_safe_cmd[i].cmds->cmd_cnt;
			for (j = 0; j < fps_safe_cmd[i].cmds->cmd_cnt; j++)
				lcd_kit_cmds_to_dsi_cmds(&fps_safe_cmd[i].cmds->cmds[j],
					&dynamic_pinfo->dfr_info.oled_info.ltpo_info.safe_frm_rates[num].dsi_cmds.cmds[j], true);
			break;
		}
	}
}

static void parse_dynamic_pinfo_ltpo_safe_cmd(int panel_id,
	struct device_node *np, struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	int i;
	struct lcd_kit_array_data safe_freq = {0};

	lcd_kit_parse_array_data(np, "lcd-kit,dynamic-safe-frame-info", &safe_freq);
	dynamic_pinfo->dfr_info.oled_info.ltpo_info.safe_frm_rates_num = (uint32_t)safe_freq.cnt;
	if (safe_freq.cnt > SAFE_FRM_RATE_MAX_NUM) {
		LCD_KIT_ERR("safe_freq.cnt is %d too big!\n", safe_freq.cnt);
		kfree(safe_freq.buf);
		return;
	}
	LCD_KIT_INFO("safe_freq.cnt is %d!\n", safe_freq.cnt);
	for (i = 0; i < safe_freq.cnt; i++) {
		dynamic_pinfo->dfr_info.oled_info.ltpo_info.safe_frm_rates[i].safe_frm_rate = safe_freq.buf[i];
		LCD_KIT_INFO("safe_freq.buf[%d] is %d!\n", i, safe_freq.buf[i]);
		dynamic_pinfo_safe_fps_cmd_to_dsi_cmds(panel_id, dynamic_pinfo, safe_freq.buf[i], i);
	}
	kfree(safe_freq.buf);
}

static void parse_dynamic_pinfo_ltpo2_info(int panel_id,
	struct device_node *np, struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	int ret;

	ret = parse_dynamic_pinfo_ltpo_te_info(np, dynamic_pinfo);
	if (ret != LCD_KIT_OK)
		 return;

	parse_dynamic_pinfo_ltpo_120hz_dimming_seq(panel_id, np, dynamic_pinfo);
	parse_dynamic_pinfo_dynamic_pinfo_ltpo_base_cmd(panel_id, np, dynamic_pinfo);
	parse_dynamic_pinfo_ltpo_safe_cmd(panel_id, np, dynamic_pinfo);
}

static void parse_dynamic_pinfo__mipi_porch_info(int panel_id,
	struct device_node *np, struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	int i;
	struct lcd_kit_array_data *fps_timing;

	dynamic_pinfo->mipi.mipi_frm_rate_mode_num = (uint32_t)disp_info->fps.panel_support_fps_list.cnt;
	for (i = 0; i < (int)dynamic_pinfo->mipi.mipi_frm_rate_mode_num; i++) {
		fps_timing = &disp_info->fps.fps_dsi_timming[i];
		dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.hsa = fps_timing->buf[FPS_HSA];
		LCD_KIT_INFO("dynamic_pinfo->mipi.mipi_timing_modes[%d].mipi_timing.hsa = %d\n",
			i, dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.hsa);
		dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.hbp = fps_timing->buf[FPS_HBP];
		LCD_KIT_INFO("dynamic_pinfo->mipi.mipi_timing_modes[%d].mipi_timing.hbp = %d\n",
			i, dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.hbp);
		dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.dpi_hsize = fps_timing->buf[FPS_HSIZE];
		LCD_KIT_INFO("dynamic_pinfo->mipi.mipi_timing_modes[%d].mipi_timing.dpi_hsize = %d\n",
			i, dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.dpi_hsize);
		dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.hline_time = fps_timing->buf[FPS_HLINE];
		LCD_KIT_INFO("dynamic_pinfo->mipi.mipi_timing_modes[%d].mipi_timing.hline_time = %d\n",
			i, dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.hline_time);
		dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.vsa = fps_timing->buf[FPS_VSA];
		LCD_KIT_INFO("dynamic_pinfo->mipi.mipi_timing_modes[%d].mipi_timing.vsa = %d\n",
			i, dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.vsa);
		dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.vbp = fps_timing->buf[FPS_VBP];
		LCD_KIT_INFO("dynamic_pinfo->mipi.mipi_timing_modes[%d].mipi_timing.vbp = %d\n",
			i, dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.vbp);
		dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.vfp = fps_timing->buf[FPS_VFP];
		LCD_KIT_INFO("dynamic_pinfo->mipi.mipi_timing_modes[%d].mipi_timing.vfp = %d\n",
			i, dynamic_pinfo->mipi.mipi_timing_modes[i].mipi_timing.vfp);
		dynamic_pinfo->mipi.mipi_timing_modes[i].dsi_bit_clk_default = fps_timing->buf[FPS_DSI_CLK];
		LCD_KIT_INFO("dynamic_pinfo->mipi.mipi_timing_modes[%d].dsi_bit_clk_default = %d\n",
			i, dynamic_pinfo->mipi.mipi_timing_modes[i].dsi_bit_clk_default);
		dynamic_pinfo->mipi.mipi_timing_modes[i].frame_rate = disp_info->fps.panel_support_fps_list.buf[i];
		LCD_KIT_INFO("dynamic_pinfo->mipi.mipi_timing_modes[%d].frame_rate = %d\n",
			i, dynamic_pinfo->mipi.mipi_timing_modes[i].frame_rate);
	}
}

static void lcd_kit_parse_dynamic_pinfo_pinfo_fps(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo, struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	int i;

	if (!disp_info->fps.support) {
		LCD_KIT_INFO("not support fps\n");
		return;
	}

	lcd_kit_parse_u32(np, "lcd-kit,dynamic-dfr-mode", &dynamic_pinfo->dfr_info.dfr_mode, 0);
	dynamic_pinfo->dfr_info.ddic_type = pinfo->dfr_info.ddic_type;
	dynamic_pinfo->dfr_info.oled_info.oled_type = pinfo->dfr_info.oled_info.oled_type;
	dynamic_pinfo->dfr_info.oled_info.fps_sup_num = (uint32_t)disp_info->fps.panel_support_fps_list.cnt;

	for (i = 0; i < (int)dynamic_pinfo->dfr_info.oled_info.fps_sup_num; i++) {
		dynamic_pinfo->dfr_info.oled_info.fps_sup_seq[i] = disp_info->fps.panel_support_fps_list.buf[i];
		LCD_KIT_INFO("dynamic_pinfo->dfr_info.oled_info.fps_sup_seq[%d] is %d\n",
			i, dynamic_pinfo->dfr_info.oled_info.fps_sup_seq[i]);
	}

	dynamic_pinfo->dfr_info.oled_info.ltpo_info.ver = pinfo->dfr_info.oled_info.ltpo_info.ver;
	lcd_kit_parse_u32(np, "lcd-kit,dynamic-dimming-mode", &dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_mode, 0);
	dynamic_pinfo->dfr_info.oled_info.ltpo_info.te2_enable = 0;

#ifdef LCD_FACTORY_MODE
	dynamic_pinfo->dfr_info.dfr_mode = 0;
#endif
	dynamic_pinfo->dfr_info.oled_info.ltpo_info.dimming_enable = 0;

	sort_dynamic_panel_info_fps_list(dynamic_pinfo);
	if (dynamic_pinfo->dfr_info.oled_info.ltpo_info.ver == PANEL_LTPO_V2)
		parse_dynamic_pinfo_ltpo2_info(panel_id, np, dynamic_pinfo);
	if (dynamic_pinfo->dfr_info.dfr_mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU)
		parse_dynamic_pinfo__mipi_porch_info(panel_id, np, dynamic_pinfo);
	return;
}

static void lcd_kit_parse_dynamic_pinfo(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo, struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	if (!dynamic_pinfo) {
		LCD_KIT_ERR("dynamic_pinfo is null\n");
		return;
	}

	lcd_kit_parse_u32(np, "lcd-kit,dynamic-panel-ifbc-type", &dynamic_pinfo->ifbc_type, 0);
	lcd_kit_parse_dynamic_pinfo_mipi(panel_id, np, pinfo, dynamic_pinfo);
	lcd_kit_parse_dynamic_pinfo_dsc_param(panel_id, np, pinfo, dynamic_pinfo);
	lcd_kit_parse_dynamic_pinfo_pinfo_fps(panel_id, np, pinfo, dynamic_pinfo);
}


int lcd_kit_dynamic_panel_info_init(int panel_id, struct device_node *np,
	struct dpu_panel_info *pinfo, struct dpu_dynamic_panel_info *dynamic_pinfo)
{
	lcd_kit_parse_u8(np, "lcd-kit,dsc-switch-enable", &pinfo->dsc_switch_enable, 0);
	if(pinfo->dsc_switch_enable == 0) {
		LCD_KIT_ERR("dsc_switch_enable is not support\n");
		return LCD_KIT_OK;
	}
	/* init dynamic_pinfo */
	lcd_kit_parse_dynamic_pinfo(panel_id, np, pinfo, dynamic_pinfo);
	return LCD_KIT_OK;
}

