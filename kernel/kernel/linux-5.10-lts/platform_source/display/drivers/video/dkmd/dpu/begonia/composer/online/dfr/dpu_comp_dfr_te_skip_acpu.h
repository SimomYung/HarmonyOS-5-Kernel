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

#ifndef DUP_COMP_DFR_TE_SKIP_ACPU_H
#define DUP_COMP_DFR_TE_SKIP_ACPU_H

#include "dpu_comp_dfr.h"
#include "dpu_comp_dfr_acpu_dimming.h"
#include <linux/semaphore.h>

struct dfr_te_skip_acpu_ctrl {
	uint32_t te_cnt;
	ktime_t vsync_timestamp;
	ktime_t te_isr_timestamp;
	bool inited;
	bool frame_update;
	bool first_frame;
	bool valid_vsync;
	bool routine_enabled;
	bool backlight_with_frame;
	bool fast_frm_rate_rise;
	bool safe_frm_rate_update;
	bool resend_safe_frm_rate;
	bool is_sampling_config_period;

	struct dfr_dimming_ctrl dimming_ctrl;

	struct semaphore dfr_acpu_sem;
};

void dfr_te_skip_acpu_setup_priv_data(struct dpu_comp_dfr_ctrl *dfr_ctrl);
void dfr_te_skip_acpu_release_priv_data(struct dpu_comp_dfr_ctrl *dfr_ctrl);
int32_t dfr_te_skip_acpu_switch_frame_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl, uint32_t frame_rate);
int32_t dfr_te_skip_acpu_commit(struct dpu_comp_dfr_ctrl *dfr_ctrl);
int32_t dfr_te_skip_acpu_set_safe_frm_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl, uint32_t safe_frm_rate);
int32_t dfr_te_skip_acpu_send_safe_frm_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl, uint32_t safe_frm_rate);
void dfr_te_skip_acpu_exit_idle_status(struct dpu_comp_dfr_ctrl *dfr_ctrl);
void dfr_te_skip_acpu_timeout_print(struct dpu_composer *dpu_comp, struct comp_online_present *present);

int32_t dfr_te_skip_acpu_set_backlight_with_frame(struct dpu_comp_dfr_ctrl *dfr_ctrl, struct disp_effect_params *effect_params);
void dfr_te_skip_release_irq(struct dpu_comp_dfr_ctrl *dfr_ctrl);
int32_t dfr_te_skip_setup_irq(struct dpu_comp_dfr_ctrl *dfr_ctrl);
void dfr_te_skip_acpu_restore_dvfs_level(struct dpu_comp_dfr_ctrl *dfr_ctrl);
void dfr_te_skip_acpu_set_dvfs_level_on(struct dpu_comp_dfr_ctrl *dfr_ctrl);
int32_t dfr_te_skip_acpu_waiting_tunnel_timing(struct dpu_composer *dpu_comp, struct comp_online_present *present);
bool dfr_te_skip_can_set_tunnel_reg(struct dpu_composer *dpu_comp);
void dfr_te_skip_acpu_register_ops(struct dpu_comp_dfr_ctrl *dfr_ctrl);
#endif
