/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
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

#include <linux/mutex.h>
#include "dkmd_object.h"
#include "dp_hdmi_common.h"
#include "dkmd_log.h"

#define MIN_DP_POST_CH_INDEX 2
#define MAX_DP_POST_CH_INDEX 5
#define DP_POST_CH_FIVE_OFFSET 0x0100

static bool init_mutex_flag = false;
int32_t g_dp_dpc_status[MAX_DP_DPC_COUNT] = {-1, -1, -1};
int32_t g_dp_post_chn_status[MAX_DP_POST_CHN_COUNT] = {-1, -1, -1, -1};
int g_dp_link_num_max = 3;
int g_dp_link_num = 0;
struct mutex g_dp_link_mutex;
struct mutex g_dp_post_chn_mutex;
struct mutex g_dp_dpc_mutex;
struct mutex g_dp_reg_mutex;
void dp_hdmi_mutex_init(void)
{
	if (!init_mutex_flag) {
		dpu_pr_info("dp_hdmi_mutex_init enter");
		mutex_init(&g_dp_link_mutex);
		mutex_init(&g_dp_post_chn_mutex);
		mutex_init(&g_dp_dpc_mutex);
		mutex_init(&g_dp_reg_mutex);
		init_mutex_flag = true;
	}
}

int dptx_hdmitx_init_dpc_num(int index)
{
	int i;

	if (index < 0) {
		dpu_pr_err("index %d is valid!", index);
		return -ENXIO;
	}
	mutex_lock(&g_dp_dpc_mutex);
	for (i = 0; i < MAX_DP_DPC_COUNT; i++) {
		if (g_dp_dpc_status[i] < 0) {
			g_dp_dpc_status[i] = index;
			dpu_pr_info("get dpc %d for index %d", i, index);
			mutex_unlock(&g_dp_dpc_mutex);
			return i;
		} else {
			dpu_pr_info("dpc %d is used by index %d", i, g_dp_dpc_status[i]);
		}
	}

	dpu_pr_err("not find dpc for index %d", index);
	mutex_unlock(&g_dp_dpc_mutex);
	return -ENXIO;
}
 
int dptx_hdmitx_deinit_dpc_num(int index)
{
	int i;

	if (index < 0) {
		dpu_pr_err("index %d is valid!", index);
		return -ENXIO;
	}
	mutex_lock(&g_dp_dpc_mutex);
	for (i = 0; i < MAX_DP_DPC_COUNT; i++) {
		if (g_dp_dpc_status[i] == index) {
			g_dp_dpc_status[i] = -1;
			dpu_pr_info("[DP] remove dpc %d for index %d", i, index);
			mutex_unlock(&g_dp_dpc_mutex);
			return 0;
		}
	}

	dpu_pr_warn("[DP] not find dpc for index %d", index);
	mutex_unlock(&g_dp_dpc_mutex);
	return 0;
}

int dptx_hdmitx_init_post_chn_num(int index)
{
	int i;

	if (index < 0) {
		dpu_pr_err("index %d is valid!", index);
		return -ENXIO;
	}
	mutex_lock(&g_dp_post_chn_mutex);
	for (i = 0; i < MAX_DP_POST_CHN_COUNT; i++) {
		if (g_dp_post_chn_status[i] < 0) {
			g_dp_post_chn_status[i] = index;
			dpu_pr_info("get post_chn %d for index %d", i, index);
			mutex_unlock(&g_dp_post_chn_mutex);
			return i + 2;
		} else {
			dpu_pr_info("post_chn %d is used by index %d", i, g_dp_post_chn_status[i]);
		}
	}

	dpu_pr_err("not find dpc for index %d", index);
	mutex_unlock(&g_dp_post_chn_mutex);
	return -ENXIO;
}

int dptx_hdmitx_deinit_post_chn_num(int index)
{
	int i;

	if (index < 0) {
		dpu_pr_err("index %d is valid!", index);
		return -ENXIO;
	}
	mutex_lock(&g_dp_post_chn_mutex);
	for (i = 0; i < MAX_DP_POST_CHN_COUNT; i++) {
		if (g_dp_post_chn_status[i] == index) {
			g_dp_post_chn_status[i] = -1;
			dpu_pr_info("[DP] remove post_chn %d for index %d", i, index);
			mutex_unlock(&g_dp_post_chn_mutex);
			return 0;
		}
	}

	dpu_pr_warn("[DP] not find post_chn for index %d", index);
	mutex_unlock(&g_dp_post_chn_mutex);
	return 0;
}

