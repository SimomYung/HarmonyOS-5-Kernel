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

#include <linux/kernel.h>
#include "dp_phy_config_interface.h"
#include "dp_ctrl.h"
#include "dp_ctrl_dev.h"
#include "dp_drv.h"
#include "dpu_dp_dbg.h"
#include "drm_dp_helper_additions.h"
#include <linux/platform_drivers/usb/chip_usb.h>
#include "dpu_dp_dbg_v800.h"

/* PLL */
#define PLL1UPDATECTRL							(0xc00 + 0x10)
#define PLL1UPDATECTRL_RATE_MASK				GENMASK(11, 8)
#define PLL1UPDATECTRL_RATE_OFFSET				8

#define PLL1CKGCTRLR0							(0xc00 + 0x14)
#define PLL1CKGCTRLR1							(0xc00 + 0x1c)
#define PLL1CKGCTRLR2							(0xc00 + 0x24)
#define PLL_LPF_CS_R_MASK						GENMASK(7, 6)
#define PLL_LPF_RS_R_MASK						GENMASK(11, 8)

/* ssc */
#define PLL1SSCG_CTRL							(0xc00 + 0x80)
#define PLL1_SSCG_CNT_STEPSIZE_FORCE_0			BIT(1)

#define pll1sscg_ctrl_initial_r(n)				(0xc00 + 0x10 * (n) + 0x84)
#define pll1sscg_cnt_r(n)						(0xc00 + 0x10 * (n) + 0x88)
#define pll1sscg_cnt2_r(n)						(0xc00 + 0x10 * (n) + 0x8c)

/* pre sw */
#define txdrvctrl(n)							(0x2000 + 0x0400 * (n) + 0x314)
#define txeqcoeff(n)							(0x2000 + 0x0400 * (n) + 0x318)

#define AUX_EYE_LEVEL		0x3
#define AUX_HYS_TUNE_VAL	0x1
#define AUX_CTRL_VAL		0x3
#define AUX_RTERMSEL_VAL		0x6
#define DPTX_DP_AUX_CTRL						0x20
/* aux disreset */
#define REG_DP_AUX_PWDNB						BIT(9)
#define STATIC_TERM_CTRL 		0x1B44
/* eye diagram */
#define AUX_RTERMSEL_MASK						GENMASK(2, 0)
#define AUX_NEN_RTERM_MASK						BIT(3)
#define REG_DP_AUX_AUX_CTRL_MASK				GENMASK(7, 4)
#define AUX_CTRL_OFFSET							4
#define REG_DP_AUX_AUX_HYS_TUNE_MASK			GENMASK(11, 10)
#define AUX_HYS_TUNE_OFFSET						10
#define REG_DP_AUX_RONSELN_MASK					GENMASK(14, 12)
#define AUX_RONSELN_OFFSET						12
#define REG_DP_AUX_RONSELP_MASK					GENMASK(18, 16)
#define AUX_RONSELP_OFFSET						16

/* NSV phy value */
#define PLL_INITIAL_R2_VAL	0x8CA00000
#define PLL_CNT_R2_VAL		0x011c0119
#define PLL_CNT2_R2_VAL		0x000131

/* CHS phy value */
#define PLL_INITIAL_R0_VAL	0x54600000
#define PLL_CNT_R0_VAL		0x540055
#define PLL_CNT2_R0_VAL		0x000262
#define PLL_INITIAL_R1_VAL	0x46500000
#define PLL_CNT_R1_VAL		0x460047
#define PLL_CNT2_R1_VAL		0x000262
#define PLL_INITIAL_R2_CHSVAL	0x46500000
#define PLL_CNT_R2_CHSVAL		0x460047
#define PLL_CNT2_R2_CHSVAL		0x000262
#define TERM_CTRL_VAL		0xDF

#define PLL1SSCG_CTRL_VAL 0x7ff0e0d

