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

#include "dp_ctrl.h"
#include "dpu_conn_mgr.h"
#include "dp_drv.h"
#include "dp_aux.h"
#include <linux/types.h>
#include <linux/of.h>
#include "dp_ctrl_config.h"
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <platform_include/display/linux/dpu_dss_dp.h>

#include <securec.h>
#include "drm_dp_helper_additions.h"
#include "dpu_connector.h"
#include "dp_hdmi_common.h"
#include "dksm_utils.h"

#define WORKQUEUE_NAME_SIZE 128
#define EDID_FACTORY_START	0x08
#define EDID_FACTORY_END	0x11
#define EDID_BLOCK_NUM_MAX 4
#define EDID_BLOCK_LENGTH 128

struct dp_panel_mgr *g_dp_panel_mgr[MAX_DPTX_DEV_INDEX] = {};
struct platform_device *g_dkmd_dp_devive[MAX_DPTX_DEV_INDEX] = {};

bool dp_init_dpc_and_post_chn(struct dp_ctrl *dptx)
{
	int post_chn = -1;
	int dpc_index = dptx_hdmitx_init_dpc_num(dptx->port_id);
	if (dpc_index < 0) {
		dpu_pr_err("[DP] dptx_hdmitx_init_dpc_num failed!");
		return false;
	}

	post_chn = dptx_hdmitx_init_post_chn_num(dptx->port_id);
	if (post_chn < 0) {
		dpu_pr_err("[DP] dptx_hdmitx_init_post_chn_num failed!");
		dptx_hdmitx_deinit_dpc_num(dptx->port_id);
		return false;
	}
	dptx->dpc_index_selected = dpc_index;
	dptx->post_chn = post_chn;
	dpu_pr_info("[DP] dp_init_dpc_and_post_chn dpc_index_selected is %d, post_chn is %d", dpc_index, post_chn);
	return true;
}

void dp_deinit_dpc_and_post_chn(int index)
{
	dptx_hdmitx_deinit_post_chn_num(index);
	dptx_hdmitx_deinit_dpc_num(index);
}

int get_dp_devive_index(int port_id, uint32_t *dev_index)
{
	switch (port_id) { // usb/dp combophy：port 0~3, dp/edp combophy：port 4~5
	case DPTX_INVALID_PORT_ID: /* history platform use gfx_dp */
	case DPTX_PORT_ID_0:
		*dev_index = GFX_DP;
		break;
	case DPTX_PORT_ID_1:
		*dev_index = GFX_DP1;
		break;
	case DPTX_PORT_ID_2:
		*dev_index = GFX_DP2;
		break;
	case DPTX_PORT_ID_3:
		*dev_index = GFX_DP3;
		break;
	case DPTX_PORT_ID_4:
		*dev_index = GFX_DP4;
		break;
	case DPTX_PORT_ID_5:
		*dev_index = GFX_DP5;
		break;
	default:
		dpu_pr_err("[DP] input error port id, not support!");
		return -1;
	}
	dpu_pr_info("[DP] get_dp_devive_index: port id is %d, dev_index is %d", port_id, *dev_index);

	return 0;
}

void aux_set_backlight(int port_id, uint32_t bl_level)
{
	uint8_t cap = 0;
	int ret;
	struct dp_ctrl *dptx = NULL;
	uint32_t high_bit = (EDP_BACKLIGHT_HIGH_BIT_MASK & bl_level) >> 8;
	uint32_t low_bit = EDP_BACKLIGHT_LOW_BIT_MASK & bl_level;
 
	dptx = dpu_get_dptx_by_portid(port_id);
	if (!dptx || !dptx->dptx_enable) {
		dpu_pr_err("[DP] dptx is not ready\n");
		return ;
	}
 
	// config MSB/LSB mode.
	ret = dptx_write_dpcd(dptx, EDP_BACKLIGHT_MODE_SET, EDP_BACKLIGHT_MODE);
	if (ret)
		dpu_pr_err("[DP] EDP_BACKLIGHT_MODE_SET set failed. ret = %d\n", ret);
 
	// enable edp aux set backlight
	ret = dptx_write_dpcd(dptx, EDP_DISPLAY_CONTROL, 0x1);
	if (ret)
		dpu_pr_err("[DP] EDP_DISPLAY_CONTROL set failed. ret = %d\n", ret);

	// read MAX addr value
	ret = dptx_read_dpcd(dptx, EDP_PWMGEN_BIT_COUNT_CAP_MAX, &cap);
	if (ret)
		dpu_pr_err("[DP] get EDP_PWMGEN_BIT_COUNT_CAP_MAX failed. ret = %d\n", ret);

	// config bit count
	ret = dptx_write_dpcd(dptx, EDP_PWMGEN_BIT_COUNT, cap);
	if (ret)
		dpu_pr_err("[DP] EDP_PWMGEN_BIT_COUNT set failed. ret = %d\n", ret);
 
	// get capabilities
	ret = dptx_read_dpcd(dptx, EDP_BACKLIGHT_ADJUSTMENT_CAPABILITIES, &cap);
	if (ret)
		dpu_pr_err("[DP] get backlight capa failed. ret = %d\n", ret);
 
	// write LSB
	ret = dptx_write_dpcd(dptx, EDP_BACKLIGHT_BRIGHTNESS_LSB, (uint8_t)low_bit);
	if (ret)
		dpu_pr_err("[DP] set backlight LSB failed. ret = %d\n", ret);
 
	if ((cap & EDP_CAPABILITIES_MSB_LSB_MODE) == EDP_CAPABILITIES_MSB_LSB_MODE) {
		dpu_pr_info("[DP] support LSB/MSB config.\n");
		// write MSB
		ret = dptx_write_dpcd(dptx, EDP_BACKLIGHT_BRIGHTNESS_MSB, (uint8_t)high_bit);
		if (ret)
			dpu_pr_err("[DP] set backlight MSB failed. ret = %d\n", ret);
	}
	dpu_pr_info("[DP] high_bit:0x%lx, low_bit:0x%lx, bl_level:%d\n", high_bit, low_bit, bl_level);
}

