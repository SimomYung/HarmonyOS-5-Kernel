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

#include "mipi_dsi_async.h"
#include <dpu/soc_dpu_define.h>
#include "mipi_dsi_dev.h"
#include "dkmd_log.h"
#include "res_mgr.h"
#include "ukmd_cmdlist.h"
#include "cmdlist_interface.h"
#include "dpu_conn_mgr.h"
#include "dkmd_comp.h"
#include "mipi_config_utils.h"

#define WAIT_CNT 3000
#define TASK_ACK_TIMEOUT_THRES_US 300000

static bool is_cmdlist_applied_long_time(struct mipi_dsi_cmdlist_desc *cmdlist)
{
	ktime_t current_time = ktime_get();
	uint64_t applied_duration_us;
	if (cmdlist->is_applied) {
		applied_duration_us = (uint64_t)(ktime_to_us(current_time) - ktime_to_us(cmdlist->applied_timestamp));
		if (applied_duration_us > TASK_ACK_TIMEOUT_THRES_US) {
			dpu_pr_warn("cmdlist wasn't gave back for a long time(300ms), force to recycle it");
			return true;
		}
	}

	return false;
}

static int32_t cmdlist_apply(struct mipi_dsi_async_ctrl *async_ctrl)
{
	int32_t i;
	bool force_recycle;
	unsigned long flags = 0;

	spin_lock_irqsave(&async_ctrl->cmdlist_lock, flags);

	for (i = 0; i < MIPI_ASYNC_TX_MAX_NUM; i++) {
		if (!async_ctrl->cmdlists[i].is_applied) {
			async_ctrl->cmdlists[i].applied_timestamp = ktime_get();
			async_ctrl->cmdlists[i].is_applied = true;
			async_ctrl->applied_cmdlist_num++;
			spin_unlock_irqrestore(&async_ctrl->cmdlist_lock, flags);
			ukmd_cmdlist_clear_client(async_ctrl->cmdlist_dev_id, async_ctrl->cmdlist_scene_id,
				async_ctrl->cmdlists[i].cmdlist_payload);
			return i;
		}
	}

	spin_unlock_irqrestore(&async_ctrl->cmdlist_lock, flags);
	dpu_pr_warn("no cmdlist to apply");
	return -1;
}

static void cmdlist_recycle(struct mipi_dsi_async_ctrl *async_ctrl, uint32_t cmdlist_header_id)
{
	int32_t i;
	unsigned long flags = 0;

	spin_lock_irqsave(&async_ctrl->cmdlist_lock, flags);
	for (i = 0; i < MIPI_ASYNC_TX_MAX_NUM; i++) {
		if (async_ctrl->cmdlists[i].cmdlist_header == cmdlist_header_id &&
			async_ctrl->cmdlists[i].is_applied == true) {
			async_ctrl->cmdlists[i].is_applied = false;
			async_ctrl->cmdlists[i].cmds_type = DKMD_CMDS_TYPE_MAX;
			async_ctrl->applied_cmdlist_num--;
			break;
		}
	}

	spin_unlock_irqrestore(&async_ctrl->cmdlist_lock, flags);
}

static void mipi_dsi_async_recycle_replaced_cmdlist(struct dpu_connector *connector)
{
	uint32_t cmdlist_header_id = 0;
	struct mipi_dsi_async_ctrl *async_ctrl = &connector->dsi_async_ctrl;

	cmdlist_header_id = read_reg_with_retry(DPU_ASYNC_TX_REL_REPEATED_TASK_CMDLIST_ID_ADDR(connector->dpu_base));
	cmdlist_recycle(async_ctrl, cmdlist_header_id);
	dpu_pr_info("give back replaced cmdlist_header_id = %u", cmdlist_header_id);
}

static void mipi_dsi_async_tx_print_dacc_queue_info(struct dpu_connector *connector)
{
	dpu_pr_debug("flw_size=%u, normal_size=%u, flw_num_to_tx=%u, nor_num_to_tx=%u",
		inp32(DPU_ASYNC_TX_QUEUE_FLW_SIZE(connector->dpu_base)),
		inp32(DPU_ASYNC_TX_QUEUE_NOR_SIZE(connector->dpu_base)),
		inp32(DPU_ASYNC_TX_QUEUE_FLW_NUM_TO_TX(connector->dpu_base)),
		inp32(DPU_ASYNC_TX_QUEUE_NOR_NUM_TO_TX(connector->dpu_base)));
}

