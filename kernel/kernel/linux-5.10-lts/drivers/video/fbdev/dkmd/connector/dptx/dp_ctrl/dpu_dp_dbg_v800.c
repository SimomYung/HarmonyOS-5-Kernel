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

#include <linux/module.h>
#include "dp_ctrl.h"
#include "dp_aux.h"
#include "dp_drv.h"
#include "dp_ctrl_dev.h"
#include <huawei_platform/dp_source/dp_source_switch.h>
#include "controller/psr/psr_config_base.h"
#include "dp_hdr.h"
#include "hidptx/hidptx_reg.h"
#include "dp_phy_config_interface.h"
#include <securec.h>
#include "dpu_dp_dbg_v800.h"

/*******************************************************************************
 * DP GRAPHIC DEBUG TOOL
 */
static int32_t g_dp_id = 0;
#define DPID_MIN 0
#define DPID_MAX 1

int g_clk_hblank_code_enable = 0;
/* 27M, 720*480 */
uint32_t g_pixel_clock = 27000;
uint32_t g_video_code = 3;
uint32_t g_hblank = 138;
/*
 * for debug, S_IRUGO
 * /sys/module/dp_ctrl/parameters
 */
int g_dp_debug_mode_enable = 0;
int g_dp_debug_jump_linktraining_enable = 0;
static uint32_t g_dp_preemphasis_swing[DPTX_COMBOPHY_PARAM_NUM];
int g_dp_pe_sw_length = 0;
static uint32_t g_dp_preemphasis_swing1[DPTX_COMBOPHY_PARAM_NUM];
int g_dp_pe_sw_attach_length = 0;
int g_dp_vactive_pixels_debug = 0;
int g_dp_hactive_pixels_debug = 3840;
int g_dp_same_source_debug = 1;
int g_dp_dmd_delay_time = 172800000; // 24x2 小时
/* rate and ssc config, rate_coef0, rate_coef1, rate_coef2 */
uint32_t g_dp_rbr_pll1_rate0_coef[4] = {0x6546061C, 0x54600000, 0xA900AB, 0x131};
int g_dp_rbr_pll1_rate0_coef_length = 0;
uint32_t g_dp_hbr_pll1_rate1_coef[4] = {0x64640618, 0x46500000, 0x8C008E, 0x131};
int g_dp_hbr_pll1_rate1_coef_length = 0;
uint32_t g_dp_hbr2_pll1_rate2_coef[4] = {0x64640615, 0x46500000, 0x8C008E, 0x131};
int g_dp_hbr2_pll1_rate2_coef_length = 0;
uint32_t g_dp_hbr3_pll1_rate3_coef[4] = {0x46960610, 0x69780000, 0xD300D5, 0x131};
int g_dp_hbr3_pll1_rate3_coef_length = 0;
uint32_t g_edp_r216_rate0_coef[4] = {0x47083418, 0x70800000, 0xE200E4, 0x131};
int g_edp_r216_rate0_coef_length = 0;
uint32_t g_edp_r243_rate1_coef[4] = {0x63F43418, 0x3F480000, 0x7E0080, 0x131};
int g_edp_r243_rate1_coef_length = 0;
uint32_t g_edp_r324_rate2_coef[4] = {0x65463418, 0x54600000, 0xA900AB, 0x131};
int g_edp_r324_rate2_coef_length = 0;
uint32_t g_edp_r432_rate3_coef[4] = {0x47083414, 0x70800000, 0xE200E4, 0x131};
int g_edp_r432_rate3_coef_length = 0;
uint32_t g_edp_custom_rate4_coef[4] = {0x44103410, 0x69780000, 0xD300D5, 0x131};
int g_edp_custom_rate4_coef_length = 0;

int g_update_region_flag = 0;


#define DPTX_PHYIF_CTRL_RATE_RBR	0x0
#define DPTX_PHYIF_CTRL_RATE_HBR	0x1
#define DPTX_PHYIF_CTRL_RATE_HBR2	0x2

static struct dp_ctrl *g_dptx_debug[2];
struct dp_debug_params g_dp_params[2];