int get_dp_edid_info(int port_id, struct dp_link_info* out_data)
{
	int ret = 0;
	uint32_t dev_index = GFX_DP; // dev_index: 0 1 2
	uint32_t edid_factory_len = EDID_FACTORY_END - EDID_FACTORY_START + 1;

	struct dkmd_connector_info *pinfo = NULL;
	struct dp_private *dp_priv = NULL;
	struct dp_ctrl *dptx = NULL;

	dpu_check_and_return(!out_data, -EINVAL, err, "[DP] out_data is null pointer!");
	dpu_check_and_return(!out_data->edid, -EINVAL, err, "[DP] out_data edid is null pointer!");

	ret = get_dp_devive_index(port_id, &dev_index);
	if (ret < 0) {
		dpu_pr_err("[DP] get_dp_devive_index: dev_index get error\n");
		return -EINVAL;
	}
	dpu_pr_info("[DP] get_dp_devive_index done: dev_index is %u\n", dev_index);

	dpu_check_and_return(!g_dkmd_dp_devive[dev_index], -EINVAL, err, "[DP] dp device is null pointer!");
	pinfo = platform_get_drvdata(g_dkmd_dp_devive[dev_index]);
	dp_priv = to_dp_private(pinfo);
	dptx = &dp_priv->dp[0];
	if (dptx->edid_len > EDID_BLOCK_NUM_MAX * EDID_BLOCK_LENGTH) {
		dpu_pr_err("[DP] edid_len[%d] is too long", dptx->edid_len);
		return -EINVAL;
	}

	out_data->xres = pinfo->base.xres;
	out_data->yres = pinfo->base.yres;
	out_data->fps = pinfo->base.fps;
	out_data->edid_len = dptx->edid_len;
	if (memcpy_s(out_data->edid, out_data->edid_len, dptx->edid, out_data->edid_len) != EOK) {
		dpu_pr_err("[DP] memcpy edid buffer error!");
		return -EINVAL;
	}
		
	if (memcpy_s(out_data->factory_edid_info, edid_factory_len, dptx->factory_edid_info, edid_factory_len) != EOK) {
		dpu_pr_err("[DP] memcpy factory_edid_info buffer error!");
		return -EINVAL;
	}

	dpu_pr_info("[DP] get_dp_edid_info edid_len is %d, xres is %d, yres is %d, fps is %d",
		out_data->edid_len, out_data->xres, out_data->yres, out_data->fps);
	return 0;
}

int get_dp_name(int port_id, char* out_dp_name)
{
	int ret = 0;
	uint32_t dev_index = GFX_DP; // dev_index: 0 1 2
	struct dkmd_connector_info *pinfo = NULL;

	dpu_check_and_return(!out_dp_name, -EINVAL, err, "[DP] out_data is null pointer!");

	ret = get_dp_devive_index(port_id, &dev_index);
	if (ret < 0) {
		dpu_pr_err("[DP] get_dp_devive_index: dev_index get error\n");
		return -EINVAL;
	}
	dpu_pr_info("[DP] get_dp_devive_index done: dev_index is %u\n", dev_index);

	dpu_check_and_return(!g_dkmd_dp_devive[dev_index], -EINVAL, err, "[DP] dp device is null pointer!");
	pinfo = platform_get_drvdata(g_dkmd_dp_devive[dev_index]);
	if (memcpy_s(out_dp_name, strlen(pinfo->base.name), pinfo->base.name, strlen(pinfo->base.name)) != EOK) {
		dpu_pr_err("[DP] memcpy dp_name error!");
		return -EINVAL;
	}
	return 0;
}