static int32_t wait_for_dmcu_ops_done(struct dpu_connector *connector)
{
	uint32_t status;
	uint32_t try_cnt = 0;
	int32_t ret = 0;

	do {
		status = read_reg_with_retry(DPU_ASYNC_TX_OPS_STATUS_ADDR(connector->dpu_base));
		try_cnt++;
	} while ((status == TX_OPS_REQ) && (try_cnt < WAIT_CNT));

	dpu_pr_debug("wait count = %d", try_cnt);
	mipi_dsi_async_tx_print_dacc_queue_info(connector);

	switch (status) {
	case TX_OPS_DONE:
		ret = 0;
		break;
	case TX_OPS_REPLACE_TASK_DONE:
	case TX_OPS_NEW_TASK_DROP:
		mipi_dsi_async_recycle_replaced_cmdlist(connector);
		ret = 0;
		break;
	case TX_OPS_E_REPLACE_TASK_FAILED:
		dpu_pr_err("dmcu replace task failed");
		ret = MIPI_E_REPLACE_TASK_FAILED;
		break;
	case TX_OPS_REQ:
		dpu_pr_err("dmcu ops timeout");
		ret = MIPI_E_TIMEOUT;
		break;
	case TX_OPS_E_FIFO_FULL:
	case TX_OPS_E_TOO_MANY_TASK:
		dpu_pr_err("dmcu handle one task fail,  fifo full");
		ret = MIPI_E_TOO_MANY_TXS;
		break;
	default:
		dpu_pr_warn("dmcu handle task req unknown status = %u", status);
		ret = MIPI_E_UNKNOWN;
		break;
	}

	return ret;
}

static int32_t one_cmdlist_create(uint32_t dev_id, uint32_t scene_id,
	uint32_t *header_id, uint32_t *payload_id)
{
	int32_t ret;
	uint32_t cmdlist_header_id;
	uint32_t cmdlist_payload_id;

	dev_id = CMDLIST_DEV_ID_DPU;

	cmdlist_header_id = cmdlist_create_user_client(dev_id, scene_id, SCENE_NOP_TYPE, 0, 0);
	if (unlikely(cmdlist_header_id == 0)) {
		dpu_pr_err("scene_id=%u, create header failed", scene_id);
		return -1;
	}

	cmdlist_payload_id = cmdlist_create_user_client(dev_id, scene_id,
		REGISTER_CONFIG_TYPE, 0, PAGE_SIZE);
	if (unlikely(cmdlist_payload_id == 0)) {
		dpu_pr_err("scene_id=%u create payload failed", scene_id);
		ukmd_cmdlist_release_locked(dev_id, scene_id, cmdlist_header_id);
		return -1;
	}

	ret = cmdlist_append_client(dev_id, scene_id, cmdlist_header_id, cmdlist_payload_id);
	if (unlikely(ret != 0)) {
		dpu_pr_err("append payload failed");
		ukmd_cmdlist_release_locked(dev_id, scene_id, cmdlist_header_id);
		return -1;
	}

	*header_id = cmdlist_header_id;
	*payload_id = cmdlist_payload_id;
	return 0;
}

static uint32_t calc_cmdlist_num(uint32_t cmds_num)
{
	uint32_t cmdlist_num = cmds_num / DSI_HP_FIFO_CMDS_MAX;
	return ((cmds_num % DSI_HP_FIFO_CMDS_MAX) != 0) ? (cmdlist_num + 1) : cmdlist_num;
}

static void cmdlists_destroy(struct mipi_dsi_async_ctrl *async_ctrl)
{
	int i;
	for (i = 0; i < MIPI_ASYNC_TX_MAX_NUM; i++)
		ukmd_cmdlist_release_locked(async_ctrl->cmdlist_dev_id,
			async_ctrl->cmdlist_scene_id, async_ctrl->cmdlists[i].cmdlist_header);
}

