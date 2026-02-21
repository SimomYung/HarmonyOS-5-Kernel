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
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/memblock.h>
#include <uapi/linux/sched/types.h>
#include "dpu_comp_mgr.h"
#include "dpu_config_utils.h"
#include "dpu_isr_mdp.h"
#include "dpu_isr_sdp.h"
#include "dpu_isr_dvfs.h"
#include "gfxdev_mgr.h"
#include "dkmd_log.h"
#include "dpu_comp_vsync.h"
#include "dpu_sh_aod.h"
#include "res_mgr.h"
#include "dpu_effect_alsc.h"
#include "dksm_dmd.h"
#include "dpu_conn_mgr.h"
#include "dpu_comp_config_utils.h"
#include "dpu_dvfs.h"
#include "dpu_comp_tui.h"
#include "dpu_mntn.h"

uint32_t mIsVideoPanel = 0;
struct composer_manager *g_composer_manager = NULL;
static void composer_manager_get_fps_seq(struct composer *comp, struct product_config *out_config)
{
	int i;
	uint32_t fps_sup_num;
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	struct dfr_info *dfr_info = NULL;
	struct comp_online_present *present = NULL;
	struct dpu_comp_dfr_ctrl* dfr_ctrl = NULL;
	if (!dpu_comp)
		return;

	dfr_info = dkmd_get_dfr_info(dpu_comp->conn_info);
	present = (struct comp_online_present *)dpu_comp->present_data;
	if (!dfr_info || !present)
		return;

	dfr_ctrl = &present->dfr_ctrl;
	if (!dfr_ctrl)
		return;

	dpu_pr_info("comp %d, +",comp->index);

	out_config->dfr_mode = dfr_info->dfr_mode;
	out_config->connector_trans_mode =
		is_mipi_video_panel(&dpu_comp->conn_info->base) ? DSI_VIDEO_MODE : DSI_CMD_MODE;

	/* get fps support seq from each connecter dtsc */
	fps_sup_num = dfr_info->oled_info.fps_sup_num < FPS_LEVEL_MAX ?
		dfr_info->oled_info.fps_sup_num : FPS_LEVEL_MAX;

	out_config->fps_info_count = fps_sup_num;
	for (i = 0; i < (int)fps_sup_num; i++) {
		out_config->fps_info[i] = dfr_info->oled_info.fps_sup_seq[i];
		out_config->fps_dvfs_info[i] = dfr_info->oled_info.fps_dvfs_seq[i];
	}

	dpu_pr_info("comp %u, -",comp->index);
}

static bool composer_manager_get_async_online(struct composer *comp)
{
	if (is_dp_panel(&comp->base) || is_hdmi_panel(&comp->base))
		return comp->base.enable_async_online == 1;

	if (is_offline_panel(&comp->base) || is_fake_panel(&comp->base))
		return false;

	if (is_builtin_panel(&comp->base) || is_primary_panel(&comp->base))
		return true;

	return false;
}

static void composer_manager_get_porch_info(struct dpu_composer *dpu_comp, struct product_config *out_config)
{
	uint32_t pixel_clk;
	uint64_t lane_byte_clk;
	struct dpu_porch_info *porch_info = NULL;
	struct mipi_panel_info *mipi = NULL;
	struct dpu_connector *connector = NULL;

	connector = get_primary_connector(dpu_comp->conn_info);
	if (connector == NULL)
		return;

	porch_info = &(out_config->porch_info);
	(void)memset_s(porch_info, sizeof(struct dpu_porch_info), 0, sizeof(struct dpu_porch_info));

	mipi = &(connector->post_info[0]->mipi); // using default params

	porch_info->vfp = mipi->vfp;
	porch_info->vbp = mipi->vbp;
	porch_info->vsa = mipi->vsa;

	pixel_clk = get_mipi_pixel_clk(mipi);
	if (pixel_clk == 0)
		return;

	lane_byte_clk = get_default_lane_byte_clk(mipi);
	if (lane_byte_clk == 0)
		return;

	if (((mipi->hsa + mipi->hbp) == 0) ||
		(mipi->hline_time <= (mipi->dpi_hsize + mipi->hsa + mipi->hbp)))
		return;

	porch_info->hsa = round1(mipi->hsa * pixel_clk, lane_byte_clk);
	porch_info->hbp = round1(mipi->hbp * pixel_clk, lane_byte_clk);
	porch_info->hfp = round1((mipi->hline_time - mipi->dpi_hsize - mipi->hsa - mipi->hbp) *
		pixel_clk, lane_byte_clk);
}

