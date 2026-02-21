
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

#include "dpu_conn_mgr.h"
#include "dkmd_rect.h"
#include "dp_ctrl.h"
#include "dp_drv.h"
#include "hidptx_reg.h"
#include "dp_ctrl_partial_update.h"
#include "psr_config_base.h"

static bool is_dirty_region_empty(const struct dkmd_rect *disp_rect)
{
	return (disp_rect->x == 0 && disp_rect->y == 0 && disp_rect->w == 0 && disp_rect->h == 0);
}

static bool is_dirty_region_valid(const struct dkmd_rect *dirty_rect,
	const struct dkmd_object_info *pinfo, struct dpu_connector *connector)
{
	uint32_t dsc_slice_height = 0;
	if (connector->active_idx != 0 && connector->post_info[1] == NULL) {
		dpu_pr_warn("[DP] dirty active_idx %d not support\n", connector->active_idx);
		connector->active_idx = 0;
	}
	dsc_slice_height = connector->post_info[connector->active_idx]->dsc.dsc_info.slice_height;

	/* dpu dirty_rect width should be panel width */
	if ((dirty_rect->x != 0) || (dirty_rect->w != pinfo->xres)) {
		dpu_pr_err("dirty_rect x or w is not panel_width for dpu!");
		return false;
	}

	if (pinfo->dsc_en == 0) /* dsc is disable */
		return true;

	/* dsc dirty_rect height should be align with dsc slice_height */
	if (dsc_slice_height == 0) {
		dpu_pr_err("slice height is 0 for dsc!");
		return false;
	}

	if ((dirty_rect->y % dsc_slice_height != 0) || (dirty_rect->h % dsc_slice_height != 0)) {
		dpu_pr_err("dirty_rect y or h is not aligned with dsc_slice_height for dsc!");
		return false;
	}

	return true;
}

static void get_dpu_dirty_rect(const struct dkmd_object_info *pinfo, const struct dkmd_rect *disp_rect,
	struct dkmd_rect *dirty_rect)
{
	if (!is_dirty_region_empty(disp_rect)) {
		*dirty_rect = *disp_rect;
		return;
	}

	dirty_rect->x = 0;
	dirty_rect->y = 0;
	dirty_rect->w = pinfo->xres;
	dirty_rect->h = pinfo->yres;
	dpu_pr_debug("[DP] empty dirty region, reset region[%d %d %u %u]",
		dirty_rect->x, dirty_rect->y, dirty_rect->w, dirty_rect->h);
}

static bool is_same_dirty_rect(const struct dkmd_rect *dirty_rect, const struct dkmd_rect *new_dirty_rect)
{
	return ((dirty_rect->x == new_dirty_rect->x) && (dirty_rect->y == new_dirty_rect->y) &&
		(dirty_rect->w == new_dirty_rect->w) && (dirty_rect->h == new_dirty_rect->h));
}

static void dp_ctrl_partial_update_set_coord(const struct dpu_connector *connector)
{
	struct dkmd_rect dirty_rect = connector->dirty_rect;
	struct dp_private *dp_priv = to_dp_private(connector->conn_info);
	struct dp_ctrl *dptx = &dp_priv->dp[MASTER_DPTX_IDX];

	if (!dptx || !dptx->dptx_enable || dptx->power_saving_mode) {
		dpu_pr_warn("[DP] dptx is not exist or has already been off!");
		return;
	}

	dptx->su_rect.top_x = dirty_rect.x;
	dptx->su_rect.top_y = dirty_rect.y;
	dptx->su_rect.bottom_x = (uint16_t)dirty_rect.x + (uint16_t)dpu_width(dirty_rect.w);
	dptx->su_rect.bottom_y = (uint16_t)dirty_rect.y + (uint16_t)dpu_height(dirty_rect.h);

	dptx->dptx_psr2_config_su_region(dptx);

	dpu_pr_info("dirty_rect_reg[%d %d %u %u]",
		dptx->su_rect.top_x, dptx->su_rect.top_y, dptx->su_rect.bottom_x, dptx->su_rect.bottom_y);
}

static void dp_ctrl_check_su_region(struct dpu_connector *connector)
{
	uint32_t bottom_x = 0;
	uint32_t bottom_y = 0;
	struct psr2_su_region su_val = {0};
	struct dp_ctrl *dptx = NULL;
	struct dp_private *dp_priv = to_dp_private(connector->conn_info);
	dpu_check_and_no_retval(!dp_priv, err, "[DP] dp_priv is null pointer");
	dptx = &dp_priv->dp[MASTER_DPTX_IDX];

	dptx_psr2_read_su_region(dptx, &su_val);

	bottom_x = (uint32_t)connector->dirty_rect.x + (uint32_t)dpu_width(connector->dirty_rect.w);
	bottom_y = (uint32_t)connector->dirty_rect.y + (uint32_t)dpu_height(connector->dirty_rect.h);
	if (connector->dirty_rect.x != (int32_t)su_val.top_x || connector->dirty_rect.y != (int32_t)su_val.top_y
			|| bottom_x != su_val.bottom_x || bottom_y != su_val.bottom_y) {
		dpu_pr_warn("connector dirty region[%d %d %u %u] is not equal to current su region[%u %u %u %u]",
			connector->dirty_rect.x, connector->dirty_rect.y, bottom_x, bottom_y,
			su_val.top_x, su_val.top_y, su_val.bottom_x, su_val.bottom_y);
		dp_ctrl_partial_update_set_coord(connector);
	}
}

int32_t dp_ctrl_partial_update(struct dpu_connector *connector, const void *value)
{
	struct dkmd_connector_info *pinfo = connector->conn_info;
	struct dkmd_rect dirty_rect = {0};

	if (pinfo->dirty_region_updt_support == 0) {
		return 0;
	}

	get_dpu_dirty_rect(&pinfo->base, (const struct dkmd_rect *)value, &dirty_rect);

	if (is_same_dirty_rect(&connector->dirty_rect, &dirty_rect)) {
		dp_ctrl_check_su_region(connector);
		return 0;
	}

	dpu_pr_debug("dirty_rect[%d %d %u %u] pre dirty_rect[%d %d %u %u]",
		dirty_rect.x, dirty_rect.y, dirty_rect.w, dirty_rect.h,
		connector->dirty_rect.x, connector->dirty_rect.y, connector->dirty_rect.w, connector->dirty_rect.h);

	if (!is_dirty_region_valid(&dirty_rect, &pinfo->base, connector)) {
		dpu_pr_err("dirty_rect[%d %d %u %u] is invalid!", dirty_rect.x, dirty_rect.y, dirty_rect.w, dirty_rect.h);
		return 0;
	}

	connector->dirty_rect = dirty_rect;

	dp_ctrl_partial_update_set_coord(connector);

	return 0;
}