int32_t mipi_dsi_async_init(struct dpu_connector *connector)
{
	int32_t i, ret;
	dma_addr_t phy_addr = 0;
	uint32_t header_id, payload_id;
	struct mipi_dsi_async_ctrl *async_ctrl = &connector->dsi_async_ctrl;
	struct dfr_info *dfr_info = dkmd_get_dfr_info(connector->conn_info);
	if (!dfr_info) {
		dpu_pr_warn("dfr_info is nullptr");
		async_ctrl->async_status = ASYNC_UNINITIALIZED;
		return -1;
	}

	if (dfr_info->dfr_mode != DFR_MODE_TE_SKIP_BY_MCU) {
		async_ctrl->async_status = ASYNC_UNINITIALIZED;
		dpu_pr_info("dfr te skip is not by MCU, async tx is not support");
		return 0;
	}

	async_ctrl->applied_cmdlist_num = 0;
	async_ctrl->cmdlist_scene_id = DPU_SCENE_ONLINE_3;
	async_ctrl->cmdlist_dev_id = CMDLIST_DEV_ID_DPU;
	async_ctrl->async_status = ASYNC_STARTED;

	spin_lock_init(&async_ctrl->cmdlist_lock);
	dpu_pr_info("+");

	for (i = 0; i < MIPI_ASYNC_TX_MAX_NUM; i++) {
		ret = one_cmdlist_create(async_ctrl->cmdlist_dev_id,
			async_ctrl->cmdlist_scene_id, &header_id, &payload_id);
		if (ret) {
			dpu_pr_err("create cmdlist failed, ret = %d", ret);
			return MIPI_E_ALLOC_FAILED;
		}

		phy_addr = cmdlist_get_phy_addr(async_ctrl->cmdlist_dev_id,
			async_ctrl->cmdlist_scene_id, payload_id);
		if (phy_addr == 0) {
			dpu_pr_err("cmdlist phy addr is null");
			return MIPI_E_UNKNOWN;
		}

		async_ctrl->cmdlists[i].is_applied = false;
		async_ctrl->cmdlists[i].cmdlist_header = header_id;
		async_ctrl->cmdlists[i].cmdlist_payload = payload_id;
		async_ctrl->cmdlists[i].cmdlist_phy_addr = phy_addr;
		async_ctrl->cmdlists[i].cmds_type = DKMD_CMDS_TYPE_MAX;

		dpu_pr_debug("request cmdlist_header = %d, payload_id = %d, phy_addr = 0x%x",
			async_ctrl->cmdlists[i].cmdlist_header,
			async_ctrl->cmdlists[i].cmdlist_payload,
			async_ctrl->cmdlists[i].cmdlist_phy_addr);
	}

	return 0;
}

void mipi_dsi_async_deinit(struct mipi_dsi_async_ctrl *async_ctrl)
{
	if (async_ctrl->async_status == ASYNC_UNINITIALIZED) {
		dpu_pr_debug("async is not initialized");
		return;
	}

	cmdlists_destroy(async_ctrl);
	async_ctrl->async_status = ASYNC_UNINITIALIZED;
}

static void mipi_dsi_async_dump_unsend_cmdlist(struct mipi_dsi_async_ctrl *async_ctrl)
{
	uint32_t i = 0;
	for (i = 0; i < MIPI_ASYNC_TX_MAX_NUM; i++) {
		if (async_ctrl->cmdlists[i].is_applied == true) {
			dpu_pr_warn("unsend cmdlist_header = %d, payload_id = %d",
				async_ctrl->cmdlists[i].cmdlist_header,
				async_ctrl->cmdlists[i].cmdlist_payload);
#ifdef CONFIG_UKMD_DEBUG_ENABLE
			ukmd_cmdlist_dump_by_id(async_ctrl->cmdlist_dev_id, async_ctrl->cmdlist_scene_id,
				async_ctrl->cmdlists[i].cmdlist_payload);
#endif
		}
	}
}

int32_t mipi_dsi_async_tx_stop(struct dpu_connector *connector)
{
	int32_t ret = 0;
	struct mipi_dsi_async_ctrl *async_ctrl = NULL;

	dpu_pr_debug("+");
	if (!connector) {
		dpu_pr_err("connector is NULL!");
		return MIPI_E_NULL_PTR;
	}
	async_ctrl = &connector->dsi_async_ctrl;
	if (async_ctrl->async_status != ASYNC_STARTED) {
		dpu_pr_debug("async tx already stoped");
		return 0;
	}

	async_ctrl->async_status = ASYNC_STOPED;
	outp32(DPU_ASYNC_TX_OPS_TYPE_ADDR(connector->dpu_base), TX_STOP);
	outp32(DPU_ASYNC_TX_OPS_STATUS_ADDR(connector->dpu_base), TX_OPS_REQ);
	/* trigger riscv interrupt, bit 5 used by async tx, unmasked by ltpo */
	outp32(DPU_RISCV_INTR_TRIG_ADDR(connector->dpu_base), 0x20);
	
	ret = wait_for_dmcu_ops_done(connector);
	mipi_dsi_async_dump_unsend_cmdlist(async_ctrl);
	return ret;
}

static uint32_t mipi_dsi_set_bit_val(uint32_t org_val, uint32_t set_val, uint8_t bw, uint8_t bs)
{
	uint32_t mask = (1UL << bw) - 1UL;
	uint32_t temp = org_val;
	temp &= ~(mask << bs);
	return  (temp | ((set_val & mask) << bs));
}