static int32_t composer_manager_get_prodcut_config(struct composer *comp, struct product_config *out_config)
{
	struct dpu_connector *connector = NULL;
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	int i = 0;

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return -1;
	}

	if (unlikely(!out_config)) {
		dpu_pr_err("out config is null");
		return -1;
	}

	connector = get_primary_connector(dpu_comp->conn_info);
	if (unlikely(!connector)) {
		dpu_pr_err("connector is null");
		return -1;
	}

	/* used for framebuffer device regitser */
	comp->base.xres = dpu_comp->conn_info->base.xres;
	comp->base.yres = dpu_comp->conn_info->base.yres;
	comp->base.width = dpu_comp->conn_info->base.width;
	comp->base.height = dpu_comp->conn_info->base.height;
	comp->base.fps = dpu_comp->conn_info->base.fps;
	/* used for dsc_info send to dumd */
	comp->base.dsc_out_width = dpu_comp->conn_info->base.dsc_out_width;
	comp->base.dsc_out_height = dpu_comp->conn_info->base.dsc_out_height;
	comp->base.dsc_en = dpu_comp->conn_info->base.dsc_en;
	comp->base.spr_en = dpu_comp->conn_info->base.spr_en;

	/* get product config information
	 * get split mode frome connecotr info, it must get from panel dtsi
	 */
	out_config->display_count = 1;
	out_config->split_count = 1;
	out_config->split_mode = SCENE_SPLIT_MODE_NONE;
	out_config->split_ratio[SCENE_SPLIT_MODE_NONE] = 0;
	out_config->split_ratio[SCENE_SPLIT_MODE_V] = 0;
	out_config->split_ratio[SCENE_SPLIT_MODE_H] = 0;
	out_config->dim_info_count = 0;
	out_config->fps_info_count = 0;
	out_config->fps_of_longv_for_dvfs = dpu_comp->conn_info->base.fps_of_longv_for_dvfs;

	if (dpu_comp->conn_info->support_bt2020) {
		out_config->color_mode_count = 4;
		out_config->color_modes[3] = COLOR_MODE_BT2020;
		out_config->support_color_mode = COLOR_MODE_BT2020;
	} else {
		out_config->color_mode_count = 3;
		out_config->support_color_mode = COLOR_MODE_DISPLAY_P3;
	}
	out_config->color_modes[0] = COLOR_MODE_NATIVE;
	out_config->color_modes[1] = COLOR_MODE_SRGB;
	out_config->color_modes[2] = COLOR_MODE_DISPLAY_P3;

	out_config->init_panel_display_status = dpu_comp->comp_mgr->init_panel_display_status;
	out_config->dbuf_discount_factor = dpu_comp->conn_info->base.dbuf_discount_factor;

	if (composer_manager_get_scene_switch(dpu_comp)) {
		out_config->dsc_out_width = connector->post_info[0]->dsc.dsc_info.output_width;
		out_config->dsc_out_height = connector->post_info[0]->dsc.dsc_info.output_height;
		out_config->feature_switch.bits.enable_dsc = connector->post_info[0]->dsc.dsc_en & 0x1;
		out_config->feature_switch.bits.enable_spr = connector->post_info[0]->dsc.spr_en;
		out_config->spr_ctrl = connector->post_info[0]->spr.spr_ctrl.value;
		out_config->spr_border_r_tb = connector->post_info[0]->spr.spr_r_bordertb.value;
		out_config->spr_border_g_tb = connector->post_info[0]->spr.spr_g_bordertb.value;
		out_config->spr_border_b_tb = connector->post_info[0]->spr.spr_b_bordertb.value;
	} else {
		out_config->dsc_out_width = comp->base.dsc_out_width;
		out_config->dsc_out_height = comp->base.dsc_out_height;
		out_config->feature_switch.bits.enable_dsc = comp->base.dsc_en;
		out_config->feature_switch.bits.enable_spr = comp->base.spr_en;
		out_config->spr_ctrl = dpu_comp->conn_info->spr_ctrl;
		out_config->spr_border_r_tb = dpu_comp->conn_info->spr_border_r_tb;
		out_config->spr_border_g_tb = dpu_comp->conn_info->spr_border_g_tb;
		out_config->spr_border_b_tb = dpu_comp->conn_info->spr_border_b_tb;
	}

	out_config->feature_switch.bits.enable_online_async = composer_manager_get_async_online(comp);
	out_config->feature_switch.bits.enable_hdr10 = 1;
	out_config->fold_info.bits.fold_type = comp->base.fold_type;
	out_config->feature_switch.bits.enable_scene_switch = composer_manager_get_scene_switch(dpu_comp);

	composer_manager_get_fps_seq(comp, out_config);
	if (dpu_comp->conn_info->get_panel_user_info) // dsi panel
		dpu_comp->conn_info->get_panel_user_info(dpu_comp->conn_info, &out_config->user_pinfo);
	if (dpu_comp->conn_info->get_ppu_config_info)
		dpu_comp->conn_info->get_ppu_config_info(dpu_comp->conn_info, &out_config->ppu_cfg_info);
	if (is_ppc_support(&comp->base)) {
		for (i = 0; i < PPC_CONFIG_ID_CNT; i++) {
			out_config->ppc_config_id_rect_info[i] = dpu_comp->conn_info->ppc_rect_info[i];
		}
	}

	if (out_config->ppu_cfg_info.ppu_support && g_debug_partial_refresh_support == 0)
		out_config->ppu_cfg_info.ppu_support = 0;

	if (out_config->user_pinfo.dirty_region_updt_support && g_debug_partial_tx_support == 0)
		out_config->user_pinfo.dirty_region_updt_support = 0;

	composer_manager_get_porch_info(dpu_comp, out_config);
	return 0;
}

static int32_t composer_manager_get_ext_info(struct composer *comp, struct product_ext_config *out_config)
{
	uint32_t pixel_clk = 0;
	uint64_t lane_byte_clk = 0;
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	struct dpu_connector *connector = NULL;
	struct mipi_panel_info *mipi = NULL;

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return -1;
	}

	if (unlikely(!out_config)) {
		dpu_pr_err("out config is null");
		return -1;
	}

	if (!composer_manager_get_scene_switch(dpu_comp)) {
		dpu_pr_info("not support scene switch");
		return 0;
	}

	connector = get_primary_connector(dpu_comp->conn_info);
	if (connector->post_info[1] == NULL) {
		dpu_pr_err("comp %u post_info is null, error", comp->index);
		return -1;
	}

	mipi = &(connector->post_info[1]->mipi);
	out_config->dfr_mode = connector->post_info[1]->dfr_info.dfr_mode;
	out_config->dsc_en = 0;
	out_config->dsc_out_width = comp->base.xres;
	out_config->dsc_out_height = comp->base.yres;

	out_config->porch_info.vfp = mipi->vfp;
	out_config->porch_info.vbp = mipi->vbp;
	out_config->porch_info.vsa = mipi->vsa;

	pixel_clk = get_mipi_pixel_clk(mipi);
	lane_byte_clk = get_default_lane_byte_clk(mipi);
	if (pixel_clk == 0 || lane_byte_clk == 0) {
		dpu_pr_err("pixel_clk %u, lane_byte_clk %u fail", pixel_clk, lane_byte_clk);
		return -1;
	}

	if (((mipi->hsa + mipi->hbp) == 0) ||
		(mipi->hline_time <= (mipi->dpi_hsize + mipi->hsa + mipi->hbp))) {
		dpu_pr_err("check mipi param fail");
		return -1;
	}

	out_config->porch_info.hsa = (uint32_t)(round1((uint64_t)mipi->hsa * pixel_clk, lane_byte_clk));
	out_config->porch_info.hbp = (uint32_t)(round1((uint64_t)mipi->hbp * pixel_clk, lane_byte_clk));
	out_config->porch_info.hfp = (uint32_t)(round1((uint64_t)(mipi->hline_time - mipi->dpi_hsize - mipi->hsa - mipi->hbp) *
		pixel_clk, lane_byte_clk));

	dpu_pr_info("get ext_info %u, %u, %u, porch info %u, %u, %u, %u, %u, %u",
		out_config->dfr_mode, out_config->dsc_out_width, out_config->dsc_out_height,
		out_config->porch_info.vfp, out_config->porch_info.vbp, out_config->porch_info.vsa,
		out_config->porch_info.hsa, out_config->porch_info.hbp, out_config->porch_info.hfp);

	return 0;
}

