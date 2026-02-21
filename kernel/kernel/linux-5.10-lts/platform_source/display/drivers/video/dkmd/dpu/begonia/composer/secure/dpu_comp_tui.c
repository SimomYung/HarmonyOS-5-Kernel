/* Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "dpu_comp_mgr.h"
#include "dpu_conn_mgr.h"
#include "tui.h"
#if defined(CONFIG_TEE_INDICATOR)
#include "indicator_agent.h"
#endif
#include "dpu_comp_tui.h"
#include "dpu_isr.h"
#include "gfxdev_mgr.h"
#include "dpu_comp_vsync.h"
#include "dkmd_tui_interface.h"
#include "dvfs.h"
#include "dkmd_connector.h"

#define UEVENT_BUF_LEN 120
#define TUI_POLL_CFG_FAIL 1
#define TUI_POLL_CFG_OK   0
#define TUI_MSG_VAL 0

struct dpu_composer *g_active_dpu_comp = NULL;
struct dpu_secure_info g_secure_info; // send to secure_os

static void dpu_tui_send_msg(uint32_t mode, int type)
{
	if (mode == TUI_LEVEL_0)
		send_tui_msg_config(type, TUI_MSG_VAL, "DSS");
#if defined(CONFIG_TEE_INDICATOR)
	else if (mode == TUI_LEVEL_1)
		send_secure_display_msg_config(type);
#endif
}

static void dpu_notify_tzdriver_power_off(uint32_t mode, bool is_normal)
{
	if (mode == TUI_LEVEL_0) {
		tui_poweroff_work_start();
	}
#if defined(CONFIG_TEE_INDICATOR)
	else if (mode == TUI_LEVEL_1) {
		dpu_pr_info("send power off to tzdriver, normal=%u", is_normal);
		secure_display_poweroff_work_start(is_normal);
	}
#else
	void_unused(is_normal);
#endif
}
 
static void dpu_notify_tzdriver_power_on(uint32_t mode, bool is_normal)
{
#if defined(CONFIG_TEE_INDICATOR)
	if (mode == TUI_LEVEL_1) {
		dpu_pr_info("send power on to tzdriver, normal=%u", is_normal);
		secure_display_poweron_work_start(is_normal);
	}
#else
	void_unused(mode);
	void_unused(is_normal);
#endif
}

static void dpu_tui_notify_status(struct dpu_composer *dpu_comp, struct dpu_secure *secure_ctrl)
{
	if ((secure_ctrl->secure_status == TUI_SEC_RUNNING) &&
		(secure_ctrl->secure_event == TUI_SEC_ENABLE) &&
		(secure_ctrl->tui_need_switch)) {
		if (dpu_comp->secure_ctrl.tui_quit_flag) {
			dpu_tui_send_msg(secure_ctrl->tui_mode, TUI_POLL_CFG_FAIL);
			secure_ctrl->secure_event = TUI_SEC_DISABLE;
			dpu_pr_info("power off, tui_mode[%u] switch to TUI_SEC_RUNNING failed !\n", secure_ctrl->tui_mode);
		} else {
			dpu_tui_send_msg(secure_ctrl->tui_mode, TUI_POLL_CFG_OK);
			dpu_pr_info("tui_mode[%u] switch to TUI_SEC_RUNNING succ !\n", secure_ctrl->tui_mode);
		}
		secure_ctrl->tui_need_switch = false;
	}
}

// return value: 0:event change, -1:event not change, exit tui mode, -2:event not change, enter tui mode
static int dpu_tui_request_update(struct dpu_secure *secure_ctrl, int tui_request)
{
	uint32_t new_event = (tui_request == DPU_SEC_EXIT_ALL) ? TUI_SEC_DISABLE : TUI_SEC_ENABLE;

	// new event change
	if (new_event != secure_ctrl->secure_event) {
		// tui quit delay several frames to enable intra dvfs
		if (new_event == TUI_SEC_DISABLE && secure_ctrl->secure_event == TUI_SEC_ENABLE)
			dpu_dvfs_set_inter_vote_index_count(TUI_QUIT_INTER_DVFS_CNT);

		secure_ctrl->secure_event = new_event;
		secure_ctrl->tui_need_switch = true;
		return 0;
	}

	dpu_pr_info("tui_mode[%u] event = %u is not changed!\n", secure_ctrl->tui_mode, secure_ctrl->secure_event);

	// new event not change, exit tui mode
	if (new_event != TUI_SEC_ENABLE)
		return -1;

	// new event not change, enter tui mode
	if (secure_ctrl->tui_mode == TUI_LEVEL_0)
		dpu_tui_send_msg(secure_ctrl->tui_mode , TUI_POLL_CFG_FAIL);

	return -1;
}

void dpu_tui_switch_event(struct composer *comp, int state)
{
	char *envp[2];
	char state_buf[UEVENT_BUF_LEN];
	int ret;

	ret = snprintf(state_buf, sizeof(state_buf), "TUI_EVENT=%d\n", state);
	dpu_pr_debug("state_buf init return value is=%d\n", ret);

	envp[0] = state_buf;
	envp[1] = NULL;

	if (!comp) {
		dpu_pr_err("comp is NULL Pointer\n");
		return;
	}

	kobject_uevent_env(&(comp->base.peri_device->dev.kobj), KOBJ_CHANGE, envp);
	dpu_pr_info("tui state need switch to %d!", state);
}

static int dpu_tui_check_panel_power_status(struct dpu_composer *dpu_comp,
	struct dpu_secure *secure_ctrl, int secure)
{
	if (!dpu_comp->comp.power_on) {
		if (secure) {
			dpu_tui_send_msg(secure_ctrl->tui_mode, TUI_POLL_CFG_FAIL);
			secure_ctrl->secure_event = TUI_SEC_DISABLE;
		}
		return -1;
	}
	return 0;
}

static int dpu_tui_check_secure_created(struct dpu_secure *secure_ctrl, int tui_request)
{
	if (!secure_ctrl->secure_created) {
		if (tui_request != DPU_SEC_EXIT_ALL) {
			dpu_tui_send_msg(secure_ctrl->tui_mode, TUI_POLL_CFG_FAIL);
			secure_ctrl->secure_event = TUI_SEC_DISABLE;
		}
		return -1;
	}
	return 0;
}

static bool is_current_tui_request_supported(struct dpu_secure *secure_ctrl, int tui_request)
{
	if ((tui_request == DPU_SEC_ENTER_LEVEL0) && ((g_debug_tui_level_disable & BIT(0)) != 0)) {
		dpu_pr_warn("tui level0 not support!\n");
		dpu_tui_send_msg(TUI_LEVEL_0, TUI_POLL_CFG_FAIL);
		return false;
	}

	if ((tui_request == DPU_SEC_ENTER_LEVEL1) && ((g_debug_tui_level_disable & BIT(1)) != 0)) {
		dpu_pr_warn("tui level1 not support!\n");
		dpu_tui_send_msg(TUI_LEVEL_1, TUI_POLL_CFG_FAIL);
		return false;
	}

	return true;
}

/* tui_request: 
 * 0-DPU_SEC_EXIT_ALL, 1-DPU_SEC_ENTER_LEVEL0, 2-DPU_SEC_ENTER_LEVEL1, 255-DPU_SEC_RESTART_ACK
 */
