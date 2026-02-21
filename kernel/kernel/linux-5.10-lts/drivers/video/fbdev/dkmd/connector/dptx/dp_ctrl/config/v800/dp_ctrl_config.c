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

#include "dp_ctrl_config.h"
#include <platform_include/see/ffa/ffa_msg_id.h>
#include <platform_include/display/linux/dpu_drmdriver.h>
#include "hidptx/hidptx_reg.h"
#include "hidptx_dp_core.h"
#include "controller/dp_avgen_base.h"
#include <soc_hsdt1_crg_interface.h>
#include "dpu/dpu_base_addr.h"
#include "dp_drv.h"
#include "dp_ctrl_dev.h"
#include "platform_include/basicplatform/linux/soc_spec.h"

#define CLKDIV6_NUM 24

static int dptx_ctrl_clk_enable(struct dp_ctrl *dptx)
{
	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!");

	/* enable clock */
	if (clk_prepare_enable(dptx->connector->connector_clk[CLK_DPCTRL_16M]) != 0) {
		dpu_pr_err("[DP] enable dpctrl 16m clk failed!\n");
		return -1;
	}

	if (clk_prepare_enable(dptx->connector->connector_clk[CLK_DPCTRL_PCLK]) != 0) {
		dpu_pr_err("[DP] enable dpctrl pclk failed!\n");
		return -1;
	}

	if (clk_prepare_enable(dptx->clk_dpctrl_pixel) != 0) {
		dpu_pr_err("[DP] enable dpctrl pixel clk failed!\n");
		return -1;
	}

	return 0;
}

static void dptx_get_sync_calib_param(struct dp_ctrl *dptx, int stream)
{
	uint32_t bpp;
	uint32_t sync_delay;
	uint32_t sync_calib_param = 0;
	uint32_t reg;

	bpp = (uint32_t)dptx_get_color_depth(dptx->vparams.bpc, dptx->vparams.pix_enc);
	sync_delay = dptx->link.lanes * 16 * 8 * 8 / (2 * bpp);

	if (sync_delay + 16 >= dptx->vparams.mdtd.h_blanking / 2) {
		sync_calib_param = sync_delay + 64 - dptx->vparams.mdtd.h_blanking / 2 + 32;
		sync_calib_param = sync_calib_param / 2 * 2; /* sync_calib_param has to be even */
	}

	reg = dptx_readl(dptx, dptx_video_packet_stream(stream));
	reg = reg & (~DPTX_VIDEO_PACKET_TU_SYNC_CALIB_MASK);
	reg = reg | (sync_calib_param << DPTX_VIDEO_PACKET_TU_SYNC_CALIB_SHIFT);
	dptx_writel(dptx, dptx_video_packet_stream(stream), reg);

	dpu_pr_info("[DP] stream %d set reg sync_calib_param %u", stream, sync_calib_param);
	return;
}

static void dptx_dsc_sample_en(struct dp_ctrl *dptx, int stream)
{
	uint32_t vsamplectrl;
	vsamplectrl = dptx_readl(dptx, dptx_video_ctrl_stream(stream));
	if (dptx->dsc)
		vsamplectrl &= ~DPTX_VIDEO_RGB_SAMPLE_EN;
	else
		vsamplectrl |= DPTX_VIDEO_RGB_SAMPLE_EN;
	dptx_writel(dptx, dptx_video_ctrl_stream(stream), vsamplectrl);
}

int dptx_pixel_pll_preinit(void)
{
	return 0;
}

int dptx_pixel_pll_init(struct dp_ctrl *dptx, uint64_t pixel_clock)
{
	void_unused(dptx);
	void_unused(pixel_clock);
	return 0;
}

int dptx_pixel_pll_deinit(struct dp_ctrl *dptx)
{
	void_unused(dptx);
	return 0;
}

void enable_dptx_timing_gen(struct dp_ctrl *dptx)
{
    uint32_t val;
    uint32_t reg;

	if (unlikely(!dptx)) {
		dpu_pr_warn("dptx is null");
		return;
	}

	dptx_writel(dptx, dptx_dp_color_bar_stream(0), 0);

	if (dptx->dptx_triger_media_transfer)
		dptx->dptx_triger_media_transfer(dptx, true);

	val = dptx_readl(dptx, DPTX_GCTL0);
    val |= DPTX_CFG_TIMING_GEN_ENABLE;
    if (dptx->mst) {
		val |= DPTX_CFG_SST1_TIMING_GEN_ENABLE;
		dptx_writel(dptx, dptx_dp_color_bar_stream(1), 0x01);
	}
	dptx_writel(dptx, DPTX_GCTL0, val);
	reg = dptx_readl(dptx, DPTX_GCTL0);
	dpu_pr_debug("[DP] DPTX_GCTL0 = 0x%x, dptx timing gen enabled!", reg);
}

