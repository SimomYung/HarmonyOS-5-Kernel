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
#include "dp_ctrl_refresh_count.h"
 
#define EDP_BASE_PANEL_ID 100

int32_t get_dp_refresh_count(uint32_t port_id, struct panel_refresh_statistic_info *refresh_stat_info)
{
	struct dp_panel_mgr *dp_panel_mgr = NULL;
	struct panel_refresh_statistic_rslt *stat_rslt = NULL;
	dpu_pr_debug("+");
	dpu_check_and_return(port_id < EDP_BASE_PANEL_ID || port_id >= EDP_BASE_PANEL_ID + MAX_DPTX_DEV_INDEX, -1,
		err, "port_id is invalid value");
	port_id -= EDP_BASE_PANEL_ID;
	dpu_check_and_return(!refresh_stat_info, -1, err, "refresh_stat_info is not null");

	dp_panel_mgr = get_dp_panel_mgr(port_id);
	dpu_check_and_return(!dp_panel_mgr, -1, warn, "dp_panel_mgr is not registered");

	stat_rslt = dp_panel_mgr->panel_get_refresh_statistic_data((int)port_id);
	dpu_check_and_return(!stat_rslt, -1, err, "stat_rslt is not null");

	*refresh_stat_info = stat_rslt->stat_info;
	atomic_set(&(stat_rslt->clear_flag), 1);
	dpu_pr_debug("-");
	return 0;
}

static struct panel_refresh_statistic_rslt *dp_get_refresh_statistic(struct dpu_connector *connector)
{
	struct dp_private *dp_priv = NULL;
	struct dp_ctrl *dptx = NULL;
	struct panel_refresh_statistic_rslt *stat_rslt = NULL;
	dpu_pr_debug("+");
	dpu_check_and_return(!connector->conn_info, NULL, err, "conn_info is null");

	dp_priv = to_dp_private(connector->conn_info);
	dpu_check_and_return(!dp_priv, NULL, err, "dp_priv is null");

	dptx = &dp_priv->dp[MASTER_DPTX_IDX];

	dpu_check_and_return(!dptx->dp_panel_mgr, NULL, err, "dp_panel_mgr is null");
	dpu_check_and_return(!dptx->dp_panel_mgr->panel_get_refresh_statistic_data, NULL,
	    err, "panel_get_refresh_statistic_data is null");

	stat_rslt = dptx->dp_panel_mgr->panel_get_refresh_statistic_data(dptx->port_id);
	dpu_pr_debug("-");

	return stat_rslt;
}

int32_t dp_set_refresh_statistic(struct dpu_connector *connector, const void *value)
{
	struct panel_refresh_statistic_rslt *stat_rslt = NULL;
	dpu_pr_debug("+");
	dpu_check_and_return(!connector, -1, err, "connector is null");
	dpu_check_and_return(!value, -1, err, "value is null");

	stat_rslt = dp_get_refresh_statistic(connector);
	dpu_check_and_return(!stat_rslt, -1, warn, "stat rslt is null");

	stat_rslt->stat_info = *(struct panel_refresh_statistic_info*)(value);
	dpu_pr_debug("-");

	return 0;
}

int32_t dp_get_stat_clear_flag(struct dpu_connector *connector, const void *value)
{
	int32_t clear_flag = 0;
	struct panel_refresh_statistic_rslt *stat_rslt = NULL;
	dpu_pr_debug("+");
	dpu_check_and_return(!connector, -1, err, "connector is null");
	dpu_check_and_return(!value, -1, err, "value is null");

	stat_rslt = dp_get_refresh_statistic(connector);
	dpu_check_and_return(!stat_rslt, -1, warn, "stat rslt is null");

	clear_flag = atomic_read(&(stat_rslt->clear_flag));
	atomic_set(&(stat_rslt->clear_flag), 0);

	dpu_pr_debug("-");
	return clear_flag;
}