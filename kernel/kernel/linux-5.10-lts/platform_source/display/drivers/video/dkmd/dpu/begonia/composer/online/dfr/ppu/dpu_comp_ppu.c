/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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
#include "dpu_comp_ppu.h"
#include "dfr/dpu_comp_dfr.h"
#include "dfr/dpu_comp_dfr_te_skip.h"
#include "mipi_dsi_dev.h"
#include "peri/dkmd_peri.h"
#include "dpu_comp_mgr.h"
#include "dkmd_log.h"

#define PPU_MODE_TE_RATE 120
#define PPU_MODE_TE_MASK_NUM 0

void dpu_ppu_setup_priv_data(struct dpu_comp_ppu_ctrl *ppu_ctrl, struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	uint32_t w_h_value = 0;
	char __iomem *dpu_base = dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
	struct dkmd_connector_info *pinfo = dfr_ctrl->dpu_comp->conn_info;

	ppu_ctrl->dfr_ctrl = dfr_ctrl;
	ppu_ctrl->panel_refresh_mode = PANEL_REFRESH_MODE_FULL;

	w_h_value = pinfo->base.xres & 0xFFFF;
	w_h_value |= (pinfo->base.yres & 0xFFFF) << 16;

	outp32(DPU_DFR_FULL_REFRESH_RECT(dpu_base), 0);
	outp32(DPU_DFR_FULL_REFRESH_RECT(dpu_base) + 0x4, w_h_value);

	outp32(DPU_DFR_HIGH_REFRESH_RECT(dpu_base), 0);
	outp32(DPU_DFR_HIGH_REFRESH_RECT(dpu_base) + 0x4, w_h_value);
}

void dpu_ppu_release_priv_data(struct dpu_comp_ppu_ctrl *ppu_ctrl)
{
	struct dkmd_connector_info *pinfo = ppu_ctrl->dfr_ctrl->dpu_comp->conn_info;
	struct mipi_dsi_cmds_info dsi_cmds_info = { 0 };

	// Power-off need to exit PPU, because enter full screen aod, panel does not need to power-off.
	// Power-off need to call mipi sync, mipi async causes problems.
	if (ppu_ctrl->panel_refresh_mode == PANEL_REFRESH_MODE_PPU) {
		dsi_cmds_info.cmds_info.cmds_type = DKMD_CMDS_TYPE_PPU_EXIT;
		dsi_cmds_info.cmds_info.data_len = 0;
		dsi_cmds_info.follow_frame = false;
		dsi_cmds_info.is_force_sync = true;
		pipeline_next_ops_handle(pinfo->conn_device, pinfo, SEND_CMDS, &dsi_cmds_info);
	}

	ppu_ctrl->ppu_ctrl_mode = PPU_CTRL_MODE_NORMAL;
	ppu_ctrl->panel_refresh_mode = PANEL_REFRESH_MODE_FULL;
}

static bool is_same_dirty_rect(const struct dkmd_rect *dirty_rect, const struct dkmd_rect *new_dirty_rect)
{
	return ((dirty_rect->x == new_dirty_rect->x) && (dirty_rect->y == new_dirty_rect->y) &&
		(dirty_rect->w == new_dirty_rect->w) && (dirty_rect->h == new_dirty_rect->h));
}

static void dpu_ppu_enter(struct dpu_comp_ppu_ctrl *ppu_ctrl)
{
	struct dkmd_connector_info *pinfo = ppu_ctrl->dfr_ctrl->dpu_comp->conn_info;
	char __iomem *dpu_base = ppu_ctrl->dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
	struct mipi_dsi_cmds_info dsi_cmds_info = {0};
	struct sfr_info sfr_info = {0};

	dsi_cmds_info.cmds_info.cmds_type = DKMD_CMDS_TYPE_PPU_ENTER;
	dsi_cmds_info.cmds_info.data_len = 0;
	dsi_cmds_info.follow_frame = true;
	dsi_cmds_info.is_force_sync = false;
	pipeline_next_ops_handle(pinfo->conn_device, pinfo, SEND_CMDS, &dsi_cmds_info);

	dpu_pr_debug("partial panel cmds_type: %u", dsi_cmds_info.cmds_info.cmds_type);

	// panel refresh mode had changed, sfr need to set follow frame
	ppu_ctrl->dfr_ctrl->follow_frame = true;
	sfr_info.info_type = INFO_TYPE_PPU_ENTER;
	pipeline_next_ops_handle(pinfo->conn_device, pinfo, NOTIFY_SFR_INFO, &sfr_info);
	// after notify sfr info to DE-kernel, need to clear follow_frame flag
	ppu_ctrl->dfr_ctrl->follow_frame = false;

	outp32(DPU_DFR_PANEL_REFRESH_MODE(dpu_base), PANEL_REFRESH_MODE_PPU);
}