static int32_t composer_manager_get_composer_frame_index(struct composer *comp)
{
	dpu_check_and_return(!comp, -1, err, "comp is null pointer");
	return (int32_t)comp->comp_frame_index;
}

static ssize_t composer_frame_index_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret;
	int32_t comp_idx;

	dpu_check_and_return((!dev || !buf), -1, err, "input is null pointer");

	comp_idx = composer_manager_get_composer_frame_index(get_comp_from_device(dev));
	if (comp_idx < 0) {
		dpu_pr_err("get invalid index");
		return -1;
	}

	ret = snprintf(buf, PAGE_SIZE, "%d\n", comp_idx);
	buf[strlen(buf) + 1] = '\0';

	dpu_pr_info("buf:%s ", buf);

	return ret;
}

static DEVICE_ATTR(comp_frame_index, 0440, composer_frame_index_show, NULL);

static int32_t composer_manager_get_sysfs_attrs(struct composer *comp, struct ukmd_attr **out_attr)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return -1;
	}

	*out_attr = &(dpu_comp->attrs);
	if (!(*out_attr)) {
		dpu_pr_err("*out_attr is nullptr");
		return -1;
	}
	ukmd_sysfs_attrs_append(&(dpu_comp->attrs), &dev_attr_comp_frame_index.attr);

	return 0;
}

static int32_t composer_manager_create_fence(struct composer *comp, struct disp_present_fence *fence)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return -1;
	}

	if (unlikely(!dpu_comp->create_fence)) {
		dpu_pr_err("create_fence is nullptr");
		return -1;
	}

	return dpu_comp->create_fence(dpu_comp, fence);
}

static int32_t composer_manager_release_fence(struct composer *comp, int32_t fd)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return -1;
	}

	if (fd < 0 || !dpu_comp->release_fence)
		return 0;

	return dpu_comp->release_fence(dpu_comp, fd);
}

static void dpu_comp_mgr_thread_setup(struct dpu_composer *dpu_comp)
{
	char tmp_name[256] = {0};
	struct composer *comp = &dpu_comp->comp;
	struct sched_param param = {
		.sched_priority = MAX_RT_PRIO - 1,
	};
	int ret;
	if (!comp) {
		dpu_pr_err("comp is nullptr");
		return;
	}

	/* public each scene kernel processing threads, would be initialized only once */
	kthread_init_worker(&dpu_comp->handle_worker);
	ret = snprintf_s(tmp_name, sizeof(tmp_name), (sizeof(tmp_name) - 1), "dpu_%s", comp->base.name);
	if (ret < 0)
		dpu_pr_warn("failed to format tmp name!");

	dpu_comp->handle_thread = kthread_create(kthread_worker_fn, &dpu_comp->handle_worker, tmp_name);
	if (IS_ERR_OR_NULL(dpu_comp->handle_thread)) {
		dpu_pr_warn("%s failed to create handle_thread!", comp->base.name);
		return;
	}
	(void)sched_setscheduler_nocheck(dpu_comp->handle_thread, SCHED_FIFO, &param);
	(void)wake_up_process(dpu_comp->handle_thread);
}

static void dpu_comp_mgr_thread_relase(struct dpu_composer *dpu_comp)
{
	if (IS_ERR_OR_NULL(dpu_comp->handle_thread))
		return;

	kthread_stop(dpu_comp->handle_thread);
	dpu_comp->handle_thread = NULL;
}

static int32_t composer_manager_set_hiace_lut(struct composer *comp, const void __user* argp)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return -1;
	}

	if (dpu_comp->hiace_ctrl)
		return dpu_comp->hiace_ctrl->set_lut(dpu_comp->hiace_ctrl, argp);

	return 0;
}

static int32_t composer_manager_wake_up_hiace_hist(struct composer *comp)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return -1;
	}

	if (dpu_comp->hiace_ctrl)
		return dpu_comp->hiace_ctrl->wake_up_hist(dpu_comp->hiace_ctrl);

	return 0;
}

static int32_t composer_manager_get_hiace_hist(struct composer *comp, void __user* argp)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return -1;
	}

	if (!comp->power_on) {
		dpu_pr_debug("panel is power off!");
		return 1; /* panel off, do not get hist, return 1. */
	}

	if (dpu_comp->hiace_ctrl)
		return dpu_comp->hiace_ctrl->get_hist(dpu_comp->hiace_ctrl, argp);

	return 0;
}

static int32_t composer_manager_set_safe_frm_rate(struct composer *comp, uint32_t safe_frm_rate)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return -1;
	}
	return composer_present_set_safe_frm_rate(dpu_comp, safe_frm_rate);
}