static void mipi_dsi_add_lp_to_cmdlist(uint32_t dev_id, uint32_t scene_id,uint32_t cmdlist_id,
	char __iomem *connector_base)
{
	uint32_t dsi_tx_mode_ctrl_lp = inp32(DPU_DSI_CMD_MODE_CTRL_ADDR(connector_base));
	dpu_pr_info("dsi_tx_mode_ctrl_value = %#x", dsi_tx_mode_ctrl_lp);
	dsi_tx_mode_ctrl_lp = mipi_dsi_set_bit_val(dsi_tx_mode_ctrl_lp, 0x7, 3, 8);
	dsi_tx_mode_ctrl_lp = mipi_dsi_set_bit_val(dsi_tx_mode_ctrl_lp, 0x1, 1, 14);
	dsi_tx_mode_ctrl_lp = mipi_dsi_set_bit_val(dsi_tx_mode_ctrl_lp, 0x3, 2, 16);
	dpu_pr_info("dsi_tx_mode_ctrl_lp = %#x", dsi_tx_mode_ctrl_lp);
	ukmd_set_reg(dev_id, scene_id, cmdlist_id, DPU_DSI_CMD_MODE_CTRL_ADDR(connector_base),
		dsi_tx_mode_ctrl_lp);
}

static void mipi_dsi_add_hs_to_cmdlist(uint32_t dev_id, uint32_t scene_id,uint32_t cmdlist_id,
	char __iomem *connector_base)
{
	uint32_t dsi_tx_mode_ctrl_hs = inp32(DPU_DSI_CMD_MODE_CTRL_ADDR(connector_base));
	dpu_pr_info("dsi_tx_mode_ctrl_value = %#x", dsi_tx_mode_ctrl_hs);
	dsi_tx_mode_ctrl_hs = mipi_dsi_set_bit_val(dsi_tx_mode_ctrl_hs, 0x0, 3, 8);
	dsi_tx_mode_ctrl_hs = mipi_dsi_set_bit_val(dsi_tx_mode_ctrl_hs, 0x0, 1, 14);
	dsi_tx_mode_ctrl_hs = mipi_dsi_set_bit_val(dsi_tx_mode_ctrl_hs, 0x0, 2, 16);
	dpu_pr_info("dsi_tx_mode_ctrl_hs = %#x", dsi_tx_mode_ctrl_hs);
	ukmd_set_reg(dev_id, scene_id, cmdlist_id, DPU_DSI_CMD_MODE_CTRL_ADDR(connector_base),
		dsi_tx_mode_ctrl_hs);
}

static uint32_t mipi_dsi_async_tx_cmdlist_delay(struct mipi_dsi_tx_params *params, uint32_t cur_index)
{
	uint32_t delay_us = 0;
	uint32_t cmdlist_last_index = 0;
	uint32_t wait = 0;
	uint32_t waittype = 0;

	if (params->power_mode == MIPI_DSI_POWER_MODE_LP || !(g_debug_dsi_lp_power_mode == 0))
		return ASYNC_LP_DELAY_US;

	cmdlist_last_index = params->write_cmds.cmds_num < cur_index + DSI_HP_FIFO_CMDS_MAX ?
		params->write_cmds.cmds_num - 1 : cur_index + DSI_HP_FIFO_CMDS_MAX - 1;
	wait = params->write_cmds.cmds[cmdlist_last_index].wait;

	waittype = params->write_cmds.cmds[cmdlist_last_index].waittype;
	if (waittype == 0) {
		if (wait < ASYNC_HS_DELAY_MAX_US) {
			delay_us = wait;
		} else {
			dpu_pr_warn("wait %d us is too long, force to %d us", wait, ASYNC_HS_DELAY_MAX_US);
			delay_us = ASYNC_HS_DELAY_MAX_US;
		}
	} else {
		dpu_pr_warn("waittype ms is not support, use default delay");
		/* if delay_us == 0, dacc use default delay 31.25us */
		delay_us = 0;
	}

	dpu_pr_debug("cmdlist_last_index = %u, wait = %u, waittype = %u, delay_us = %u",
		cmdlist_last_index, wait, waittype, delay_us);
	return delay_us;
}

static uint32_t async_tx_real_time(uint32_t delay_us)
{
    uint32_t timer_cnt = 0;
	uint32_t delay_ns = delay_us * 1000;
    timer_cnt = delay_ns / TIME_PER_CYCLE_NS;
    if (delay_ns % TIME_PER_CYCLE_NS > 0 )
        timer_cnt++;

    if (timer_cnt < TIMER_STEP_COUNT_MIN)
        timer_cnt = TIMER_STEP_COUNT_MIN;

    if(timer_cnt > TIMER_STEP_COUNT_MAX)
        timer_cnt = TIMER_STEP_COUNT_MAX;

    return timer_cnt * TIME_PER_CYCLE_NS / 1000;
}