static int notify_dpu_tui_request(void *data, int tui_request)
{
	int ret;
	struct dpu_secure *secure_ctrl = NULL;
	struct dpu_composer *dpu_comp = NULL;
	struct composer *comp = NULL;
	bool blank_sem_status = false;

	dpu_comp = g_active_dpu_comp;
	dpu_check_and_return(!dpu_comp, -EINVAL, err, "dpu_comp is NULL\n");
	secure_ctrl = &dpu_comp->secure_ctrl;
	comp = &dpu_comp->comp;

	dpu_pr_info("tzdriver indicator agent notify request, tui_request = %d, tui_quit_flag = %d \n",
		tui_request, secure_ctrl->tui_quit_flag);

	if (!is_current_tui_request_supported(secure_ctrl, tui_request))
		return -1;

	if ((!is_builtin_panel(&dpu_comp->comp.base)) && (!is_primary_panel(&dpu_comp->comp.base))) {
		dpu_pr_warn("comp->index=%d not support TUI!\n", comp->index);
		return -1;
	}

	if (tui_request == DPU_SEC_RESTART_ACK) {
		dpu_pr_info("tui_mode[%u]: get restart power off ack, secure_status = %u, secure_event = %u\n",
			secure_ctrl->tui_mode, secure_ctrl->secure_status, secure_ctrl->secure_event);
		secure_ctrl->tui_restart_ack_flag = false;
		return 0;
	}

	// if tui_request is idle, keep current tui_mode, otherwise update tui_mode
	if (tui_request != DPU_SEC_EXIT_ALL) {
		secure_ctrl->tui_mode = (uint32_t)tui_request;
		dpu_pr_info("update to tui_mode[%u]", secure_ctrl->tui_mode);
	}

	if (dpu_tui_check_secure_created(secure_ctrl, tui_request) < 0) {
		dpu_pr_warn("secure is not created yet!\n");
		return -1;
	}

	if (!secure_ctrl->tui_quit_flag && !secure_ctrl->tui_restart_ack_flag) {
		down(&comp->blank_sem);
		dpu_print_sem_count(&comp->blank_sem, true);
		blank_sem_status = true;
	}

	if (dpu_tui_check_panel_power_status(dpu_comp, secure_ctrl, tui_request) < 0) {
		dpu_pr_warn("panel is power off!\n");
		ret = -1;
		goto err_out;
	}

	dpu_pr_info("tui_mode[%u]: secure_status = %u, secure_event = %u, tui_request = %d +++\n",
		secure_ctrl->tui_mode, secure_ctrl->secure_status, secure_ctrl->secure_event, tui_request);

	ret = dpu_tui_request_update(secure_ctrl, tui_request);
	if (ret) {
		ret = 0;
		goto err_out;
	}

	if (secure_ctrl->secure_event == TUI_SEC_ENABLE) {
		dpu_comp_active_vsync(dpu_comp);
	} else {
		dpu_comp_deactive_vsync(dpu_comp);
		secure_ctrl->secure_status = TUI_SEC_IDLE;
	}
	dpu_tui_switch_event(&dpu_comp->comp, tui_request);
	dpu_pr_info("tui_mode[%u]: secure_status = %d, secure_event = %d, tui_request = %d ---\n",
		secure_ctrl->tui_mode, secure_ctrl->secure_status, secure_ctrl->secure_event, tui_request);

err_out:
	if (blank_sem_status) {
		dpu_print_sem_count(&comp->blank_sem, false);
		up(&comp->blank_sem);
	}

	secure_ctrl->tui_quit_flag = false;

	return ret;
}