static struct dp_debug_params default_dp_params = {
	4, 2, 0, 0, 0, 0, 1, 0, 3840, 0x11630,
	{0x054610df, 0x54600000, 0xA900AB, 0x131},
	{0x046510da, 0x46500000, 0x8C008E, 0x131},
	{0x046500d5, 0x8CA00000, 0x11c0119, 0x131},
	1, {{}, {}}, 0, 0, 0, 27000, 3, 138, -1, 0, false, 0x7ff0e0d, 0,
};

void dp_graphic_debug_node_init(struct dp_ctrl *dptx)
{
	if (dptx->id < DPID_MIN || dptx->id >= DPID_MAX) {
		dpu_pr_err("[DP] index or type is invaild, index %d", dptx->port_id);
		return;
	}
	g_dptx_debug[dptx->id] = dptx;
	g_dp_params[dptx->id] = default_dp_params;
	g_dp_debug_mode_enable = 0;
}

void dptx_debug_get_double_tx_sync_para(struct dp_ctrl *dptx)
{
	void_unused(dptx);

	return;
}

static bool dp_lane_num_valid(int dp_lane_num_debug)
{
	if (dp_lane_num_debug == 1 || dp_lane_num_debug == 2 || dp_lane_num_debug == 4)
		return true;

	return false;
}

int dptx_debug_get_combophy_rate_param_csv2(struct dp_ctrl *dptx, int dptx_phyif_ctrl_rate,
	struct combophy_rate_csv2 *combophy_rate)
{
	void_unused(dptx);
	void_unused(dptx_phyif_ctrl_rate);
	void_unused(combophy_rate);
	return 0;
}

int dptx_debug_get_dptxphy_rate_param_csv2(struct dp_ctrl *dptx, int dptx_phyif_ctrl_rate,
	struct dptx_phy_rate *dptx_rate)
{
	void_unused(dptx);
	void_unused(dptx_phyif_ctrl_rate);
	void_unused(dptx_rate);
	return 0;
}

int dp_debug_set_psr_state(int psr_state)
{
	void_unused(psr_state);
	return 0;
}

int dp_debug_get_psr_state(void)
{
	return 0;
}

int dp_debug_set_hpd_state(int hpd_state)
{
	void_unused(hpd_state);
	return 0;
}

int dp_debug_get_hpd_state(void)
{
	return 0;
}

uint32_t dptx_debug_get_combophy_rate_param(struct dp_ctrl *dptx, int dptx_phyif_ctrl_rate,
	uint32_t *ckgctrl, uint32_t *sscginital, uint32_t *sscgcnt, uint32_t *sscgcnt2)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	if (g_dptx_debug[g_dp_id] == NULL) {
		dpu_pr_err("[DP] debug dptx is null!");
		return 0xff;
	}
	switch (dptx_phyif_ctrl_rate) {
	case DPTX_PHYIF_CTRL_RATE_RBR:
		*ckgctrl = g_dp_params[dptx->id].dp_rbr_pll1_rate0_coef[0];
		*sscginital = g_dp_params[dptx->id].dp_rbr_pll1_rate0_coef[1];
		*sscgcnt= g_dp_params[dptx->id].dp_rbr_pll1_rate0_coef[2];
		*sscgcnt2 = g_dp_params[dptx->id].dp_rbr_pll1_rate0_coef[3];
		break;
	case DPTX_PHYIF_CTRL_RATE_HBR:
		*ckgctrl = g_dp_params[dptx->id].dp_hbr_pll1_rate1_coef[0];
		*sscginital = g_dp_params[dptx->id].dp_hbr_pll1_rate1_coef[1];
		*sscgcnt= g_dp_params[dptx->id].dp_hbr_pll1_rate1_coef[2];
		*sscgcnt2 = g_dp_params[dptx->id].dp_hbr_pll1_rate1_coef[3];
		break;
	case DPTX_PHYIF_CTRL_RATE_HBR2:
		*ckgctrl = g_dp_params[dptx->id].dp_hbr2_pll1_rate2_coef[0];
		*sscginital = g_dp_params[dptx->id].dp_hbr2_pll1_rate2_coef[1];
		*sscgcnt= g_dp_params[dptx->id].dp_hbr2_pll1_rate2_coef[2];
		*sscgcnt2 = g_dp_params[dptx->id].dp_hbr2_pll1_rate2_coef[3];
		break;
	default:
		dpu_pr_err("[DP] Invalid PHY rate %d", dptx_phyif_ctrl_rate);
		return 0xff;
	}

	return 0;
}

