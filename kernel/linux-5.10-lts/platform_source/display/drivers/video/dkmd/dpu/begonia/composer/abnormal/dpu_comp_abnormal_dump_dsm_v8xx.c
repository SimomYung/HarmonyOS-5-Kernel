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

#include <linux/delay.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include <dpu/dpu_dm.h>
#include <dpu/soc_dpu_define.h>
#include "dpu_comp_abnormal_handle.h"
#include "dksm_dmd.h"
#include "dksm_debug.h" 
#include "dpu_comp_mgr.h"
#include "dpu_conn_mgr.h"
#include "dpu_comp_abnormal_handle.h"
#include "dksm_debug.h"
#include "dpu_comp_config_utils.h"
#include "dpu_debug_dump.h"

#define DM_SCENE_INFO_OFFSET      0x50
#define DM_HDR_OFFSET      0x70
#define DM_DPP_OFFSET      0x100
#define DM_DSC_OFFSET      0x130
#define DM_SCL0_OFFSET      0xB0
#define DM_SCL4_OFFSET      0x100
#define EACH_SCL_REG_SIZE   0x10
#define ITF_CH_NUM 4
#define DM_LAYER_OFFSET 0x180
#define EACH_LAYER_REG_OFFSET 0x60
#define EACH_LAYER_REG_SIZE 0x50

// range example:[DM_SCENE_INFO_OFFSET, DM_UVUP_OFFSET)
struct dpu_dsm_dm_dump dsm_dm_dump_range[] = {
	{DM_SCENE_INFO_OFFSET, DM_HDR_OFFSET},
	{DM_DPP_OFFSET, DM_DSC_OFFSET},
	{DM_SCL0_OFFSET, DM_SCL4_OFFSET},
};

uint32_t dpu_comp_get_dsm_dm_dump_cnt(void)
{
	return ARRAY_SIZE(dsm_dm_dump_range);
}

static void dpu_comp_abnormal_dsm_dump_reg_layer(uint32_t *dm_addr, uint32_t start_layer_index,
		uint32_t layer_cnt, uint32_t ldi_vstate)
{
	uint32_t i = 0;
	uint32_t cnt = 0;
	uint32_t *current_addr = NULL;
	uint32_t layer_index = 0;
	uint32_t inc_step = 4;
	uint32_t val1, val2, val3, val4;

	while((dsm_client_ocuppy(dsm_lcd_client) != 0) && (cnt < DSM_OCCUPY_RETRY_TIMES)) {
		dpu_pr_warn("dsm_client_ocuppy failed, retry %d times", ++cnt);
		usleep_range(500, 600);
	}

	if (cnt == DSM_OCCUPY_RETRY_TIMES) {
		dpu_pr_warn("dsm_client_ocuppy failed");
		return;
	}
	dpu_pr_warn("add new client to last layer %d", layer_cnt - start_layer_index);
	dsm_client_record(dsm_lcd_client, "Lastlayer %x|", layer_cnt - start_layer_index);
	for (layer_index = start_layer_index; layer_index < layer_cnt; ++layer_index) {
		current_addr = dm_addr + (DM_LAYER_OFFSET + layer_index * EACH_LAYER_REG_OFFSET) / 4;
		for (i = (DM_LAYER_OFFSET + layer_index * EACH_LAYER_REG_SIZE) / (inc_step * 4);
				i < (DM_LAYER_OFFSET + (layer_index + 1) * EACH_LAYER_REG_SIZE) / (inc_step * 4); ++i) {
			val1 = inp32(current_addr++);
			val2 = inp32(current_addr++);
			val3 = inp32(current_addr++);
			val4 = inp32(current_addr++);
			dsm_client_record(dsm_lcd_client, "%x|%x|%x|%x|", val1, val2, val3, val4);
		}
	}
	dsm_client_record(dsm_lcd_client, "%x#", g_dpu_dsm_info.underflow_clear_type);
	dpu_pr_warn("last layer used size = %d", dsm_lcd_client->used_size);
	if (ldi_vstate == LDI_VSTATE_V_WAIT_TE0)
		dsm_client_notify(dsm_lcd_client, DSM_LCD_TE_TIME_OUT_ERROR_NO);
	else {
		if (g_dpu_dsm_info.underflow_clear_type == CLEAR_SUC)
			dsm_client_notify(dsm_lcd_client, DSM_LCD_UNDERRUN_ERROR_NO);
		else
			dsm_client_notify(dsm_lcd_client, DSM_LCD_LDI_UNDERFLOW_NO);
	}
}