int32_t dp_set_backlight(struct dpu_connector *connector, const void *value)
{
	struct dp_private *dp_priv = NULL;
	struct dp_connect_ctrl_info *pinfo = NULL;
	uint32_t bl_level = 0;

	if (unlikely(!connector || !value))
		return -1;
	dp_priv = to_dp_private(connector->conn_info);
	if (unlikely(!dp_priv))
		return -1;

	if (dp_priv->dp[0].psr_params.is_psr_active) {
		dp_priv->dp[0].psr_params.backlight = *((uint32_t *)value);
		dpu_pr_debug("Set backlight_value is %d", dp_priv->dp[0].psr_params.backlight);
	} else {
		pinfo = dp_priv->dp[0].dp_info;
		bl_level = *((uint32_t *)value);
		if (pinfo && pinfo->set_backlight)
			pinfo->set_backlight(pinfo->port_id, bl_level);
		dpu_pr_debug("Set bl_level is %d", bl_level);
	}
	return 0;
}

int32_t register_dp_panel_mgr(struct dp_panel_mgr *dp_panel_mgr, int port_id)
{
	dpu_check_and_return(port_id < 0 || port_id >= MAX_DPTX_DEV_INDEX, -EINVAL, err, "[DP] port id is invalid\n");
	g_dp_panel_mgr[port_id] = dp_panel_mgr;
	return 0;
}

static void dp_panel_paser_rate_lane(struct device_node *np, struct dkmd_connector_info *pinfo) {
	int ret;
	uint32_t primary_panel = 0;

	ret = of_property_read_u32(np, "min_dptx_rate", &pinfo->min_dptx_rate);
	if (ret) {
		dpu_pr_info("[DP] get min_dptx_rate failed!\n");
		pinfo->min_dptx_rate = MIN_DPTX_RATE;
	}

	ret = of_property_read_u32(np, "max_dptx_rate", &pinfo->max_dptx_rate);
	if (ret) {
		dpu_pr_info("[DP] get max_dptx_rate failed!\n");
		pinfo->max_dptx_rate = DPTX_PHYIF_CTRL_MAX_RATE;
	}

	ret = of_property_read_u32(np, "min_dptx_lane", &pinfo->min_dptx_lane);
	if (ret) {
		dpu_pr_info("[DP] get min_dptx_lane failed!\n");
		pinfo->min_dptx_lane = MIN_DPTX_LANE;
	}

	ret = of_property_read_u32(np, "ssc_dptx_flag", &pinfo->ssc_dptx_flag);
	if (ret) {
		dpu_pr_info("[DP] get ssc_dptx_flag failed!\n");
		pinfo->ssc_dptx_flag = 1;
	}

	ret = of_property_read_u32(np, "dp_leakage_protect", &pinfo->dp_leakage_protect);
	if (ret) {
		dpu_pr_info("[DP] get dp_leakage_protect failed!");
		pinfo->dp_leakage_protect = 0;
	}

	ret = of_property_read_u32(np, "support_async_online", &pinfo->base.enable_async_online);
	if (ret) {
		dpu_pr_info("[DP] get enable_async_online failed!\n");
		pinfo->base.enable_async_online = 0;
	}
	dpu_pr_info("[DP] support_async_online=%#x", pinfo->base.enable_async_online);

	ret = of_property_read_u32(np, "fpga_flag", &pinfo->base.fpga_flag);
	if (ret) {
		dpu_pr_info("[DP] get fpga_flag failed!\n");
		pinfo->base.fpga_flag = 0;
	}
	dpu_pr_info("[DP] fpga_flag=%#x", pinfo->base.fpga_flag);
	ret = of_property_read_u32(np, "pipe_sw_itfch_idx", &pinfo->base.pipe_sw_itfch_idx);
	if (ret) {
		dpu_pr_info("[DP] get pipe_sw_itfch_idx failed!\n");
		pinfo->base.pipe_sw_itfch_idx = PIPE_SW_PRE_ITFCH1;
	}

	ret = of_property_read_u32(np, "fake_panel_flag", &pinfo->base.fake_panel_flag);
	if (ret)
		pinfo->base.fake_panel_flag = 0;

	if (of_property_read_u32(np, "is_primary_panel", &primary_panel)) {
		dpu_pr_info("[DP] get is_primary_panel failed! set to false\n");
		pinfo->base.is_primary_panel = false;
	}
	if (primary_panel) {
		dpu_pr_info("[DP] get is_primary_panel sucess! set to true\n");
		pinfo->base.is_primary_panel = true;
	}
}
static void dp_panel_parse_dsc(struct device_node *np, struct dkmd_connector_info *pinfo, struct dp_ctrl *dptx)
{
	int ret;

	ret = of_property_read_u32(np, "dsc_enable", &pinfo->dsc_enable);
	if (ret) {
		dpu_pr_info("[DP] get dsc_enable failed!\n");
		pinfo->dsc_enable = 0;
	}

	ret = of_property_read_u32(np, "dsc_height_div", &pinfo->dsc_height_div);
	if (ret) {
		dpu_pr_info("[DP] get dsc_height_div failed!\n");
		pinfo->dsc_height_div = 0;
	}

	ret = of_property_read_u32(np, "fec_enable", &dptx->fec);
	if (ret) {
		dpu_pr_info("[DP] get fec_enable failed!\n");
		dptx->fec = 0;
	}
	dpu_pr_info("[DP] dsc_enable=%d dsc_height_div=%d fec_enable=%d",
		pinfo->dsc_enable, pinfo->dsc_height_div, dptx->fec);
}

