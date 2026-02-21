/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
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

#ifndef DPU_COMP_MULTI_PRESENT_H
#define DPU_COMP_MULTI_PRESENT_H
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/ktime.h>

enum PRESENT_CLIENT_TYPE {
    PRESENT_CLIENT_MAIN = 0,
    PRESENT_CLIENT_TUNNEL = 1,
    PRESENT_CLIENT_SELF_REFRESH = 2,
    PRESENT_CLIENT_TYPE_MAX
};

struct present_client {
    bool has_present_request;
    bool need_execute_present;
};

struct dpu_multi_present_ctrl;

struct multi_present_ops {
	void (*init_param)(struct dpu_multi_present_ctrl *multi_present_ctrl);
	void (*present_request_set)(struct dpu_multi_present_ctrl *multi_present_ctrl, int32_t client_type);
	void (*set_sfu2vactive_param)(struct dpu_multi_present_ctrl *multi_present_ctrl);
	bool (*need_config_sfu)(struct dpu_multi_present_ctrl *multi_present_ctrl);
	void (*clear_sfu2vactive)(struct dpu_comp_dfr_ctrl *dfr_ctrl);
	bool (*is_safe_period)(struct dpu_comp_dfr_ctrl *dfr_ctrl);
};

struct dpu_multi_present_ctrl {
	ktime_t last_vsync_timestamp;
	bool is_sfu2vactive_period;
    struct present_client clients[PRESENT_CLIENT_TYPE_MAX];
    struct multi_present_ops *ops;
};

void dpu_multi_present_init(struct dpu_multi_present_ctrl *multi_present_ctrl, uint32_t dfr_mode);
void dpu_multi_present_register_ops_dfr_by_acpu(struct dpu_multi_present_ctrl *multi_present_ctrl);
void dpu_multi_present_init_param(struct dpu_multi_present_ctrl *multi_present_ctrl);
void dpu_multi_present_request_set(struct dpu_multi_present_ctrl *multi_present_ctrl, int32_t client_type);
void dpu_multi_present_set_sfu2vactive_param(struct dpu_multi_present_ctrl *multi_present_ctrl);
bool dpu_multi_present_need_config_sfu(struct dpu_multi_present_ctrl *multi_present_ctrl);

#endif