int32_t init_dp_sctrl(struct dp_private *priv, struct device_node *np)
{
	priv->dp_sctrl_base = of_iomap(np, REG_DP_SCTRL_BASE);
	if (!priv->dp_sctrl_base) {
		dpu_pr_err("[DP] failed to get dp_sctrl_base!\n");
		return -ENXIO;
	}
	return 0;
}

int32_t init_dpctrl_pixel_clk(struct dp_ctrl *dptx0, struct dp_ctrl *dptx1, struct device_node *np)
{
	if (dptx0 != NULL) {
		dptx0->clk_dpctrl_pixel = of_clk_get(np, DPTX0_PIXEL_CLOCK);
		if (IS_ERR_OR_NULL(dptx0->clk_dpctrl_pixel)) {
			dpu_pr_err("[DP] failed to get dptx0 clk_dpctrl_pixel!\n");
			return -ENXIO;
		}
	}
	/* for dual edp-tx */
	if (dptx1 != NULL) {
		dptx1->clk_dpctrl_pixel = of_clk_get(np, DPTX1_PIXEL_CLOCK);
		if (IS_ERR_OR_NULL(dptx1->clk_dpctrl_pixel)) {
			dpu_pr_err("[DP] failed to get dptx1 clk_dpctrl_pixel!\n");
			return -ENXIO;
		}
	}

	return 0;
}

int32_t init_dpctrl_16m_clk(struct dp_ctrl *dptx, struct device_node *np)
{
	void_unused(dptx);
	void_unused(np);
	return 0;
}

int32_t init_hsdt1_sysctrl_base(struct dp_private *priv, struct device_node *np)
{
	void_unused(priv);
	void_unused(np);
	return 0;
}

int32_t init_sub_harden_crg_base(struct dp_private *priv, struct device_node *np)
{
	void_unused(priv);
	void_unused(np);
	return 0;
}

static void dp_disable_clk(struct dp_ctrl *dptx)
{
	/* disable clk */
	clk_disable_unprepare(dptx->connector->connector_clk[CLK_DPCTRL_16M]);
	clk_disable_unprepare(dptx->connector->connector_clk[CLK_DPCTRL_PCLK]);
	clk_disable_unprepare(dptx->clk_dpctrl_pixel);
}

int dp_pm_suspend(struct device *dev)
{
	void_unused(dev);
	return 0;
}

int dp_pm_resume(struct device *dev)
{
	void_unused(dev);
	return 0;
}

int dp_pm_freeze(struct device *dev)
{
	struct dp_private *priv = NULL;
	struct dp_ctrl *dptx = NULL;
	struct dkmd_connector_info *pinfo = NULL;
	struct dpu_connector *connector = NULL;

	dpu_pr_info("[DP] enter +!\n");
	priv = (struct dp_private *)dev_get_drvdata(dev);
	if (!priv) {
		dpu_pr_err("[DP] get dp private data failed!\n");
		return 0;
	}
	dptx = &priv->dp[MASTER_DPTX_IDX];
	if (!dptx) {
		dpu_pr_err("[DP] get dp dptx failed!\n");
		return 0;
	}
	pinfo = &priv->connector_info;
	if (!pinfo) {
		dpu_pr_err("[DP] get dp pinfo failed!\n");
		return 0;
	}
	connector = get_primary_connector(pinfo);
	if (!connector) {
		dpu_pr_err("[DP] get dp connector failed!\n");
		return 0;
	}
	dpu_pr_info("[DP] reset startup flag\n");

	atomic_set(&(dptx->edid_reads), NEED_READ_EDID);
	dpu_pr_info("[DP] edid_reads:%d!!!", atomic_read(&dptx->edid_reads));

	connector_device_suspend(&priv->connector_info);

	if (dptx->connector->off_func && dptx->connector->conn_info->base.mode != EDP_MODE)
		dptx->connector->off_func(dptx->connector->conn_info);

	dpu_pr_info("[DP] exit - portid: %d!\n", dptx->port_id);

	return 0;
}

void dp_shutdown(struct platform_device *pdev)
{
	void_unused(pdev);
}

