
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

#include "dp_drv.h"
#include "dfr/dp_ctrl_sfr_update.h"
 
int32_t dp_notify_sfr_info(struct dpu_connector *connector, const void *value)
{
	struct dp_private *dp_priv = NULL;
	struct dp_ctrl *dptx = NULL;
	struct sfr_info *sfr_info = NULL;

	dpu_check_and_return(!connector, -1, err, "connector is null");
	dpu_check_and_return(!connector->conn_info, -1, err, "conn_info is null");
	dpu_check_and_return(!value, -1, err, "value is null");

	dp_priv = to_dp_private(connector->conn_info);
	dpu_check_and_return(!dp_priv, -1, err, "dp_priv is null");
	dptx = &dp_priv->dp[MASTER_DPTX_IDX];

	dpu_check_and_return(!dptx->dp_panel_mgr, -1, err, "dp_panel_mgr is null");

	sfr_info = (struct sfr_info *)value;

	if (dptx->dp_panel_mgr->notify_sfr_info) {
		dpu_pr_debug("notify sfr info to eDP");
		return dptx->dp_panel_mgr->notify_sfr_info(sfr_info);
	}
	return -1;
}