void dpu_comp_abnormal_dsm_dump_reg_dm(struct dpu_composer *dpu_comp, uint32_t scene_id)
{
	uint32_t i = 0;
	uint32_t dump_cnt= 0;
	uint32_t layer_index = 0;
	uint32_t inc_step = 4;
	uint32_t val1, val2, val3, val4, ldi_vstate;
	uint32_t *dm_addr = NULL;
	uint32_t *current_addr = NULL;
	uint32_t cnt = 0;
	uint32_t layer_cnt = 0;
	uint32_t scl_layer_cnt = 0;
	uint32_t cld_layer_cnt = 0;
	uint32_t layer_dump_cnt = 0;
	struct rtc_time tm = {0};
	struct timespec64 tv = {0};
	char __iomem *dpu_base = NULL;
	DPU_DM_LAYER_NUMBER_UNION reg_layer_number;
	DPU_DM_DDIC_NUMBER_UNION reg_ddic_number;

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is nullptr");
		return;
	}

	if (unlikely(scene_id >= DPU_SCENE_MAX)) {
		dpu_pr_err("scene_id = %u is out of range", scene_id);
		return;
	}

	dpu_base = dpu_comp->comp_mgr->dpu_base;
	dm_addr = (uint32_t *)(dpu_base + g_dm_tlb_info[scene_id].dm_data_addr);
	reg_layer_number.value = inp32(DPU_DM_LAYER_NUMBER_ADDR(
								(char __iomem *)(dpu_base + g_dm_tlb_info[scene_id].dm_data_addr)));
	reg_ddic_number.value = inp32(DPU_DM_DDIC_NUMBER_ADDR(
								(char __iomem *)(dpu_base + g_dm_tlb_info[scene_id].dm_data_addr)));
	layer_cnt = reg_layer_number.reg.layer_number;
	scl_layer_cnt = reg_ddic_number.reg.scl_number;
	cld_layer_cnt = reg_ddic_number.reg.cld_number;
	if (layer_cnt > DM_LAYER_NUM) {
		dpu_pr_err("layer_cnt %d exceed max", layer_cnt);
		return;
	}
	g_dpu_dsm_info.cld_num = cld_layer_cnt;
	g_dpu_dsm_info.layer_num = layer_cnt;
	dpu_check_and_no_retval(!dsm_lcd_client, info, "dsm_lcd_client is null!");

	while((dsm_client_ocuppy(dsm_lcd_client) != 0) && (cnt < DSM_OCCUPY_RETRY_TIMES)) {
		dpu_pr_warn("dsm_client_ocuppy failed, retry %d times", ++cnt);
		usleep_range(500, 600);
	}

	if (cnt == DSM_OCCUPY_RETRY_TIMES) {
		dpu_pr_warn("dsm_client_ocuppy failed");
		return;
	}
	g_dpu_dsm_info.dsm_client_init = 1;
	ktime_get_real_ts64(&tv);
	tv.tv_sec -= (long)sys_tz.tz_minuteswest * 60;
	rtc_time64_to_tm(tv.tv_sec, &tm);

	ldi_vstate = inp32(DPU_DSI_VSTATE_ADDR(dpu_base));
	dsm_client_record(dsm_lcd_client, "dpu %04d%02d%02d%02d%02d%02d%03d|%d|%d|", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
					tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_nsec / 1000000,ldi_vstate, scene_id);

	dpu_comp_abnormal_dsm_dump_dvfs(dpu_comp);

	dsm_client_record(dsm_lcd_client, "%x|", layer_cnt);

	for (dump_cnt = 0; dump_cnt < dpu_comp_get_dsm_dm_dump_cnt(); ++dump_cnt) {
		uint32_t reg_start = dsm_dm_dump_range[dump_cnt].start_addr / (inc_step * 4);
		uint32_t reg_end = dsm_dm_dump_range[dump_cnt].end_addr / (inc_step * 4);
		if ((dump_cnt + 1) == dpu_comp_get_dsm_dm_dump_cnt()) {
			if (scl_layer_cnt == 0)
				break;
			else
				reg_end = (dsm_dm_dump_range[dump_cnt].start_addr + EACH_SCL_REG_SIZE * scl_layer_cnt) / (inc_step * 4); 
		}

		current_addr = dm_addr + dsm_dm_dump_range[dump_cnt].start_addr / 4;
		for (i = reg_start; i < reg_end; ++i) {
			val1 = inp32(current_addr++);
			val2 = inp32(current_addr++);
			val3 = inp32(current_addr++);
			val4 = inp32(current_addr++);
			dsm_client_record(dsm_lcd_client, "%x|%x|%x|%x|", val1, val2, val3, val4);
		}
	}

	dsm_client_record(dsm_lcd_client, "%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|%x|",
		g_dpu_dsm_info.phy_status, g_dpu_dsm_info.dsi_indata,g_dpu_dsm_info.dsi_vactive_num,
		g_dpu_dsm_info.dsi_vactive_num_act, g_dpu_dsm_info.isr_status, g_dpu_dsm_info.itf_indata,
		g_dpu_dsm_info.ov_cnt[0], g_dpu_dsm_info.ov_cnt[1], g_dpu_dsm_info.ov_cnt[2],
		g_dpu_dsm_info.ov_cnt[3], g_dpu_dsm_info.hebc_err, g_dpu_dsm_info.cmdlist_status,
		g_dpu_dsm_info.info_src, g_dpu_dsm_info.dacc_pc);
	if (layer_cnt <= MAX_DUMP_LAYER_CNT) {
		layer_dump_cnt = layer_cnt;
	} else if ((layer_cnt - cld_layer_cnt) <= MAX_DUMP_LAYER_CNT) {
		layer_dump_cnt = layer_cnt - cld_layer_cnt;
	} else {
		layer_dump_cnt = MAX_DUMP_LAYER_CNT;
	}

	for (layer_index = 0; layer_index < layer_dump_cnt; ++layer_index) {
		current_addr = dm_addr + (DM_LAYER_OFFSET + layer_index * EACH_LAYER_REG_OFFSET) / 4;
		for (i = (DM_LAYER_OFFSET + layer_index * EACH_LAYER_REG_SIZE) / (inc_step * 4);
				i < (DM_LAYER_OFFSET + (layer_index + 1) * EACH_LAYER_REG_SIZE) / (inc_step * 4); ++i) {
			val1 = inp32(current_addr++);
			val2 = inp32(current_addr++);
			val3 = inp32(current_addr++);
			val4 = inp32(current_addr++);
			dsm_client_record(dsm_lcd_client, "%x|%x|%x|%x|", val1, val2, val3, val4);
		}
	}
}