/* (plug_type, lane_num) -> (phylane) mapping */
static const struct phylane_mapping g_phylane_mappings[] = {
	{ DP_PLUG_TYPE_NORMAL, 0, 3 },
	{ DP_PLUG_TYPE_NORMAL, 1, 1 },
	{ DP_PLUG_TYPE_NORMAL, 2, 0 },
	{ DP_PLUG_TYPE_NORMAL, 3, 2 },
	{ DP_PLUG_TYPE_FLIPPED, 0, 2 },
	{ DP_PLUG_TYPE_FLIPPED, 1, 0 },
	{ DP_PLUG_TYPE_FLIPPED, 2, 1 },
	{ DP_PLUG_TYPE_FLIPPED, 3, 3 },
};

static int dptx_mapping_phylane(uint32_t lane, int plug_type,
	uint16_t *txdrvctrl, uint16_t *txeqcoeff)
{
	uint32_t i;
	uint8_t phylane = 0;

	for (i = 0; i < (uint32_t)ARRAY_SIZE(g_phylane_mappings); i++) {
		if (plug_type == g_phylane_mappings[i].plug_type && lane == g_phylane_mappings[i].lane) {
			phylane = (uint8_t)g_phylane_mappings[i].phylane;
			break;
		}
	}

	if (i == (uint32_t)ARRAY_SIZE(g_phylane_mappings)) {
		dpu_pr_err("[DP] lane number error!\n");
		return -EINVAL;
	}

	*txdrvctrl = txdrvctrl(phylane);
	*txeqcoeff = txeqcoeff(phylane);

	return 0;
}

static int dptx_config_swing_and_preemphasis(struct dp_ctrl *dptx, uint32_t sw_level, uint32_t pe_level,
	uint16_t txdrvctrl, uint16_t txeqcoeff)
{
	uint32_t offset = 0;
	uint32_t index0, index1;

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!\n");

	/**
	 * @brief The index value is in line with the following rules:
	 * (sw_level, pe_level) --> (0, 0 ~ 3) --> index0=0,2,4,6 index1=1,3,5,7
	 * (sw_level, pe_level) --> (1, 0 ~ 2) --> index0=8,10,12 index1=9,11,13
	 * (sw_level, pe_level) --> (2, 0 ~ 1) --> index0=14,16 index1=15,17
	 * (sw_level, pe_level) --> (3, 0    ) --> index0=18 index1=19
	 */
	if ((sw_level > 3) || (pe_level > 3) || (pe_level > 4 - sw_level)) {
		dpu_pr_warn("[DP] Don't support sw %u & ps level %u", sw_level, pe_level);
		return -1;
	}

	index0 = sw_level * 8 + pe_level * 2;
	index1 = sw_level * 8 + pe_level * 2 + 1;
	while (sw_level-- > 0)
		offset += sw_level * 2;
	index0 = index0 - offset;
	index1 = index1 - offset;

	dpu_pr_info("[DP] index0:%d, index1:%d, phy_param_num:%d\n", index0, index1, dptx->combophy_param_num);
	if ((index0 >= dptx->combophy_param_num) || (index1 >= dptx->combophy_param_num)) {
		dpu_pr_err("[DP] index0 %u or index0 %u is out of range\n", index0, index1);
		return -1;
	}
	outp32(dptx->combo_phy_base + txdrvctrl, dptx->combophy_pree_swing[DPTX_PREE_SWING_DEFAULT][index0]);
	outp32(dptx->combo_phy_base + txeqcoeff, dptx->combophy_pree_swing[DPTX_PREE_SWING_DEFAULT][index1]);

	dpu_pr_info("[DP] combophy_pree_swing[0][%d]: 0x%x, combophy_pree_swing[0][%d]: 0x%x \n",
		index0, dptx->combophy_pree_swing[DPTX_PREE_SWING_DEFAULT][index0],
		index1, dptx->combophy_pree_swing[DPTX_PREE_SWING_DEFAULT][index1]);

	return 0;
}

static void dptx_combophy_set_preemphasis_swing(struct dp_ctrl *dptx,
	uint32_t lane, uint32_t sw_level, uint32_t pe_level)
{
	uint16_t txdrvctrl = 0;
	uint16_t txeqcoeff = 0;
	int ret;

	dpu_check_and_no_retval(!dptx, err, "[DP] dptx is NULL!\n");

	ret = dptx_mapping_phylane(lane, dptx->dptx_plug_type, &txdrvctrl, &txeqcoeff);
	if (ret)
		return;
	ret = dptx_config_swing_and_preemphasis(dptx, sw_level, pe_level, txdrvctrl, txeqcoeff);
	if (ret)
		return;
}