void dptx_phy_lane_reset(struct dp_ctrl *dptx, bool mainlink_open)
{
	int retval;
	uint32_t phyifctrl;

	dpu_check_and_no_retval(!dptx, err, "[DP] null pointer");

	phyifctrl = dptx_readl(dptx, DPTX_PHYIF_CTRL1);

	if (mainlink_open) /* P0 mode, disreset lanes */
		phyifctrl = (uint32_t)cfg_phy_lanes(DPTX_MAX_LINK_LANES);
	else
		phyifctrl &= ~(uint32_t)cfg_phy_lanes(DPTX_MAX_LINK_LANES);

	dptx_writel(dptx, DPTX_PHYIF_CTRL1, phyifctrl);

	if (mainlink_open) {
		retval = dptx_phy_wait_busy(dptx, dptx->max_lanes);
		if (retval)
			dpu_pr_err("[DP] phy release fail");
	}
}

static void dptx_hdcp_enable_video_transform(struct dp_ctrl *dptx, bool enable)
{
	configure_dss_service_security(FID_BL31_DISPLAY_FN_SUB_ID_HDCP_CTRL, DSS_HDCP_DP_ENABLE, enable, 0);
	return;
}

static void dptx_config_stream_timing(struct dp_ctrl *dptx)
{
	uint32_t reg;
	struct ldi_panel_info *ldi = NULL;
	struct dkmd_connector_info *pinfo = dptx->connector->conn_info;
	ldi = &dptx->connector->ldi;

	reg = 0;
	reg |= (pinfo->base.yres << DPTX_TIMING_GEN_VACTIVE_SHIFT);
	reg |= (ldi->v_back_porch + ldi->v_front_porch + ldi->v_pulse_width);
	dptx_writel(dptx, dptx_timing_gen_config2_stream(0), reg);
	if (dptx->mst)
		dptx_writel(dptx, dptx_timing_gen_config2_stream(1), reg);

	reg = 0;
	reg |= (ldi->v_front_porch << DPTX_TIMING_GEN_VFP_SHIFT);
	dptx_writel(dptx, dptx_timing_gen_config3_stream(0), reg);
	if (dptx->mst)
		dptx_writel(dptx, dptx_timing_gen_config3_stream(1), reg);
}