static void dpu_ppu_exit(struct dpu_comp_ppu_ctrl *ppu_ctrl)
{
	struct dkmd_connector_info *pinfo = ppu_ctrl->dfr_ctrl->dpu_comp->conn_info;
	char __iomem *dpu_base = ppu_ctrl->dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
	struct mipi_dsi_cmds_info dsi_cmds_info = {0};
	struct sfr_info sfr_info = {0};

	dsi_cmds_info.cmds_info.cmds_type = DKMD_CMDS_TYPE_PPU_EXIT;
	dsi_cmds_info.cmds_info.data_len = 0;
	dsi_cmds_info.follow_frame = true;
	dsi_cmds_info.is_force_sync = false;
	pipeline_next_ops_handle(pinfo->conn_device, pinfo, SEND_CMDS, &dsi_cmds_info);

	dpu_pr_debug("partial panel cmds_type: %u", dsi_cmds_info.cmds_info.cmds_type);

	// panel refresh mode had changed, sfr need to set follow_ frame
	ppu_ctrl->dfr_ctrl->follow_frame = true;
	sfr_info.info_type = INFO_TYPE_PPU_EXIT;
	pipeline_next_ops_handle(pinfo->conn_device, pinfo, NOTIFY_SFR_INFO, &sfr_info);
	// after notify sfr info to DE-kernel, need to clear follow_frame flag
	ppu_ctrl->dfr_ctrl->follow_frame = false;

	outp32(DPU_DFR_PANEL_REFRESH_MODE(dpu_base), PANEL_REFRESH_MODE_FULL);

	memset_s(&ppu_ctrl->panel_rect, sizeof(struct dkmd_rect), 0, sizeof(struct dkmd_rect));
}

static void dpu_ppu_set_dirty_rect(struct dpu_comp_ppu_ctrl *ppu_ctrl)
{
	struct dkmd_connector_info *pinfo = ppu_ctrl->dfr_ctrl->dpu_comp->conn_info;
	char __iomem *dpu_base = ppu_ctrl->dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
	struct mipi_dsi_cmds_info dsi_cmds_info = {0};
	uint32_t dirty_rect_value = 0;

	struct dkmd_rect pre_panel_rect = ppu_ctrl->panel_rect;
	struct dkmd_rect cur_panel_rect = ppu_ctrl->panel_refresh_ctrl.panel_rect;

	if (g_debug_panel_refresh_rect != 0) {
		cur_panel_rect.x = 0;
		cur_panel_rect.y = g_debug_panel_refresh_rect & 0xFFFF;
		cur_panel_rect.w = pinfo->base.xres;
		cur_panel_rect.h = (g_debug_panel_refresh_rect & 0xFFFF0000) >> 16;
	}

	if (is_same_dirty_rect(&pre_panel_rect, &cur_panel_rect)) {
		return;
	}

	ppu_ctrl->panel_rect = cur_panel_rect;

	dpu_pr_info("cur dirty_rect[%d %d %u %u] pre dirty_rect[%d %d %u %u]",
		cur_panel_rect.x, cur_panel_rect.y, cur_panel_rect.w, cur_panel_rect.h,
		pre_panel_rect.x, pre_panel_rect.y, pre_panel_rect.w, pre_panel_rect.h);

	dsi_cmds_info.cmds_info.cmds_type = DKMD_CMDS_TYPE_PPU_RECT;
	dsi_cmds_info.cmds_info.data_len = 4;
	dsi_cmds_info.cmds_info.data[0] = cur_panel_rect.x;
	dsi_cmds_info.cmds_info.data[1] = cur_panel_rect.y;
	dsi_cmds_info.cmds_info.data[2] = (int32_t)cur_panel_rect.w;
	dsi_cmds_info.cmds_info.data[3] = (int32_t)cur_panel_rect.h;
	dsi_cmds_info.follow_frame = true;
	dsi_cmds_info.is_force_sync = false;

	dpu_pr_debug("partial panel cmds_type: %u", dsi_cmds_info.cmds_info.cmds_type);

	dirty_rect_value = (uint32_t)cur_panel_rect.x & 0xFFFF;
	dirty_rect_value |= ((uint32_t)cur_panel_rect.y & 0xFFFF) << 16;
	outp32(DPU_DFR_HIGH_REFRESH_RECT(dpu_base), dirty_rect_value);

	dirty_rect_value = cur_panel_rect.w & 0xFFFF;
	dirty_rect_value |= (cur_panel_rect.h & 0xFFFF) << 16;
	outp32(DPU_DFR_HIGH_REFRESH_RECT(dpu_base) + 0x4, dirty_rect_value);

	pipeline_next_ops_handle(pinfo->conn_device, pinfo, SEND_CMDS, &dsi_cmds_info);
}