static bool dp_lane_rate_valid(int dp_lane_rate_debug)
{
	if (dp_lane_rate_debug >= 0 && dp_lane_rate_debug <= DPTX_PHYIF_CTRL_RATE_HBR2)
		return true;
	dpu_pr_err("[DP] Invalid rate %d", dp_lane_rate_debug);
	return false;
}

void dptx_debug_set_params(struct dp_ctrl *dptx)
{
	int i;

	dpu_check_and_no_retval(!dptx, err, "[DP] dptx is null pointor!");

	if (g_dp_debug_mode_enable == 0)
		return;

	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return;
	}
	if (g_dptx_debug[g_dp_id] == NULL) {
		dpu_pr_err("[DP] debug dptx is null!");
		return;
	}

	if (dp_lane_num_valid(g_dp_params[dptx->id].dp_lane_num_debug))
		dptx->max_lanes = (uint8_t)g_dp_params[dptx->id].dp_lane_num_debug;

	if (dp_lane_rate_valid(g_dp_params[dptx->id].dp_lane_rate_debug)) {
		dptx->max_rate = (uint8_t)g_dp_params[dptx->id].dp_lane_rate_debug;
		dpu_pr_info("[DP] set rate %d", dptx->max_rate);
	}

	dptx->max_rcm_rate = dptx->max_rate;
	dptx->mst = (bool)g_dp_params[dptx->id].dp_mst_enable_debug;
	dptx->ssc_en = (bool)g_dp_params[dptx->id].dp_ssc_enable_debug;
	dptx->dptx_link_params.compress_params.fec = (bool)g_dp_params[dptx->id].dp_fec_enable_debug;
	dptx->dsc = (bool)g_dp_params[dptx->id].dp_dsc_enable_debug;
	dptx->efm_en = (bool)g_dp_params[dptx->id].dp_efm_enable_debug;

	dptx->connector->conn_info->base.mode = g_dp_params[dptx->id].dp_mode;
	dpu_pr_info("[DP] set dp_mode : %d, ssc:%d, phy_param_num:%u\n",
		dptx->connector->conn_info->base.mode, dptx->ssc_en, dptx->combophy_param_num);

	if (g_dp_params[dptx->id].dp_pe_sw_length == (int)dptx->combophy_param_num) {
		for (i = 0; i < g_dp_params[dptx->id].dp_pe_sw_length; i++)
			dptx->combophy_pree_swing[DPTX_PREE_SWING_DEFAULT][i] =
				g_dp_params[dptx->id].dp_preemphasis_swing[DPTX_PREE_SWING_DEFAULT][i];
	}
	if (g_dp_params[dptx->id].dp_pe_sw_attach_length == dptx->combophy_param_num) {
		for (i = 0; i < g_dp_params[dptx->id].dp_pe_sw_attach_length; i++)
			dptx->combophy_pree_swing[DPTX_PREE_SWING_EXTERA][i] =
				g_dp_params[dptx->id].dp_preemphasis_swing[DPTX_PREE_SWING_EXTERA][i];
	}
}

bool dp_debug_get_clk_hblank_enable(struct dp_ctrl *dptx)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return false;
	}
	if (g_dptx_debug[g_dp_id] == NULL) {
		dpu_pr_err("[DP] debug dptx is null!");
		return false;
	}
	if (g_dp_params[dptx->id].clk_hblank_code_enable)
		return true;

	return false;
}

uint dp_debug_get_aux_ctrl_param(struct dp_ctrl *dptx)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	if (g_dptx_debug[g_dp_id] == NULL) {
		dpu_pr_err("[DP] debug dptx is null!");
		return 0xff;
	}

	return g_dp_params[dptx->id].dp_aux_ctrl;
}

void dp_debug_get_clk_hblank_params(struct dp_ctrl *dptx, uint64_t *pixel_clock,
	uint32_t *video_code, uint16_t *hblank)
{
	*pixel_clock = g_pixel_clock;
	*video_code = g_video_code;
	*hblank = g_hblank;

	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return ;
	}
	if (g_dptx_debug[g_dp_id] == NULL) {
		dpu_pr_err("[DP] debug dptx is null!");
		return;
	}

	*pixel_clock = g_dp_params[dptx->id].pixel_clock_debug;
	*video_code = g_dp_params[dptx->id].video_code_debug;
	*hblank = g_dp_params[dptx->id].hblank_debug;
}