static void dp_panel_parse_bl_info(struct dkmd_connector_info *pinfo, struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "backlight_max_value", &pinfo->bl_info.bl_max);
	if (ret)
		pinfo->bl_info.bl_max = EDP_BACKLIGHT_MAX_VALUE;

	ret = of_property_read_u32(np, "bl_default", &pinfo->bl_info.bl_default);
	if (ret) {
		dpu_pr_info("[DP] get bl_default failed");
		pinfo->bl_info.bl_default = pinfo->bl_info.bl_max / 2;
	}
}

static void dp_panel_parse_esd_info(struct dp_private *priv, struct device_node *np)
{
	int ret;
	struct dkmd_connector_info *pinfo = &priv->connector_info;
	ret = of_property_read_u8(np, "esd_enable", &pinfo->esd_enable);
	if ((ret != 0) || (pinfo->esd_enable == 0)) {
		dpu_pr_info("[DP] %s no support esd or esd_enable parse failed\n", pinfo->base.name);
		return;
	}
	ret = of_property_read_u32(np, "esd_recovery_max_count", &priv->esd_info.esd_recovery_max_count);
	if (ret) {
		priv->esd_info.esd_recovery_max_count = 10;
		dpu_pr_info("[DP] esd_recovery_max_count parse failed\n");
	}

	ret = of_property_read_u32(np, "esd_check_max_count", &priv->esd_info.esd_check_max_count);
	if (ret) {
		priv->esd_info.esd_check_max_count = 3;
		dpu_pr_info("[DP] esd_check_max_count parse failed\n");
	}

	ret = of_property_read_u8(np, "esd_timing_ctrl", &priv->esd_info.esd_timing_ctrl);
	if (ret) {
		priv->esd_info.esd_timing_ctrl = 0;
		dpu_pr_info("[DP] esd_timing_ctrl parse failed\n");
	}

	ret = of_property_read_u32(np, "esd_check_time_period", &priv->esd_info.esd_check_time_period);
	if (ret) {
		priv->esd_info.esd_check_time_period = 5000;
		dpu_pr_info("[DP] esd_check_time_period parse failed\n");
	}

	ret = of_property_read_u32(np, "esd_first_check_delay", &priv->esd_info.esd_first_check_delay);
	if (ret) {
		priv->esd_info.esd_first_check_delay = 0;
		dpu_pr_info("[DP] esd_first_check_delay parse failed\n");
	}

	dpu_pr_info("[DP] %s parse esd info success!\n", pinfo->base.name);
}

static void dp_panel_get_vsync_type(struct dp_private *priv, struct device_node *np)
{
	int ret = 0;
	uint32_t vsync_ctrl_type = 0;
	ret = of_property_read_u32(np, "vsync_idle_flag", &vsync_ctrl_type);
	if (ret != 0) {
		return ;
	}

	if (vsync_ctrl_type != 0)
		priv->connector_info.vsync_ctrl_type |= VSYNC_IDLE_PSR2_CMD_MODE;
}

static void dp_panel_parse_common_for_priv(struct dp_private *priv, struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "dp_aux_phy_config", &priv->dp_aux_phy_config);
	if (ret != 0) {
		dpu_pr_info("[DP] get dp_aux_phy_config failed! set to 0");
		priv->dp_aux_phy_config = 0;
	}
}