void dptx_hdmitx_config_stream_timing_set_sysctrl(char __iomem *hsdt1_sysctrl_base, uint32_t offset,
	uint32_t value_index, uint32_t value)
{
	int reg_arr[MAX_DP_STREAM_COUNT] = {-1, -1, -1, -1};
	uint32_t i = 0;
	uint32_t mask = 0xF, flag = 0, reg_value = 0;

	mutex_lock(&g_dp_reg_mutex);
	reg_value = readl(hsdt1_sysctrl_base + offset);
	dpu_pr_debug("[DP] hsdt1_sysctrl_base reg_value is %#x", reg_value);
	
	for (i = 0; i < MAX_DP_STREAM_COUNT; i++) {
		reg_arr[i] = (int)((reg_value >> (i * 4)) & mask);
		if (reg_arr[i] == (int)value && i != value_index) {
			dpu_pr_warn("hsdt1_sysctrl_base reg %u index value is same to %u", i, value);
			flag = flag | BIT(i);
		}
	}
	if (flag == 0) {
		mutex_unlock(&g_dp_reg_mutex);
		return;
	}
 
	dpu_pr_debug("hsdt1_sysctrl_base reg flag %#x", flag);
	for (i = 0; i < MAX_DP_STREAM_COUNT; i++) {
		if (flag & BIT(i)) {
			set_reg(hsdt1_sysctrl_base + offset, 0xF, 4, (uint8_t)(i * 4));
		}
	}
	mutex_unlock(&g_dp_reg_mutex);
}

int dptx_hdmi_clear_sceneid(struct composer_scene* scene, struct dkmd_connector_info *pinfo, int post_chn)
{
	uint32_t primary_post_ch_offset = (uint32_t)post_chn * 0x1C;
	if (post_chn < MIN_DP_POST_CH_INDEX || post_chn > MAX_DP_POST_CH_INDEX) {
		dpu_pr_err("[DP] post_chn: %d is invalid", post_chn);
		return -EINVAL;
	}

	if (composer_active_vsync(pinfo, true)) {
		dpu_pr_err("active vsync failed");
		return -1;
	}

	if (post_chn == MAX_DP_POST_CH_INDEX)
		primary_post_ch_offset = DP_POST_CH_FIVE_OFFSET;
	dpu_pr_info("[DP] dpu_dptx_handle_plugout_chandle_sceneid primary_post_ch_offset is %#x", primary_post_ch_offset);

	set_reg(DPU_PIPE_SW_SIG_CTRL_0_ADDR(scene->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, 0x0, 32, 0);
	set_reg(DPU_PIPE_SW_SW_POS_CTRL_SIG_EN_0_ADDR(scene->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, 0x0, 32, 0);
	set_reg(DPU_PIPE_SW_DAT_CTRL_0_ADDR(scene->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, 0x0, 32, 0);
	set_reg(DPU_PIPE_SW_SW_POS_CTRL_DAT_EN_0_ADDR(scene->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, 0x0, 32, 0);
	composer_active_vsync(pinfo, false);

	return 0;
}

void dp_hdmi_link_add(bool is_add)
{
	int add_num = is_add ? 1 : -1;
	mutex_lock(&g_dp_link_mutex);
	g_dp_link_num += add_num;
	dpu_pr_info("[DP] is_add is %d, g_dp_link_num is %d, g_dp_link_num_max is %d\n",
		is_add, g_dp_link_num, g_dp_link_num_max);
	mutex_unlock(&g_dp_link_mutex);
}

bool is_dp_hdmi_link_over(void)
{
	bool res = false;
	mutex_lock(&g_dp_link_mutex);
	dpu_pr_info("[DP] is_dp_link_over g_dp_link_num is %d,  g_dp_link_num_max is %d\n",
		g_dp_link_num, g_dp_link_num_max);
	res = (g_dp_link_num >= g_dp_link_num_max);
	mutex_unlock(&g_dp_link_mutex);
	return res;
}
 
void set_dp_hdmi_link_num(int max_num)
{
	mutex_lock(&g_dp_link_mutex);
	g_dp_link_num_max = max_num;
	mutex_unlock(&g_dp_link_mutex);
}