static int32_t composer_manager_set_active_rect(struct composer *comp, uint32_t ppc_config_id)
{
	int32_t ret = -1;
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return ret;
	}

	return composer_present_set_active_rect(dpu_comp, ppc_config_id);
}

struct composer_manager *get_dpu_composer_manager(void)
{
	dpu_check_and_return(!g_composer_manager, NULL, warn, "composer manager is not ready!");
	return g_composer_manager->dpu_comps[DEVICE_COMP_PRIMARY_ID]->comp_mgr;
}

static void dpu_comp_display_init(struct dpu_composer *dpu_comp)
{
	dpu_comp->comp_scene_id = (int32_t)dpu_comp->conn_info->base.pipe_sw_itfch_idx;

	if (is_builtin_panel(&(dpu_comp->conn_info->base)) && dpu_comp->conn_info->base.fold_type == PANEL_FLIP) {
		if (composer_manager_get_scene_switch(dpu_comp)) {
			dpu_comp->comp_scene_id = DPU_SCENE_ONLINE_0;
			dpu_comp->comp_mgr->init_panel_display_status = ONE_PANEL_DISPLAY;
		} else {
			dpu_comp->comp_mgr->init_panel_display_status = MULTI_PANEL_DISPLAY;
		}
		dpu_pr_info("init_panel_display_status %u comp_scene_id %d pipe_sw_itfch_idx %u",
			dpu_comp->comp_mgr->init_panel_display_status, dpu_comp->comp_scene_id, dpu_comp->conn_info->base.pipe_sw_itfch_idx);
	}
}

static int32_t composer_manager_send_pq_data(struct composer *comp, uint8_t *data, uint32_t len)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
 
	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return -1;
	}
 
	if (dpu_comp->conn_info->base.is_send_pqdata == 0 || !dpu_comp->conn_info->send_display_pq_data)
		return 0;
 
	return dpu_comp->conn_info->send_display_pq_data(dpu_comp->conn_info, data, len);
}
 
static void dpu_comp_mgr_common_init(struct dpu_composer *dpu_comp)
{
	struct composer *comp = &dpu_comp->comp;
	if (!comp) {
		dpu_pr_err("comp is nullptr");
		return;
	}

	dpu_comp->comp_mgr = g_composer_manager;
	ukmd_sysfs_init(&dpu_comp->attrs);
	sema_init(&comp->blank_sem, 1);
	sema_init(&dpu_comp->secure_ctrl.tui_sem, 1);
	dpu_comp->dpu_boot_complete = false;

	/* copy obj_info from connector */
	comp->base = dpu_comp->conn_info->base;

	device_mgr_register_comp(comp);
	if (unlikely(comp->index >= DEVICE_COMP_MAX_COUNT)) {
		dpu_pr_err("comp->index=%u is out of range", comp->index);
		return;
	}

	dpu_comp->comp_mgr->dpu_comps[comp->index] = dpu_comp;

	/* sensorhub aod init */
	dpu_sh_aod_init(dpu_comp);

	/* init panel display type */
	dpu_comp_display_init(dpu_comp);

	/* update dpu_comp peri device pointer to connector manager */
	comp->base.peri_device = dpu_comp->conn_info->conn_device;

	/* initialze the comp idle params */
	dpu_comp->comp_idle_enable = true;
	dpu_comp->comp_idle_flag = 0;

	/* initialze the debug dvfs info */
	dpu_comp->curr_active_level = 0;

	/* initialize register listeners flag */
	dpu_comp->has_dfr_related_listener_registered = false;

	dpu_comp->is_peri_starting = false;

	/* initialize the composer interface which would be called by device component */
	if (is_offline_panel(&comp->base)) {
		comp->on = composer_manager_on_no_lock; /* MUST */
		comp->off = composer_manager_off_no_lock; /* MUST */
	} else {
		comp->on = composer_manager_on; /* MUST */
		comp->off = composer_manager_off; /* MUST */
	}
	comp->present = composer_manager_present; /* MUST */

	/* follow interfaces are optional */
	comp->set_fastboot = composer_manager_set_fastboot;
	comp->create_fence = composer_manager_create_fence;
	comp->release_fence = composer_manager_release_fence;
	comp->get_product_config = composer_manager_get_prodcut_config;
	comp->get_ext_info = composer_manager_get_ext_info;
	comp->get_sysfs_attrs = composer_manager_get_sysfs_attrs;
	comp->get_hdr_mean = dpu_effect_hdr_get_mean;
	comp->get_hdr_statistic = dpu_effect_hdr_get_statistic;
	comp->register_alsc = dpu_effect_register_alsc_composer;
	comp->unregister_alsc = dpu_effect_unregister_alsc_composer;
	comp->get_alsc_info = dpu_effect_get_alsc_info;
	comp->get_tui_level1_layer_info = dpu_get_tui_level1_layer_info;
	comp->get_fusa_info = NULL;
	comp->effect_hiace_get_hist = composer_manager_get_hiace_hist;
	comp->effect_hiace_set_lut = composer_manager_set_hiace_lut;
	comp->effect_wake_up_hiace_hist = composer_manager_wake_up_hiace_hist;
	comp->wait_for_blank = composer_wait_for_blank;
	comp->restore_fast_unblank_status = composer_restore_fast_unblank_status;
	comp->blank_peri_handle = composer_blank_peri_handle;
	comp->set_safe_frm_rate = composer_manager_set_safe_frm_rate;
	comp->release = composer_manager_release;
	comp->set_active_rect = composer_manager_set_active_rect;
	comp->get_online_crc = dpu_composer_get_online_crc;
	comp->tunnel_present = composer_manager_tunnel_present;
	comp->send_pq_data = composer_manager_send_pq_data;
}

struct dpu_bl_ctrl *get_bl_ctrl_from_device(struct device *dev)
{
	struct dpu_composer *dpu_comp = NULL;
	if (!dev) {
		dpu_pr_err("pointer dev is null!\n");
		return NULL;
	}

