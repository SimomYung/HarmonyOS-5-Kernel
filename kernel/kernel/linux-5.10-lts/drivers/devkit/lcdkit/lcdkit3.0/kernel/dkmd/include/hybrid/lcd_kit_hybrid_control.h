/* SPDX-License-Identifier: GPL-2.0 */
/*
 * lcd_kit_hybrid_control.h
 *
 * head file for hybrid control
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __LCD_KIT_HYBRID_CONTROL_H_
#define __LCD_KIT_HYBRID_CONTROL_H_

#define STATUS_IDLE 0xdc
#define STATUS_NORMAL 0x9c
#define STATUS_POWERON 0x08
#define STATUS_NOPOWER 0x00

#include "dpu_comp_mgr.h"
#include "fb/dpu_fb.h"
#include "lcd_kit_hybrid_parse.h"
#include <linux/wait.h>
#include <linux/kthread.h>

struct display_hybrid_ctrl {
	// struct dsi_panel *panel;
	struct dpu_composer *dpu_composer;

	struct lcd_kit_hybrid_info hybrid_info;
	/* lock for hybrid mode */
	struct mutex hybrid_lock;
	struct hybrid_sw_ops *display_sw_ops;

	bool skip_frame_commit;
	/* flag for AP aod and set normal to MCU */
	int current_state;

	/* factory RD state*/
	bool sta_state;
	/* screen on str als state*/
	bool als_done;
	/* hybrid work thread */
	struct kthread_worker hybrid_worker;
	struct task_struct *hybrid_worker_task;
	wait_queue_head_t resp_wait;

	spinlock_t comp_on_spin;
	bool composer_on_ongoing;

	bool skip_panel_onoff;
	bool hybrid_control_switching;

	/* quick on */
	int gesture_quickon_seq;
	spinlock_t gesture_quickon_spin;
	struct semaphore gesture_qon_sema;

	/* SR sema */
	struct semaphore sr_wake_sema;

	/* record real func for composer */
	int32_t (*composer_on)(struct composer *comp, uint8_t on_mode);
	int32_t (*composer_off)(struct composer *comp, uint8_t off_mode);
	int32_t (*composer_present)(struct composer *comp, void *frame);
};

int hybrid_on_switch_mcu(struct display_hybrid_ctrl *display_ctrl);
int hybrid_idle_switch_mcu(struct display_hybrid_ctrl *display_ctrl);
int hybrid_off_switch_mcu(struct display_hybrid_ctrl *display_ctrl);

int hybrid_on_switch_ap(struct display_hybrid_ctrl *display_ctrl);
int hybrid_off_switch_ap(struct display_hybrid_ctrl *display_ctrl);
int hybrid_idle_switch_ap(struct display_hybrid_ctrl *display_ctrl);

int lcd_kit_set_normal(struct display_hybrid_ctrl *display_ctrl);
int hybrid_str_switch_mcu(struct display_hybrid_ctrl *display_ctrl);

int hybrid_lcd_status_check(struct display_hybrid_ctrl *display_ctrl);
int hybrid_get_lcd_status(struct display_hybrid_ctrl *display_ctrl, u8 *status);
int hybrid_te_off(struct display_hybrid_ctrl *display_ctrl);
int hybrid_te_on(struct display_hybrid_ctrl *display_ctrl);

void hybrid_dss_on(struct display_hybrid_ctrl *display_ctrl, uint8_t on_mode);
void hybrid_dss_off(struct display_hybrid_ctrl *display_ctrl);

bool hybrid_check_power_on(struct display_hybrid_ctrl *display_ctrl);

void hybrid_set_block_brightness_flag(bool flag);
bool hybrid_get_block_brightness_flag(struct display_hybrid_ctrl *display_ctrl);

void set_current_gesture_status(u8 status);
u8 get_current_gesture_status(void);

void lcd_kit_hybrid_register_inner_cmd(void);
#endif
