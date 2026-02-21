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

#include "dp_msr.h"
#include "dp_drv.h"
#include "dp_ctrl.h"
#include "dkmd_dp_interface.h"

int32_t set_dptx_display_active_region(struct dkmd_connector_info *pinfo, struct dkmd_rect_coord *region)
{
	struct dp_private *dp_priv = NULL;
	struct dp_ctrl *dptx = NULL;
	struct dp_panel_mgr *dp_panel_mgr = NULL;

	dpu_pr_info("[msr] +");

	dp_priv = to_dp_private(pinfo);
	dpu_check_and_return(!dp_priv, -1, err, "[msr] - dp_priv is NULL");
	dptx = &dp_priv->dp[MASTER_DPTX_IDX];
	dpu_check_and_return(!dptx, -1, err, "[msr] - dptx is NULL");
	dp_panel_mgr = dptx->dp_panel_mgr;
	dpu_check_and_return(!dp_panel_mgr, -1, err, "[msr] - dp_panel_mgr is NULL");

	if (!dptx->dptx_enable) {
		dpu_pr_info("[DP] dptx has already off");
		return -1;
	}

	if (dp_panel_mgr->panel_set_display_active_region != NULL &&
		dp_panel_mgr->panel_set_display_active_region(region) != 0) {
		dpu_pr_err("[msr] - multi screen switch failed");
		return -1;
	}

	dpu_pr_info("[msr] -");
	return 0;
}