int dp_debug_set_lane_num(int lane_num)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}

	if (lane_num > 4 || lane_num < 1) {
		dpu_pr_err("[DP] lane_num is invaild, lane_num %d", lane_num);
		return 0xff;
	}

	g_dp_params[g_dp_id].dp_lane_num_debug = lane_num;
	dpu_pr_info("[DP] dp_debug_set_lane_num set lane_num: %d", lane_num);
	return 0;
}

int dp_debug_get_lane_num(void)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_get_lane_num get dp_lane_num_debug: %d", g_dp_params[g_dp_id].dp_lane_num_debug);
	return g_dp_params[g_dp_id].dp_lane_num_debug;
}

int dp_debug_set_pll_value(uint32_t value)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	g_dp_params[g_dp_id].pmcc_pll = value;
	dpu_pr_info("[DP] dp_debug_set_pll_value set lane_num: 0x%x", value);
	return 0;
}

uint32_t dp_debug_get_pll_value(void)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_get_pll_value get pmcc_pll: %u", g_dp_params[g_dp_id].pmcc_pll);
	return g_dp_params[g_dp_id].pmcc_pll;
}

int dp_debug_set_lane_rate(int lane_rate)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	g_dp_params[g_dp_id].dp_lane_rate_debug = lane_rate;
	dpu_pr_info("[DP] dp_debug_set_lane_rate set lane_rate: %d", lane_rate);
	return 0;
}

int dp_debug_get_lane_rate(void)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_get_lane_rate get dp_lane_rate_debug: %d", g_dp_params[g_dp_id].dp_lane_rate_debug);
	return g_dp_params[g_dp_id].dp_lane_rate_debug;
}

int dp_debug_set_mst_mode(int mst_mode)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	g_dp_params[g_dp_id].dp_mst_enable_debug = mst_mode;
	dpu_pr_info("[DP] dp_debug_set_mst_mode set mst_mode: %d", mst_mode);
	return 0;
}

int dp_debug_get_mst_mode(void)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_get_mst_mode get dp_mst_enable_debug: %d", g_dp_params[g_dp_id].dp_mst_enable_debug);
	return g_dp_params[g_dp_id].dp_mst_enable_debug;
}

int dp_debug_set_ssc_mode(int ssc_mode)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	g_dp_params[g_dp_id].dp_ssc_enable_debug = ssc_mode;
	dpu_pr_info("[DP] dp_debug_set_ssc_mode set ssc_mode: %d", ssc_mode);
	return 0;
}

int dp_debug_get_ssc_mode(void)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_get_ssc_mode get dp_ssc_enable_debug: %d", g_dp_params[g_dp_id].dp_ssc_enable_debug);
	return g_dp_params[g_dp_id].dp_ssc_enable_debug;
}

int dp_debug_set_fec_mode(int fec_mode)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	g_dp_params[g_dp_id].dp_fec_enable_debug = fec_mode;
	dpu_pr_info("[DP] dp_debug_set_fec_mode set fec_mode: %d", fec_mode);
	return 0;
}

int dp_debug_get_fec_mode(void)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_get_fec_mode get dp_fec_enable_debug: %d", g_dp_params[g_dp_id].dp_fec_enable_debug);
	return g_dp_params[g_dp_id].dp_fec_enable_debug;
}

int dp_debug_set_dsc_mode(int dsc_mode)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	g_dp_params[g_dp_id].dp_dsc_enable_debug = dsc_mode;
	dpu_pr_info("[DP] dp_debug_set_dsc_mode set dsc_mode: %d", dsc_mode);
	return 0;
}

int dp_debug_get_dsc_mode(void)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_get_dsc_mode get dp_dsc_enable_debug: %d", g_dp_params[g_dp_id].dp_dsc_enable_debug);
	return g_dp_params[g_dp_id].dp_dsc_enable_debug;
}

int dp_debug_set_dp_mode(int dp_mode)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	g_dp_params[g_dp_id].dp_mode = (uint32_t)dp_mode;
	dpu_pr_info("[DP] dp_debug_set_dp_mode set dp_mode: %d", dp_mode);
	return 0;
}

