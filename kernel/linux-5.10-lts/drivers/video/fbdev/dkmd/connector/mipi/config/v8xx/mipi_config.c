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
#include "mipi_config_utils.h"
#include "dkmd_log.h"
#include "dkmd_object.h"
#include <dpu/soc_dpu_define.h>
#include "dpu_connector.h"

void mipi_dsi_reset_init(struct dpu_connector *connector)
{
	dpu_check_and_no_retval(!connector, err, "connector is NULL!");

	set_reg(DPU_DSI_GLB_DSI_RESET_EN_ADDR(connector->dpu_base + DPU_MIPI_DSI_GLB_OFFSET),
		1, 1, (uint8_t)get_connector_phy_id(connector->connector_id));
}

void mipi_dsi_reset_deinit(struct dpu_connector *connector)
{
	void_unused(connector);
}

void set_phy_ref_clk_ctl_en(struct dpu_connector *connector)
{
	void_unused(connector);
}

void set_phy_ref_clk_ctl_disable(struct dpu_connector *connector)
{
	void_unused(connector);
}

void mipi_dsi_mem_init(char __iomem *mipi_dsi_base)
{
	/* dsi_mem_ctrl bit[28:27] TRA = 0x01 */
	set_reg(DPU_DSI_MEM_CTRL_ADDR(mipi_dsi_base), 0x1, 2, 27);
}

/* TE_CTRL_3 is used by dacc */
void mipi_dsi_te_ctrl3_init(char __iomem *mipi_dsi_base, uint32_t te_pin_val, uint64_t lane_byte_clk)
{
	set_reg(DPU_DSI_TE_CTRL_3_ADDR(mipi_dsi_base), (0x1 << 17) | (te_pin_val << 6) | 0x1, 18, 0);

	set_reg(DPU_DSI_TE_HS_NUM_3_ADDR(mipi_dsi_base), 0x0, 32, 0);
	set_reg(DPU_DSI_TE_HS_WD_3_ADDR(mipi_dsi_base), 0x24024, 32, 0);

	set_reg(DPU_DSI_TE_VS_WD_3_ADDR(mipi_dsi_base), (uint32_t)((0x3FC << 12) | (2 * lane_byte_clk / 1000000)), 32, 0);
}

bool mipi_dsi_is_bypass_by_pg(uint32_t connector_id)
{
	void_unused(connector_id);
	return false;
}

void mipi_dsi_enable_hs_pkt_discontin_ctrl(char __iomem *mipi_dsi_base)
{
	void_unused(mipi_dsi_base);
}