static void dpu_tui_irq_handler_work(struct kthread_work *work)
{
	struct dpu_secure *secure_ctrl = NULL;
	struct dpu_composer *dpu_comp = NULL;

	secure_ctrl = container_of(work, struct dpu_secure, secure_ctrl_work);
	dpu_check_and_no_retval(!secure_ctrl, err, "secure_ctrl is NULL!\n");

	dpu_comp = secure_ctrl->dpu_comp;
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL!\n");

	if (!dpu_comp->comp.power_on) {
		dpu_pr_debug("panel is power off!");
		return; /* panel off; */
	}

	dpu_pr_debug("tui_mode[%u]: secure_status = %u, secure_event = %u, ready_status = %u, +++\n",
		secure_ctrl->tui_mode, secure_ctrl->secure_status, secure_ctrl->secure_event, secure_ctrl->tui_ready_status);

	if ((secure_ctrl->secure_status == TUI_SEC_RUNNING) &&
		(secure_ctrl->secure_event == TUI_SEC_DISABLE)) {
			dpu_comp_deactive_vsync(dpu_comp);
			secure_ctrl->secure_status = TUI_SEC_IDLE;
			return;
	}

	if (secure_ctrl->tui_ready_status != USER_STATE_EXIT_ALL) {
		if (secure_ctrl->secure_event == TUI_SEC_ENABLE &&
			(dpu_tui_check_panel_power_status(dpu_comp, secure_ctrl, TUI_SEC_ENABLE) < 0)) {
			dpu_pr_info("panel is power off!\n");
			return;
		}

		if ((secure_ctrl->secure_status == TUI_SEC_IDLE) && (secure_ctrl->secure_event == TUI_SEC_ENABLE))
			secure_ctrl->secure_status = TUI_SEC_RUNNING;

		dpu_tui_notify_status(dpu_comp, secure_ctrl);
	}

	dpu_pr_debug("tui_mode[%u]: secure_status = %u, secure_event = %u---\n",
		secure_ctrl->tui_mode, secure_ctrl->secure_status, secure_ctrl->secure_event);
}