static int32_t dp_panel_parse_dt(struct dp_private *priv, struct device_node *np)
{
	int ret;
	uint32_t connector_count = 0;
	uint32_t max_dp_link = 3;
	uint32_t value = 0;
	const __be32 *p = NULL;
	struct property *prop = NULL;
	struct dp_ctrl *dptx0 = &priv->dp[0];
	struct dp_ctrl *dptx1 = &priv->dp[1];
	struct dkmd_connector_info *pinfo = &priv->connector_info;

	dpu_check_and_return(init_dpctrl_pixel_clk(dptx0, NULL, np), -ENXIO, err, "[DP] init_dpctrl_pixel_clk failed");

	dptx0->hidptx_base = of_iomap(np, REG_HIDPTX_BASE);
	if (!dptx0->hidptx_base) {
		dpu_pr_err("[DP] failed to get hidptx_base!\n");
		return -ENXIO;
	}

	priv->hsdt1_crg_base = of_iomap(np, REG_HSDT1_CRG_BASE);
	if (!priv->hsdt1_crg_base) {
		dpu_pr_err("[DP] failed to get hsdt1_crg_base!\n");
		return -ENXIO;
	}

	dptx0->combo_phy_base = of_iomap(np, REG_COMBOPHY_BASE);
	if (!dptx0->combo_phy_base) {
		dpu_pr_err("[DP] failed to get combo_phy_base!\n");
		return -ENXIO;
	}

	/* v740 adjust difference add */
	dpu_check_and_return(init_dpctrl_16m_clk(dptx0, np), -ENXIO, err, "[DP] init_dpctrl_16m_clk failed");
	dpu_check_and_return(init_hsdt1_sysctrl_base(priv, np), -ENXIO, err, "[DP] init_hsdt1_sysctrl_base failed");
	dpu_check_and_return(init_sub_harden_crg_base(priv, np), -ENXIO, err, "[DP] init_sub_harden_crg_base failed");
	dpu_check_and_return(init_dp_sctrl(priv, np), -ENXIO, err, "[DP] init_dp_sctrl failed");

	priv->hpd_gpio = (uint32_t)of_get_named_gpio(np, "gpios", 0);
	dpu_pr_info("[DP] hpd_gpio= %d", priv->hpd_gpio);

	if (of_property_read_u32(np, "dp_hpd_plug", &priv->dp_hpd_plug)) {
		dpu_pr_info("[DP] get dp_hpd_plug failed! set to 0\n");
		priv->dp_hpd_plug = 0;
	}

	if (of_property_read_u32(np, "dp_hpd_jump_plugout", &priv->dp_hpd_jump_plugout)) {
		dpu_pr_info("[DP] get dp_hpd_jump_plugout failed! set to 0\n");
		priv->dp_hpd_jump_plugout = 0;
	}

	ret = of_property_read_string(np, "gfx_dp_dev_name", &pinfo->base.name);
	if (ret) {
		dpu_pr_info("[DP] get gfx_dp_dev_name failed!\n");
		pinfo->base.name = DTS_GFX_DP_NAME;
	}
	if (ret == 0 && strcmp(pinfo->base.name, DTS_GFX_DP_NAME) == 0) {
		ret = of_property_read_u32(np, "max_dp_link", &max_dp_link);
		dpu_pr_info("[DP] get max_dp_link ret %d, max_dp_link is %d!\n", ret, max_dp_link);
		if (ret == 0)
			set_dp_hdmi_link_num((int)max_dp_link);
	}
	dpu_pr_info("[DP] pinfo->base.name=%s\n", pinfo->base.name);

	dp_panel_paser_rate_lane(np, pinfo);
	dp_panel_parse_dsc(np, pinfo, dptx0);
	dp_panel_parse_common_for_priv(priv, np);

	ret = of_property_read_u32(np, "mode", &pinfo->base.mode);
	if (ret)
		pinfo->base.mode = 0;

	ret = of_property_read_u32(np, "channel_id", &pinfo->base.id);
	if (ret)
		pinfo->base.id = 0;
		
	ret = of_property_read_u32(np, "support_aux_backlight", &priv->support_aux_backlight);
	if (ret)
		priv->support_aux_backlight = 0;

	if (priv->support_aux_backlight != 0)
		dp_panel_parse_bl_info(pinfo, np);

	if (of_property_read_u32(np, "filter_resolution_enable", &pinfo->filter_resolution_enable)) {
		dpu_pr_info("[DP] get filter_resolution_enable failed! set to 0");
		pinfo->filter_resolution_enable = 0;
	}

	dpu_pr_info("[DP] min_lane=%#x, min_rate=%#x, max_rate=%#x, ssc_flag=%#x, pipe_sw_pre_itfch_idx=%#x, \
		fake_panel_flag=%#x, dptx_mode=%#x, dptx_id=%#x, dp_hpd_plug=%d, leakpro=%d, jump_plugout=%d, \
		filter_resolution_enable=%d, aux_phy=0x%x",
		pinfo->min_dptx_lane, pinfo->min_dptx_rate, pinfo->max_dptx_rate, pinfo->ssc_dptx_flag,
		pinfo->base.pipe_sw_itfch_idx, pinfo->base.fake_panel_flag, pinfo->base.mode, pinfo->base.id,
		priv->dp_hpd_plug, pinfo->dp_leakage_protect, priv->dp_hpd_jump_plugout,
		pinfo->filter_resolution_enable, priv->dp_aux_phy_config);

	of_property_for_each_u32(np, "connector_id", prop, p, value) {
		if (connector_count >= MAX_CONNECT_CHN_NUM)
			break;
		++connector_count;
	}

	ret = of_property_read_u32(np, "support_hardware_cursor", &value);
	pinfo->base.is_hardware_cursor_support = (ret == 0) && (value != 0);
	dpu_pr_info("[DP] hardware cursor support: %u\n", (uint32_t)pinfo->base.is_hardware_cursor_support);

	dptx0->is_dual_tx = false;
	dptx1->is_dual_tx = false;
	if (connector_count > EXTERNAL_CONNECT_CHN_IDX) {
		dpu_check_and_return(init_dpctrl_pixel_clk(NULL, dptx1, np), -ENXIO, err, "[DP] init_dpctrl_pixel_clk failed");

		dptx1->hidptx_base = of_iomap(np, REG_HIDPTX_EXTERNAL_BASE);
		if (!dptx1->hidptx_base) {
			dpu_pr_err("[DP] failed to get hidptx_base!\n");
			return -ENXIO;
		}

		dptx1->combo_phy_base = of_iomap(np, REG_COMBOPHY_EXTERNAL_BASE);
		if (!dptx1->combo_phy_base) {
			dpu_pr_err("[DP] failed to get combo_phy_base!\n");
			return -ENXIO;
		}

		dptx0->is_dual_tx = true;
		dptx1->is_dual_tx = true;
	}
	base_panel_connector_dts_parse(pinfo, np);
	dp_panel_parse_esd_info(priv, np);
	dp_panel_get_vsync_type(priv, np);

	return 0;
}

