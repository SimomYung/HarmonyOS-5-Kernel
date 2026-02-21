/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
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

#include <linux/delay.h>
#include <dpu/soc_dpu_define.h>

#include "dkmd_log.h"
#include "dkmd_connector.h"
#include "dpu_conn_mgr.h"
#include "mipi_dsi_dev.h"

#define C08_LCD_POWER_STATUS 0x9c
#define HO1_LCD_POWER_STATUS 0x1c

int32_t lcd_check_power_status(struct dpu_connector *connector)
{
	int32_t ret;
	uint32_t lcd_state_value = 0;
	char __iomem *dsi_base = NULL;
	struct dfr_info *dfr_info = NULL;
	char lcd_cmd[] = {0x0A};
	struct dsi_cmd_desc lcd_state_dsi_cmd = {DTYPE_DCS_READ, 0, 10, WAIT_TYPE_US, sizeof(lcd_cmd), lcd_cmd};

	dpu_check_and_return(!connector, -EINVAL, err, "connector is NULL!");
	dsi_base = connector->connector_base;
	dpu_check_and_return(!dsi_base, -EINVAL, err, "dsi_base is NULL!");

	ret = mipi_dsi_lread_reg(&lcd_state_value, 1, &lcd_state_dsi_cmd, lcd_state_dsi_cmd.dlen, dsi_base);
	if (ret != 0) {
		dpu_pr_err("read lcd status failed");
		return ret;
	}
	dpu_pr_info("Power State = 0x%x", lcd_state_value);
 
	dpu_check_and_return(!connector->conn_info, -EINVAL, err, "connector->conn_info is NULL!");
	dfr_info = dkmd_get_dfr_info(connector->conn_info);
 
	dpu_check_and_return(!dfr_info, -EINVAL, err, "dfr_info is NULL!");
	dpu_pr_info("dfr_info->ddic_type = 0x%x", dfr_info->ddic_type);
 
	if (dfr_info->ddic_type == DDIC_TYPE_H01)
		return ((lcd_state_value & HO1_LCD_POWER_STATUS) == HO1_LCD_POWER_STATUS) ? 0 : -1;
	if (dfr_info->ddic_type == DDIC_TYPE_C08 || dfr_info->ddic_type == DDIC_TYPE_F01)
		return ((lcd_state_value & C08_LCD_POWER_STATUS) == C08_LCD_POWER_STATUS) ? 0 : -1;
 
	return -1;
}