void dpu_comp_dsm_notify(struct dpu_composer *dpu_comp, uint32_t scene_id)
{
	uint32_t *dm_addr = NULL;
	char __iomem *dpu_base = NULL;

	if (g_dpu_dsm_info.dsm_client_init == 0 || g_debug_underflow_dump_enable == 0) {
		dpu_pr_err("dsm_lcd_client not create or dump disable");
		return;
	}
	dsm_client_record(dsm_lcd_client, "%x|", dpu_comp->self_healing_ctrl.vote);
	dsm_client_record(dsm_lcd_client, "%x#", g_dpu_dsm_info.underflow_clear_type);
	dpu_pr_warn("dsm_lcd_client used size = %d", dsm_lcd_client->used_size);
	dpu_base = dpu_comp->comp_mgr->dpu_base;
	dm_addr = (uint32_t *)(dpu_base + g_dm_tlb_info[scene_id].dm_data_addr);
	if (g_dpu_dsm_info.dsi_vstate == LDI_VSTATE_V_WAIT_TE0)
		dsm_client_notify(dsm_lcd_client, DSM_LCD_TE_TIME_OUT_ERROR_NO);
	else {
		if (g_dpu_dsm_info.underflow_clear_type == CLEAR_SUC)
			dsm_client_notify(dsm_lcd_client, DSM_LCD_UNDERRUN_ERROR_NO);
		else
			dsm_client_notify(dsm_lcd_client, DSM_LCD_LDI_UNDERFLOW_NO);
	}

	if ((g_dpu_dsm_info.layer_num - g_dpu_dsm_info.cld_num) > MAX_DUMP_LAYER_CNT)
		dpu_comp_abnormal_dsm_dump_reg_layer(dm_addr, MAX_DUMP_LAYER_CNT, g_dpu_dsm_info.layer_num, g_dpu_dsm_info.dsi_vstate);
}