static uint32_t mipi_dsi_calc_hardware_wait_cnt(struct dpu_connector *connector, struct mipi_dsi_tx_params *params, uint32_t cur_index, uint32_t dealy_us)
{
	struct mipi_panel_info *mipi = &connector->post_info[connector->active_idx]->mipi;
	uint32_t count = 0;
	uint64_t lane_byte_clk = (mipi->phy_mode == DPHY_MODE) ? ((uint64_t)2 * mipi->dsi_bit_clk_upt / 8) :
		((uint64_t)mipi->dsi_bit_clk_upt / 7);
	uint32_t cmd_nums = ((cur_index + DSI_HP_FIFO_CMDS_MAX) <= params->write_cmds.cmds_num) ? DSI_HP_FIFO_CMDS_MAX :
		(params->write_cmds.cmds_num % DSI_HP_FIFO_CMDS_MAX);
	uint32_t hardware_time = params->hardware_wait;
	/* ASYNC_CMD_SEND_TIME_US: consider the time that sending async cmds from fifo to dsi */
	uint32_t total_hardware_time = (hardware_time + ASYNC_CMD_SEND_TIME_US) * cmd_nums;
	uint32_t real_dealy_us = async_tx_real_time(dealy_us);
	if (total_hardware_time > real_dealy_us) {
		dpu_pr_warn("mipi hardware time need change, time = %u, cmd_nums = %u", hardware_time, cmd_nums);	
		hardware_time = ((real_dealy_us / cmd_nums) >= ASYNC_CMD_SEND_TIME_US) ?
			((real_dealy_us / cmd_nums) - ASYNC_CMD_SEND_TIME_US) : 0;
		dpu_pr_warn("mipi final hardware time = %u, real_dealy_us = %u", hardware_time, real_dealy_us);
	}

	dpu_pr_debug("lane_byte_clk = %llu M , phy_ctrl->lane_byte_clk = %llu M, hardware_wait = %u, cmd_nums = %u",
			lane_byte_clk, mipi->dsi_bit_clk_upt, hardware_time, cmd_nums);
	count = (uint32_t)((uint64_t)hardware_time * lane_byte_clk);

	return count;
}

static struct mipi_dsi_cmdlist_desc *mipi_dsi_async_tx_apply_cmdlist(struct dpu_connector *connector,
	uint32_t cmdlist_cnt, struct mipi_dsi_tx_params *params, uint32_t cmd_index)
{
	int32_t idx = -1;
	uint32_t delay_us = 0;
	uint32_t cmds_flag = 0;
	uint32_t hardware_wait_cnt = 0;
	struct mipi_dsi_async_ctrl *async_ctrl = &connector->dsi_async_ctrl;
	idx = cmdlist_apply(async_ctrl);
	if (idx == -1)
		return NULL;

	dpu_pr_info("request cmdlist_header = %u, cmds_type = %d",
		async_ctrl->cmdlists[idx].cmdlist_header, params->cmds_type);

	delay_us = mipi_dsi_async_tx_cmdlist_delay(params, cmd_index);
	hardware_wait_cnt = mipi_dsi_calc_hardware_wait_cnt(connector, params, cmd_index, delay_us);
	async_ctrl->cmdlists[idx].cmds_type = params->cmds_type;
	if (params->has_refresh) {
		cmds_flag |= CMDS_HAS_REFRESH;
	}

	if (params->follow_frame) {
		cmds_flag |= CMDS_FOLLOW_FRAME;
	}

	outp32(DPU_ASYNC_TX_REQ_CMDLIST_ID_ADDR(connector->dpu_base, cmdlist_cnt),
		async_ctrl->cmdlists[idx].cmdlist_header);
	outp32(DPU_ASYNC_TX_REQ_CMDLIST_PHY_ADDR(connector->dpu_base, cmdlist_cnt),
		async_ctrl->cmdlists[idx].cmdlist_phy_addr);
	outp32(DPU_ASYNC_TX_REQ_CMDLIST_TYPE_ADDR(connector->dpu_base, cmdlist_cnt),
		params->cmds_type);
	outp32(DPU_ASYNC_TX_REQ_CMDLIST_DELAY_ADDR(connector->dpu_base, cmdlist_cnt),
		delay_us);
	outp32(DPU_ASYNC_TX_REQ_CMDLIST_FLAG_ADDR(connector->dpu_base, cmdlist_cnt),
		cmds_flag);
	outp32(DPU_ASYNC_TX_REQ_CMDS_VALIDATE_TYPE_ADDR(connector->dpu_base, cmdlist_cnt),
		params->validate_repetition);

	mipi_dsi_set_dacc_hardware_cnt(connector, cmdlist_cnt, hardware_wait_cnt);

	dpu_pr_debug("is flw frame %d, has refresh %d, validate repetition %d", params->follow_frame, params->has_refresh,
		params->validate_repetition);
	return &async_ctrl->cmdlists[idx];
}