int dp_debug_get_dp_mode(void)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_get_dp_mode get dp_mode_enable_debug: %d", g_dp_params[g_dp_id].dp_mode);
	return (int)g_dp_params[g_dp_id].dp_mode;
}

int dp_debug_combophy_set_rate(void)
{
	int i;

	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_combophy_set_rate, length:%d",  g_dp_rbr_pll1_rate0_coef_length);
	if (g_dp_rbr_pll1_rate0_coef_length >= 4) {
		dpu_pr_err("[DP] debug g_dp_rbr_pll1_rate0_coef_length is invalid!");
		return 0xff;
	}
	for (i = 0; i < g_dp_rbr_pll1_rate0_coef_length; i++) {
		g_dp_params[g_dp_id].dp_rbr_pll1_rate0_coef[i] = g_dp_rbr_pll1_rate0_coef[i];
		g_dp_params[g_dp_id].dp_hbr_pll1_rate1_coef[i] = g_dp_hbr_pll1_rate1_coef[i];
		g_dp_params[g_dp_id].dp_hbr2_pll1_rate2_coef[i] = g_dp_hbr2_pll1_rate2_coef[i];
		dpu_pr_info("[DP] [%d]dp_debug_combophy_set_rate set rate: 1. %#x, 2. %#x, 3. %#x",
			i, g_dp_rbr_pll1_rate0_coef[i], g_dp_hbr_pll1_rate1_coef[i],
			g_dp_hbr2_pll1_rate2_coef[i]);
	}
	return 0;
}

int dp_debug_set_efm_mode(int efm_mode)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	if (g_dptx_debug[g_dp_id] == NULL) {
		dpu_pr_err("[DP] debug dptx is null!");
		return 0xff;
	}
	g_dp_params[g_dp_id].dp_efm_enable_debug = efm_mode;
	dpu_pr_info("[DP] dp_debug_set_efm_mode set efm_mode: %d", efm_mode);
	return 0;
}

int dp_debug_get_efm_mode(void)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	if (g_dptx_debug[g_dp_id] == NULL) {
		dpu_pr_err("[DP] debug dptx is null!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_get_efm_mode get dp_efm_enable_debug: %d", g_dp_params[g_dp_id].dp_efm_enable_debug);
	return g_dp_params[g_dp_id].dp_efm_enable_debug;
}

int dp_debug_set_aux_ctrl(uint aux_ctrl)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	if (g_dptx_debug[g_dp_id] == NULL) {
		dpu_pr_err("[DP] debug dptx is null!");
		return 0xff;
	}
	g_dp_params[g_dp_id].dp_aux_ctrl = aux_ctrl;
	dpu_pr_info("[DP] dp_debug_set_aux_ctrl set aux_ctrl: %u", aux_ctrl);
	return 0;
}

int dp_debug_get_aux_ctrl(void)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	if (g_dptx_debug[g_dp_id] == NULL) {
		dpu_pr_err("[DP] debug dptx is null!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_get_aux_ctrl get dp_aux_ctrl: %u", g_dp_params[g_dp_id].dp_aux_ctrl);
	return (int)g_dp_params[g_dp_id].dp_aux_ctrl;
}

int dp_debug_set_preemphasis_swing(void)
{
	int i;

	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	if (g_dp_pe_sw_length >= DPTX_COMBOPHY_PARAM_NUM) {
		dpu_pr_err("[DP] debug g_dp_pe_sw_length is invalid!");
		return 0xff;
	}
	if (g_dp_pe_sw_attach_length >= DPTX_COMBOPHY_PARAM_NUM) {
		dpu_pr_err("[DP] debug g_dp_pe_sw_attach_length is invalid!");
		return 0xff;
	}
	g_dp_params[g_dp_id].dp_pe_sw_length = g_dp_pe_sw_length;
	for (i = 0; i < g_dp_pe_sw_length; i++) {
		g_dp_params[g_dp_id].dp_preemphasis_swing[DPTX_PREE_SWING_DEFAULT][i] =
			g_dp_preemphasis_swing[i];
		dpu_pr_info("[DP] dp_debug_set_preemphasis_swing set swing[%d]: %#x", i, g_dp_preemphasis_swing[i]);
	}
	g_dp_params[g_dp_id].dp_pe_sw_attach_length = (uint32_t)g_dp_pe_sw_attach_length;
	for (i = 0; i < g_dp_pe_sw_attach_length; i++) {
		g_dp_params[g_dp_id].dp_preemphasis_swing[DPTX_PREE_SWING_EXTERA][i] =
			g_dp_preemphasis_swing1[i];
		dpu_pr_info("[DP] dp_debug_set_preemphasis_swing set swing[%d]: %#x", i, g_dp_preemphasis_swing1[i]);
	}

	return 0;
}

