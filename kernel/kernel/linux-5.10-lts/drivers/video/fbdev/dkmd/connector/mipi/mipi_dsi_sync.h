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
#ifndef MIPI_DSI_SYNC_H
#define MIPI_DSI_SYNC_H

#include "dkmd_mipi_dsi_itf.h"
#include "dkmd_mipi_panel_info.h"

struct dpu_connector;

int32_t mipi_dsi_sync_write(struct dpu_connector *connector, struct mipi_dsi_tx_params *params);
int32_t mipi_dsi_sync_read(struct dpu_connector *connector, struct mipi_dsi_rx_params *params);
int32_t dual_mipi_dsi_sync_write(struct dpu_connector *connector0, struct dpu_connector *connector1,
    struct mipi_dsi_tx_params *dsi0_params, struct mipi_dsi_tx_params *dsi1_params);
int32_t dual_mipi_dsi_sync_read(struct dpu_connector *connector0, struct dpu_connector *connector1,
    struct mipi_dsi_rx_params *dsi0_params, struct mipi_dsi_rx_params *dsi1_params);
#endif