static void dptx_aux_disreset(struct dp_ctrl *dptx, bool enable)
{
	uint32_t reg;
	struct dp_private *dp_priv = NULL;

	dpu_check_and_no_retval(!dptx, err, "[DP] NULL Pointer\n");
	dp_priv = to_dp_private(dptx->connector->conn_info);
	dpu_check_and_no_retval(!dp_priv, err, "[DP] dp_priv is NULL");

	/* Enable AUX Block */
	if (g_dp_debug_mode_enable) {
		reg = (uint32_t)dp_debug_get_aux_ctrl_param(dptx);
	} else {
		reg = (uint32_t)inp32(dptx->combo_phy_base + DPTX_DP_AUX_CTRL);
		if (enable)
			reg |= REG_DP_AUX_PWDNB;
		else
			reg &= ~REG_DP_AUX_PWDNB;

		reg &= ~REG_DP_AUX_RONSELN_MASK;
		reg &= ~REG_DP_AUX_RONSELP_MASK;
		reg |=  AUX_EYE_LEVEL << AUX_RONSELN_OFFSET;
		reg |=  AUX_EYE_LEVEL << AUX_RONSELP_OFFSET;
		reg &= ~REG_DP_AUX_AUX_HYS_TUNE_MASK;
		reg |= (AUX_HYS_TUNE_VAL << AUX_HYS_TUNE_OFFSET);
		reg &= ~REG_DP_AUX_AUX_CTRL_MASK;
		reg |= (AUX_CTRL_VAL << AUX_CTRL_OFFSET);
		reg &= ~AUX_NEN_RTERM_MASK;
		reg &= ~AUX_RTERMSEL_MASK;
		reg |= AUX_RTERMSEL_VAL;

		if (dp_priv->dp_aux_phy_config != 0) {
			reg = dp_priv->dp_aux_phy_config;
			dpu_pr_info("[DP] set aux phy config to 0x%x", reg);
		}
	}
	dpu_pr_info("[DP] DPTX_DP_AUX_CTRL config value: 0x%x\n", reg);
	outp32(dptx->combo_phy_base + DPTX_DP_AUX_CTRL, reg);
	dpu_pr_info("[DP] DPTX_DP_AUX_CTRL get value: 0x%x\n", inp32(dptx->combo_phy_base + DPTX_DP_AUX_CTRL));
	mdelay(1);
	dpu_pr_info("[DP] PHY_MODE value: 0x%x", inp32(dptx->combo_phy_base + 0x10));
}

static void dptx_combophy_set_ssc_dis(struct dp_ctrl *dptx, bool ssc_disable)
{
	uint32_t reg;

	dpu_check_and_no_retval(!dptx, err, "[DP] dptx is NULL!\n");

	reg = PLL1SSCG_CTRL_VAL; // default value for chip spec

	if (!ssc_disable) {
		reg &= ~PLL1_SSCG_CNT_STEPSIZE_FORCE_0;
		dpu_pr_info("[DP] SSC enable\n");
	} else {
		reg |= PLL1_SSCG_CNT_STEPSIZE_FORCE_0;
		dpu_pr_info("[DP] SSC disable\n");
	}
	if (g_dp_debug_mode_enable)
		reg = dp_debug_get_pll_value();

	dpu_pr_info("[DP] PLL1SSCG_CTRL config value: 0x%x\n", reg);
	outp32(dptx->combo_phy_base + PLL1SSCG_CTRL, reg);
}

static void dptx_combophy_cfg(struct dp_ctrl *dptx)
{
	outp32(dptx->combo_phy_base + PLL1CKGCTRLR0, 0x054610df);
	outp32(dptx->combo_phy_base + PLL1CKGCTRLR1, 0x046510da);
	outp32(dptx->combo_phy_base + PLL1CKGCTRLR2, 0x046500d5);
	outp32(dptx->combo_phy_base + pll1sscg_ctrl_initial_r(2), PLL_INITIAL_R2_VAL);
	outp32(dptx->combo_phy_base + pll1sscg_cnt_r(2), PLL_CNT_R2_VAL);
	outp32(dptx->combo_phy_base + pll1sscg_cnt2_r(2), PLL_CNT2_R2_VAL);
}

