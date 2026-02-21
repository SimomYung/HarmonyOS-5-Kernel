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
#ifndef MIPI_DSI_ASYNC_H
#define MIPI_DSI_ASYNC_H

#include "dkmd_mipi_panel_info.h"
#include "dkmd_connector.h"
#include "dkmd_mipi_dsi_itf.h"
#include <linux/dma-mapping.h>
#include <linux/spinlock.h>
#include <linux/ktime.h>

#define ASYNC_LP_DELAY_US 125
#define ASYNC_HS_DELAY_MAX_US 125
#define ASYNC_CMD_SEND_TIME_US 2
#define TIMER_STEP_COUNT_MIN 1
#define TIMER_STEP_COUNT_MAX 14
#define TIMER_CLK 32000 // dacc timer clk 32k
#define TIME_PER_CYCLE_NS (1000000000 / TIMER_CLK)  // 31250ns
struct dpu_connector;

enum TX_OPS_TYPE{
    TX_SEND_TASKS = 0,
    TX_STOP,
    TX_OPS_TYPE_MAX,
};

enum TX_OPS_STATUS{
    TX_OPS_REQ = 0,
    TX_OPS_DONE,
    TX_OPS_REPLACE_TASK_DONE,
    TX_OPS_NEW_TASK_DROP,
    TX_OPS_E_FIFO_FULL,
    TX_OPS_E_TOO_MANY_TASK,
    TX_OPS_E_REPLACE_TASK_FAILED,
    TX_OPS_STATUS_MAX,
};

enum CMDS_FLAG {
	CMDS_HAS_REFRESH  =  BIT(0),
	CMDS_FOLLOW_FRAME =  BIT(1),
};

enum CMDS_VALIDATE_REPETITION {
	CMDS_NO_VALIDATE = 0,
	CMDS_ORDER_FIRST,
	CMDS_FOLLOW_FRAME_FIRST,
};

enum ASYNC_STATUS {
	ASYNC_UNINITIALIZED = 0,
	ASYNC_INITIALIZED,
	ASYNC_STARTED,
	ASYNC_STOPED,
};

struct mipi_dsi_cmdlist_desc {
	bool is_applied;
	ktime_t applied_timestamp;
	uint32_t cmdlist_header;
	uint32_t cmdlist_payload;
	dma_addr_t cmdlist_phy_addr;
	enum dkmd_cmds_type cmds_type;
};

struct mipi_dsi_async_ctrl {
	uint32_t cmdlist_dev_id;
	uint32_t cmdlist_scene_id;
	uint32_t applied_cmdlist_num;
	spinlock_t cmdlist_lock;
	enum ASYNC_STATUS async_status;
	struct mipi_dsi_cmdlist_desc cmdlists[MIPI_ASYNC_TX_MAX_NUM];
};

int32_t mipi_dsi_async_init(struct dpu_connector *connector);
void mipi_dsi_async_deinit(struct mipi_dsi_async_ctrl *async_ctrl);

int32_t mipi_dsi_async_tx(struct dpu_connector *connector, struct mipi_dsi_tx_params *params);
void mipi_dsi_async_tx_done(struct dpu_connector *connector);
int32_t mipi_dsi_async_tx_stop(struct dpu_connector *connector);
int32_t dual_mipi_dsi_async_tx(struct dpu_connector *connector0, struct dpu_connector *connector1,
	struct mipi_dsi_tx_params *dsi0_params, struct mipi_dsi_tx_params *dsi1_params);

#endif