static int dptx_dis_reset(struct dp_ctrl *dptx, bool benable)
{
	struct dpu_connector *connector = NULL;
	struct dp_private *dp_pri = NULL;
	dpu_pr_info("[DP] dptx_dis_reset start");

	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is NULL!");
	connector = dptx->connector;
	dp_pri = to_dp_private(connector->conn_info);

	if (benable) {
		set_reg(SOC_HSDT1_CRG_PERRSTDIS0_ADDR(dp_pri->hsdt1_crg_base), 1, 1, 3); /* unreset apb 0xE614B000 + 0x24 bit3 */
		set_reg(SOC_CRGPERIPH_PERRSTDIS0_ADDR(connector->peri_crg_base), 1, 1, 0); /* unreset DPC 0xFFB85000 + 0x64 bit0 */
		set_reg(SOC_HSDT1_CRG_PEREN0_ADDR(dp_pri->hsdt1_crg_base), 1, 1, 3); /* open apb 0xE614B000 + 0x0 bit3 */
		set_reg(SOC_CRGPERIPH_PEREN12_ADDR(connector->peri_crg_base), 1, 1, 22); /* open pixel 0xFFB85000 + 0x470 bit22 */
		if (dptx->mst) {
			set_reg(SOC_HSDT1_CRG_PERRSTDIS0_ADDR(dp_pri->hsdt1_crg_base), 1, 1, 24); /* 0xE614B000 + 0x24 bit24 for mst */
			set_reg(SOC_HSDT1_CRG_PEREN1_ADDR(dp_pri->hsdt1_crg_base), 1, 1, 19); /* 0xE614B000 + 0x10 bit19 for mst */
		}
		set_reg(SOC_HSDT1_CRG_PEREN0_ADDR(dp_pri->hsdt1_crg_base), 0, 1, 7);  /* 0xE614B000 + 0x0 bit7 */
		set_reg(SOC_CRGPERIPH_PEREN12_ADDR(connector->peri_crg_base), 1, 1, 21); /* open aux 0xFFB85000 + 0x470 bit21 */
		set_reg(SOC_HSDT1_CRG_PEREN0_ADDR(dp_pri->hsdt1_crg_base), 0, 1, 27);  /* 0xE614B000 + 0x0 bit27 */
		// ip_rst_qic_ufs_hci xiaobao-todo？
		set_reg(SOC_HSDT1_CRG_PERRSTDIS0_ADDR(dp_pri->hsdt1_crg_base), 1, 1, 25); /* 0xE614B000 + 0x24 bit25 */
		// HPD polarity inversion configuration: DPTX CTRL
		set_reg(dptx->hidptx_base + DPTX_GCTL0, 1, 1, 9);
		// SDP enable
		set_reg(dptx->hidptx_base + 0x708, 0x1, 1, 11);
		// Mem power on：DPTX_CTRL, 0x718 set to 0x850, 0x71C set to 0x15858
		set_reg(dptx->hidptx_base + 0x718, 0x850, 32, 0);
		set_reg(dptx->hidptx_base + 0x71C, 0x15858, 32, 0);
		// open reset
		set_reg(dptx->hidptx_base + 0x700, 0x1, 1, 8);
		set_reg(dptx->hidptx_base + 0x700, 0x1, 1, 9);
		// xiaobao-todo？
		outp32(dp_pri->dp_sctrl_base, 0x850); /* 0xEFA70000: 0x850 */
	} else {
		// close reset
		set_reg(dptx->hidptx_base + 0x700, 0x0, 1, 8);
		set_reg(dptx->hidptx_base + 0x700, 0x0, 1, 9);
		set_reg(SOC_CRGPERIPH_PERDIS12_ADDR(connector->peri_crg_base), 1, 1, 22); /* close pixel 0xFFB85000 + 0x474 bit22 */
		set_reg(SOC_HSDT1_CRG_PERDIS0_ADDR(dp_pri->hsdt1_crg_base), 1, 1, 3); /* close apb 0xE614B000 + 0x0 bit3 */
		set_reg(SOC_CRGPERIPH_PERDIS12_ADDR(connector->peri_crg_base), 1, 1, 21); /* close aux 0xFFB85000 + 0x474 bit21 */
		set_reg(SOC_HSDT1_CRG_PERRSTEN0_ADDR(dp_pri->hsdt1_crg_base), 1, 1, 3); /* reset apb 0xE614B000 + 0x20 bit3 */

		set_reg(SOC_HSDT1_CRG_PERDIS0_ADDR(dp_pri->hsdt1_crg_base), 0, 1, 7); /* 0xE614B000 + 0x4 bit7 */
		set_reg(SOC_CRGPERIPH_PERRSTEN0_ADDR(connector->peri_crg_base), 1, 1, 0); /* unreset DPC 0xFFB85000 + 0x60 bit0 */
	}
	dpu_pr_info("[DP] --> PERRSTSTAT0=0x%x, PERRSTSTAT0=0x%x, PERSTAT0=0x%x",
		inp32(SOC_HSDT1_CRG_PERRSTSTAT0_ADDR(dp_pri->hsdt1_crg_base)),
		inp32(SOC_CRGPERIPH_PERRSTSTAT0_ADDR(connector->peri_crg_base)),
		inp32(SOC_HSDT1_CRG_PERSTAT0_ADDR(dp_pri->hsdt1_crg_base)));

	return 0;
}