static void mipi_dsi_async_recycle_consumed_cmdlist(struct dpu_connector *connector, uint32_t cmdlist_cnt)
{
	uint32_t i = 0;
	uint32_t cmdlist_header_id = 0;
	struct mipi_dsi_async_ctrl *async_ctrl = &connector->dsi_async_ctrl;
	for (i = 0; i < cmdlist_cnt; i++) {
		cmdlist_header_id = read_reg_with_retry(DPU_ASYNC_TX_REQ_CMDLIST_ID_ADDR(connector->dpu_base, i));
		cmdlist_recycle(async_ctrl, cmdlist_header_id);
		dpu_pr_info("give back cmdlist_header_id = %u", cmdlist_header_id);
	}
}

static void mipi_dsi_async_tx_add_cmd_to_cmdlist(struct dpu_connector *connector,
	struct mipi_dsi_cmdlist_desc *cmdlist_desc, struct mipi_dsi_tx_params *params, uint32_t cmd_index)
{
	struct mipi_dsi_async_ctrl *async_ctrl = &connector->dsi_async_ctrl;

	if (cmdlist_desc == 0) {
		dpu_pr_err("cmdlist_desc is null");
		return;
	}

	if ((cmd_index == 0) && g_debug_dsi_lp_power_mode) {
		dpu_pr_info("MIPI_DSI_POWER_MODE_LP index 0");
		mipi_dsi_add_lp_to_cmdlist(async_ctrl->cmdlist_dev_id, async_ctrl->cmdlist_scene_id,
			cmdlist_desc->cmdlist_payload, connector->connector_base);
	}

	mipi_dsi_cmd_add_to_cmdlist(async_ctrl->cmdlist_dev_id, async_ctrl->cmdlist_scene_id,
		cmdlist_desc->cmdlist_payload, g_connector_offset[connector->connector_id], &params->write_cmds.cmds[cmd_index]);
}

static void mipi_dsi_async_tx_print_debug_info(struct dpu_connector *connector)
{
	dpu_pr_debug("init_ok=%d, start_cnt=%d, received_cnt=%d, timer_cnt=%d, task_done=%d, sent_task=%d",
		inp32(DPU_ASYNC_TX_INIT_OK_ADDR(connector->dpu_base)),
		inp32(DPU_ASYNC_TX_START_CNT_ADDR(connector->dpu_base)),
		inp32(DPU_ASYNC_TX_RECEIVED_TASK_CNT_ADDR(connector->dpu_base)),
		inp32(DPU_ASYNC_TX_TIMER_CNT_ADDR(connector->dpu_base)),
		inp32(DPU_ASYNC_TX_TASK_DONE_CNT_ADDR(connector->dpu_base)),
		inp32(DPU_ASYNC_TX_SENT_TASK_CNT_ADDR(connector->dpu_base)));
}

static int32_t mipi_dsi_async_tx_send_tasks_set_reg(struct dpu_connector *connector, uint32_t cmdlist_cnt)
{
	struct composer *comp = container_of(connector->conn_info->base.comp_obj_info, struct composer, base);
	if (!comp) {
		dpu_pr_err("comp is nullptr!");
		return MIPI_E_NULL_PTR;
	}

	outp32(DPU_ASYNC_TX_REQ_SCENE_ADDR(connector->dpu_base), comp->index);
	outp32(DPU_ASYNC_TX_REQ_CMDLIST_NUM_ADDR(connector->dpu_base), cmdlist_cnt);
	outp32(DPU_ASYNC_TX_OPS_TYPE_ADDR(connector->dpu_base), TX_SEND_TASKS);
	outp32(DPU_ASYNC_TX_OPS_STATUS_ADDR(connector->dpu_base), TX_OPS_REQ);

	/* trigger riscv interrupt, bit 5 used by async tx, unmasked by ltpo */
	outp32(DPU_RISCV_INTR_TRIG_ADDR(connector->dpu_base), 0x20);
	return 0;
}

static void mipi_dsi_async_tx_set_validate_repetition(uint32_t cmdlist_num, struct mipi_dsi_tx_params *params)
{
	if (cmdlist_num == 1 && (params->cmds_type == DKMD_CMDS_TYPE_BL || params->cmds_type == DKMD_CMDS_TYPE_SFR)) {
		params->validate_repetition = CMDS_ORDER_FIRST;
	} else {
		params->validate_repetition = CMDS_NO_VALIDATE;
	}
}

