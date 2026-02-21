/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
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

#ifndef COMPOSER_TUI_H
#define COMPOSER_TUI_H

#include <linux/types.h>
#include <linux/kthread.h>
#include <linux/workqueue.h>
#include <linux/of_irq.h>

#define TUI_QUIT_INTER_DVFS_CNT 2

struct dpu_composer;

// single tui_mode status
enum tui_sec_status {
	TUI_SEC_IDLE = 0,
	TUI_SEC_RUNNING,
};

// single tui_mode event
enum tui_sec_event {
	TUI_SEC_DISABLE = 0,
	TUI_SEC_ENABLE,
};

// TUI_LEVEL0: hardware replace fake layer with trusted layer, secure buffer. use as secure_pay
// TUI_LEVLE1: software replace fake layer with secure layer, normal buffer with right control. use as indicator
// match with dpu_sec_event in dkmd_tui_interface.h
enum tui_sec_mode {
    TUI_LEVEL_NONE = 0,
	TUI_LEVEL_0 = 1, // use such as secure_pay
    TUI_LEVEL_1 = 2, // use such as indicator
};

enum tui_partial_update_status {
    ENTER_PARTIAL_UPDATE = 0,
    EXIT_PARTIAL_UPDATE = 1,
};

enum tui_sec_connector_type {
	CONNNECTOR_DSI0 = BIT(0),
	CONNNECTOR_DSI1 = BIT(1),
	CONNNECTOR_DSI2 = BIT(2),
	CONNNECTOR_DUAL_MIPI = BIT(3),
	CONNNECTOR_DP = BIT(4),
};

enum tui_from_tee_event {
    TEE_EVENT_REFRESH = 0,
	TEE_EVENT_START_DISPLAY = 1,
    TEE_EVENT_END_DISPLAY = 2,
};

// TUI_LEVEL0: hardware replace fake layer with trusted layer, secure buffer. use as secure_pay
// TUI_LEVLE1: software replace fake layer with secure layer, normal buffer with right control. use as indicator
// match with dpu_sec_event in dkmd_tui_interface.h
enum tui_to_user_event {
    USER_EVENT_EXIT_ALL = 0,
    USER_EVENT_ENTER_LEVEL0 = 1,  // use such as secure_pay
    USER_EVENT_ENTER_LEVEL1 = 2,  // use such as indicator
    USER_EVENT_REFRESH = 3,
    USER_EVENT_LEVEL1_START_DISP = 4,
    USER_EVENT_LEVEL1_END_DISP = 5
};

enum tui_user_state {
    USER_STATE_EXIT_ALL = 0,
    USER_STATE_LEVEL0_RUN = 1,
    USER_STATE_LEVEL1_IN_DISP = 2,
    USER_STATE_LEVEL1_OUT_DISP = 3,
	USER_STATE_NUM = 4,
};

struct dpu_secure {
	bool secure_created;
	uint32_t tui_mode; // secure_pay, indicator
	bool tui_need_switch;
	bool tui_quit_flag;
	bool tui_restart_ack_flag; // get ack from teeos to confirm restart power off status update in teeos
	uint32_t secure_status;
	uint32_t secure_event;
	uint32_t tui_ready_status;

	struct kthread_work secure_ctrl_work;
	struct dpu_composer *dpu_comp;
	struct semaphore tui_sem;
};

#if defined(CONFIG_TEE_TUI)
void dpu_tui_register(struct dpu_composer *dpu_comp);
void dpu_tui_unregister(struct dpu_composer *dpu_comp);
int dpu_tui_power_work(struct dpu_composer *dpu_comp, uint32_t power_type);
void dpu_tui_update_ready_status(struct dpu_composer *dpu_comp, uint32_t tui_ready_status);
void dpu_tui_frame_start_isr_handler(struct dpu_composer *dpu_comp);
void dpu_tui_add_attrs(struct dpu_composer *dpu_comp);
struct dpu_composer *get_active_dpu_comp(void);
void dpu_tui_switch_event(struct composer *comp, int state);
int32_t dpu_tui_irq_register(struct platform_device *pdev);
bool dpu_tui_no_need_disable_ldi(struct dpu_composer *dpu_comp);
void dpu_tui_request_frame_update(struct dpu_composer *dpu_comp);
bool is_tui_running(struct dpu_composer *dpu_comp, uint32_t sec_mode);
void dpu_tui_set_power_mode(uint32_t power_type);
void dpu_tui_set_lowpower_status(uint32_t user_state);
int32_t dpu_get_tui_level1_layer_info(struct composer *comp, struct tui_level1_layer_info *info);
#else
#define dpu_tui_register(dpu_comp)
#define dpu_tui_unregister(dpu_comp)
#define dpu_tui_update_ready_status(dpu_comp, tui_ready_status)
#define dpu_tui_frame_start_isr_handler(dpu_comp)
#define dpu_tui_add_attrs(dpu_comp)
#define dpu_tui_switch_event(comp, state)
#define dpu_tui_irq_register(pdev) (1)
#define dpu_tui_no_need_disable_ldi(dpu_comp) (true)
#define get_active_dpu_comp(void) (NULL)
#define dpu_tui_request_frame_update(dpu_comp)
#define dpu_tui_set_power_mode(power_type)
#define dpu_tui_set_lowpower_status(user_state)

static inline int32_t dpu_get_tui_level1_layer_info(struct composer *comp, struct tui_level1_layer_info *info)
{
	void_unused(comp);
	void_unused(info);
	return 0;
}

static inline int dpu_tui_power_work(struct dpu_composer *dpu_comp, uint32_t power_type)
{
	void_unused(dpu_comp);
	void_unused(power_type);
	return 0;
}

static inline bool is_tui_running(struct dpu_composer *dpu_comp, uint32_t sec_mode)
{
	void_unused(dpu_comp);
	void_unused(sec_mode);
	return false;
}
#endif

#endif