static const struct of_device_id dp_device_match_table[] = {
	{
		.compatible = DTS_COMP_DP,
		.data = NULL,
	},
	{
		.compatible = DTS_COMP_DP1,
		.data = NULL,
	},
	{
		.compatible = DTS_COMP_DP2,
		.data = NULL,
	},
	{
		.compatible = DTS_COMP_DP3,
		.data = NULL,
	},
	{
		.compatible = DTS_COMP_DP4,
		.data = NULL,
	},
	{
		.compatible = DTS_COMP_DP5,
		.data = NULL,
	},
	{},
};
MODULE_DEVICE_TABLE(of, dp_device_match_table);

static void dp_save_device(struct platform_device *pdev, struct dp_private *priv)
{
	int dev_index = -1;

	if (priv == NULL || pdev == NULL) {
		dpu_pr_err("[DP] pdev or priv is null ptr, error return!\n");
		return;
	}

	if (strstr(priv->connector_info.base.name, DTS_GFX_DP_NAME) != NULL) {
		dev_index = get_str_suffix_num(priv->connector_info.base.name, DTS_GFX_DP_NAME);
		if (dev_index < 0) {
			dpu_pr_err("[DP] gfx dp name is invalid!!!\n");
			return;
		}
	} else {
		dpu_pr_err("[DP] gfx dp name is invalid!!!\n");
		return;
	}

	dpu_pr_debug("[DP] gfx dp name %s dev_index is %d", priv->connector_info.base.name, dev_index);
	g_dkmd_dp_devive[dev_index] = pdev;
	return;
}

static void dptx_isr_pluggable_wq_handler(struct work_struct *work)
{
	struct dp_ctrl *dptx = NULL;
	uint32_t is_pluged = 0;
	struct dp_private *dp_priv = NULL;

	dptx = container_of(work, struct dp_ctrl, dptx_pluggable_work);
	dpu_check_and_no_retval((dptx == NULL), err, "[DPTX] dptx is NULL!");
	dpu_pr_info("[DP] workqueue irq handler start for dpport %d", dptx->port_id);

	dp_priv = to_dp_private(dptx->connector->conn_info);
	dpu_check_and_no_retval(!dp_priv, err, "DPTX is null!");
	is_pluged = (uint32_t)gpio_get_value(dp_priv->hpd_gpio);
	dpu_pr_info("[DP] is_pluged %d", is_pluged);
	if (is_pluged == 1) {
		mdelay(100);
		is_pluged = (uint32_t)gpio_get_value(dp_priv->hpd_gpio);
		dpu_pr_info("[DP] mdelay is_pluged %d", is_pluged);
		if (is_pluged != 1) {
			dpu_pr_info("[DP] ignore the irq is_pluged %d", is_pluged);
		} else {
			dpu_pr_info("[DP] port %d hotplug in!", dptx->port_id);
			dpu_multi_dptx_hpd_trigger(TCA_IRQ_HPD_IN, TCPC_DP, DP_PLUG_TYPE_NORMAL, MASTER_DPTX_IDX, dptx->port_id);
		}
	}
}

static int dptx_pluggable_wq_handler(struct dp_ctrl *dptx, bool setup)
{
	int ret = -1;
	char wq_name[WORKQUEUE_NAME_SIZE] = {0};
	if (setup) {
		ret = snprintf_s(wq_name, sizeof(wq_name), sizeof(wq_name) - 1, "dptx_edp_wq_%d", dptx->port_id);
		if (ret < 0) {
			dpu_pr_err("[DP] create wq_name err");
			return -EINVAL;
		}
		dpu_pr_info("[DP] create retraining workqueue : %s", wq_name);
		dptx->dptx_pluggable_wq = create_singlethread_workqueue(wq_name);
		if (dptx->dptx_pluggable_wq == NULL) {
			dpu_pr_err("[DP] create dptx_pluggable_wq failed");
			return -1;
		}
		INIT_WORK(&dptx->dptx_pluggable_work, dptx_isr_pluggable_wq_handler);
	} else {
		if (dptx->dptx_pluggable_wq != NULL) {
			destroy_workqueue(dptx->dptx_pluggable_wq);
			dptx->dptx_pluggable_wq = NULL;
		}
	}
	return 0;
}

static int32_t dp_switch_register(struct dp_ctrl *dptx)
{
	dpu_check_and_return(!dptx, -EINVAL, err, "[DP] dptx is null\n");
	// register dp hpd devive
	if (switch_dev_register(&dptx->sdev) < 0)
		dpu_pr_err("[DP] dp switch register [%s] failed!", dptx->sdev.name);
	// register hdmi_audio hpd devive
	if (switch_dev_register(&dptx->dp_switch) < 0)
		dpu_pr_err("[DP] dp switch register [%s] failed!", dptx->dp_switch.name);

	return 0;
}