static uint32_t mipi_dsi_async_force_recycle_handle(struct mipi_dsi_async_ctrl *async_ctrl)
{
	uint32_t i = 0;
	uint32_t recycle_cmdlist_nums = 0;

	for (i = 0; i < MIPI_ASYNC_TX_MAX_NUM; ++i) {
		if (is_cmdlist_applied_long_time(&async_ctrl->cmdlists[i])) {
			++recycle_cmdlist_nums;
			cmdlist_recycle(async_ctrl, async_ctrl->cmdlists[i].cmdlist_header);
			dpu_pr_warn("force recycle cmdlist_header_id = %u", async_ctrl->cmdlists[i].cmdlist_header);
		}
	}

	return recycle_cmdlist_nums;
}

static int32_t mipi_dsi_async_tx_pre_handle(struct dpu_connector *connector, struct mipi_dsi_tx_params *params)
{
	uint32_t cmdlist_num = 0;
	uint32_t total_cmdlist_num = 0;
	uint32_t recycle_cmdlist_nums = 0;
	struct mipi_dsi_async_ctrl *async_ctrl = &connector->dsi_async_ctrl;
	if (async_ctrl->async_status != ASYNC_STARTED) {
		dpu_pr_info("async tx stoped");
		return MIPI_E_ASYNC_STOPED;
	}

	cmdlist_num = calc_cmdlist_num(params->write_cmds.cmds_num);
	mipi_dsi_async_tx_set_validate_repetition(cmdlist_num, params);
	total_cmdlist_num = async_ctrl->applied_cmdlist_num + cmdlist_num;

	if (total_cmdlist_num > MIPI_ASYNC_TX_MAX_NUM) {
		dpu_pr_warn("cmdlist fifo is full, cur applied cmdlist nums=%u, need cmdlist nums=%u",
			async_ctrl->applied_cmdlist_num, cmdlist_num);
		recycle_cmdlist_nums = mipi_dsi_async_force_recycle_handle(async_ctrl);
		if ((total_cmdlist_num - recycle_cmdlist_nums) > MIPI_ASYNC_TX_MAX_NUM) {
			dpu_pr_err("cur command requeires too many cmdlist, recycled cmdlist nums =%u", recycle_cmdlist_nums);
			return MIPI_E_TOO_MANY_TXS;
		}
	}
	return 0;
}

static int32_t mipi_dsi_async_tx_send_tasks(struct dpu_connector *connector, uint32_t cmdlist_cnt)
{
	int32_t ret = 0;
	mipi_dsi_async_tx_print_debug_info(connector);
	ret = mipi_dsi_async_tx_send_tasks_set_reg(connector, cmdlist_cnt);
	if (ret != 0)
		return ret;

	return wait_for_dmcu_ops_done(connector);
}

int32_t mipi_dsi_async_tx(struct dpu_connector *connector, struct mipi_dsi_tx_params *params)
{
	uint32_t i = 0;
	uint32_t cmdlist_cnt = 0;
	int32_t ret = 0;
	struct mipi_dsi_cmdlist_desc *cmdlist_desc = NULL;

	ret = mipi_dsi_async_tx_pre_handle(connector, params);
	if (ret != 0)
		return ret;

	for (i = 0; i < params->write_cmds.cmds_num; i++) {
		if ((i % DSI_HP_FIFO_CMDS_MAX) == 0) {
			cmdlist_desc = mipi_dsi_async_tx_apply_cmdlist(connector, cmdlist_cnt, params, i);
			if (cmdlist_desc == NULL) {
				dpu_pr_err("cmdlist_desc is null");
				mipi_dsi_async_recycle_consumed_cmdlist(connector, cmdlist_cnt);
				return MIPI_E_NULL_PTR;
			}
			cmdlist_cnt++;
		}
		mipi_dsi_async_tx_add_cmd_to_cmdlist(connector, cmdlist_desc, params, i);
	}

	return mipi_dsi_async_tx_send_tasks(connector, cmdlist_cnt);
}

