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

#include "mipi_dsi_sync.h"
#include "mipi_dsi_dev.h"
#include "dkmd_object.h"
#include "dkmd_log.h"
#include "dpu_conn_mgr.h"

int32_t mipi_dsi_sync_write(struct dpu_connector *connector, struct mipi_dsi_tx_params *params)
{
	uint32_t i;
	struct mipi_dsi_cmds *dsi_cmds = &params->write_cmds;

	if (mipi_dsi_fifo_is_full(connector->connector_base)) {
		dpu_pr_err("wait 10ms for fifo consuming, but failed");
		return -1;
	}

	/* if dsi power mode is enhanced, we don't need chang power mode */
	if (params->power_mode == MIPI_DSI_POWER_MODE_SINGLE) {
		mipi_dsi_set_interval(connector, params->hardware_wait, dsi_cmds->cmds_num);
		for (i = 0; i < dsi_cmds->cmds_num; i++) {
			mipi_dsi_cmd_add(&dsi_cmds->cmds[i], connector->connector_base);
			delay_for_next_cmd(dsi_cmds->cmds[i].wait, dsi_cmds->cmds[i].waittype);
		}
		return 0;
	}

	/* default is high speed mode, change to low power mode */
	if (params->power_mode == MIPI_DSI_POWER_MODE_LP)
		mipi_dsi_tx_lp_mode_cfg(connector->connector_base);

	mipi_dsi_set_interval(connector, params->hardware_wait, dsi_cmds->cmds_num);
	for (i = 0; i < dsi_cmds->cmds_num; i++) {
		mipi_dsi_cmd_add(&dsi_cmds->cmds[i], connector->connector_base);
		delay_for_next_cmd(dsi_cmds->cmds[i].wait, dsi_cmds->cmds[i].waittype);
	}

	/* resume to high speed mode */
	if (params->power_mode == MIPI_DSI_POWER_MODE_LP)
		mipi_dsi_tx_hs_mode_cfg(connector->connector_base);

	return 0;
}

int32_t mipi_dsi_sync_read(struct dpu_connector *connector, struct mipi_dsi_rx_params *params)
{
	int ret;
	uint32_t tmp_value[READ_MAX] = {0};

	struct dsi_cmd_desc *cmd = &params->read_cmd;
	struct mipi_dsi_out *dsi_out = &params->read_outs;

	if (mipi_dsi_fifo_is_full(connector->connector_base)) {
		dpu_pr_err("wait 10ms for fifo consuming, but failed");
		return -1;
	}
	dpu_pr_debug("power mode = %d", params->power_mode);

	if (params->power_mode == MIPI_DSI_POWER_MODE_LP)
		mipi_dsi_rx_lp_mode_cfg(connector->connector_base);

	ret = mipi_dsi_lread_reg(tmp_value, READ_MAX, cmd, cmd->dlen, connector->connector_base);

	if (params->power_mode == MIPI_DSI_POWER_MODE_LP)
		mipi_dsi_rx_hs_mode_cfg(connector->connector_base);

	if (ret) {
		dpu_pr_err("mipi read error");
		return ret;
	}

	ret = mipi_dsi_get_read_value(cmd, dsi_out->out, tmp_value, (uint32_t)dsi_out->out_len, params->is_little_endian);
	if (ret) {
		dpu_pr_err("get read value error");
		return ret;
	}

	connector->need_check_mipi_connected = false;
	return 0;
}

int32_t dual_mipi_dsi_sync_write(struct dpu_connector *connector0, struct dpu_connector *connector1,
	struct mipi_dsi_tx_params *dsi0_params, struct mipi_dsi_tx_params *dsi1_params)
{
	uint32_t i;
	dpu_pr_debug("+");

	if (mipi_dual_dsi_fifo_is_full(connector0->connector_base, connector1->connector_base)) {
		dpu_pr_err("wait 10ms for fifo consuming, but failed");
		return -1;
	}