static void dp_panel_set_esd_info(struct dp_ctrl *dptx, struct dp_private *priv)
{
	/* set dp default esd info */
	if (priv->connector_info.esd_enable == 1) {
		dpu_pr_info("[DP] %s set dp default esd info!", priv->connector_info.base.name);
		dptx->connector = get_primary_connector(&priv->connector_info);
		dptx->connector->esd_info = priv->esd_info;
		dptx->esd_debug_mode = 0;
	}

	/* set dp esd info */
	if (dptx->dp_panel_mgr && dptx->dp_panel_mgr->esd_is_support) {
		dpu_pr_info("[DP] %s esd_is_support:%u, set dp_panel_mgr esd info\n",
			priv->connector_info.base.name, dptx->dp_panel_mgr->esd_is_support);
		priv->connector_info.esd_enable = dptx->dp_panel_mgr->esd_is_support;
		dptx->connector = get_primary_connector(&priv->connector_info);
		dptx->connector->esd_info = dptx->dp_panel_mgr->esd_info;
	}
}

static void get_edp_dfr_info(struct dkmd_connector_info *pinfo, struct dfr_info **out)
{
	struct dp_private *priv = to_dp_private(pinfo);
	if (unlikely(!priv)) {
		dpu_pr_err("[DP] get private data failed!\n");
		return;
	}
 
	*out = &priv->dp[MASTER_DPTX_IDX].dp_panel_mgr->dfr_info;
}

static void dp_panel_set_dfr_info(struct dp_ctrl *dptx, struct dp_private *priv)
{
	if (dptx->dp_panel_mgr != NULL && dptx->dp_panel_mgr->fps_support != 0) {
		priv->connector_info.support_te = true;
		priv->te_from_tcon_gpio = dptx->dp_panel_mgr->te_from_tcon_gpio;
		priv->connector_info.get_dfr_info = get_edp_dfr_info;
		dpu_pr_info("[DP] te_from_tcon_gpio: %u, support_te: %d",
			priv->te_from_tcon_gpio, priv->connector_info.support_te);
	} else {
		priv->connector_info.support_te = false;
	}
}

static void get_dpu_pu_cfg_info(struct dkmd_connector_info *pinfo, struct user_panel_info *dpu_pu_cfg_info)
{
	struct dp_private *priv = to_dp_private(pinfo);

	*dpu_pu_cfg_info = priv->dpu_pu_cfg_info;
}

