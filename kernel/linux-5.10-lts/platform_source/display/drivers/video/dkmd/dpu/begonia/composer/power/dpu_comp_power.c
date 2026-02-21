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

#include "dkmd_log.h"

#include <linux/delay.h>
#include <dpu/soc_dpu_define.h>
#include "dpu_dacc.h"
#include "dpu_config_utils.h"
#include "dpu_comp_smmu.h"
#include "dpu_comp_online.h"
#include "dpu_comp_offline.h"
#include "dpu_comp_mgr.h"
#include "dpu_comp_init.h"
#include "dpu_comp_config_utils.h"
#include "dpu_isr_mdp.h"
#include "dpu_isr_sdp.h"
#include "dpu_isr_dvfs.h"
#include "dpu_sh_aod.h"
#include "dpu_comp_tui.h"
#include "dpu_comp_esd.h"
#include "dvfs.h"
#include "ddr_dvfs.h"
#include "dpu_conn_mgr.h"
#include "dpu_comp_low_temp_handler.h"
#include "gfxdev_mgr.h"
#include "dkmd_connector.h"

int32_t media_regulator_enable(struct composer_manager *comp_mgr)
{
	int32_t ret = 0;

	ret = regulator_enable(comp_mgr->media1_subsys_regulator);
	if (ret)
		dpu_pr_err("regulator enable media1_subsys failed, error=%d!", ret);

	ret = regulator_enable(comp_mgr->vivobus_regulator);
	if (ret)
		dpu_pr_err("regulator enable vivobus failed, error=%d!", ret);

	return ret;
}

int32_t media_regulator_disable(struct composer_manager *comp_mgr)
{
	int32_t ret = 0;

	ret = regulator_disable(comp_mgr->vivobus_regulator);
	if (ret)
		dpu_pr_err("regulator disable vivobus failed, error=%d!", ret);

	ret = regulator_disable(comp_mgr->media1_subsys_regulator);
	if (ret)
		dpu_pr_err("regulator disable media1_subsys failed, error=%d!", ret);

	return ret;
}

void vivobus_autodiv_regulator_enable(struct composer_manager *comp_mgr)
{
	int32_t ret = 0;
	if (IS_ERR_OR_NULL(comp_mgr) || IS_ERR_OR_NULL(comp_mgr->vivobus_autodiv_regulator))
		return;

	ret = regulator_enable(comp_mgr->vivobus_autodiv_regulator);
	if (ret)
		dpu_pr_err("vivobus_autodiv_regulator enable failed, error=%d!", ret);

	return;
}

void vivobus_autodiv_regulator_disable(struct composer_manager *comp_mgr)
{
	int32_t ret = 0;
	if (IS_ERR_OR_NULL(comp_mgr) || IS_ERR_OR_NULL(comp_mgr->vivobus_autodiv_regulator))
		return;

	// multiple disable is allowed
	ret = regulator_disable(comp_mgr->vivobus_autodiv_regulator);
	if (ret)
		dpu_pr_info("vivobus_autodiv_regulator enable failed, error=%d!", ret);

	return;
}

static int32_t composer_regulator_enable(struct composer_manager *comp_mgr)
{
	int32_t ret = 0;
	dpu_pr_info("composer_regulator_enable + ");
	if (!IS_ERR_OR_NULL(comp_mgr->dsssubsys_regulator)) {
		ret = regulator_enable(comp_mgr->dsssubsys_regulator);
		if (ret)
			dpu_pr_err("dsssubsys_regulator enable failed, error=%d!", ret);
	}

	if (!IS_ERR_OR_NULL(comp_mgr->disp_ch1subsys_regulator)) {
		ret = regulator_enable(comp_mgr->disp_ch1subsys_regulator);
		if (ret)
			dpu_pr_err("disp_ch1subsys_regulator enable failed, error=%d!", ret);
	}

	if (!IS_ERR_OR_NULL(comp_mgr->regulator_smmu_tcu)) {
		ret = regulator_enable(comp_mgr->regulator_smmu_tcu);
		if (ret)
			dpu_pr_warn("smmu tcu regulator_enable failed, error=%d!", ret);
	}

	return ret;
}

static int32_t composer_regulator_disable(struct composer_manager *comp_mgr)
{
	int32_t ret = 0;
	dpu_pr_info("composer_regulator_disable + ");
	if (!IS_ERR_OR_NULL(comp_mgr->regulator_smmu_tcu)) {
		ret = regulator_disable(comp_mgr->regulator_smmu_tcu);
		if (ret)
			dpu_pr_warn("smmu tcu regulator_disable failed, error=%d!", ret);
	}

	if (!IS_ERR_OR_NULL(comp_mgr->disp_ch1subsys_regulator)) {
		ret = regulator_disable(comp_mgr->disp_ch1subsys_regulator);
		if (ret)
			dpu_pr_err("disp_ch1subsys_regulator disable failed, error=%d!", ret);
	}

	if (!IS_ERR_OR_NULL(comp_mgr->dsssubsys_regulator)) {
		ret = regulator_disable(comp_mgr->dsssubsys_regulator);
		if (ret)
			dpu_pr_err("dsssubsys_regulator disable failed, error=%d!", ret);
	}

	return 0;
}

static void composer_manager_hiace_on(struct dpu_composer *dpu_comp)
{
	if (!dpu_comp->hiace_ctrl)
		return;

	dpu_comp->hiace_ctrl->init_params(dpu_comp);
	dpu_comp->hiace_ctrl->unblank(dpu_comp->hiace_ctrl);
}