int dp_debug_dptx_connected(void)
{
	struct dp_ctrl *dptx = NULL;
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_dptx_connected");
	dptx = g_dptx_debug[g_dp_id];
	if (!dptx) {
		dpu_pr_err("[DP] dptx is null!");
		return 0xff;
	}
	if (dptx->video_transfer_enable)
		return 1;

	return 0;
}

int dp_debug_read_dpcd(uint32_t addr)
{
	uint8_t byte = 0;
	int retval;
	struct dp_ctrl *dptx = NULL;

	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dptx = g_dptx_debug[g_dp_id];
	if (!dptx || !dptx->dptx_enable || dptx->power_saving_mode) {
		dpu_pr_err("[DP] dptx is null or has already been off!");
		return 0xff;
	}
	dpu_pr_info("[DP] dp_debug_read_dpcd\n");
	retval = dptx_read_dpcd(dptx, addr, &byte);
	if (retval) {
		   dpu_pr_err("[DP] read dpcd fail");
		   return retval;
	}
	return (int)byte;
}

int dp_debug_write_dpcd(uint32_t addr, uint8_t byte)
{
	int retval;
	struct dp_ctrl *dptx = NULL;

	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dptx = g_dptx_debug[g_dp_id];
	if (!dptx) {
		dpu_pr_err("[DP] dptx is null!");
		return 0xff;
	}
	retval = dptx_write_dpcd(dptx, addr, byte);
	if (retval) {
		dpu_pr_err("[DP] write dpcd fail");
		return retval;
	}
	dpu_pr_info("[DP] dp_debug_write_dpcd");
	return 0;
}

int set_dp_rx_tx_mode(uint32_t dp_rx_tx_mode)
{
	struct dp_ctrl *dptx = NULL;
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dptx = g_dptx_debug[g_dp_id];	
	dptx->source_is_external = dp_rx_tx_mode;
	return 0;
}

int set_dp_rx_tx_arsr_mode(uint32_t dp_rx_tx_arsr_mode)
{
	struct dp_ctrl *dptx = NULL;
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	dptx = g_dptx_debug[g_dp_id];
	dptx->arsr_enable = dp_rx_tx_arsr_mode;
	dpu_pr_info("[DP] set_dp_rx_tx_arsr_mode set dp_rx_tx_arsr_mode: %u", dp_rx_tx_arsr_mode);
	return 0;
}

int set_dp_mode(uint32_t mode)
{
	struct dp_ctrl *dptx = NULL;
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	if (mode >= DP_MODE_MAX)
		mode = DP_MODE;

	dptx = g_dptx_debug[g_dp_id];
	dptx->connector->conn_info->base.mode = mode;
	dpu_pr_info("[DP] set_dp_mode set dp_mode: %u", mode);
	return 0;
}

int set_clk_hblank_code(uint32_t clock, uint32_t hblank, uint32_t code)
{
	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}
	g_dp_params[g_dp_id].pixel_clock_debug = clock;
	g_dp_params[g_dp_id].hblank_debug = hblank;
	g_dp_params[g_dp_id].video_code_debug = code;
	dpu_pr_info("[DP] set_clk_hblank_code set pixel_clock_debug: %u, pixel_clock_debug: %u, pixel_clock_debug: %u",
		clock, hblank, code);
	return 0;
}

int set_dp_psr_mode(uint32_t mode)
{
	struct dp_ctrl *dptx = NULL;

	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}

	dptx = g_dptx_debug[g_dp_id];
	if (!dptx) {
		dpu_pr_err("[DP] dptx is null!");
		return 0xff;
	}

	if (mode > PSR_EXIT)
		dptx_psr_entry(dptx);
	else
		dptx_change_psr_status(dptx, mode);
	dpu_pr_info("[DP] set_dp_psr_mode mode:%u", mode);
	return 0;
}