    /* if dsi power mode is enhanced, we don't need chang power mode */
	if (dsi0_params->power_mode == MIPI_DSI_POWER_MODE_SINGLE) {
		if (dsi0_params->write_cmds.cmds_num == dsi1_params->write_cmds.cmds_num) {
			mipi_dsi_set_interval(connector0, dsi0_params->hardware_wait, dsi0_params->write_cmds.cmds_num);
			for (i = 0; i < dsi0_params->write_cmds.cmds_num; i++) {
				mipi_dsi_cmd_add(&dsi0_params->write_cmds.cmds[i], connector0->connector_base);
				mipi_dsi_cmd_add(&dsi1_params->write_cmds.cmds[i], connector1->connector_base);
				delay_for_next_cmd(dsi0_params->write_cmds.cmds[i].wait, dsi0_params->write_cmds.cmds[i].waittype);
			}
		} else {
			dpu_pr_warn("diff cmd cnt, not support yet");
		}

		dpu_pr_debug("-");
        return 0;
	}

	if (dsi0_params->power_mode == MIPI_DSI_POWER_MODE_LP) {
		mipi_dsi_tx_lp_mode_cfg(connector0->connector_base);
		mipi_dsi_tx_lp_mode_cfg(connector1->connector_base);
	}

	if (dsi0_params->write_cmds.cmds_num == dsi1_params->write_cmds.cmds_num) {
		mipi_dsi_set_interval(connector0, dsi0_params->hardware_wait, dsi0_params->write_cmds.cmds_num);
		for (i = 0; i < dsi0_params->write_cmds.cmds_num; i++) {
			mipi_dsi_cmd_add(&dsi0_params->write_cmds.cmds[i], connector0->connector_base);
			mipi_dsi_cmd_add(&dsi1_params->write_cmds.cmds[i], connector1->connector_base);
			delay_for_next_cmd(dsi0_params->write_cmds.cmds[i].wait, dsi0_params->write_cmds.cmds[i].waittype);
		}
	} else {
		dpu_pr_warn("diff cmd cnt, not support yet");
	}

	if (dsi0_params->power_mode == MIPI_DSI_POWER_MODE_LP) {
		mipi_dsi_tx_hs_mode_cfg(connector0->connector_base);
		mipi_dsi_tx_hs_mode_cfg(connector1->connector_base);
	}
	dpu_pr_debug("-");
	return 0;
}

int32_t dual_mipi_dsi_sync_read(struct dpu_connector *connector0, struct dpu_connector *connector1,
    struct mipi_dsi_rx_params *dsi0_params, struct mipi_dsi_rx_params *dsi1_params)
{
	int32_t ret;
	struct mipi_dual_dsi_param dual_dsi0 = {0};
	struct mipi_dual_dsi_param dual_dsi1 = {0};
	uint32_t dsi0_tmp_val[READ_MAX] = {0};
	uint32_t dsi1_tmp_val[READ_MAX] = {0};
	dpu_pr_debug("+");

	dual_dsi0.dsi_base = connector0->connector_base;
	dual_dsi0.value_out = dsi0_tmp_val;
	dual_dsi1.dsi_base = connector1->connector_base;
	dual_dsi1.value_out = dsi1_tmp_val;

	ret = mipi_dual_dsi_lread_reg(&dual_dsi0, &dsi0_params->read_cmd, dsi0_params->read_cmd.dlen, &dual_dsi1);
	if (ret != 0) {
		dpu_pr_err("mipi read error");
		return -EINVAL;
	}

	ret = mipi_dsi_get_read_value(&dsi0_params->read_cmd, dsi0_params->read_outs.out, dsi0_tmp_val,
		dsi0_params->read_outs.out_len, dsi0_params->is_little_endian);
	if (ret != 0) {
		dpu_pr_err("get read value error");
		return ret;
	}

	ret = mipi_dsi_get_read_value(&dsi0_params->read_cmd, dsi1_params->read_outs.out, dsi1_tmp_val,
		dsi0_params->read_outs.out_len, dsi0_params->is_little_endian);
	if (ret != 0) {
		dpu_pr_err("get read value error");
		return ret;
	}
	dpu_pr_debug("-");
	return 0;
}