static void composer_manager_hiace_off(struct dpu_composer *dpu_comp)
{
	if (!dpu_comp->hiace_ctrl)
		return;

	dpu_comp->hiace_ctrl->blank(dpu_comp->hiace_ctrl);
}

static void composer_dpu_simple_power_on(struct composer_manager *comp_mgr, struct composer *comp)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	if (!is_mipi_video_panel(&dpu_comp->conn_info->base))
		return;
	dpu_ddr_vote_max();
	dpu_dvfs_enable_core_clock(true);
	dpu_dvfs_direct_vote(comp->index, DPU_PERF_LEVEL_MAX, false);
	composer_regulator_enable(comp_mgr);
	dpu_dvfs_qos_qic_media1_config(DPU_PERF_LEVEL_MAX);
	dpu_power_on_state_for_ddr_dfs(comp_mgr->pctrl_base, true);
	vivobus_autodiv_regulator_enable(comp_mgr);

	/* Enable peri dvfs to make sure HW_DSS_DVFS receive frm_start signal */
	dpu_dvfs_ctrl_config(comp_mgr->dpu_base);
}

void composer_dpu_power_on(struct composer_manager *comp_mgr, struct composer *comp)
{
	dpu_dvfs_enable_core_clock(true);
	dpu_dvfs_direct_vote(comp->index, DPU_CORE_LEVEL_ON, false);
	composer_regulator_enable(comp_mgr);
	dpu_dvfs_qos_qic_media1_config(DPU_CORE_LEVEL_ON);
	dpu_power_on_state_for_ddr_dfs(comp_mgr->pctrl_base, true);
	vivobus_autodiv_regulator_enable(comp_mgr);

	/* Cmdlist preparation, need to use the CPU configuration */
	set_reg(comp_mgr->dpu_base + DPU_GLB_PM_CTRL_ADDR(DPU_GLB0_OFFSET), GLB_PM_CTRL_VALUE, 32, 0);
	set_reg(comp_mgr->dpu_base + DPU_DSI_PM_CTRL_ADDR(DPU_MIPI_DSI0_OFFSET), DSI_PM_CTRL_VALUE, 32, 0);
	set_reg(comp_mgr->dpu_base + DPU_DSI_PM_CTRL_ADDR(DPU_MIPI_DSI2_OFFSET), DSI_PM_CTRL_VALUE, 32, 0);

	set_reg(DPU_GLB_SDMA_DBG_RESERVED0_ADDR(comp_mgr->dpu_base + DPU_GLB0_OFFSET, 0), 1, 1, 20);
	set_reg(DPU_GLB_SDMA_DBG_RESERVED0_ADDR(comp_mgr->dpu_base + DPU_GLB0_OFFSET, 1), 1, 1, 20);
	set_reg(DPU_GLB_SDMA_DBG_RESERVED0_ADDR(comp_mgr->dpu_base + DPU_GLB0_OFFSET, 2), 1, 1, 20);
	set_reg(DPU_GLB_SDMA_DBG_RESERVED0_ADDR(comp_mgr->dpu_base + DPU_GLB0_OFFSET, 3), 1, 1, 20);

	dpu_sdma_load_balance_config(comp_mgr->dpu_base);

	/* Enable peri dvfs to make sure HW_DSS_DVFS receive frm_start signal */
	dpu_dvfs_ctrl_config(comp_mgr->dpu_base);
}

void composer_dpu_power_on_sub(struct dpu_composer *dpu_comp)
{
	struct dkmd_rect panel_rect = { 0 };
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;

	panel_rect.w = pinfo->base.xres;
	panel_rect.h = pinfo->base.yres;

	dpu_comp_smmuv3_on(comp_mgr, dpu_comp);
	composer_manager_hiace_on(dpu_comp);

	(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, INIT_DSC, pinfo);
	(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, INIT_SPR, NULL);
	(void)pipeline_next_on(pinfo->conn_device, pinfo);
	(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, ENABLE_ISR, &dpu_comp->isr_ctrl);
	(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, SET_PARTIAL_UPDATE, &panel_rect);
}

void composer_dpu_power_off(struct composer_manager *comp_mgr, struct composer *comp)
{
	vivobus_autodiv_regulator_disable(comp_mgr);
	dpu_power_on_state_for_ddr_dfs(comp_mgr->pctrl_base, false);
	composer_regulator_disable(comp_mgr);
	dpu_dvfs_direct_vote(comp->index, DPU_CORE_LEVEL_OFF, false);
	dpu_dvfs_disable_core_clock();
}

static bool composer_dpu_pipesw_special_platform(void)
{
	/* v800 v820 v900 when clear itf occur underflow in multiple scenarios */
	return (g_dpu_config_data.version.info.version == DPU_ACCEL_DPUV800 ||
			g_dpu_config_data.version.info.version == DPU_ACCEL_DPUV820 ||
			g_dpu_config_data.version.info.version == DPU_ACCEL_DPUV900);
}