static int32_t dpu_tui_irq_handle(struct notifier_block *self, unsigned long action, void *data)
{
	struct ukmd_listener_data *listener_data = (struct ukmd_listener_data *)data;
	struct dpu_composer *dpu_comp = (struct dpu_composer *)(listener_data->data);
	struct dpu_secure *secure_ctrl = NULL;

	dpu_tui_request_frame_update(dpu_comp);

	secure_ctrl = &dpu_comp->secure_ctrl;
	if (secure_ctrl->secure_status != TUI_SEC_IDLE)
		dpu_tui_set_lowpower_status(secure_ctrl->tui_ready_status);

	if (secure_ctrl->secure_status != secure_ctrl->secure_event)
		kthread_queue_work(&dpu_comp->handle_worker, &secure_ctrl->secure_ctrl_work);
	return 0;
}

static struct notifier_block secure_handle_isr_notifier = {
	.notifier_call = dpu_tui_irq_handle,
};

static void dpu_tui_irq_init(struct ukmd_isr *isr_ctrl, struct dpu_composer *dpu_comp, uint32_t listening_bit)
{
	struct dpu_secure *secure_ctrl =  &dpu_comp->secure_ctrl;

	kthread_init_work(&secure_ctrl->secure_ctrl_work, dpu_tui_irq_handler_work);

	ukmd_isr_register_listener(isr_ctrl, &secure_handle_isr_notifier, listening_bit, dpu_comp);
}

static void dpu_tui_irq_deinit(struct ukmd_isr *isr_ctrl, uint32_t listening_bit)
{
	ukmd_isr_unregister_listener(isr_ctrl, &secure_handle_isr_notifier, listening_bit);
}

static bool is_power_restart_in_tui_level1(struct dpu_composer *dpu_comp)
{
	struct dpu_secure *secure_ctrl = NULL;
	struct composer_manager *comp_mgr = NULL;

	dpu_check_and_return(!dpu_comp->comp_mgr, -EINVAL, err, "comp_mgr is NULL\n");
	secure_ctrl = &dpu_comp->secure_ctrl;
 
	dpu_pr_info("tui_mode[%u]: secure_status=%u, secure_event=%u, is_power_restarting=%u",
		secure_ctrl->tui_mode, secure_ctrl->secure_status, secure_ctrl->secure_event, dpu_comp->comp_mgr->is_power_restarting);
	return (dpu_comp->comp_mgr->is_power_restarting &&
			secure_ctrl->tui_mode == TUI_LEVEL_1 && secure_ctrl->secure_status == TUI_SEC_RUNNING);
}
 
static int dpu_tui_notify_restart_power_status(struct dpu_composer *dpu_comp, bool is_power_on)
{
	struct dpu_secure *secure_ctrl = NULL;
	uint32_t timeout_count = 5000;  /* wait 5s */
	uint32_t delay_count = 0;

	secure_ctrl = &dpu_comp->secure_ctrl;
 
	if (is_power_on) {
		dpu_notify_tzdriver_power_on(secure_ctrl->tui_mode, false);
		return 0;
	}
 
	// send abnormal power off status to teeos and wait ack
	secure_ctrl->tui_restart_ack_flag = true;
	dpu_notify_tzdriver_power_off(secure_ctrl->tui_mode, false);
	dpu_pr_info("tui_mode[%u]: wait for abnormal power off ack\n", secure_ctrl->tui_mode);
	while (secure_ctrl->tui_restart_ack_flag && (delay_count < timeout_count)) {
		mdelay(1);
		++delay_count;
	}
	secure_ctrl->tui_restart_ack_flag = false;
	if (delay_count < timeout_count) {
		dpu_pr_info("tui_mode[%u] wait power off ack success, delay_count = %u, secure_status=%u, secure_event=%u!\n",
			secure_ctrl->tui_mode, delay_count, secure_ctrl->secure_status, secure_ctrl->secure_event);
		return 0;		
	} else {
		dpu_pr_err("tui_mode[%u] wait power off ack fail, delay_count = %u, secure_status=%u, secure_event=%u!\n",
			secure_ctrl->tui_mode, delay_count, secure_ctrl->secure_status, secure_ctrl->secure_event);
		return -1;			
	}
}