	dpu_comp = to_dpu_composer(get_comp_from_device(dev));
	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is error");
		return NULL;
	}

	return &dpu_comp->bl_ctrl;
}

/* animation must play panel 1 after panel 0 */
static bool can_free_logo_buffer(struct dpu_composer *dpu_comp)
{
	if (dpu_comp->comp_mgr->init_panel_display_status == MULTI_PANEL_DISPLAY) {
		if (dpu_comp->comp.index == DEVICE_COMP_BUILTIN_ID) {
			dpu_pr_info("can free logo buffer: %d now\n", dpu_comp->comp.index);
			return true;
		}
	} else {
		if (dpu_comp->comp.index == DEVICE_COMP_PRIMARY_ID ||
			dpu_comp->comp.index == DEVICE_COMP_BUILTIN_ID) {
			dpu_pr_info("can free logo buffer: %d now\n", dpu_comp->comp.index);
			return true;
		}
	}
	return false;
}

bool composer_dpu_free_logo_buffer(struct dpu_composer *dpu_comp)
{
	int ret, i;
	uint32_t logo_buffer_base = 0;
	uint32_t logo_buffer_size = 0;
	uint32_t logo_buffer_tmp = 0;
	struct device_node *np = NULL;

	if (!can_free_logo_buffer(dpu_comp)) {
		dpu_pr_warn("NOTICE: can not free logo buffer: %d\n", dpu_comp->comp.index);
		return false;
	}

	np = of_find_node_by_path(DTS_PATH_LOGO_BUFFER);
	if (!np)
		dpu_pr_err("NOT FOUND dts path: %s!\n", DTS_PATH_LOGO_BUFFER);

	ret = of_property_read_u32_index(np, "reg", 1, &logo_buffer_base);
	if (ret != 0) {
		dpu_pr_err("failed to get logo_buffer_base resource! ret=%d\n", ret);
		logo_buffer_base = 0;
	}
	ret = of_property_read_u32_index(np, "reg", 3, &logo_buffer_size);
	if (ret != 0) {
		dpu_pr_err("failed to get logo_buffer_base resource! ret=%d\n", ret);
		logo_buffer_size = 0;
	}
	dpu_pr_info("free logo_buffer_size=0x%x, logo_buffer_size=0x%x\n", logo_buffer_base, logo_buffer_size);

	logo_buffer_tmp = logo_buffer_base;
#ifdef CONFIG_LIBLINUX
	liblinux_pal_mem_enable_rsv_mem(phys_to_pfn(logo_buffer_base),
					phys_to_pfn(logo_buffer_base + logo_buffer_size));
#else
	for (i = 0; i < (int)(logo_buffer_size / PAGE_SIZE); i++) {
		free_reserved_page(phys_to_page(logo_buffer_tmp));
		logo_buffer_tmp += PAGE_SIZE;
	}
	memblock_free(logo_buffer_base, logo_buffer_size);
#endif

	return true;
}
 
bool composer_dpu_free_cmdlist_buffer(struct dpu_composer *dpu_comp)
{
	int ret, i;
	uint32_t cmdlist_buffer_base = 0;
	uint32_t cmdlist_buffer_size = 0;
	uint32_t cmdlist_buffer_tmp = 0;
	struct device_node *np = NULL;
 
	if (!can_free_logo_buffer(dpu_comp) || !can_pre_init_config()) {
		dpu_pr_warn("NOTICE: can not free cmdlist buffer: %d\n", dpu_comp->comp.index);
		return false;
	}
 
	np = of_find_node_by_path(DTS_PATH_CMDLIST_BUFFER);
	if (!np)
		dpu_pr_err("NOT FOUND dts path: %s!\n", DTS_PATH_CMDLIST_BUFFER);
 
	ret = of_property_read_u32_index(np, "reg", 1, &cmdlist_buffer_base);
	if (ret != 0) {
		dpu_pr_err("failed to get cmdlist_buffer_base resource! ret=%d\n", ret);
		cmdlist_buffer_base = 0;
	}
	ret = of_property_read_u32_index(np, "reg", 3, &cmdlist_buffer_size);
	if (ret != 0) {
		dpu_pr_err("failed to get cmdlist_buffer_size resource! ret=%d\n", ret);
		cmdlist_buffer_size = 0;
	}
	dpu_pr_info("free cmdlist_buffer_base=0x%x, cmdlist_buffer_size=0x%x\n", cmdlist_buffer_base, cmdlist_buffer_size);
 
	cmdlist_buffer_tmp = cmdlist_buffer_base;
#ifdef CONFIG_LIBLINUX
	liblinux_pal_mem_enable_rsv_mem(phys_to_pfn(cmdlist_buffer_base),
					phys_to_pfn(cmdlist_buffer_base + cmdlist_buffer_size));
#else
	for (i = 0; i < (int)(cmdlist_buffer_size / PAGE_SIZE); i++) {
		free_reserved_page(phys_to_page(cmdlist_buffer_tmp));
		cmdlist_buffer_tmp += PAGE_SIZE;
	}
	memblock_free(cmdlist_buffer_base, cmdlist_buffer_size);
#endif
 
	return true;
}

static void composer_set_panel_type(struct dpu_composer *dpu_comp)
{
	uint32_t panel_type;
	panel_type = dpu_comp->conn_info->base.type;
	if (panel_type == PANEL_MIPI_VIDEO || panel_type == PANEL_DUAL_MIPI_VIDEO)
		mIsVideoPanel = 1;
}

bool is_video_panel_type(void)
{
	if (mIsVideoPanel == 1)
		return true;
 
	return false;
}