int select_dp_port(int port_id)
{
	if (port_id >= 2 || port_id < 0)
		g_dp_id = 0;
	else
		g_dp_id = port_id;

	if (g_dptx_debug[g_dp_id] == NULL) {
		dpu_pr_err("[DP] debug dptx is null!");
		return 0xff;
	}

	dpu_pr_info("[DP] select_dp_port port_id:%d", port_id);
	return 0;
}

int dpu_dp_get_current_dp_source_mode(void)
{
	return get_current_dp_source_mode();
}

void dptx_debug_enable_timegen(void)
{
	uint32_t reg;
	struct dp_ctrl *dptx = NULL;

	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return;
	}

	dptx = g_dptx_debug[g_dp_id];
	if (dptx == NULL) {
		dpu_pr_err("[DP] debug dptx is null!");
		return;
	}

	if (!dptx || !dptx->dptx_enable || dptx->power_saving_mode) {
		dpu_pr_err("[DP] port not exist or has already been off!");
		return;
	}

	reg = dptx_readl(dptx, DPTX_GCTL0);
	reg |= DPTX_CFG_TIMING_GEN_ENABLE;
	dptx_writel(dptx, DPTX_GCTL0, reg);
}

// ---------------- psr end -------------------------------------------

int dptx_infoframe_sdp_manul_send_debug(void)
{
	uint32_t reg;
	struct dp_ctrl *dptx = NULL;

	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}

	dptx = g_dptx_debug[g_dp_id];
	if (!dptx) {
		dpu_pr_err("[DP] dptx is null!");
		return 0xff;
	}

	reg = readl(dptx->hidptx_base + 0x12c);
	reg |= GENMASK(20, 13);
	writel(reg, dptx->hidptx_base + 0x12c);
	dpu_pr_info("[DP] dptx_infoframe_sdp_manul_send_debug set ok: %#x", reg);
	return 0;
}

uint32_t dptx_infoframe_sdp_config_debug(int mode)
{
	uint32_t reg;
	struct dp_ctrl *dptx = NULL;

	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}

	dptx = g_dptx_debug[g_dp_id];
	if (!dptx || !dptx->dptx_enable || dptx->power_saving_mode) {
		dpu_pr_err("[DP] port id %d not exist or has already been off!", g_dp_id);
		return 0xff;
	}

	reg = (uint32_t)readl(dptx->hidptx_base + 0x12c);
	if (mode == 1) // manual
		reg |= GENMASK(9, 2);
	else
		reg &= ~GENMASK(9, 2);
	writel(reg, dptx->hidptx_base + 0x12c);
	dpu_pr_info("[DP] dptx_infoframe_sdp_config_debug set ok: %#x", reg);
	return reg;
}