void mipi_dsi_dfr_print_debug_info(struct dpu_connector *connector)
{
	uint32_t curr_frm_rate = 0;
	uint32_t te_rate = 0;
	uint32_t te_msk_num = 0;
	uint32_t init_config = 0;
	uint32_t dfr_enable = 0;

	uint32_t dimming_state = 0;
	uint32_t dimming_te_cnt = 0;
	uint32_t target_safe_frm_rate = 0;
	uint32_t current_safe_frm_rate = 0;
	uint32_t dimming_real_frm_rate = 0;
	uint32_t dimming_cur_frm_rate = 0;
	uint32_t dimming_cur_repeat_cnt = 0;

	uint32_t dimming_state_h = 0;
	uint32_t dimming_te_cnt_h = 0;
	uint32_t dimming_real_frm_rate_h = 0;
	uint32_t dimming_cur_frm_rate_h = 0;
	uint32_t dimming_cur_repeat_cnt_h = 0;

	uint32_t panel_refresh_mode = 0;
	uint32_t capture_isr_count = 0;
	uint32_t te_isr_count = 0;
	uint32_t timer_count = 0;
	uint32_t te_count_tmp = 0;
	uint32_t self_refresh_count_tmp = 0;
	uint32_t is_vsync_te = 0;
	uint32_t send_frm_flag = 0;
	uint32_t dimming_status = 0;
	uint32_t need_self_refresh_flag = false;
	uint32_t self_refresh_count = 0;
	uint32_t te_count = 0;
	uint32_t te_type = 0;
	uint32_t async_has_refresh = 0;
	uint32_t async_gen_task_num = 0;
	uint32_t async_flw_task_num = 0;

	static ktime_t pre_te0_timestamp = 0;
	static ktime_t cur_te0_timestamp = 0;

	if (unlikely(!connector)) {
		dpu_pr_err("connectoris null\n");
		return;
	}

	pre_te0_timestamp = cur_te0_timestamp;
	cur_te0_timestamp = ktime_get();
	dpu_pr_info("get te itr notify, connector_id is %d, te0 start diff=%lld, cur_te0_timestamp = %lld",
		connector->connector_id, ktime_us_delta(cur_te0_timestamp, pre_te0_timestamp), cur_te0_timestamp);
	if (g_debug_ltpo_by_mcu == 0)
		return;
	curr_frm_rate = inp32(DFR_DACC_CURR_FRM_RATE(connector->dpu_base));
	te_rate = inp32(DFR_DACC_TE_RATE(connector->dpu_base));
	te_msk_num = inp32(DFR_DACC_TE_MSK_NUM(connector->dpu_base));
	init_config = inp32(DFR_DACC_INIT_CONFIG(connector->dpu_base));
	dfr_enable = inp32(DFR_DACC_DFR_ENABLE(connector->dpu_base));

	dimming_state = inp32(DFR_DACC_FRM_DIMMING_STATE(connector->dpu_base));
	dimming_te_cnt = inp32(DFR_DACC_FRM_DIMMING_TE_CNT(connector->dpu_base));
	target_safe_frm_rate = inp32(DFR_DACC_TARGET_SAFE_FRM_RATE(connector->dpu_base));
	current_safe_frm_rate = inp32(DFR_DACC_CURRENT_SAFE_FRM_RATE(connector->dpu_base));
	dimming_real_frm_rate = inp32(DFR_DACC_FRM_DIMMING_REAL_FRM_RATE(connector->dpu_base));
	dimming_cur_frm_rate = inp32(DFR_DACC_FRM_DIMMING_CUR_FRM_RATE(connector->dpu_base));
	dimming_cur_repeat_cnt = inp32(DFR_DACC_FRM_DIMMING_CUR_REPEAT_CNT(connector->dpu_base));

	dimming_state_h = inp32(DFR_DACC_FRM_DIMMING_STATE_H(connector->dpu_base));
	dimming_te_cnt_h = inp32(DFR_DACC_FRM_DIMMING_TE_CNT_H(connector->dpu_base));
	dimming_real_frm_rate_h = inp32(DFR_DACC_FRM_DIMMING_REAL_FRM_RATE_H(connector->dpu_base));
	dimming_cur_frm_rate_h = inp32(DFR_DACC_FRM_DIMMING_CUR_FRM_RATE_H(connector->dpu_base));
	dimming_cur_repeat_cnt_h = inp32(DFR_DACC_FRM_DIMMING_CUR_REPEAT_CNT_H(connector->dpu_base));

	panel_refresh_mode = inp32(DFR_DACC_PANEL_REFRESH_MODE(connector->dpu_base));
	capture_isr_count = inp32(DFR_DACC_CAPTURE_ISR_COUNT(connector->dpu_base));
	te_isr_count = inp32(DFR_DACC_TE_ISR_COUNT(connector->dpu_base));
	timer_count = inp32(DFR_DACC_TIMER_COUNT(connector->dpu_base));
	te_count_tmp = inp32(DFR_DACC_TE_COUNT_TMP(connector->dpu_base));
	self_refresh_count_tmp = inp32(DFR_DACC_SELF_REFRESH_COUNT_TMP(connector->dpu_base));
	is_vsync_te = inp32(DFR_DACC_IS_VSYNC_TE(connector->dpu_base));
	send_frm_flag = inp32(DFR_DACC_SEND_FRM_FLAG(connector->dpu_base));
	dimming_status = inp32(DFR_DACC_DIMMING_STATUS(connector->dpu_base));
	need_self_refresh_flag = inp32(DFR_DACC_NEED_SELF_REFRESH_FLAG(connector->dpu_base));
	self_refresh_count = inp32(DFR_DACC_SELF_REFRESH_COUNT(connector->dpu_base));
	te_count = inp32(DFR_DACC_TE_COUNT(connector->dpu_base));
	te_type = inp32(DFR_DACC_TE_TYPE(connector->dpu_base));
	async_has_refresh = inp32(DFR_DACC_ASYNC_HAS_REFRESH(connector->dpu_base));
	async_gen_task_num = inp32(DFR_DACC_ASYNC_TX_GEN_TASK_NUM(connector->dpu_base));
	async_flw_task_num = inp32(DFR_DACC_ASYNC_TX_FLW_TASK_NUM(connector->dpu_base));

	dpu_pr_debug("curr_frm_rate:%u, te_rate:%u, te_msk_num:%u, init_config:%u, dfr_enable:%u,"
		"target_safe_frm_rate:%u,current_safe_frm_rate:%u",
		curr_frm_rate, te_rate, te_msk_num, init_config, dfr_enable, target_safe_frm_rate,
		current_safe_frm_rate);

	dpu_pr_debug("capture_isr_count:%u, te_isr_count:%u, timer_count:%u, te_count_tmp:%u,"
		"self_refresh_count_tmp:%u, is_vsync_te:%u, send_frm_flag:%u, need_self_refresh_flag:%u,"
		"self_refresh_count:%u, te_count:%u, te_type:%u, refresh_mode:%u,"
		"async_has_refresh:%u, async_gen_task_num:%u, async_flw_task_num:%u",
		capture_isr_count, te_isr_count, timer_count, te_count_tmp, self_refresh_count_tmp, is_vsync_te,
		send_frm_flag, need_self_refresh_flag, self_refresh_count, te_count, te_type, panel_refresh_mode,
		async_has_refresh, async_gen_task_num, async_flw_task_num);

	dpu_pr_debug("frm_dimming: ops=%u, state=%u, te_cnt=%u, real_frm_rate=%u, cur_frm_rate=%u, dimming_repeat_cnt=%u",
		dimming_status, dimming_state, dimming_te_cnt, dimming_real_frm_rate, dimming_cur_frm_rate, dimming_cur_repeat_cnt);

	dpu_pr_debug("frm_dimming_h: ops=%u, state=%u, te_cnt_=%u, real_frm_rate=%u, cur_frm_rate=%u, dimming_repeat_cnt=%u",
		dimming_status, dimming_state_h, dimming_te_cnt_h, \
		dimming_real_frm_rate_h, dimming_cur_frm_rate_h, dimming_cur_repeat_cnt_h);
}

