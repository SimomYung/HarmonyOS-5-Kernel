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

#include "dkmd_mipi_dsi_itf.h"
#include "mipi_dsi_sync.h"
#include "mipi_dsi_async.h"
#include "dpu_conn_mgr.h"

static int32_t mipi_dsi_tx_params_check(struct mipi_dsi_tx_params *params)
{
	if (unlikely(!params)) {
		dpu_pr_err("params is null");
		return -1;
	}

	if (unlikely(params->write_cmds.cmds_num > TX_CMD_MAX || params->write_cmds.cmds_num == 0)) {
		dpu_pr_err("cmds_num = %u is invalid", params->write_cmds.cmds_num);
		return -1;
	}
 
	if (unlikely(!params->write_cmds.cmds)) {
		dpu_pr_err("cmds is null");
		return -1;
	}

	return 0;
}

static int32_t mipi_dsi_rx_params_out_buffer_check(struct mipi_dsi_rx_params *params)
{
	if (unlikely(!params)) {
		dpu_pr_err("params is null");
		return -1;
	}

	if (unlikely(params->read_cmd.dlen > BUF_MAX || params->read_cmd.dlen == 0)) {
		dpu_pr_err("dlen = %u is invalid", params->read_cmd.dlen);
		return -1;
	}

	return 0;
}

int32_t dpu_mipi_dsi_tx(struct mipi_dsi_tx_params *params)
{
	int32_t ret = 0;
	struct dpu_connector *connector = NULL;

	if (mipi_dsi_tx_params_check(params) != 0) {
		return -1;
	}

	connector = get_real_connector(params->dsi_id, params->panel_type);
	if (!connector) {
		dpu_pr_err("connector_id=%u is not available!", params->dsi_id);
		return -1;
	}

	if (composer_active_vsync(connector->conn_info, true)) {
		dpu_pr_err("active vsync failed");
		return -1;
	}

	mutex_lock(&connector->mipi_itf_lock);

	if (params->is_force_sync) {
		ret = mipi_dsi_sync_write(connector, params);
		mutex_unlock(&connector->mipi_itf_lock);
		composer_active_vsync(connector->conn_info, false);
		return ret;
	}

	switch (params->trans_mode) {
	case MIPI_DSI_MODE_TRANS_SYNC:
		ret = mipi_dsi_sync_write(connector, params);
		break;
	case MIPI_DSI_MODE_TRANS_ASYNC:
		ret = mipi_dsi_async_tx(connector, params);
		break;
	default:
		dpu_pr_err("invalid trans mode");
		break;
	}

	mutex_unlock(&connector->mipi_itf_lock);

	composer_active_vsync(connector->conn_info, false);
	return ret;
}

int32_t dpu_mipi_dsi_rx(struct mipi_dsi_rx_params *params)
{
	int32_t ret = 0;
	struct dpu_connector *connector = NULL;

	if (mipi_dsi_rx_params_out_buffer_check(params) != 0) {
		return -1;
	}

	if (unlikely(params->read_cmd.dlen > BUF_MAX || params->read_cmd.dlen == 0)) {
		dpu_pr_err("dlen = %u is invalid", params->read_cmd.dlen);
		return -1;
	}

	if (unlikely(!params->read_cmd.payload)) {
		dpu_pr_err("payload is null");
		return -1;
	}

	connector = get_real_connector(params->dsi_id, params->panel_type);
	if (!connector) {
		dpu_pr_err("connector_id=%u is not available!", params->dsi_id);
		return -1;
	}

	if (composer_active_vsync(connector->conn_info, true)) {
		dpu_pr_err("active vsync failed");
		return -1;
	}

	mutex_lock(&connector->mipi_itf_lock);
	ret = mipi_dsi_sync_read(connector, params);
	mutex_unlock(&connector->mipi_itf_lock);

	composer_active_vsync(connector->conn_info, false);

	return ret;
}