static int dpu_tui_wait_quit(struct dpu_composer *dpu_comp)
{
	struct dpu_secure *secure_ctrl = NULL;
	uint32_t timeout_count = 5000;  /* wait 5s */
	bool is_timeout = false;
	uint32_t delay_count = 0;
	dpu_check_and_return(!dpu_comp, -EINVAL, err, "dpu_comp is NULL\n");
	secure_ctrl = &dpu_comp->secure_ctrl;

	/* 1. if tui is running, dss should not powerdown,
	 *    because register will be writen in tui mode.
	 * 2. if tui is enable, but not running, then tui should not be ok,
	 *    send the msg that tui config fail.
	 */
	dpu_pr_info("tui_mode[%u]: secure_status=%u, secure_event=%u\n",
		secure_ctrl->tui_mode, secure_ctrl->secure_status, secure_ctrl->secure_event);

	if (secure_ctrl->secure_status == TUI_SEC_RUNNING) {
		secure_ctrl->tui_quit_flag = true;
		dpu_notify_tzdriver_power_off(secure_ctrl->tui_mode, true);
		dpu_pr_info("tui_mode[%u]: wait for quit\n", secure_ctrl->tui_mode);
		while (1) {
			if ((secure_ctrl->secure_status != TUI_SEC_RUNNING) || (delay_count > timeout_count)) {
				is_timeout = (delay_count > timeout_count) ? true : false;
				break;
			}
			mdelay(1);
			++delay_count;
		}
		if (!is_timeout) {
			dpu_pr_info("tui_mode[%u] quite success, delay_count = %u, secure_status=%u, secure_event=%u!\n",
				secure_ctrl->tui_mode, delay_count, secure_ctrl->secure_status, secure_ctrl->secure_event);
		} else {
			dpu_pr_err("tui_mode[%u] quit failed, delay_count = %u, secure_status=%u, secure_event=%u!\n",
				secure_ctrl->tui_mode, delay_count, secure_ctrl->secure_status, secure_ctrl->secure_event);
			return -1;
		}
	} else if (secure_ctrl->secure_event == TUI_SEC_ENABLE) {
		secure_ctrl->secure_event = TUI_SEC_DISABLE;
		dpu_tui_send_msg(secure_ctrl->tui_mode, TUI_POLL_CFG_FAIL);
		dpu_pr_info("In power down, tui_mode[%u] secure event will not be handled\n", secure_ctrl->tui_mode);
	}

	return 0;
}

static bool is_multi_panel_active(struct dpu_composer *dpu_comp)
{
	int32_t i;
	uint32_t power_on_comp_cnt = 0;
	bool is_multi_panel_active;

	if (dpu_comp->conn_info->base.fold_type != PANEL_FLIP)
		return false;

	for (i = DEVICE_COMP_PRIMARY_ID; i <= DEVICE_COMP_BUILTIN_ID; i++) {
		if (dpu_comp->comp_mgr->power_status.refcount.value[i] != 0)
			power_on_comp_cnt++;
	}

	is_multi_panel_active = (power_on_comp_cnt > 1);
	dpu_pr_debug("multi panel display=%d", is_multi_panel_active);
	return is_multi_panel_active;
}

int dpu_tui_power_work(struct dpu_composer *dpu_comp, uint32_t power_type)
{
	dpu_check_and_return(!dpu_comp, -EINVAL, err, "dpu_comp is NULL\n");
 
	if ((!is_builtin_panel(&dpu_comp->comp.base)) && (!is_primary_panel(&dpu_comp->comp.base)))
		return 0;

	if (!is_multi_panel_active(dpu_comp))
		dpu_tui_set_power_mode(power_type);

	if (power_type == DPU_CHANGE_ACT_DISPLAY)
		return dpu_tui_wait_quit(dpu_comp);
 
	if (power_type == DPU_POWER_ON) {
		if (is_power_restart_in_tui_level1(dpu_comp))
			return dpu_tui_notify_restart_power_status(dpu_comp, true);
		else
			return 0;
	}
 
	// power off
	if (is_power_restart_in_tui_level1(dpu_comp))
		return dpu_tui_notify_restart_power_status(dpu_comp, false);
	else
		return dpu_tui_wait_quit(dpu_comp);
}

void dpu_tui_update_ready_status(struct dpu_composer *dpu_comp, uint32_t tui_ready_status)
{
	dpu_pr_debug("cur_tui_status=%u, tui_ready_status=%u\n", dpu_comp->secure_ctrl.tui_ready_status, tui_ready_status);
	dpu_comp->secure_ctrl.tui_ready_status = tui_ready_status;
}