void mipi_dsi_async_tx_done(struct dpu_connector *connector)
{
	uint32_t i;
	uint32_t cmdlist_header_id;
	uint32_t rel_cmdlist_num = 0;
	uint32_t flw_task_left_num = 0;
	struct mipi_dsi_async_ctrl *async_ctrl = &connector->dsi_async_ctrl;
	uint32_t tx_status = 0;

	rel_cmdlist_num = read_reg_with_retry(DPU_ASYNC_TX_REL_CMDLIST_NUM_ADDR(connector->dpu_base));
	if (rel_cmdlist_num > MIPI_ASYNC_TX_MAX_NUM) {
		dpu_pr_err("rel_cmdlist_num > MIPI_ASYNC_TX_MAX_NUM");
		return;
	}

	mipi_dsi_async_tx_print_debug_info(connector);

	tx_status = read_reg_with_retry(DPU_ASYNC_TX_ERROR_STATUS(connector->dpu_base));
	if (tx_status != 0)
		dpu_pr_info("tx_status: %u", tx_status);

	flw_task_left_num = read_reg_with_retry(DPU_ASYNC_TX_FLW_TASK_LEFT_NUM_ADDR(connector->dpu_base));
	if (flw_task_left_num > 0)
		dpu_pr_warn("flw task left num: %d", flw_task_left_num);

	for (i = 0; i < rel_cmdlist_num; i++) {
		cmdlist_header_id = read_reg_with_retry(DPU_ASYNC_TX_REL_CMDLIST_ID_ADDR(connector->dpu_base, i));
		cmdlist_recycle(async_ctrl, cmdlist_header_id);
		dpu_pr_info("give back cmdlist_header_id = %u", cmdlist_header_id);
	}
}

static void dual_mipi_dsi_async_tx_add_cmd_to_cmdlist(struct dpu_connector *connector0,
	struct dpu_connector *connector1, struct mipi_dsi_cmdlist_desc *cmdlist_desc,
	struct mipi_dsi_tx_params *dsi0_params, struct mipi_dsi_tx_params *dsi1_params,
	uint32_t cmd_index)
{
	struct mipi_dsi_async_ctrl *async_ctrl = NULL;
	if (!connector0 || !connector1 || !cmdlist_desc || !dsi0_params || !dsi1_params) {
		dpu_pr_err("connector, cmdlist_desc or params is null");
		return;
	}

	async_ctrl = &connector0->dsi_async_ctrl;
	if ((cmd_index == 0) && g_debug_dsi_lp_power_mode) {
		dpu_pr_info("MIPI_DSI_POWER_MODE_LP index 0");
		mipi_dsi_add_lp_to_cmdlist(async_ctrl->cmdlist_dev_id, async_ctrl->cmdlist_scene_id,
			cmdlist_desc->cmdlist_payload, connector0->connector_base);
		mipi_dsi_add_lp_to_cmdlist(async_ctrl->cmdlist_dev_id, async_ctrl->cmdlist_scene_id,
			cmdlist_desc->cmdlist_payload, connector1->connector_base);
	}

	mipi_dsi_cmd_add_to_cmdlist(async_ctrl->cmdlist_dev_id, async_ctrl->cmdlist_scene_id,
		cmdlist_desc->cmdlist_payload, g_connector_offset[connector0->connector_id],
		&dsi0_params->write_cmds.cmds[cmd_index]);
	mipi_dsi_cmd_add_to_cmdlist(async_ctrl->cmdlist_dev_id, async_ctrl->cmdlist_scene_id,
		cmdlist_desc->cmdlist_payload, g_connector_offset[connector1->connector_id],
		&dsi1_params->write_cmds.cmds[cmd_index]);
}

int32_t dual_mipi_dsi_async_tx(struct dpu_connector *connector0, struct dpu_connector *connector1,
	struct mipi_dsi_tx_params *dsi0_params, struct mipi_dsi_tx_params *dsi1_params)
{
	uint32_t i = 0;
	uint32_t cmdlist_cnt = 0;
	int32_t ret = 0;
	struct mipi_dsi_cmdlist_desc *cmdlist_desc = NULL;

	ret = mipi_dsi_async_tx_pre_handle(connector0, dsi0_params);
	if (ret != 0)
		return ret;

	for (i = 0; i < dsi0_params->write_cmds.cmds_num; i++) {
		if ((i % DSI_HP_FIFO_CMDS_MAX) == 0) {
			cmdlist_desc = mipi_dsi_async_tx_apply_cmdlist(connector0, cmdlist_cnt, dsi0_params, i);
			if (cmdlist_desc == NULL) {
				dpu_pr_err("cmdlist_desc is null");
				mipi_dsi_async_recycle_consumed_cmdlist(connector0, cmdlist_cnt);
				return MIPI_E_NULL_PTR;
			}
			cmdlist_cnt++;
		}
		dual_mipi_dsi_async_tx_add_cmd_to_cmdlist(connector0, connector1, cmdlist_desc, dsi0_params, dsi1_params, i);
	}

	return mipi_dsi_async_tx_send_tasks(connector0, cmdlist_cnt);
}
