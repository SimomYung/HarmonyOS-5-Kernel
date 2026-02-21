/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
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

#ifndef MIPI_CONFIG_UTILS_H
#define MIPI_CONFIG_UTILS_H

#include <linux/types.h>
#include "dkmd_rect.h"

struct dpu_connector;
void mipi_dsi_mem_init(char __iomem *mipi_dsi_base);
void mipi_dsi_te_ctrl3_init(char __iomem *mipi_dsi_base, uint32_t te_pin_val, uint64_t lane_byte_clk);
bool mipi_dsi_is_bypass_by_pg(uint32_t connector_id);
void mipi_dsi_enable_hs_pkt_discontin_ctrl(char __iomem *mipi_dsi_base);
void mipi_dsi_dfr_print_debug_info(struct dpu_connector *connector);
void mipi_config_div_set(char __iomem *mipi_dsi_base, uint32_t post_div, uint32_t pre_div);
void mipi_dsi_partial_update_set_display_region(struct dpu_connector *connector);
#endif /* MIPI_CONFIG_UTILS_H */