static uint64_t dp_pixel_ppll7_param_calc(uint64_t pixel_clock_cur, struct ppll7_cfg_param *cfg)
{
	int i, ceil_temp;
	uint64_t ppll7_freq_divider, vco_freq_output;
	int freq_divider_list[CLKDIV6_NUM] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
		12, 14, 15, 16, 20, 21, 24,
		25, 30, 36, 42, 49, 56, 64
	};
	int postdiv1_list[CLKDIV6_NUM] = {
		1, 2, 3, 4, 5, 6, 7, 4, 3, 5, 4,
		7, 5, 4, 5, 7, 6, 5, 6, 6, 7, 7, 8, 8
	};
	int postdiv2_list[CLKDIV6_NUM] = {
		1, 1, 1, 1, 1, 1, 1, 2, 3, 2, 3,
		2, 3, 4, 4, 3, 4, 5, 5, 6, 6, 7, 7, 8
	};

	/* Fractional PLL can not output the so small clock */
	cfg->post_div = 1;
	if (pixel_clock_cur * (uint64_t)freq_divider_list[CLKDIV6_NUM - 1] < VCO_MIN_FREQ_630_OUPUT ) {
		cfg->post_div = 2;
		pixel_clock_cur *= 2; /* multiple frequency */
	}

	ceil_temp = dp_ceil(VCO_MIN_FREQ_630_OUPUT , pixel_clock_cur);
	if (ceil_temp < 0)
		return pixel_clock_cur;

	ppll7_freq_divider = (uint64_t)ceil_temp;
	for (i = 0; i < CLKDIV6_NUM; i++) {
		if (freq_divider_list[i] >= (int)ppll7_freq_divider) {
			ppll7_freq_divider = (uint64_t)freq_divider_list[i];
			cfg->postdiv1 = (uint64_t)postdiv1_list[i] - 1;
			cfg->postdiv2 = (uint64_t)postdiv2_list[i] - 1;
			dpu_pr_info("[DP] postdiv1=0x%llx, postdiv2=0x%llx\n", cfg->postdiv1, cfg->postdiv2);
			break;
		}
	}

	vco_freq_output = ppll7_freq_divider * pixel_clock_cur;
	if (vco_freq_output == 0)
		return pixel_clock_cur;

	dpu_pr_info("[DP] vco_freq_output=%llu\n", vco_freq_output);
	ceil_temp = dp_ceil(300000000UL, vco_freq_output);
	if (ceil_temp < 0)
		return pixel_clock_cur;

	cfg->refdiv = 2;
	dpu_pr_info("[DP] refdiv=0x%llx, ceil_temp=0x%llx\n", cfg->refdiv, ceil_temp);

	cfg->fbdiv = vco_freq_output  * (uint64_t)ceil_temp / SYS_FREQ;
	dpu_pr_info("[DP] fbdiv=0x%llx\n", cfg->fbdiv);

	cfg->frac = (uint64_t)ceil_temp * 0x1000000 * vco_freq_output * cfg->refdiv / \
		SYS_FREQ / 2 - (uint64_t)(cfg->fbdiv) * 0x1000000;

	dpu_pr_info("[DP] frac=0x%llx\n", cfg->frac);

	return pixel_clock_cur;
}

void dp_ctrl_config_init(struct dp_ctrl *dptx)
{
	dptx->dptx_get_sync_calib_param = dptx_get_sync_calib_param;
	dptx->dp_dis_reset = dptx_dis_reset;
	dptx->dptx_config_stream_timing = dptx_config_stream_timing;
	dptx->dptx_dsc_sample_en = dptx_dsc_sample_en;
	dptx->is_all_pll_config_by_dp_soft = false;
	dptx->dp_pixel_ppll7_param_calc = dp_pixel_ppll7_param_calc;
	dptx->lane_low_power_mode = LANE_POWER_MODE_P2;
	dptx->monitor_max_fps = PREFERRED_MAX_FPS_OF_MONITOR;
	dptx->max_tu = DPTX_MAX_TU_OF_MONITOR;
	dptx->max_hactive_low_temp = FHD_TIMING_H_ACTIVE;
	dptx->max_vactive_low_temp = FHD_TIMING_V_ACTIVE;
	dptx->dptx_ctrl_clk_enable = dptx_ctrl_clk_enable;
	dptx->dptx_hdcp_enable_video_transform = dptx_hdcp_enable_video_transform;
	dptx->dp_disable_clk = dp_disable_clk;
	dptx->dptx_phy_lane_reset = dptx_phy_lane_reset;

	return;
}

bool dp_ctrl_is_bypass_by_pg(uint32_t dp_id)
{
    struct soc_component_pg_info pg_info;
	void_unused(dp_id);

    get_soc_pg_info_by_index(SOC_COMPONENT_USB3_DP_INFO, &pg_info);
    dpu_pr_info("[DP] bypass by pg start");

    switch (pg_info.tailor_strategy) {
    case SOC_COMPONENT_NOT_TAILOR:
        dpu_pr_info("[DP] support DP");
        return false;
    case SOC_COMPONENT_TAILOR_STRATEGY_1:
        dpu_pr_warn("[DP] not support DP");
        return true;
    default:
        break;
    }

    dpu_pr_info("[DP] tailor_strategy = %d, default support DP", pg_info.tailor_strategy);
    return false;
}

void dp_enable_dpi_clk(char __iomem *dpu_base, bool enable)
{
	if (enable)
		set_reg(DPU_GLB_DISP_MODULE_CLK_EN_ADDR(dpu_base + DPU_GLB0_OFFSET), 1, 1, 1);
	else
		set_reg(DPU_GLB_DISP_MODULE_CLK_EN_ADDR(dpu_base + DPU_GLB0_OFFSET), 0, 1, 1);

	dpu_pr_debug("[DP] DISP_MODULE_CLK_EN = 0x%x",
		inp32(DPU_GLB_DISP_MODULE_CLK_EN_ADDR(dpu_base + DPU_GLB0_OFFSET)));
}