static int32_t dp_probe(struct platform_device *pdev)
{
	struct dp_private *priv = NULL;
	struct dp_ctrl *dptx = NULL;

	dpu_check_and_return(!pdev, -EINVAL, err, "[DP] pdev is null\n");

	dpu_pr_info("[DP] +\n");

	if (!is_connector_manager_available())
		return -EPROBE_DEFER;

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	dpu_check_and_return(!priv, -EINVAL, err, "[DP] alloc offline driver private data failed!\n");

	priv->device = pdev;
	if (dp_panel_parse_dt(priv, pdev->dev.of_node)){
		dpu_pr_err("[DP] parse dts error!\n");
		return -EINVAL;
	}

	if (priv->connector_info.base.name == NULL) {
		dpu_pr_warn("[DP] get priv->connector_info.base.name from dtsi failed!");
		return -EINVAL;
	}
	dpu_pr_info("[DP] %s complete!\n", priv->connector_info.base.name);

	/* this is the tail device */
	priv->connector_info.base.peri_device = pdev;
	priv->connector_info.base.type = PANEL_DP;
	if (priv->connector_info.base.is_primary_panel)
		priv->connector_info.base.type |= PANEL_PRIMARY;

	priv->connector_info.base.pluggable_connect_mutex = devm_kzalloc(&pdev->dev, sizeof(struct mutex), GFP_KERNEL);
	if (!priv->connector_info.base.pluggable_connect_mutex) {
		dpu_pr_warn("[DP] alloc pluggable_connect_mutex failed!\n");
		return -EINVAL;
	}
	mutex_init(priv->connector_info.base.pluggable_connect_mutex);

	/* add private data to device */
	platform_set_drvdata(pdev, &priv->connector_info);

	dptx = &priv->dp[MASTER_DPTX_IDX];
	if (priv->connector_info.connector_idx[PRIMARY_CONNECT_CHN_IDX] == CONNECTOR_ID_EDP)
		dptx->port_id = EDP0_PORT_ID;

	/* set dp panel mgr */
	if (dptx->port_id >= 0 && dptx->port_id < MAX_DPTX_DEV_INDEX)
		dptx->dp_panel_mgr = g_dp_panel_mgr[dptx->port_id];

	dp_panel_set_esd_info(dptx, priv);
	dp_panel_set_dfr_info(dptx, priv);

	if (dptx->dp_panel_mgr != NULL) {
		priv->dpu_pu_cfg_info = dptx->dp_panel_mgr->dpu_pu_cfg_info;
		priv->connector_info.dirty_region_updt_support = dptx->dp_panel_mgr->dpu_pu_cfg_info.dirty_region_updt_support;
		priv->connector_info.base.dirty_region_updt_support = dptx->dp_panel_mgr->dpu_pu_cfg_info.dirty_region_updt_support;
	}

	/* will create chrdev: /dev/gfx_dpX  */
	if (register_connector(&priv->connector_info) != 0) {
		dpu_pr_warn("[DP] device register failed, need retry!\n");
		return -EPROBE_DEFER;
	}

	priv->connector_info.get_panel_user_info = get_dpu_pu_cfg_info;

	dp_switch_register(dptx);

	/* save dp_private to list */
	dp_save_device(pdev, priv);

	/* set dp power saving mode */
	if (dptx->dptx_combophy_power_saving_handler)
		dptx->dptx_combophy_power_saving_handler(dptx);

	if (dptx->connector->conn_info->base.mode == EDP_MODE) {
		dpu_pr_info("[DP] enable pluggable wq, portid: %d", dptx->port_id);
		if (dptx_pluggable_wq_handler(dptx, true))
			dpu_pr_err("[DP] dptx retraining workqueue failed!\n");

		if (dptx->dptx_pluggable_wq != NULL)
			queue_work(dptx->dptx_pluggable_wq, &dptx->dptx_pluggable_work);
	}

	if (priv->dp_hpd_plug == 1) {
		dpu_pr_info("[DP] enable_irq priv->hpd_irq_no: %d", priv->hpd_irq_no);
		enable_irq(priv->hpd_irq_no);
	}

	if (priv->support_aux_backlight) {
		dpu_pr_info("[DP] EDP set backlight by aux.\n");
		if (!dptx->dp_info)
			dptx->dp_info = kzalloc(sizeof(struct dp_connect_ctrl_info), GFP_KERNEL);

		if (dptx->dp_info) {
			dptx->dp_info->port_id = dptx->port_id;
			if (dptx->dp_panel_mgr && dptx->dp_panel_mgr->panel_power_set_backlight) {
				dpu_pr_info("[DP] EDP set backlight by aux LCD_BACKLIGHT");
				dptx->dp_info->set_backlight = dptx->dp_panel_mgr->panel_power_set_backlight;
			} else {
				dpu_pr_info("[DP] set aux_set_backlight");
				dptx->dp_info->set_backlight = aux_set_backlight;
			}
		} else {
			dpu_pr_err("[DP] EDP set backlight interface error");
		}
	}

	dp_hdmi_mutex_init();
	return 0;
}

/**
 * Clear resource when device removed but not for devicetree device
 */
static int32_t dp_remove(struct platform_device *pdev)
{
	struct dp_private *priv = platform_get_drvdata(pdev);
	struct dp_ctrl *dptx = NULL;

	if (!priv) {
		dpu_pr_err("[DP] get dsi private data failed!\n");
		return -EINVAL;
	}
	dptx = &priv->dp[MASTER_DPTX_IDX];
	if (dptx->connector->conn_info->base.mode == EDP_MODE) {
		dpu_pr_info("[DP] dp_remove priv->hpd_gpio: %d, priv->hpd_irq_no: %d",
			priv->hpd_gpio, priv->hpd_irq_no);
		gpio_free(priv->hpd_gpio);
		free_irq(priv->hpd_irq_no, NULL);
		if (dptx_pluggable_wq_handler(dptx, false) != 0)
			dpu_pr_err("[DP] dptx rm wq failed!\n");
	}

	unregister_connector(&priv->connector_info);

	// unregister dp hpd devive
	switch_dev_unregister(&dptx->sdev);
	// unregister hdmi_audio hpd devive
	switch_dev_unregister(&dptx->dp_switch);

	if (dptx->edid != NULL) {
		kfree(dptx->edid);
		dptx->edid = NULL;
	}
	if (dptx->dp_info)
		kfree(dptx->dp_info);
		
	dpu_pr_info("[DP] %s remove complete!\n", priv->connector_info.base.name);

	return 0;
}

static const struct dev_pm_ops dp_dev_pm_ops = {
#ifdef CONFIG_PM_SLEEP
	.suspend = dp_pm_suspend,
	.resume = NULL,
#endif
#ifdef CONFIG_HIBERNATION
	.freeze = dp_pm_freeze,
	.restore = NULL,
#endif
};

static struct platform_driver dp_platform_driver = {
	.probe  = dp_probe,
	.remove = dp_remove,
	.driver = {
		.name  = DEV_NAME_DP,
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(dp_device_match_table),
		.pm = &dp_dev_pm_ops,
	}
};

static int32_t __init dp_register(void)
{
	return platform_driver_register(&dp_platform_driver);
}

static void __exit dp_unregister(void)
{
	platform_driver_unregister(&dp_platform_driver);
}

late_initcall(dp_register);
module_exit(dp_unregister);

MODULE_AUTHOR("Graphics Display");
MODULE_DESCRIPTION("DP Module Driver");
MODULE_LICENSE("GPL");