int32_t register_composer(struct dkmd_connector_info *pinfo)
{
	int ret = 0;
	size_t present_data_size = 0;
	struct dpu_composer *dpu_comp = NULL;
	struct platform_device *peri_device = NULL;

	dpu_check_and_return(!g_composer_manager, -EINVAL, warn, "composer manager is not ready!");
	dpu_check_and_return(!pinfo, -EINVAL, err, "pinfo is null!");

	peri_device = pinfo->base.peri_device;
	dpu_check_and_return(!peri_device, -EINVAL, err, "peri_device is null!");
	dpu_pr_info("register %s connector device!", peri_device->name);

	/* each connector interface has owner dpu_composer struct */
	dpu_comp = (struct dpu_composer *)devm_kzalloc(&peri_device->dev, sizeof(*dpu_comp), GFP_KERNEL);
	dpu_check_and_return(!dpu_comp, -ENOMEM, err, "alloc dpu_comp failed!");

	/* Initialize the dpu_comp struct */
	dpu_comp->conn_info = pinfo;
	dpu_comp_mgr_common_init(dpu_comp);
	dpu_comp_mgr_thread_setup(dpu_comp);

	// set panel type
	composer_set_panel_type(dpu_comp);

	present_data_size = is_offline_panel(&pinfo->base) ?
		sizeof(struct comp_offline_present) : sizeof(struct comp_online_present);
	dpu_comp->present_data = devm_kzalloc(&peri_device->dev, present_data_size, GFP_KERNEL);
	if (!dpu_comp->present_data) {
		dpu_comp_mgr_thread_relase(dpu_comp);
		devm_kfree(&peri_device->dev, dpu_comp);
		dpu_pr_err("alloc present_data failed!");
		return -ENOMEM;
	}
	composer_present_data_setup(dpu_comp, true);

	ret = device_mgr_create_gfxdev(&dpu_comp->comp);
	if (ret != 0) {
		dpu_comp_mgr_thread_relase(dpu_comp);
		composer_present_data_release(dpu_comp, true);
		devm_kfree(&peri_device->dev, dpu_comp->present_data);
		devm_kfree(&peri_device->dev, dpu_comp);
		return -EINVAL;
	}

	pinfo->base.comp_obj_info = &dpu_comp->comp.base;
	dksm_dmd_register(dpu_comp->comp.index, pinfo->base.lcd_name);
	return 0;
}

void unregister_composer(struct dkmd_connector_info *pinfo)
{
	struct composer *comp = NULL;
	struct dpu_composer *dpu_comp = NULL;
	struct platform_device *peri_device = NULL;
	if (!pinfo) {
		dpu_pr_err("pointer pinfo is null!\n");
		return;
	}

	comp = container_of(pinfo->base.comp_obj_info, struct composer, base);
	dpu_check_and_no_retval(!comp, err, "comp is null!");

	device_mgr_destroy_gfxdev(comp);

	dpu_comp = to_dpu_composer(comp);
	/* sensorhub aod deinit */
	dpu_sh_aod_deinit(dpu_comp);
	composer_present_data_release(dpu_comp, true);
	dpu_comp_mgr_thread_relase(dpu_comp);

	dpu_comp->comp_mgr->dpu_comps[comp->index] = NULL;

	peri_device = pinfo->base.peri_device;
	dpu_check_and_no_retval(!peri_device, err, "peri_device is null!");

	devm_kfree(&peri_device->dev, dpu_comp->present_data);
	devm_kfree(&peri_device->dev, dpu_comp);
	pinfo->base.comp_obj_info = NULL;
}

void composer_device_shutdown(struct dkmd_connector_info *pinfo)
{
	struct composer *comp = NULL;

	if (!pinfo || !pinfo->base.comp_obj_info) {
		dpu_pr_err("pinfo or comp_obj_info is nullptr!");
		return;
	}

	comp = container_of(pinfo->base.comp_obj_info, struct composer, base);
	if (!comp) {
		dpu_pr_err("comp is nullptr!");
		return;
	}

	device_mgr_shutdown_gfxdev(comp);
}

void composer_device_suspend(struct dkmd_connector_info *pinfo)
{
	struct composer *comp = NULL;

	if (!pinfo || !pinfo->base.comp_obj_info) {
		dpu_pr_err("pinfo or comp_obj_info is nullptr!");
		return;
	}

	comp = container_of(pinfo->base.comp_obj_info, struct composer, base);
	if (!comp) {
		dpu_pr_err("comp is nullptr!");
		return;
	}

	device_mgr_suspend_gfxdev(comp);
}

int32_t composer_active_vsync(struct dkmd_connector_info *pinfo, bool need_active_vsync)
{
	struct composer *comp = NULL;
	struct dpu_composer *dpu_comp = NULL;

	if (!pinfo || !pinfo->base.comp_obj_info) {
		dpu_pr_err("pinfo or comp_obj_info is nullptr!");
		return -1;
	}

	comp = container_of(pinfo->base.comp_obj_info, struct composer, base);
	if (!comp) {
		dpu_pr_err("comp is nullptr!");
		return -1;
	}

	dpu_comp = to_dpu_composer(comp);
	if (need_active_vsync)
		dpu_comp_active_vsync(dpu_comp);
	else
		dpu_comp_deactive_vsync(dpu_comp);

	return 0;
}

static enum hrtimer_restart dkmd_dvfs_dump_hrtimer_fnc(struct hrtimer *timer)
{
	struct dpu_composer *dpu_comp = NULL;
	struct comp_online_present *present = NULL;
	uint32_t current_total_freq;

	dpu_comp = container_of(timer, struct dpu_composer, dkmd_dvfs_dump_hrtimer);
	if(unlikely(!dpu_comp)) {
		dpu_pr_err("dpu_comp is null");
		return HRTIMER_NORESTART;
	}
	present = (struct comp_online_present *)dpu_comp->present_data;
	if(unlikely(!present)) {
		dpu_pr_err("present is null");
		return HRTIMER_NORESTART;
	}
	current_total_freq = 
		present->frames[present->displayed_idx].in_frame.dvfs_info.vote_freq_info.current_total_freq;
	