void composer_dpu_pipesw_disconnect(struct dpu_composer *dpu_comp)
{
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;
	int32_t primary_post_ch_offset = 0x0;
	uint32_t external_post_ch_offset = 0x0;
	uint32_t pre_itfch_offset = 0x0;

	dpu_pr_debug("enter");
	if ((!is_primary_panel(&pinfo->base)) && (!is_builtin_panel(&pinfo->base))) {
		dpu_pr_debug("comp index %u isnot primary_panel or builtin_panel", dpu_comp->comp.index);
		return;
	}
	if (pinfo->sw_post_chn_num == 0) {
		dpu_pr_err("comp index %u sw_post_chn_num invalid", dpu_comp->comp.index);
		return;
	}
	primary_post_ch_offset = (int32_t)pinfo->sw_post_chn_idx[PRIMARY_CONNECT_CHN_IDX] * 0x1C;
	/* cpro dp5 */
	if (pinfo->sw_post_chn_idx[PRIMARY_CONNECT_CHN_IDX] == CONNECTOR_ID_OFFLINE)
		primary_post_ch_offset = 0x0100;

	dpu_pr_info("primary_post_ch_offset=%d comp_scene_id=%d",
		pinfo->sw_post_chn_idx[PRIMARY_CONNECT_CHN_IDX], dpu_comp->comp_scene_id);

	if (composer_dpu_pipesw_special_platform() && dpu_comp->comp_scene_id <= DPU_SCENE_ONLINE_2) {
		pre_itfch_offset = (uint32_t)(DPU_ITF_CH0_OFFSET + dpu_comp->comp_scene_id * 0x100);
		/* 0xFF70FF7 is The maximum value of the chip’s recommended circumvention solution */
		set_reg(DPU_ITF_CH_IMG_SIZE_ADDR(comp_mgr->dpu_base + pre_itfch_offset), 0xFF70FF7, 32, 0);
		set_reg(DPU_ITF_CH_REG_CTRL_FLUSH_EN_ADDR(comp_mgr->dpu_base + pre_itfch_offset), 0x1, 32, 0);
	}

	set_reg(DPU_PIPE_SW_SIG_CTRL_0_ADDR(comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, 0, 32, 0);
	set_reg(DPU_PIPE_SW_SW_POS_CTRL_SIG_EN_0_ADDR(comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, 0, 32, 0);
	set_reg(DPU_PIPE_SW_DAT_CTRL_0_ADDR(comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, 0, 32, 0);
	set_reg(DPU_PIPE_SW_SW_POS_CTRL_DAT_EN_0_ADDR(comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, 0, 32, 0);

	if (pinfo->sw_post_chn_num > EXTERNAL_CONNECT_CHN_IDX) {
		external_post_ch_offset = pinfo->sw_post_chn_idx[EXTERNAL_CONNECT_CHN_IDX] * 0x1C;
		set_reg(DPU_PIPE_SW_SIG_CTRL_0_ADDR(comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
			+ external_post_ch_offset, 0, 32, 0);
		set_reg(DPU_PIPE_SW_SW_POS_CTRL_SIG_EN_0_ADDR(comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
			+ external_post_ch_offset, 0, 32, 0);
		set_reg(DPU_PIPE_SW_DAT_CTRL_0_ADDR(comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
			+ external_post_ch_offset, 0, 32, 0);
		set_reg(DPU_PIPE_SW_SW_POS_CTRL_DAT_EN_0_ADDR(comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
			+ external_post_ch_offset, 0, 32, 0);
	}
}

static void composer_dpu_wait_vactive_end(struct dpu_composer *dpu_comp)
{
	struct comp_online_present *present = (struct comp_online_present *)dpu_comp->present_data;
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;
	int32_t ret = 0;
	uint32_t delay_count_max = 17;

	dpu_pr_info("the number of composers is %d", dpu_comp_power_on_count(&comp_mgr->power_status));
	while ((dpu_comp_power_on_count(&comp_mgr->power_status) >= 2) && (present->vactive_end_flag == 0)) {
		ret = (int32_t)wait_event_interruptible_timeout(present->vactive_end_wq,
			present->vactive_end_flag,
			(long)msecs_to_jiffies(delay_count_max));
		if (ret == -ERESTARTSYS || ret == 0) {
			dpu_pr_warn("Did not receive vactive end within %u ms!", delay_count_max);
			break;
		}
	}
}

void composer_dpu_power_off_sub(struct dpu_composer *dpu_comp)
{
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;
	struct dpu_connector *connector = NULL;
	bool is_fake_off = is_fake_power_off(dpu_comp);

	pipeline_next_ops_handle(pinfo->conn_device, pinfo, RESET_PARTIAL_UPDATE, NULL);

	composer_present_power_off_sub(dpu_comp);

	if ((!is_offline_panel(&pinfo->base)) && (!is_dp_panel(&pinfo->base)) && (!is_hdmi_panel(&pinfo->base)))
		dpu_backlight_cancel(&dpu_comp->bl_ctrl, is_fake_off);
	else
		(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, DISABLE_ISR, &dpu_comp->isr_ctrl);

	dpu_pr_info("video_panel:%d cmd_panel:%d power_on_count:%d",
		is_mipi_video_panel(&pinfo->base),
		is_mipi_cmd_panel(&pinfo->base),
		dpu_comp_power_on_count(&comp_mgr->power_status));

	connector = get_primary_connector(dpu_comp->conn_info);
	if (is_mipi_video_panel(&pinfo->base) || is_fake_off ||
		(is_mipi_cmd_panel(&pinfo->base) &&
		(dpu_comp_power_on_count(&comp_mgr->power_status) >= 2))) {
		dpu_pr_debug("need_wait_idle");
		connector->need_wait_idle_flag = true;
	}

	if (dpu_comp->comp.is_fake_power_off)
		(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, FAKE_POWER_OFF, pinfo);
	else
		(void)pipeline_next_off(pinfo->conn_device, pinfo);

	if ((!is_offline_panel(&pinfo->base)) && (!is_dp_panel(&pinfo->base)) && (!is_hdmi_panel(&pinfo->base)))
		(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, DISABLE_ISR, &dpu_comp->isr_ctrl);

	dpu_comp_smmuv3_off(comp_mgr, dpu_comp);
	composer_manager_hiace_off(dpu_comp);
	dpu_dvfs_reset_vote(dpu_comp->comp.index);

	dpu_comp->comp_idle_enable = false;
	dpu_comp->comp_idle_flag = 0;
	dpu_comp->curr_active_level = 0;
	composer_dpu_pipesw_disconnect(dpu_comp);
}

static void register_vsync_listener(struct dpu_composer *dpu_comp, struct comp_online_present *present)
{
	uint32_t te_bit;
	uint32_t vsync_bit;
	struct ukmd_isr *isr_ctrl = NULL;
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	struct dpu_comp_dfr_ctrl* dfr_ctrl = &present->dfr_ctrl;

	switch (dfr_ctrl->mode) {
	case DFR_MODE_CONSTANT:
	case DFR_MODE_LONG_V:
	case DFR_MODE_LONG_H:
	case DFR_MODE_LONG_VH:
		dpu_pr_info("lcd_te_idx:%d,", pinfo->base.lcd_te_idx);
		te_bit = pinfo->base.lcd_te_idx == 0 ? DSI_INT_LCD_TE0 : DSI_INT_LCD_TE1;
		vsync_bit = (is_mipi_cmd_panel(&pinfo->base) && (!is_force_update(&pinfo->base))) ? te_bit : DSI_INT_VSYNC;
		isr_ctrl = &dpu_comp->isr_ctrl;
		break;
	case DFR_MODE_TE_SKIP_BY_MCU:
	case DFR_MODE_LONGH_TE_SKIP_BY_MCU:
		vsync_bit = NOTIFY_BOTH_VSYNC_TIMELINE;
		isr_ctrl = &dpu_comp->comp_mgr->mdp_isr_ctrl;
		break;
	case DFR_MODE_TE_SKIP_BY_ACPU:
		vsync_bit = DSI_INT_VSYNC_COUNT_BY_TE;
		isr_ctrl = &dpu_comp->isr_ctrl;
		break;
	default:
		dpu_pr_err("invalid dfr mode %d", dfr_ctrl->mode);
		return;
	}

	present->vsync_ctrl.isr = isr_ctrl;
	present->vsync_ctrl.listening_isr_bit = vsync_bit;
	ukmd_isr_register_listener(isr_ctrl, present->vsync_ctrl.notifier,
		present->vsync_ctrl.listening_isr_bit, &present->vsync_ctrl);
}

static void register_timeline_listener(struct dpu_composer *dpu_comp, struct comp_online_present *present)
{
	uint32_t te_bit;
	uint32_t timeline_bit;
	struct ukmd_isr *isr_ctrl = NULL;
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	struct dpu_comp_dfr_ctrl* dfr_ctrl = &present->dfr_ctrl;

	switch (dfr_ctrl->mode) {
	case DFR_MODE_CONSTANT:
	case DFR_MODE_LONG_V:
	case DFR_MODE_LONG_H:
	case DFR_MODE_LONG_VH:
		te_bit = pinfo->base.lcd_te_idx == 0 ? DSI_INT_LCD_TE0 : DSI_INT_LCD_TE1;
		timeline_bit = (is_mipi_cmd_panel(&pinfo->base) && (!is_force_update(&pinfo->base))) ? te_bit : DSI_INT_VSYNC;
		isr_ctrl = &dpu_comp->isr_ctrl;
		break;
	case DFR_MODE_TE_SKIP_BY_MCU:
	case DFR_MODE_LONGH_TE_SKIP_BY_MCU:
		timeline_bit = NOTIFY_BOTH_VSYNC_TIMELINE;
		isr_ctrl = &dpu_comp->comp_mgr->mdp_isr_ctrl;
		break;
	case DFR_MODE_TE_SKIP_BY_ACPU:
		timeline_bit = DSI_INT_VSYNC_COUNT_BY_TE;
		isr_ctrl = &dpu_comp->isr_ctrl;
		break;
	default:
		dpu_pr_err("invalid dfr mode %d", dfr_ctrl->mode);
		return;
	}

	/* The fence release time is changed from the TE interrupt trigger time to the vactive start trigger time
	 * to prevent the TE from incorrectly triggering fence release.
	 */
#ifdef CONFIG_DKMD_DPU_OHOS
	if ((pinfo != NULL) && is_mipi_cmd_panel(&pinfo->base)) {
		timeline_bit = DSI_INT_VACT0_START;
		isr_ctrl = &dpu_comp->isr_ctrl;
	}
	dpu_pr_info("register timeline listener, mode=%d, ohos listening_isr_bit=0x%x",
		dfr_ctrl->mode, timeline_bit);
#endif

	present->timeline.isr = isr_ctrl;
	present->timeline.listening_isr_bit = timeline_bit;
	ukmd_isr_register_listener(isr_ctrl, present->timeline.notifier,
		present->timeline.listening_isr_bit, &present->timeline);
}

static void register_listeners(struct dpu_composer *dpu_comp)
{
	struct comp_online_present *present = (struct comp_online_present *)dpu_comp->present_data;
	struct dpu_comp_dfr_ctrl* dfr_ctrl = &present->dfr_ctrl;

	dpu_comp_dfr_ctrl_setup(dpu_comp, present);
	register_vsync_listener(dpu_comp, present);
	register_timeline_listener(dpu_comp, present);

	if (dfr_ctrl->mode == DFR_MODE_TE_SKIP_BY_MCU || dfr_ctrl->mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU ||
			dfr_ctrl->mode == DFR_MODE_TE_SKIP_BY_ACPU) {
		if (!dpu_comp->has_dfr_related_listener_registered) {
			present->comp_maintain.dpu_comp = dpu_comp;
			comp_mntn_refresh_stat_init(&present->comp_maintain);
		}
		register_comp_mntn_listener(&present->comp_maintain);
	}

	dpu_low_temp_register(dpu_comp, present);
}

static void register_dfr_related_listeners(struct dpu_composer *dpu_comp)
{
	if (is_offline_panel(&dpu_comp->conn_info->base))
		return;

	dpu_pr_info("comp %d, has_dfr_related_listener_registered mode: %d",
		dpu_comp->comp.index, dpu_comp->has_dfr_related_listener_registered);

	if (!dpu_comp->has_dfr_related_listener_registered) {
		register_listeners(dpu_comp);
		dpu_comp->has_dfr_related_listener_registered = true;
	}
}

static void composer_manager_set_simlpe_fastboot(struct composer *comp)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;

	dpu_pr_info("simlpe fastboot enter.");
	if (dpu_comp_status_is_disable(&comp_mgr->power_status)) {
		media_regulator_enable(comp_mgr);
		composer_dpu_simple_power_on(comp_mgr, comp);
	}

	dpu_comp_simple_smmuv3_on(comp_mgr, dpu_comp);

	if (dpu_comp->hiace_ctrl && dpu_comp->hiace_ctrl->unblank){
		dpu_comp->hiace_ctrl->unblank(dpu_comp->hiace_ctrl);
	}

	register_dfr_related_listeners(dpu_comp);
	composer_present_power_on(dpu_comp);

	if (dpu_comp_status_is_disable(&comp_mgr->power_status)) {
		dkmd_mdp_isr_enable(dpu_comp->comp_mgr);
		dkmd_sdp_isr_enable(dpu_comp->comp_mgr);
		dkmd_dvfs_isr_enable(dpu_comp->comp_mgr);
	}

	(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, SET_FASTBOOT, (void *)&comp->fastboot_display_enabled);
	(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, ENABLE_ISR, &dpu_comp->isr_ctrl);

	dpu_comp_simple_init(dpu_comp);

	comp_mgr->power_status.refcount.value[comp->index]++;

	(void)dpu_sh_aod_blank(dpu_comp, DISP_BLANK_UNBLANK);

	restart_esd_timer(dpu_comp);

	reset_low_temperature_state(dpu_comp);

	if (unlikely(dpu_tui_power_work(dpu_comp, DPU_POWER_ON) != 0))
		dpu_pr_warn("dpu_tui_power_work fail");

	return;
}

int32_t composer_manager_set_fastboot(struct composer *comp)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;

	if (is_mipi_video_panel(&dpu_comp->conn_info->base) &&
		comp->fastboot_display_enabled && can_pre_init_config()) {
		composer_manager_set_simlpe_fastboot(comp);
		return (int32_t)comp->fastboot_display_enabled;
	}

	if (dpu_comp_status_is_disable(&comp_mgr->power_status)) {
		media_regulator_enable(comp_mgr);
		composer_dpu_power_on(comp_mgr, comp);
	}

	dpu_comp_smmuv3_on(comp_mgr, dpu_comp);
	composer_manager_hiace_on(dpu_comp);
	register_dfr_related_listeners(dpu_comp);
	composer_present_power_on(dpu_comp);

	if (dpu_comp_status_is_disable(&comp_mgr->power_status)) {
		dkmd_mdp_isr_enable(dpu_comp->comp_mgr);
		dkmd_sdp_isr_enable(dpu_comp->comp_mgr);
		dkmd_dvfs_isr_enable(dpu_comp->comp_mgr);
	}
	(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, SET_FASTBOOT, (void *)&comp->fastboot_display_enabled);
	(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, ENABLE_ISR, &dpu_comp->isr_ctrl);

	/* need after isr request and composer power on */
	if (dpu_comp_status_is_disable(&comp_mgr->power_status))
		dpu_init(comp_mgr);
	dpu_comp_init(dpu_comp);

	comp_mgr->power_status.refcount.value[comp->index]++;

	(void)dpu_sh_aod_blank(dpu_comp, DISP_BLANK_UNBLANK);

	restart_esd_timer(dpu_comp);

	reset_low_temperature_state(dpu_comp);

	if (unlikely(dpu_tui_power_work(dpu_comp, DPU_POWER_ON) != 0))
		dpu_pr_warn("dpu_tui_power_work fail");

	return (int32_t)comp->fastboot_display_enabled;
}

int32_t composer_wait_for_blank(struct composer *comp)
{
	int32_t ret = 0;
	struct dpu_composer *dpu_comp = NULL;
	dpu_check_and_return(!comp, 0, err, "comp is NULL");
	dpu_comp = to_dpu_composer(comp);
	dpu_check_and_return(!dpu_comp, 0, err, "dpu_comp is NULL");

	if (is_dp_panel(&comp->base))
		dp_wait_for_aod_stop(dpu_comp);

	if ((!is_builtin_panel(&comp->base)) && (!is_primary_panel(&comp->base)))
		return 0;

	if (dpu_sh_need_fast_unblank(dpu_comp)) {
		dpu_pr_info("[aod] power up with fast unblank");
		return FAST_UNLOCK_RET;
	}

	dpu_wait_for_aod_stop(dpu_comp);

	ret = dpu_sh_aod_blank(dpu_comp, DISP_BLANK_UNBLANK);
	if (ret == FAST_UNLOCK_RET) {
		dpu_pr_info("AOD in fast unlock mode");
		return FAST_UNLOCK_RET;
	}

	dpu_aod_wait_for_blank(dpu_comp, DISP_BLANK_UNBLANK);
	return 0;
}

void composer_restore_fast_unblank_status(struct composer *comp)
{
	struct dpu_composer *dpu_comp = NULL;
	dpu_check_and_no_retval(!comp, err, "comp is NULL");
	dpu_comp = to_dpu_composer(comp);
	if ((!is_builtin_panel(&comp->base)) && (!is_primary_panel(&comp->base)))
		return;

	dpu_restore_fast_unblank_status(dpu_comp);
	dpu_aod_wait_for_blank(dpu_comp, DISP_BLANK_POWERDOWN);
}

int32_t composer_blank_peri_handle(struct composer *comp, int32_t blank_mode)
{
	dpu_check_and_return(!comp, 0, err, "comp is NULL");
	if ((!is_builtin_panel(&comp->base)) && (!is_primary_panel(&comp->base)))
		return 0;

	return dpu_blank_peri_handle(blank_mode);
}

static void composer_manager_alsc_on(struct dpu_composer *dpu_comp)
{
	if (!dpu_comp->alsc)
		return;

	if (!dpu_comp->alsc->comp_alsc_unblank)
		return;

	dpu_comp->alsc->comp_alsc_unblank(dpu_comp->alsc);
}

static void composer_manager_alsc_off(struct dpu_composer *dpu_comp)
{
	if (!dpu_comp->alsc)
		return;

	if (!dpu_comp->alsc->comp_alsc_blank)
		return;

	dpu_comp->alsc->comp_alsc_blank(dpu_comp->alsc);
}

void composer_manager_power_down(struct dpu_composer *dpu_comp)
{
	uint32_t index;
	struct composer *comp = NULL;
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;

	if (unlikely(!comp_mgr))
		return;

	for (index = 0; index < DEVICE_COMP_MAX_COUNT; index++) {
		if (!comp_mgr->dpu_comps[index])
			continue;
		comp = &comp_mgr->dpu_comps[index]->comp;
		if (!comp)
			continue;

		down(&comp->blank_sem);
		dpu_print_sem_count(&comp->blank_sem, true);
	}

	down(&comp_mgr->power_sem);
}

void composer_manager_power_up(struct dpu_composer *dpu_comp)
{
	int32_t index;
	struct composer *comp = NULL;
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;

	if (unlikely(!comp_mgr))
		return;

	up(&comp_mgr->power_sem);

	for (index = (DEVICE_COMP_MAX_COUNT - 1); index >= 0; index--) {
		comp = &comp_mgr->dpu_comps[index]->comp;
		if (!comp)
			continue;

		dpu_print_sem_count(&comp->blank_sem, false);
		up(&comp->blank_sem);
	}
}

static void dpu_comp_power_preprocess(struct composer *comp)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;
	struct dpu_connector *connector = NULL;
	struct dkmd_connector_info *gfx_info = NULL;
	uint32_t delay_count = 50; /* need wait 3 frames, 3*16.6 */

	if (comp_mgr->dpu_comps[DEVICE_COMP_BUILTIN_ID] == NULL)
		return;

	if (!composer_manager_get_scene_switch(comp_mgr->dpu_comps[DEVICE_COMP_BUILTIN_ID]))
		return;

	gfx_info = comp_mgr->dpu_comps[DEVICE_COMP_BUILTIN_ID]->conn_info;

	/* when fb is on need wait gfx enter idle */
	if (comp->index == DEVICE_COMP_PRIMARY_ID && comp_mgr->power_status.refcount.value[DEVICE_COMP_BUILTIN_ID] == 1) {
		dpu_pr_warn("wait gfx enter idle +");
		(void)pipeline_next_ops_handle(gfx_info->conn_device, gfx_info, WAIT_LDI_VSTATE_IDLE, &delay_count);
	}

	if (comp->index != DEVICE_COMP_BUILTIN_ID)
		return;

	connector = get_primary_connector(dpu_comp->conn_info);

	if (comp_mgr->power_status.refcount.value[DEVICE_COMP_PRIMARY_ID] == 1) {
		dpu_pr_info("gfx %d need use scene 1", dpu_comp->comp_scene_id);
		dpu_comp->comp_scene_id = DPU_SCENE_ONLINE_1;
		connector->dpp_base = connector->dpu_base + DPU_DPP1_OFFSET;
		connector->dsc_base = NULL;
	} else {
		dpu_pr_info("gfx %d need use scene 0", dpu_comp->comp_scene_id);
		dpu_comp->comp_scene_id = DPU_SCENE_ONLINE_0;
		connector->dpp_base = connector->dpu_base + DPU_DPP0_OFFSET;
		connector->dsc_base = connector->dpu_base + DPU_DSC0_OFFSET;
	}

	(void)pipeline_next_ops_handle(gfx_info->conn_device, gfx_info, UPDATE_INFO, &(dpu_comp->comp_scene_id));
}

/* no lock function, need caller to lock power sem */
int32_t composer_manager_on_no_lock(struct composer *comp)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	struct comp_online_present *present = NULL;
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;

	dpu_pr_enter_log(dpu_comp, -1);

	if (unlikely(comp->power_on))
		return 0;

	dpu_comp_active_vsync(dpu_comp);

	// power_status will by protected by comp->blank_sem
	if (dpu_comp_status_is_disable(&comp_mgr->power_status)) {
		/* public function need use public data as interface parameter, such as 'comp_mgr' */
		if (comp_mgr->power_on_stage == DPU_POWER_ON_NONE) {
			if (dpu_sensorhub_aod_hw_lock(dpu_comp) == false)
				return -1;
			media_regulator_enable(comp_mgr);
			composer_dpu_power_on(comp_mgr, comp);
			dpu_dacc_load(comp_mgr->dpu_base);
		}

		comp_mgr->power_status.refcount.value[comp->index]++;

		dkmd_mdp_isr_enable(comp_mgr);
		dkmd_sdp_isr_enable(comp_mgr);
		dkmd_dvfs_isr_enable(comp_mgr);
		dpu_comp_power_preprocess(comp);
		register_dfr_related_listeners(dpu_comp);
		composer_present_power_on(dpu_comp);
		composer_dpu_power_on_sub(dpu_comp);
		dpu_init(comp_mgr);
		dpu_comp_init(dpu_comp);
	} else {
		if (comp_mgr->power_status.refcount.value[comp->index] == 0) {
			dpu_comp_power_preprocess(comp);
			register_dfr_related_listeners(dpu_comp);
			composer_present_power_on(dpu_comp);
			composer_dpu_power_on_sub(dpu_comp);
			dpu_comp_init(dpu_comp);
		}
		comp_mgr->power_status.refcount.value[comp->index]++;
	}
	dpu_comp_deactive_vsync(dpu_comp);
	composer_manager_alsc_on(dpu_comp);

	start_esd_timer(dpu_comp);
	reset_low_temperature_state(dpu_comp);

	comp->power_on = true;
	if (!is_offline_panel(&dpu_comp->conn_info->base)) {
		present = (struct comp_online_present *)dpu_comp->present_data;
		present->buffers = 0;
	}
	dpu_pr_exit_log(dpu_comp);
	dpu_comp_status_info(&comp_mgr->power_status);
	if (unlikely(dpu_tui_power_work(dpu_comp, DPU_POWER_ON) != 0))
		dpu_pr_warn("dpu_tui_power_work fail");

	return 0;
}

void composer_manager_release(struct composer *comp)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);

	dpu_comp_release(dpu_comp);
}

int32_t composer_manager_on(struct composer *comp)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;
	int32_t ret;

	if (unlikely(!comp_mgr))
		return -1;

	ret = dpu_vote_disable_doze2();
	if (ret != 0)
		return -1;

	composer_manager_power_down(dpu_comp);
	if (unlikely(comp->power_on)) {
		composer_manager_power_up(dpu_comp);
		return 0;
	}

	ret = composer_manager_on_no_lock(comp);
	if (likely(ret == 0))
		comp->power_on = true;

	composer_manager_power_up(dpu_comp);

	return ret;
}

int32_t composer_manager_off_no_lock(struct composer *comp, int32_t off_mode)
{
	int32_t ret = 0;
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;
	struct dpu_connector *connector = NULL;

	dpu_pr_enter_log(dpu_comp, off_mode);

	if (unlikely(!comp->power_on || !dpu_comp->conn_info))
		return 0;

	connector = get_primary_connector(dpu_comp->conn_info);

	dpu_comp_active_vsync(dpu_comp);

	if (comp_mgr->power_status.refcount.value[comp->index] == 1) {
		if (unlikely(dpu_tui_power_work(dpu_comp, DPU_POWER_OFF) != 0))
			dpu_pr_warn("dpu_tui_power_work fail");

		composer_dpu_power_off_sub(dpu_comp);

		comp_mgr->power_status.refcount.value[comp->index]--;
		composer_present_power_off(dpu_comp, dpu_comp_status_is_disable(&comp_mgr->power_status));
		/* public function need use public data as interface parameter, such as 'comp_mgr' */
		if (dpu_comp_status_is_disable(&comp_mgr->power_status)) {
			/* abort idle thread handle */
			mutex_lock(&comp_mgr->idle_lock);
			comp_mgr->idle_enable = false;
			comp_mgr->idle_func_flag = 0;
			mutex_unlock(&comp_mgr->idle_lock);

			dkmd_mdp_isr_disable(dpu_comp->comp_mgr);
			dkmd_sdp_isr_disable(dpu_comp->comp_mgr);
			dkmd_dvfs_isr_disable(dpu_comp->comp_mgr);
			/**
			 * After disable isr, in order to get right status for the first frame after dss power up,
			 * need to set is_vactive_end_recieved to true for ltpo panel.
			 */
			connector->is_vactive_end_recieved = VACTIVE_END_RECEIVED;
			if (comp_mgr->power_on_stage == DPU_POWER_ON_NONE) {
				composer_dpu_power_off(comp_mgr, comp);
				media_regulator_disable(comp_mgr);
				(void)dpu_sensorhub_aod_hw_unlock(dpu_comp);
			}
		}
	} else {
		if (comp_mgr->power_status.refcount.value[comp->index] != 0)
			comp_mgr->power_status.refcount.value[comp->index]--;
		dpu_pr_warn("%s set too many power down, refcount %d!", comp->base.name,
			comp_mgr->power_status.refcount.value[comp->index]);
	}

	dpu_comp_deactive_vsync(dpu_comp);
	composer_manager_alsc_off(dpu_comp);

	cancel_esd_timer(dpu_comp);
	composer_cancel_dvfs_dump_timer(dpu_comp);
	reset_low_temperature_state(dpu_comp);

	(void)dpu_sh_aod_blank(dpu_comp, DISP_BLANK_POWERDOWN);

	comp->power_on = false;
	dpu_pr_exit_log(dpu_comp);
	dpu_comp_status_info(&comp_mgr->power_status);
	return ret;
}

static void unregister_listeners(struct dpu_composer *dpu_comp)
{
	struct comp_online_present *present = NULL;
	struct dpu_comp_dfr_ctrl *dfr_ctrl = NULL;

	present = (struct comp_online_present *)dpu_comp->present_data;
	dfr_ctrl = &present->dfr_ctrl;

	dpu_pr_info("comp %d, dfr_ctrl mode: %d", dpu_comp->comp.index, dfr_ctrl->mode);

	if (dfr_ctrl->mode == DFR_MODE_TE_SKIP_BY_MCU || dfr_ctrl->mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU)
		comp_mntn_refresh_stat_deinit(&dpu_comp->comp_mgr->mdp_isr_ctrl, NOTIFY_REFRESH);
	else if (dfr_ctrl->mode == DFR_MODE_TE_SKIP_BY_ACPU)
		comp_mntn_refresh_stat_deinit(&dpu_comp->isr_ctrl, NOTIFY_REFRESH);

	dpu_low_temp_unregister(dpu_comp, present);
	dpu_comp->timeline_deinit(dpu_comp, present);
	dpu_comp->vsync_deinit(dpu_comp, present);
}

static void unregister_dfr_related_listeners(struct dpu_composer *dpu_comp) {
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;

	if (is_offline_panel(&dpu_comp->conn_info->base))
		return;

	if (comp_mgr->dpu_comps[DEVICE_COMP_BUILTIN_ID] == NULL)
		return;

	if (!composer_manager_get_scene_switch(comp_mgr->dpu_comps[DEVICE_COMP_BUILTIN_ID]))
		return;

	dpu_pr_info("comp %d, has_dfr_related_listener_registered mode: %d",
		dpu_comp->comp.index, dpu_comp->has_dfr_related_listener_registered);

	if (dpu_comp->has_dfr_related_listener_registered) {
		dpu_comp->has_dfr_related_listener_registered = false;
		unregister_listeners(dpu_comp);
	}
}

int32_t composer_manager_off(struct composer *comp, int32_t off_mode)
{
	int32_t ret;
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;
	struct comp_online_present *present = NULL;
	if (unlikely(!comp_mgr))
		return -1;

	composer_manager_power_down(dpu_comp);
	if (unlikely(!comp->power_on)) {
		comp->is_fake_power_off = false;
		composer_manager_power_up(dpu_comp);
		return 0;
	}

	ret = composer_manager_off_no_lock(comp, off_mode);
	if (likely(ret == 0))
		comp->power_on = false;

	composer_manager_timeline_resync(dpu_comp, off_mode);
	if (!is_offline_panel(&dpu_comp->comp.base)) {
		present = (struct comp_online_present *)dpu_comp->present_data;
		ukmd_timeline_disable_routine(&present->timeline);
	}

	comp->is_fake_power_off = false;
	composer_manager_power_up(dpu_comp);

	unregister_dfr_related_listeners(dpu_comp);

	return dpu_vote_enable_doze2();
}

void composer_manager_power_restart_no_lock(struct dpu_composer *dpu_comp)
{
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;
	uint32_t index;
	struct composer *comp = NULL;
	int32_t ret;
	bool power_status[DEVICE_COMP_MAX_COUNT] = {false};

	dpu_pr_warn("index:%u enter", dpu_comp->comp.index);
	comp_mgr->is_power_restarting = true;
	for (index = 0; index < DEVICE_COMP_MAX_COUNT; index++) {
		comp = &comp_mgr->dpu_comps[index]->comp;
		if (!comp || !comp->power_on)
			continue;
		ret = composer_manager_off_no_lock(comp, COMPOSER_OFF_MODE_BLANK);
		if (ret)
			continue;
		power_status[index] = true;
	}

	for (index = 0; index < DEVICE_COMP_MAX_COUNT; index++) {
		comp = &comp_mgr->dpu_comps[index]->comp;
		if (comp == NULL)
			continue;
		if (power_status[index]) {
			ret = composer_manager_on_no_lock(comp);
			if (ret)
				dpu_pr_err("power on failed index:[%u]", index);
		}
	}
	send_reset_hardware_event(&dpu_comp->comp);
	comp_mgr->is_power_restarting = false;
	dpu_pr_warn("index:%u exit", dpu_comp->comp.index);
}

void composer_manager_reset_hardware(struct dpu_composer *dpu_comp)
{
	struct comp_online_present *present = NULL;
	composer_manager_power_restart_no_lock(dpu_comp);
	dpu_pr_warn("reset hardware finished!");
	if (!is_offline_panel(&dpu_comp->conn_info->base)) {
		present = (struct comp_online_present *)dpu_comp->present_data;
		present->vactive_start_flag = 1;
		present->vactive_end_flag = 0;
		present->frame_start_flag = 0;
	}
}