static uint32_t get_tui_connector_type(struct dpu_composer *dpu_comp)
{
	uint32_t dpu_secure_connector_type = 0;
	uint32_t connect_idx = dpu_comp->conn_info->connector_idx[PRIMARY_CONNECT_CHN_IDX];

	switch (connect_idx) {
		case CONNECTOR_ID_DSI0:
		case CONNECTOR_ID_DSI0_BUILTIN:
			dpu_secure_connector_type = CONNNECTOR_DSI0;
			break;
		case CONNECTOR_ID_DSI1:
			dpu_secure_connector_type = CONNNECTOR_DSI1;
			break;
		case CONNECTOR_ID_DSI2:
		case CONNECTOR_ID_DSI2_BUILTIN:
			dpu_secure_connector_type = CONNNECTOR_DSI2;
			break;
		default:
			dpu_pr_warn("dpu_secure_connector_type invalid");
			break;
	}

	if (is_dual_mipi_panel(&dpu_comp->conn_info->base))
		dpu_secure_connector_type |= CONNNECTOR_DUAL_MIPI;

	dpu_pr_debug("secure layer panel type is %#x", dpu_secure_connector_type);
	return dpu_secure_connector_type;
}

void dpu_tui_register(struct dpu_composer *dpu_comp)
{
	ssize_t ret;
	struct dpu_secure *secure_ctrl = NULL;
	struct ukmd_isr *isr_ctrl = NULL;
	struct dpu_secure_info secure_info;

	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL\n");

	if ((!is_builtin_panel(&dpu_comp->comp.base)) && (!is_primary_panel(&dpu_comp->comp.base))) {
		dpu_pr_warn("comp->index=%d not support TUI!\n", dpu_comp->comp.index);
		return;
	}

	secure_info.panel_type = get_tui_connector_type(dpu_comp);
	secure_info.panel_xres = dpu_comp->conn_info->base.xres;
	secure_info.panel_yres = dpu_comp->conn_info->base.yres;
 
	ret = register_tui_driver(notify_dpu_tui_request, "DSS", &secure_info);	
	if (ret != 0) {
		dpu_pr_warn("register_tui_driver failed, ret value is %d\n", ret);
		return;
	}

	dpu_pr_info("+ dpu_comp->comp.index=%d\n", dpu_comp->comp.index);
	secure_ctrl = &dpu_comp->secure_ctrl;
	isr_ctrl = &dpu_comp->isr_ctrl;

	down(&secure_ctrl->tui_sem);
	if (secure_ctrl->secure_created) {
		dpu_pr_info("comp.index=%d secure scenes are already registered\n", dpu_comp->comp.index);
		up(&secure_ctrl->tui_sem);
		return;
	}
	secure_ctrl->secure_created = true;
	up(&secure_ctrl->tui_sem);

	secure_ctrl->tui_mode = TUI_LEVEL_NONE;
	secure_ctrl->secure_status = TUI_SEC_IDLE;
	secure_ctrl->secure_event  = TUI_SEC_DISABLE;
	secure_ctrl->tui_need_switch = false;
	secure_ctrl->tui_quit_flag = false;
	secure_ctrl->tui_restart_ack_flag = false;

	g_secure_info.panel_type = secure_info.panel_type;
	g_secure_info.panel_xres = secure_info.panel_xres;
	g_secure_info.panel_yres = secure_info.panel_yres;

	dpu_pr_info("g_secure_info.panel_type=%u, g_secure_info.panel_xres=%u, g_secure_info.panel_yres=%u\n",
	g_secure_info.panel_type, g_secure_info.panel_xres, g_secure_info.panel_yres);

#if defined(CONFIG_TEE_INDICATOR)
	register_secure_display_driver(notify_dpu_tui_request, &g_secure_info);
#endif

	dpu_tui_irq_init(isr_ctrl, dpu_comp, DSI_INT_VACT0_END);

	secure_ctrl->dpu_comp = dpu_comp;
	g_active_dpu_comp = dpu_comp;
	dpu_pr_info("-\n");
}

