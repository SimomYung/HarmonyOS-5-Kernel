/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef DPTX_DP_PSR_CFG_H
#define DPTX_DP_PSR_CFG_H

#include "dp_ctrl.h"
#include "dpu_comp_mgr.h"

extern int g_dp_debug_mode_enable;

#define FRAME_START_TIME 200
#define MAX_PSR_SETUP_TIME 330
#define PSR_SETUP_TIME_STEP 55
#define MIN_PSR_HEIGHT_GRANULARITY 1
#define MIN_PSR2_SCAN_LINE_CAPTURE_TIME 150
#define ALPM_PHY_PWD_DELAY_TIME 0x17
#define ENTRY_ALPM_TIME 0x18
#define EXIT_ALPM_TIME 0x18
#define INVALID_PSR_CAP 0

#define ALPM_P0 0x0
#define ALPM_P1 0x1
#define ALPM_P2 0x2

#ifdef CHIP_TYPE_CSV2
#define FW_3A_EXIT_TIME 300
#else
#define FW_3A_EXIT_TIME 120
#endif

#define ALPM_EXIT_LINE_MAX_NUM 63
#define ALPM_EXIT_MAX_TIME 150

enum dptx_psr_status {
	PSR_EXIT = 0,
	PSR_ENTRY,
	PSR_UPDATE,
	PSR2_SU_UPDATE,
	PSR_ACTIVE,
	PSR_INACTIVE
};

enum dptx_psr_frame_handle {
	GET_FRAME_IDX = 0,
	ACC_FRAME_IDX,
	RESET_FRAME_IDX
};

enum dptx_psr2_event {
	PSR2_ENTRY = 0,
	PSR2_UNDERFLOW
};

struct psr2_su_region {
	uint32_t top_x;
	uint32_t top_y;
	uint32_t bottom_x;
	uint32_t bottom_y;
};

void dptx_psr_intr_en(struct dp_ctrl *dptx);
int dptx_get_psr_cap(struct dp_ctrl *dptx);
int dptx_psr_initial(struct dp_ctrl *dptx);
int dptx_psr2_initial(struct dp_ctrl *dptx);
int dptx_psr_entry(struct dp_ctrl *dptx);
int dptx_change_psr_status(struct dp_ctrl *dptx, uint32_t psr_status);
void dptx_change_psr2_status(struct dp_ctrl *dptx, uint8_t psr2_status);
int dptx_psr_ml_config(struct dp_ctrl *dptx, int status);
// dptx_send_psr2_event call kobject_uevent_env, the function is a native linux interface
// kobject_uevent_env uses mutex lock, need to be careful when using it
void dptx_send_psr2_event(struct dp_ctrl *dptx, uint32_t event, uint8_t event_type);
bool dptx_psr_get_status_flag(struct dp_ctrl *dptx, uint8_t psr_status);
void dptx_psr_set_status_flag(struct dp_ctrl *dptx, uint8_t psr_status, bool psr_status_flag);
void dptx_psr_update_handler(struct dp_ctrl *dptx);
void dptx_psr_params_reset(struct dp_ctrl *dptx);
void dptx_psr_set_update_status(struct dp_ctrl *dptx, bool need_update);
int dptx_psr_entry_listener_init(struct ukmd_isr *isr_ctrl, struct dpu_composer *dpu_comp, uint32_t listening_bit);
bool dptx_psr2_backlight_change(struct dp_ctrl *dptx);
void dptx_psr2_update_backlight(struct dp_ctrl *dptx);
void dptx_psr_update_listener_init(struct ukmd_isr *isr_ctrl, struct dpu_composer *dpu_comp, uint32_t listening_bit);
void dptx_alpm_irq_thread_setup(struct dp_ctrl *dptx);
int dptx_psr_handle_frame_idx(struct dp_ctrl *dptx, uint8_t op_type);
void dptx_psr2_read_su_region(struct dp_ctrl *dptx, struct psr2_su_region *su_val);
void dptx_psr2_params_init(struct dp_ctrl *dptx);
void dptx_psr2_uevent_upload(struct work_struct *work);

#endif