static void dptx_hdr_calculate_infoframe_data(struct dp_ctrl *dptx)
{
	dpu_check_and_no_retval(!dptx, err, "[DP] dptx is NULL!\n");

	dptx->hdr_infoframe.data[HDR_INFOFRAME_EOTF_BYTE_NUM] =
		(uint8_t)dptx->hdr_metadata.electro_optical_transfer_function;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_METADATA_ID_BYTE_NUM] =
		(uint8_t)dptx->hdr_metadata.static_metadata_descriptor_id;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_X_0_LSB] = dptx->hdr_metadata.red_primary_x & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_X_0_MSB] =
		(uint8_t)((dptx->hdr_metadata.red_primary_x & MSB_MASK) >> SHIFT_8BIT);
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_Y_0_LSB] = dptx->hdr_metadata.red_primary_y & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_Y_0_MSB] =
		(uint8_t)((dptx->hdr_metadata.red_primary_y & MSB_MASK) >> SHIFT_8BIT);
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_X_1_LSB] = dptx->hdr_metadata.green_primary_x & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_X_1_MSB] =
		(uint8_t)((dptx->hdr_metadata.green_primary_x & MSB_MASK) >> SHIFT_8BIT);
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_Y_1_LSB] = dptx->hdr_metadata.green_primary_y & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_Y_1_MSB] =
		(uint8_t)((dptx->hdr_metadata.green_primary_y & MSB_MASK) >> SHIFT_8BIT);
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_X_2_LSB] = dptx->hdr_metadata.blue_primary_x & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_X_2_MSB] =
		(uint8_t)((dptx->hdr_metadata.blue_primary_x & MSB_MASK) >> SHIFT_8BIT);
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_Y_2_LSB] = dptx->hdr_metadata.blue_primary_y & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_DISP_PRI_Y_2_MSB] =
		(uint8_t)((dptx->hdr_metadata.blue_primary_y & MSB_MASK) >> SHIFT_8BIT);
	dptx->hdr_infoframe.data[HDR_INFOFRAME_WHITE_POINT_X_LSB] = dptx->hdr_metadata.white_point_x & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_WHITE_POINT_X_MSB] =
		(uint8_t)((dptx->hdr_metadata.white_point_x & MSB_MASK) >> SHIFT_8BIT);
	dptx->hdr_infoframe.data[HDR_INFOFRAME_WHITE_POINT_Y_LSB] = dptx->hdr_metadata.white_point_y & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_WHITE_POINT_Y_MSB] =
		(uint8_t)((dptx->hdr_metadata.white_point_y & MSB_MASK) >> SHIFT_8BIT);
	dptx->hdr_infoframe.data[HDR_INFOFRAME_MAX_LUMI_LSB] =
		dptx->hdr_metadata.max_display_mastering_luminance & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_MAX_LUMI_MSB] =
		(uint8_t)((dptx->hdr_metadata.max_display_mastering_luminance & MSB_MASK) >> SHIFT_8BIT);
	dptx->hdr_infoframe.data[HDR_INFOFRAME_MIN_LUMI_LSB] =
		dptx->hdr_metadata.min_display_mastering_luminance & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_MIN_LUMI_MSB] =
		(uint8_t)((dptx->hdr_metadata.min_display_mastering_luminance & MSB_MASK) >> SHIFT_8BIT);
	dptx->hdr_infoframe.data[HDR_INFOFRAME_MAX_LIGHT_LEVEL_LSB] =
		dptx->hdr_metadata.max_content_light_level & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_MAX_LIGHT_LEVEL_MSB] =
		(uint8_t)((dptx->hdr_metadata.max_content_light_level & MSB_MASK) >> SHIFT_8BIT);
	dptx->hdr_infoframe.data[HDR_INFOFRAME_MAX_AVERAGE_LEVEL_LSB] =
		dptx->hdr_metadata.max_frame_average_light_level & LSB_MASK;
	dptx->hdr_infoframe.data[HDR_INFOFRAME_MAX_AVERAGE_LEVEL_MSB] =
		(uint8_t)((dptx->hdr_metadata.max_frame_average_light_level & MSB_MASK) >> SHIFT_8BIT);
}

int dptx_hdr_infoframe_sdp_send_debug(int data)
{
	int i;
	struct dp_ctrl *dptx = NULL;

	if (g_dp_id < DPID_MIN || g_dp_id > DPID_MAX) {
		dpu_pr_err("[DP] debug g_dp_id is invalid!");
		return 0xff;
	}

	dptx = g_dptx_debug[g_dp_id];
	if (!dptx) {
		dpu_pr_err("[DP] dptx is null!");
		return 0xff;
	}

	dptx->hdr_infoframe.type_code = INFOFRAME_PACKET_TYPE_HDR;
	dptx->hdr_infoframe.version_number = HDR_INFOFRAME_VERSION;
	dptx->hdr_infoframe.length = HDR_INFOFRAME_LENGTH;

	memset_s(dptx->hdr_infoframe.data, HDR_INFOFRAME_LENGTH, 0, HDR_INFOFRAME_LENGTH);
	dptx->hdr_metadata.static_metadata_descriptor_id = (uint32_t)data;
	dptx_hdr_calculate_infoframe_data(dptx);

	for (i = 0; i < HDR_INFOFRAME_LENGTH; i++)
		dpu_pr_info("[DP] hdrinfoframe data[%d]: 0x%02x", i, dptx->hdr_infoframe.data[i]);

	mutex_lock(&dptx->dptx_mutex);

	if (dptx->dptx_hdr_infoframe_set_reg)
		dptx->dptx_hdr_infoframe_set_reg(dptx, 1);

	mutex_unlock(&dptx->dptx_mutex);

	return 0;
}