	dpu_pr_info("current_total_freq = %d", current_total_freq);

	if (unlikely(!g_debug_dvfs_dump_timer)) {
		return HRTIMER_NORESTART;
	}

	hrtimer_forward(timer,timer->base->get_time(),ktime_set(g_debug_dvfs_dump_timer,0));
	return HRTIMER_RESTART;
}

void composer_dvfs_dump_timer_init(struct dpu_composer *dpu_comp)
{
	if (unlikely(!g_debug_dvfs_dump_timer)) {
		return;
	}
	dpu_pr_debug("dvfs_dump_timer init and start!");
	hrtimer_init(&dpu_comp->dkmd_dvfs_dump_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	dpu_comp->dkmd_dvfs_dump_hrtimer.function = dkmd_dvfs_dump_hrtimer_fnc;

	hrtimer_start(&dpu_comp->dkmd_dvfs_dump_hrtimer, ktime_set(g_debug_dvfs_dump_timer,0), HRTIMER_MODE_REL);
}

void composer_cancel_dvfs_dump_timer(struct dpu_composer *dpu_comp)
{
	if (dpu_comp->dkmd_dvfs_dump_hrtimer.state == HRTIMER_STATE_INACTIVE) {
		return;
	}
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL\n");
	dpu_pr_debug("dvfs dump timer cancel!");
	hrtimer_cancel(&dpu_comp->dkmd_dvfs_dump_hrtimer);
	dpu_pr_debug("dvfs dump timer cancel done!");
}

int32_t regitster_product_ops(struct product_display_ops* ops)
{
	if (!ops) {
		dpu_pr_err("regitster_product_ops is nullptr!");
		return -1;
	}
	if (!g_composer_manager) {
		dpu_pr_err("g_composer_manager is nullptr!");
		return -1;
	}
	g_composer_manager->dpu_lcdkit_ops = ops;

	return 0;
}

bool composer_check_power_status(struct dpu_composer *dpu_comp)
{
	struct composer_manager *comp_mgr = NULL;
	dpu_check_and_return(!dpu_comp, false, err, "dpu_comp is null pointer");
	comp_mgr = dpu_comp->comp_mgr;
	dpu_check_and_return(!comp_mgr, false, err, "comp_mgr is null pointer");

	if (dpu_comp_status_is_disable(&comp_mgr->power_status)) {
		dpu_pr_warn("check_power_status, %u already power off!", dpu_comp->comp.index);
		return false;
	}

	if (comp_mgr->power_status.refcount.value[dpu_comp->comp.index] == 0) {
		dpu_pr_warn("check_power_status, refcount %u already power off!", dpu_comp->comp.index);
		return false;
	}

	return true;
}

int32_t composer_manager_tunnel_present(struct composer *comp, struct tunnel_layer_data *data)
{
	return 0;
}

static int32_t composer_manager_parse_dt(struct composer_manager *comp_mgr)
{
	int32_t ret;
	struct platform_device *pdev = comp_mgr->device;

	comp_mgr->dpu_base = of_iomap(comp_mgr->parent_node, 0);
	if (!comp_mgr->dpu_base) {
		dpu_pr_err("failed to get dpu_base!\n");
		return -ENXIO;
	}

	comp_mgr->pctrl_base = of_iomap(comp_mgr->parent_node, 1);
	if (!comp_mgr->pctrl_base)
		dpu_pr_warn("failed to get pctrl_base!\n");

	comp_mgr->media1_ctrl_base = of_iomap(comp_mgr->parent_node, 2);
	if (!comp_mgr->media1_ctrl_base)
		dpu_pr_warn("failed to get media1_ctrl_base!\n");

	comp_mgr->disp_ch1subsys_regulator = devm_regulator_get(&pdev->dev, "regulator_disp_ch1subsys");
	if (IS_ERR(comp_mgr->disp_ch1subsys_regulator)) {
		ret = (int32_t)PTR_ERR(comp_mgr->disp_ch1subsys_regulator);
		dpu_pr_info("disp_ch1subsys_regulator error, maybe do not support, pls check, ret=%d", ret);
	}

	comp_mgr->dsssubsys_regulator = devm_regulator_get(&pdev->dev, "regulator_dsssubsys");
	if (IS_ERR(comp_mgr->dsssubsys_regulator)) {
		ret = (int32_t)PTR_ERR(comp_mgr->dsssubsys_regulator);
		dpu_pr_err("dsssubsys_regulator error, ret=%d", ret);
		return ret;
	}

	comp_mgr->vivobus_regulator = devm_regulator_get(&pdev->dev, "regulator_vivobus");
	if (IS_ERR(comp_mgr->vivobus_regulator)) {
		ret = (int32_t)PTR_ERR(comp_mgr->vivobus_regulator);
		dpu_pr_err("vivobus_regulator error, ret=%d", ret);
		return ret;
	}

	comp_mgr->media1_subsys_regulator = devm_regulator_get(&pdev->dev, "regulator_media_subsys");
	if (IS_ERR(comp_mgr->media1_subsys_regulator)) {
		ret = (int32_t)PTR_ERR(comp_mgr->media1_subsys_regulator);
		dpu_pr_err("media1_subsys_regulator error, ret=%d", ret);
		return ret;
	}

	comp_mgr->regulator_smmu_tcu = devm_regulator_get(&pdev->dev, "regulator_smmu_tcu");
	if (IS_ERR(comp_mgr->regulator_smmu_tcu)) {
		ret = (int32_t)PTR_ERR(comp_mgr->regulator_smmu_tcu);
		dpu_pr_err("regulator_smmu_tcu error, ret=%d", ret);
		return ret;
	}

	comp_mgr->vivobus_autodiv_regulator = devm_regulator_get(&pdev->dev, "regulator_vivobus_autodiv");
	if (IS_ERR(comp_mgr->vivobus_autodiv_regulator)) {
		ret = (int32_t)PTR_ERR(comp_mgr->vivobus_autodiv_regulator);
		dpu_pr_info("regulator_vivobus_autodiv error, ret=%d", ret);
	}

	comp_mgr->sdp_irq = of_irq_get(comp_mgr->parent_node, 0);
	dpu_pr_info("comp_mgr sdp_irq=%d", comp_mgr->sdp_irq);

	comp_mgr->mdp_irq = of_irq_get(comp_mgr->parent_node, 1);
	dpu_pr_info("comp_mgr mdp_irq=%d", comp_mgr->mdp_irq);

	return 0;
}

static void composer_manager_isr_init(struct composer_manager *comp_mgr,
	struct ukmd_isr *isr_ctrl,
	int32_t irq_no, char *irq_name,
	uint32_t unmask, irqreturn_t (*isr_fnc)(int32_t irq, void *ptr))
{
	(void)snprintf(isr_ctrl->irq_name, sizeof(isr_ctrl->irq_name), irq_name);
	isr_ctrl->irq_no = irq_no;
	isr_ctrl->isr_fnc = isr_fnc;
	isr_ctrl->parent = comp_mgr;
	isr_ctrl->unmask = unmask;
	ukmd_isr_setup(isr_ctrl);
	isr_ctrl->handle_func(isr_ctrl, UKMD_ISR_REQUEST);
}

static void composer_manager_setup(struct composer_manager *comp_mgr)
{
	uint32_t unmask;
	uint32_t unmask_mdp;

	mutex_init(&(comp_mgr->idle_lock));
	spin_lock_init(&(comp_mgr->dimming_spin_lock));
	dpu_comp_status_default_setup(&comp_mgr->active_status);
	dpu_comp_status_default_setup(&comp_mgr->dimming_status);
	comp_mgr->idle_func_flag = 0;
	comp_mgr->idle_enable = true;
	comp_mgr->init_scene_cmdlist = 0;
	dpu_comp_status_default_setup(&comp_mgr->power_status);
	sema_init(&comp_mgr->power_sem, 1);
	mutex_init(&comp_mgr->tbu_sr_lock);
	INIT_LIST_HEAD(&comp_mgr->isr_list);
	mutex_init(&(comp_mgr->free_logo_buffer_lock));

	dkmd_sdp_isr_get_unmask(&unmask);
	composer_manager_isr_init(comp_mgr, &comp_mgr->sdp_isr_ctrl,
		comp_mgr->sdp_irq, "dpu_irq_sdp", unmask, dpu_sdp_isr);
	list_add_tail(&comp_mgr->sdp_isr_ctrl.list_node, &comp_mgr->isr_list);

	/* mdp only used for dacc trans to acpu */
	unmask_mdp = get_mdp_irq_unmask();
	composer_manager_isr_init(comp_mgr, &comp_mgr->mdp_isr_ctrl,
		comp_mgr->mdp_irq, "dpu_irq_mdp", unmask_mdp, dpu_mdp_isr);

}

static int32_t composer_manager_probe(struct platform_device *pdev)
{
	int ret;
	uint32_t scene_id;
	struct device_node *node = NULL;
	struct composer_manager *comp_mgr = NULL;

	comp_mgr = (struct composer_manager *)devm_kzalloc(&pdev->dev, sizeof(*comp_mgr), GFP_KERNEL);
	if (!comp_mgr) {
		dpu_pr_err("alloc dpu_comp_mgr failed!");
		return -EINVAL;
	}
	comp_mgr->device = pdev;
	comp_mgr->parent_node = pdev->dev.of_node;
	comp_mgr->hardware_reseted = 0;
	composer_manager_parse_dt(comp_mgr);

	dkmd_create_mdfx_client(comp_mgr);

	/* parse child scene node for composer manager */
	for_each_child_of_node(comp_mgr->parent_node, node) {
		if (!of_device_is_available(node))
			continue;

		of_property_read_u32(node, "scene_id", &scene_id);
		dpu_pr_info("get composer scene id=%u", scene_id);
		if (scene_id > DPU_SCENE_OFFLINE_2) {
			dpu_pr_warn("scene id=%u over %d", scene_id, DPU_SCENE_OFFLINE_2);
			continue;
		}

		comp_mgr->scene[scene_id] = devm_kzalloc(&pdev->dev, sizeof(struct composer_scene), GFP_KERNEL);
		if (!comp_mgr->scene[scene_id]) {
			dpu_pr_warn("alloc scene struct failed!");
			return -ENOMEM;
		}
		comp_mgr->scene[scene_id]->dpu_base = comp_mgr->dpu_base;
		comp_mgr->scene[scene_id]->scene_id = scene_id;

		/* To initialize composer scene module interface */
		dpu_comp_scene_device_setup(comp_mgr->scene[scene_id]);
	}
	platform_set_drvdata(pdev, comp_mgr);
	composer_manager_setup(comp_mgr);
	g_composer_manager = comp_mgr;

	ret = dpu_tui_irq_register(pdev);
	if (ret != 0)
		dpu_pr_warn("dpu_tui_irq_register failed!");

	return 0;
}

static int32_t composer_manager_remove(struct platform_device *pdev)
{
	dkmd_destroy_mdfx_client();
	return 0;
}

static const struct of_device_id g_composer_manager_match_table[] = {
	{
		.compatible = "dkmd,composer_manager",
		.data = NULL,
	},
	{},
};

static struct platform_driver g_composer_manager_driver = {
	.probe = composer_manager_probe,
	.remove = composer_manager_remove,
	.driver = {
		.name = "composer_manager",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(g_composer_manager_match_table),
	}
};

static int32_t __init composer_manager_register(void)
{
	return platform_driver_register(&g_composer_manager_driver);
}
module_init(composer_manager_register);

MODULE_AUTHOR("Graphics Display");
MODULE_DESCRIPTION("Composer Manager Module Driver");
MODULE_LICENSE("GPL");