static struct composer *get_power_on_comp(struct composer_manager *comp_mgr)
{
	uint32_t index = 0;
	struct composer *comp = NULL;
	for (index = 0; index <= DEVICE_COMP_BUILTIN_ID; index++) {
		if (!comp_mgr->dpu_comps[index])
			continue;
		comp = &(comp_mgr->dpu_comps[index]->comp);
		if (!comp) {
			dpu_pr_warn("fail to get comp %u", comp->index);
			continue;
		}
		if (comp->power_on) {
			dpu_pr_info("comp %u is power on", comp->index);
			return comp;
		}
	}
	return NULL;
}
 
static void dpu_tui_post_process(struct composer_manager *comp_mgr)
{
	struct composer *power_on_comp = NULL;

	dpu_check_and_no_retval(!comp_mgr, err, "comp_mgr is NULL\n");

	if (g_active_dpu_comp != NULL) {
		dpu_pr_err("g_active_dpu_comp of %u is not NULL, skip tui post process",
			g_active_dpu_comp->comp.index);
		return;
	}

	power_on_comp = get_power_on_comp(comp_mgr);
	if (power_on_comp != NULL)
		dpu_tui_register(to_dpu_composer(power_on_comp));
}

void dpu_tui_unregister(struct dpu_composer *dpu_comp)
{
	struct dpu_secure *secure_ctrl = NULL;
	struct ukmd_isr *isr_ctrl = NULL;

	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL\n");

	if ((!is_builtin_panel(&dpu_comp->comp.base)) && (!is_primary_panel(&dpu_comp->comp.base))) {
		dpu_pr_warn("comp->index=%d not support TUI!\n", dpu_comp->comp.index);
		return;
	}

	dpu_pr_info("+ dpu_comp->comp.index=%d\n", dpu_comp->comp.index);
	secure_ctrl = &dpu_comp->secure_ctrl;
	isr_ctrl = &dpu_comp->isr_ctrl;

	down(&secure_ctrl->tui_sem);
	if (!secure_ctrl->secure_created) {
		dpu_pr_info("comp.index=%d scene is already unregistered\n", dpu_comp->comp.index);
		up(&secure_ctrl->tui_sem);
		return;
	}
	secure_ctrl->secure_created = false;
	up(&secure_ctrl->tui_sem);

	dpu_tui_irq_deinit(isr_ctrl, DSI_INT_VACT0_END);
	if (secure_ctrl->secure_status == TUI_SEC_RUNNING) {
		dpu_comp_deactive_vsync(dpu_comp);
		secure_ctrl->secure_status = TUI_SEC_IDLE;
	}

	if (secure_ctrl->secure_event == TUI_SEC_ENABLE) {
		secure_ctrl->secure_event = TUI_SEC_DISABLE;
		dpu_tui_send_msg(secure_ctrl->tui_mode, TUI_POLL_CFG_FAIL);
		dpu_pr_warn("In power down, secure event will not be handled\n");
	}

	unregister_tui_driver("DSS");
#if defined(CONFIG_TEE_INDICATOR)
	unregister_secure_display_driver();
#endif
	g_active_dpu_comp = NULL;
	dpu_tui_post_process(dpu_comp->comp_mgr);
	dpu_pr_info("-\n");
}

bool dpu_tui_is_power_on(void)
{
	dpu_check_and_return(!g_active_dpu_comp, false, warn, "Tui is unregistered");

	return g_active_dpu_comp->comp.power_on;
}

int32_t dpu_tui_get_active_display_rect(struct dkmd_rect *active_rect)
{
	struct dkmd_connector_info *pinfo = NULL;

	dpu_check_and_return(!active_rect, -1, err, "active_rect is null");
	dpu_check_and_return(!g_active_dpu_comp, -1, warn, "Tui is unregistered");

	pinfo = g_active_dpu_comp->conn_info;
	dpu_check_and_return(!pinfo, -1, err, "pinfo is null");

	if (!pinfo->get_display_rect_by_config_id) {
		active_rect->x = 0;
		active_rect->y = 0;
		active_rect->w = pinfo->base.xres;
		active_rect->h = pinfo->base.yres;
		dpu_pr_info("active_rect[x,y,w,h]=[%d, %d, %d, %d]\n", active_rect->x, active_rect->y, active_rect->w, active_rect->h);
		return 0;		
	}

	return pinfo->get_display_rect_by_config_id(pinfo, pinfo->ppc_config_id_active, active_rect);
}