static void dpu_ppu_set_strategy(struct dpu_comp_ppu_ctrl *ppu_ctrl)
{
	uint32_t pre_ppu_mode = ppu_ctrl->ppu_ctrl_mode;
	uint32_t cur_ppu_mode = ppu_ctrl->panel_refresh_ctrl.ppu_ctrl_mode;
	char __iomem *dpu_base = ppu_ctrl->dfr_ctrl->dpu_comp->comp_mgr->dpu_base;

	if (pre_ppu_mode != cur_ppu_mode) {
		dpu_pr_debug("ppu ctrl mode changed: %u", cur_ppu_mode);
		if (cur_ppu_mode == PPU_CTRL_MODE_MRRI)
			dfr_te_skip_disable_dimming(ppu_ctrl->dfr_ctrl);
		else if (cur_ppu_mode == PPU_CTRL_MODE_NORMAL)
			dfr_te_skip_enable_dimming(ppu_ctrl->dfr_ctrl);

		ppu_ctrl->ppu_ctrl_mode = cur_ppu_mode;
	}

	outp32(DPU_DFR_AUX_FULL_REFRESH(dpu_base), ppu_ctrl->panel_refresh_ctrl.is_aux_full_refresh);
}

void dpu_ppu_process(struct dpu_comp_ppu_ctrl *ppu_ctrl)
{
	uint32_t pre_refresh_mode = ppu_ctrl->panel_refresh_mode;
	uint32_t cur_refresh_mode = ppu_ctrl->panel_refresh_ctrl.panel_refresh_mode;

	dpu_pr_debug("pre refresh mode: %u, cur refresh mode: %u", pre_refresh_mode, cur_refresh_mode);

	ppu_ctrl->panel_refresh_mode = cur_refresh_mode;

	if (pre_refresh_mode == PANEL_REFRESH_MODE_FULL && cur_refresh_mode == PANEL_REFRESH_MODE_PPU)
		dpu_ppu_enter(ppu_ctrl);
	else if (pre_refresh_mode == PANEL_REFRESH_MODE_PPU && cur_refresh_mode == PANEL_REFRESH_MODE_FULL)
		dpu_ppu_exit(ppu_ctrl);
	else if (pre_refresh_mode == PANEL_REFRESH_MODE_PPU && cur_refresh_mode == PANEL_REFRESH_MODE_PPU) {
		dpu_ppu_set_strategy(ppu_ctrl);
		dpu_ppu_set_dirty_rect(ppu_ctrl);
	}
}

uint32_t dpu_ppu_get_te_rate(struct dpu_comp_ppu_ctrl *ppu_ctrl)
{
	if (ppu_ctrl->panel_refresh_mode == PANEL_REFRESH_MODE_PPU)
		return PPU_MODE_TE_RATE;

	return ppu_ctrl->dfr_ctrl->cur_te_rate;
}

uint32_t dpu_ppu_get_te_mask_num(struct dpu_comp_ppu_ctrl *ppu_ctrl)
{
	if (ppu_ctrl->panel_refresh_mode == PANEL_REFRESH_MODE_PPU)
		return PPU_MODE_TE_MASK_NUM;

	return ppu_ctrl->dfr_ctrl->cur_te_mask_num;
}