static void dptx_combophy_debug_rate(struct dp_ctrl *dptx, int rate)
{
	uint32_t reg;

	switch (rate) {
	case DPTX_PHYIF_CTRL_RATE_RBR:
		if (g_dp_debug_mode_enable) {
			reg = (uint32_t)g_dp_params[dptx->id].dp_rbr_pll1_rate0_coef[0];
			dpu_pr_info("[DP] PLL1CKGCTRLR0 config value: 0x%x", reg);
			outp32(dptx->combo_phy_base + PLL1CKGCTRLR0, reg);
		}
		dpu_pr_info("[DP] PLL1CKGCTRLR0 get value: 0x%x", inp32(dptx->combo_phy_base + PLL1CKGCTRLR0));
		break;
	case DPTX_PHYIF_CTRL_RATE_HBR:
		if (g_dp_debug_mode_enable) {
			reg = (uint32_t)g_dp_params[dptx->id].dp_hbr_pll1_rate1_coef[0];
			dpu_pr_info("[DP] PLL1CKGCTRLR1 config value: 0x%x", reg);
			outp32(dptx->combo_phy_base + PLL1CKGCTRLR1, reg);
		}
		dpu_pr_info("[DP] PLL1CKGCTRLR1 get value: 0x%x", inp32(dptx->combo_phy_base + PLL1CKGCTRLR1));
		break;
	case DPTX_PHYIF_CTRL_RATE_HBR2:
		if (g_dp_debug_mode_enable) {
			reg = (uint32_t)g_dp_params[dptx->id].dp_hbr2_pll1_rate2_coef[0];
			outp32(dptx->combo_phy_base + pll1sscg_ctrl_initial_r(2), g_dp_params[dptx->id].dp_hbr2_pll1_rate2_coef[1]);
			outp32(dptx->combo_phy_base + pll1sscg_cnt_r(2), g_dp_params[dptx->id].dp_hbr2_pll1_rate2_coef[2]);
			outp32(dptx->combo_phy_base + pll1sscg_cnt2_r(2), g_dp_params[dptx->id].dp_hbr2_pll1_rate2_coef[3]);
			dpu_pr_info("[DP] PLL1CKGCTRLR2 config value: 0x%x", reg);
			outp32(dptx->combo_phy_base + PLL1CKGCTRLR2, reg);
		}
		dpu_pr_info("[DP] PLL1CKGCTRLR2 get value: 0x%x", inp32(dptx->combo_phy_base + PLL1CKGCTRLR2));
		break;
	case DPTX_PHYIF_CTRL_RATE_HBR3:
		dpu_pr_err("[DP] Invalid PHY rate HBR3");
		break;
	default:
		dpu_pr_err("[DP] Invalid PHY rate %d\n", rate);
		return;
	}
}

static void dptx_combophy_set_rate(struct dp_ctrl *dptx, int rate)
{
	dpu_check_and_no_retval(!dptx, err, "[DP] dptx is NULL!\n");

	dptx_combophy_cfg(dptx);
	dptx_combophy_debug_rate(dptx, rate);
}

void dptx_phy_layer_init(struct dp_ctrl *dptx)
{
	dpu_check_and_no_retval(!dptx, err, "[DP] dptx is NULL!\n");

	dptx->dptx_combophy_set_preemphasis_swing = dptx_combophy_set_preemphasis_swing;
	dptx->dptx_change_physetting_hbr2 = NULL;
	dptx->dptx_combophy_set_ssc_addtions = NULL;
	dptx->dptx_combophy_set_lanes_power = NULL;
	dptx->dptx_combophy_set_ssc_dis = dptx_combophy_set_ssc_dis;
	dptx->dptx_aux_disreset = dptx_aux_disreset;
	dptx->dptx_combophy_set_rate = dptx_combophy_set_rate;
}