int32_t dpu_tui_get_rotation(void)
{
	struct dkmd_connector_info *pinfo = NULL;

	dpu_check_and_return(!g_active_dpu_comp, -1, warn, "g_active_dpu_comp is null");

	pinfo = g_active_dpu_comp->conn_info;
	dpu_check_and_return(!pinfo, -1, err, "pinfo is null");

	if (is_ppc_support(&pinfo->base))
		return TUI_DISPLAY_ROT_180;

	return TUI_DISPLAY_ROT_0;
}

int32_t dpu_tui_get_panel_info(struct dpu_secure_info *panel_info)
{
	struct dkmd_connector_info *pinfo = NULL;

	dpu_check_and_return(!panel_info, -1, err, "panel_info is null");
	dpu_check_and_return(!g_active_dpu_comp, -1, warn, "Tui is unregistered");

	pinfo = g_active_dpu_comp->conn_info;
	dpu_check_and_return(!pinfo, -1, err, "pinfo is null");
	panel_info->panel_xres = pinfo->base.xres;
	panel_info->panel_yres = pinfo->base.yres;
	panel_info->panel_type = get_tui_connector_type(g_active_dpu_comp);
	dpu_pr_info("panel_type=%u, panel_xres=%u, panel_yres=%u", panel_info->panel_type, panel_info->panel_xres, panel_info->panel_yres);
	return 0;
}

struct dpu_composer *get_active_dpu_comp(void)
{
	return g_active_dpu_comp;
}

bool is_tui_running(struct dpu_composer *dpu_comp, uint32_t sec_mode)
{
	struct dpu_secure *secure_ctrl = NULL;
 
	dpu_check_and_return(!dpu_comp, false, err, "dpu_comp is NULL\n");
	secure_ctrl = &dpu_comp->secure_ctrl;
 
	if (sec_mode == secure_ctrl->tui_mode) {
		if ((secure_ctrl->secure_status == TUI_SEC_RUNNING) ||
			(secure_ctrl->secure_status == TUI_SEC_IDLE && secure_ctrl->secure_event == TUI_SEC_ENABLE))
			return true;
	}
	return false;
}

#ifdef CONFIG_DKMD_DEBUG_ENABLE
static ssize_t dpu_secure_event_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	ssize_t ret;
	struct dpu_composer *dpu_comp = NULL;
	struct dpu_secure *secure_ctrl = NULL;

	dpu_check_and_return((!dev || !buf), -1, err, "input is null pointer");

	dpu_comp = to_dpu_composer(get_comp_from_device(dev));
	if (unlikely(!dpu_comp)) {
		dpu_pr_err("dpu_comp is null\n");
		return -1;
	}

	secure_ctrl = &dpu_comp->secure_ctrl;
	ret = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "TUI_MODE[%u]:SECURE_STATUS=%u, SECURE_EVENT=%u\n",
		secure_ctrl->tui_mode, secure_ctrl->secure_status, secure_ctrl->secure_event);
	if (ret < 0)
		dpu_pr_err("snprintf_s failed, ret value is %d\n", ret);

	buf[strlen(buf) + 1] = '\0';
	return ret;
}

static ssize_t dpu_secure_event_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	ssize_t ret;
	int request;
	struct dpu_composer *dpu_comp = NULL;

	dpu_check_and_return((!dev || !buf), -1, err, "input is null pointer");

	dpu_comp = to_dpu_composer(get_comp_from_device(dev));
	if (unlikely(!dpu_comp)) {
		dpu_pr_err("dpu_comp is null\n");
		return -1;
	}

	ret = sscanf_s(buf, "%d", &request);
	if (ret == 0) {
		dpu_pr_err("get secure_request err");
		return -1;
	}
	notify_dpu_tui_request(NULL, request);
	return count;
}

static DEVICE_ATTR(dpu_secure, 0644, dpu_secure_event_show, dpu_secure_event_store);

void dpu_tui_add_attrs(struct dpu_composer *dpu_comp)
{
	if (unlikely(!dpu_comp)) {
		dpu_pr_err("dpu_comp is null\n");
		return;
	}

	ukmd_sysfs_attrs_append(&dpu_comp->attrs, &dev_attr_dpu_secure.attr);
}
#else
void dpu_tui_add_attrs(struct dpu_composer *dpu_comp)
{
	void_unused(dpu_comp);
}
#endif