void mipi_config_div_set(char __iomem *mipi_dsi_base, uint32_t post_div, uint32_t pre_div)
{
	void_unused(mipi_dsi_base);
	void_unused(post_div);
	void_unused(pre_div);
}

void mipi_dsi_partial_update_set_display_region(struct dpu_connector *connector)
{
	struct dkmd_rect dirty_rect = connector->dirty_rect;
	struct mipi_dsi_cmds_info dsi_cmds_info = { 0 };
	struct dkmd_connector_info *pinfo = connector->conn_info;
	struct dfr_info *dinfo = dkmd_get_dfr_info(pinfo);
	if (unlikely(!dinfo)) {
		dpu_pr_warn("dfr_info is nullptr");
		return;
	}

	if (is_dual_mipi_panel(&connector->conn_info->base)) {
		dirty_rect.x /= 2;
		dirty_rect.w /= 2;
	}

	if (dinfo->dfr_mode != DFR_MODE_TE_SKIP_BY_MCU && dinfo->dfr_mode != DFR_MODE_LONGH_TE_SKIP_BY_MCU) {
		pipeline_next_ops_handle(connector->conn_info->conn_device, connector->conn_info,
			LCD_SET_DISPLAY_REGION, &dirty_rect);
	} else {
		dsi_cmds_info.cmds_info.cmds_type = DKMD_CMDS_TYPE_PT_RECT;
		dsi_cmds_info.cmds_info.data_len = 4;
		dsi_cmds_info.cmds_info.data[0] = dirty_rect.x;
		dsi_cmds_info.cmds_info.data[1] = dirty_rect.y;
		dsi_cmds_info.cmds_info.data[2] = (int32_t)dirty_rect.w;
		dsi_cmds_info.cmds_info.data[3] = (int32_t)dirty_rect.h;
		dsi_cmds_info.follow_frame = true;
		dsi_cmds_info.is_force_sync = false;

		pipeline_next_ops_handle(connector->conn_info->conn_device, connector->conn_info,
			SEND_CMDS, &dsi_cmds_info);
	}
	dpu_pr_debug("set display region[%d %d %u %u]", dirty_rect.x, dirty_rect.y, dirty_rect.w, dirty_rect.h);
}
