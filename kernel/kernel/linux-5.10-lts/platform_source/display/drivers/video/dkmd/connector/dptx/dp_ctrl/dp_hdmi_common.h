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
#ifndef __DP_HDMI_COMMON_H__
#define __DP_HDMI_COMMON_H__

#include <linux/types.h>
#include "scene/dpu_comp_scene.h"

#define MAX_DP_DPC_COUNT 3
#define MAX_DP_POST_CHN_COUNT 4
#define MAX_DP_STREAM_COUNT 4
#define MIN_SCENE_ID 0
#define MAX_SCENE_ID 3
#define MIN_DP_HDMITX_POST_CHN_INDEX 2
#define MAX_DP_HDMITX_POST_CHN_INDEX 5

int dptx_hdmitx_init_dpc_num(int index);
int dptx_hdmitx_deinit_dpc_num(int index);

int dptx_hdmitx_init_post_chn_num(int index);
int dptx_hdmitx_deinit_post_chn_num(int index);
 
void dptx_hdmitx_config_stream_timing_set_sysctrl(char __iomem *hsdt1_sysctrl_base, uint32_t offset,
	uint32_t value_index, uint32_t value);
int dptx_hdmi_clear_sceneid(struct composer_scene* scene, struct dkmd_connector_info *pinfo, int post_chn);

void dp_hdmi_link_add(bool is_add);
bool is_dp_hdmi_link_over(void);
void set_dp_hdmi_link_num(int max_num);
void dp_hdmi_mutex_init(void);
int get_dp_hdmi_link_num(void);

int dptx_hdmitx_get_panel_id(bool is_primary_panel);
#endif