int32_t dpu_dual_mipi_dsi_tx(struct mipi_dsi_tx_params *dsi0_params, struct mipi_dsi_tx_params *dsi1_params)
{
	int32_t ret = 0;
	struct dpu_connector *connector0 = NULL;
	struct dpu_connector *connector1 = NULL;

	if (mipi_dsi_tx_params_check(dsi0_params) != 0 || mipi_dsi_tx_params_check(dsi1_params) != 0) {
		return -1;
	}

	connector0 = get_real_connector(dsi0_params->dsi_id, dsi0_params->panel_type);
	if (unlikely(!connector0)) {
		dpu_pr_err("connector_id0=%u is not available!", dsi0_params->dsi_id);
		return -1;
	}

	connector1 = get_real_connector(dsi1_params->dsi_id, dsi1_params->panel_type);
	if (unlikely(!connector1)) {
		dpu_pr_err("connector_id1=%u is not available!", dsi1_params->dsi_id);
		return -1;
	}

	if (connector0->bind_connector != connector1) {
		dpu_pr_err("connector order error, pls exchange dsi0_params and dsi1_params");
		return -1;
	}

	if (composer_active_vsync(connector0->conn_info, true)) {
		dpu_pr_err("active vsync failed");
		return -1;
	}

	mutex_lock(&connector0->mipi_itf_lock);

	if (dsi0_params->is_force_sync) {
		ret = dual_mipi_dsi_sync_write(connector0, connector1, dsi0_params, dsi1_params);
		mutex_unlock(&connector0->mipi_itf_lock);
		composer_active_vsync(connector0->conn_info, false);
		return ret;
	}

	switch (dsi0_params->trans_mode) {
	case MIPI_DSI_MODE_TRANS_SYNC:
		ret = dual_mipi_dsi_sync_write(connector0, connector1, dsi0_params, dsi1_params);
		break;
	case MIPI_DSI_MODE_TRANS_ASYNC:
		ret = dual_mipi_dsi_async_tx(connector0, connector1, dsi0_params, dsi1_params);
		break;
	default:
		dpu_pr_err("invalid trans mode");
		break;
	}

	mutex_unlock(&connector0->mipi_itf_lock);
	composer_active_vsync(connector0->conn_info, false);
	return ret;
}

int32_t dpu_dual_mipi_dsi_rx(struct mipi_dsi_rx_params *dsi0_params, struct mipi_dsi_rx_params *dsi1_params)
{
	int32_t ret = 0;
	struct dpu_connector *connector0 = NULL;
	struct dpu_connector *connector1 = NULL;
	dpu_pr_info("+");

	if (mipi_dsi_rx_params_out_buffer_check(dsi0_params) != 0 ||
		mipi_dsi_rx_params_out_buffer_check(dsi1_params) != 0) {
		return -1;
	}

	if (unlikely(dsi0_params->read_cmd.dlen > BUF_MAX || dsi0_params->read_cmd.dlen == 0)) {
		dpu_pr_err("dlen = %u is invalid.", dsi0_params->read_cmd.dlen);
		return -1;
	}

	if (unlikely(!dsi0_params->read_cmd.payload)) {
		dpu_pr_err("payload is null");
		return -1;
	}

	if (unlikely(dsi0_params->read_cmd.dlen != dsi1_params->read_cmd.dlen)) {
		dpu_pr_warn("dsi0 and dsi1 cmd is not same.");
	}

	connector0 = get_real_connector(dsi0_params->dsi_id, dsi0_params->panel_type);
	if (unlikely(!connector0)) {
		dpu_pr_err("connector_id0=%u is not available!", dsi0_params->dsi_id);
		return -1;
	}

	connector1 = get_real_connector(dsi1_params->dsi_id, dsi1_params->panel_type);
	if (unlikely(!connector1)) {
		dpu_pr_err("connector_id1=%u is not available!", dsi1_params->dsi_id);
		return -1;
	}

	if (connector0->bind_connector != connector1) {
		dpu_pr_err("connector order error, pls exchange dsi0_params and dsi1_params");
		return -1;
	}

	if (composer_active_vsync(connector0->conn_info, true)) {
		dpu_pr_err("active vsync failed");
		return -1;
	}
	mutex_lock(&connector0->mipi_itf_lock);
	ret = dual_mipi_dsi_sync_read(connector0, connector1, dsi0_params, dsi1_params);
	mutex_unlock(&connector0->mipi_itf_lock);

	composer_active_vsync(connector0->conn_info, false);
	return ret;
}

int32_t dpu_mipi_dsi_async_tx_stop(struct mipi_dsi_connector_params *con_params)
{
	int32_t ret = 0;
	struct dpu_connector *connector = NULL;

	if (!con_params) {
		dpu_pr_err("params is null");
		return MIPI_E_NULL_PTR;
	}

	connector = get_real_connector(con_params->dsi_id, con_params->panel_type);
	if (!connector) {
		dpu_pr_err("connector_id=%u is not available!", con_params->dsi_id);
		return MIPI_E_NULL_PTR;
	}

	if (composer_active_vsync(connector->conn_info, true)) {
		dpu_pr_err("active vsync failed");
		return -1;
	}

	mutex_lock(&connector->mipi_itf_lock);
	ret = mipi_dsi_async_tx_stop(connector);
	mutex_unlock(&connector->mipi_itf_lock);

	composer_active_vsync(connector->conn_info, false);
	return ret;
}
