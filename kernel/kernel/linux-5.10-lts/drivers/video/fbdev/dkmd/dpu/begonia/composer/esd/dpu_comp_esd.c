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

#include <linux/delay.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include "dpu_comp_esd.h"
#include "dkmd_log.h"
#include "dpu_conn_mgr.h"
#include "dpu_comp_mgr.h"
#include "dpu_connector.h"
#include "dpu_comp_bl.h"
#include "peri/dkmd_connector.h"
#include "gfxdev_mgr.h"
#include "gfxdev_utils.h"
#include "dpu_comp_online.h"
#include "panel_mgr.h"
#include "dpu_comp_vsync.h"
#include "dpu_comp_tui.h"
#include "dpu_comp_init.h"
#include "dpu_comp_present.h"
#include "dp_drv.h"
#include "dksm_dmd.h"
#include "dpu_comp_config_utils.h"

#define TIME_RANGE_TO_NEXT_VSYNC 3000000
#define DELAY_TIME_AFTER_TE 1000
#define DELAY_TIME_RANGE 500
#define UEVENT_BUF_LEN 120

static unsigned int g_esd_recover_disable = 0;

static void esd_send_event(struct composer *comp, uint32_t event)
{
	char *envp[2];
	char state_buf[UEVENT_BUF_LEN];
	int ret;

	ret = snprintf_s(state_buf, UEVENT_BUF_LEN, UEVENT_BUF_LEN - 1, "ESD_E=%u, COMP=%u\n", 
			 event, comp->index);	
	if (ret < 0) {
		dpu_pr_err("format string failed, truncation occurs");
		return;
	}

	envp[0] = state_buf;
	envp[1] = NULL;

	kobject_uevent_env(&(comp->base.peri_device->dev.kobj), KOBJ_CHANGE, envp);
	dpu_pr_info("esd upload event = %u!", event);
}

bool check_esd_happend(struct dpu_composer *dpu_comp)
{
	dpu_check_and_return(!dpu_comp, false, err, "dpu_comp is NULL\n");
	return (dpu_comp->conn_info->esd_enable != 0) && (atomic_read(&(dpu_comp->esd_ctrl.esd_happened)) == 1);
}

static void dpu_comp_esd_recover_dsm(void)
{
	uint32_t cnt = 0;
 	struct rtc_time tm = {0};
	struct timespec64 tv = {0};

	while((dsm_client_ocuppy(dsm_lcd_client) != 0) && (cnt < DSM_OCCUPY_RETRY_TIMES)) {
		dpu_pr_warn("dsm_client_ocuppy failed, retry %d times", ++cnt);
		usleep_range(500, 600);
	}
 
	if (cnt == DSM_OCCUPY_RETRY_TIMES) {
		dpu_pr_warn("dsm_client_ocuppy failed");
		return;
	}
 	ktime_get_real_ts64(&tv);
	tv.tv_sec -= (long)sys_tz.tz_minuteswest * 60;
	rtc_time64_to_tm(tv.tv_sec, &tm);

	dsm_client_record(dsm_lcd_client, "%04d%02d%02d%02d%02d%02d%03d|", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
					tm.tm_hour, tm.tm_min, tm.tm_sec, tv.tv_nsec / 1000000);

	dsm_client_record(dsm_lcd_client, "lcd esd recovery happened");
 
	dsm_client_notify(dsm_lcd_client, DSM_LCD_ESD_STATUS_ERROR_NO);
}

static void dpu_esd_timing_ctrl(struct dpu_composer *dpu_comp, struct comp_online_present *present)
{
	uint64_t vsync_period;
	ktime_t curr_time;
	uint64_t time_diff = 0;
	uint64_t delay_time = 0;
	uint32_t current_fps;

	struct dpu_connector *connector = NULL;
	struct esd_panel_info *esd_info = NULL;

	connector = get_primary_connector(dpu_comp->conn_info);
	dpu_check_and_no_retval(!connector, err, "connector is NULL\n");
	esd_info = &connector->esd_info;

	if (esd_info->esd_timing_ctrl == 0)
		return;

	current_fps = dpu_comp->conn_info->base.fps;

	if (current_fps == 0) {
		dpu_pr_debug("error fps %d\n", current_fps);
		return;
	}

	vsync_period = 1000000000UL / (uint64_t)current_fps;  /* convert to ns from s */
	curr_time = ktime_get();
	if (ktime_to_ns(curr_time) > ktime_to_ns(present->vsync_ctrl.timestamp)) {
		time_diff = (uint64_t)ktime_to_ns(curr_time) - (uint64_t)ktime_to_ns(present->vsync_ctrl.timestamp);
		if ((vsync_period > time_diff) &&
			((vsync_period - time_diff) < TIME_RANGE_TO_NEXT_VSYNC)) {
			delay_time = (vsync_period - time_diff) / 1000 +
				DELAY_TIME_AFTER_TE;  /* convert to us from ns */

			usleep_range(delay_time, delay_time + DELAY_TIME_RANGE);
			dpu_pr_debug("vsync %llu ns, timediff %llu ns, delay %llu us",
				vsync_period, time_diff, delay_time);
		}
	}
}

static void dpu_comp_esd_dp_debug_restore(struct dpu_composer *dpu_comp)
{
	struct dpu_connector * connector = NULL;
	struct dp_private *dp_priv = NULL;
	struct dp_ctrl *dptx = NULL;

	connector = get_primary_connector(dpu_comp->conn_info);
	dpu_check_and_no_retval(!connector, err, "connector is NULL\n");

	if (is_dp_primary_panel(&dpu_comp->comp.base)) {
		dp_priv = to_dp_private(connector->conn_info);
		if (!dp_priv) {
			dpu_pr_err("dp_priv is null\n");
			return;
		}
		dptx = &dp_priv->dp[MASTER_DPTX_IDX];
		if (dptx && dptx->esd_debug_mode == 1) {
			dptx->esd_debug_trigger = 0;
			dpu_pr_info("esd_debug_trigger restore default\n");
			return;
		}
	}
}

static void dpu_comp_esd_recover(struct dpu_composer *dpu_comp, struct comp_online_present *present)
{
	struct dpu_bl_ctrl* bl_ctrl = NULL;
	uint32_t bl_level_cur;
	struct composer_manager *comp_mgr = dpu_comp->comp_mgr;
	int vsync_enabled;

	bl_ctrl = &dpu_comp->bl_ctrl;
	dpu_check_and_no_retval(!bl_ctrl, err, "bl_ctrl is NULL\n");

	dpu_check_and_no_retval(g_esd_recover_disable, err, "g_esd_recover_disable is disable\n");

	down(&comp_mgr->power_sem);
	if (!composer_check_power_status(dpu_comp)) {
		up(&comp_mgr->power_sem);
		dpu_pr_warn("panel is off can not set backlight and exit esd recover");
		return;
	}
	bl_level_cur = bl_ctrl->bl_level;
	dpu_comp->bl_ctrl.bl_updated = 1;
	bl_ctrl->bl_level = 0;
	dpu_backlight_update(&dpu_comp->bl_ctrl, true);
	dpu_pr_debug("dpu_backlight_update, %d\n", dpu_comp->bl_ctrl.bl_level);

	vsync_enabled = present->vsync_ctrl.enabled;
	up(&comp_mgr->power_sem);
	dpu_comp_esd_recover_dsm();

	composer_manager_power_down(dpu_comp);
	composer_manager_power_restart_no_lock(dpu_comp);
	composer_manager_power_up(dpu_comp);

	present->vsync_ctrl.enabled = vsync_enabled;

	/* backlight on */
	msleep(100);  /* sleep 100ms */
	down(&comp_mgr->power_sem);
	if (!composer_check_power_status(dpu_comp)) {
		up(&comp_mgr->power_sem);
		dpu_pr_warn("panel is off can not set backlight exit esd recover");
		return;
	}
	bl_ctrl->bl_level = (bl_level_cur != 0) ? bl_level_cur : bl_ctrl->bl_level;
	dpu_comp->bl_ctrl.bl_updated = 1;
	dpu_backlight_update(&dpu_comp->bl_ctrl, true);
	if (present->dfr_ctrl.ops && present->dfr_ctrl.ops->enable_safe_frm_rate) {
		dpu_comp_active_vsync(dpu_comp);
		dpu_pr_debug("esd recover, need to set safe frm rate %u!", present->dfr_ctrl.cur_safe_frm_rate);
		present->dfr_ctrl.ops->enable_safe_frm_rate(&present->dfr_ctrl);
		dpu_comp_deactive_vsync(dpu_comp);
	}
	up(&comp_mgr->power_sem);
    if (is_async_mode(&dpu_comp->bl_ctrl))
        msleep(64);
    dpu_pr_debug("dpu_backlight_update, %d\n", dpu_comp->bl_ctrl.bl_level);
	dpu_comp_esd_dp_debug_restore(dpu_comp);
}

static int wait_flag_in_range(atomic_t *flag, int expect_val, uint32_t range_us, uint32_t max_cnt)
{
	uint32_t wait_count = 0;

	while ((atomic_read(flag) != expect_val) && (wait_count < max_cnt)) {
		wait_count++;
		usleep_range(range_us, range_us + 500);
	}

	if (wait_count >= max_cnt)
		return -1;

	return 0;
}

static int dpu_comp_dp_esd_handle(struct dp_ctrl *dptx)
{
	int ret = 0;
	if (dptx->esd_debug_mode == 1) {
		dpu_pr_debug("dp_debug_get_esd_mode esd_handle!\n");
		if (dptx->esd_debug_trigger == 1)
			ret = -1;
	} else if (dptx->dp_panel_mgr->esd_handle) {
		ret = dptx->dp_panel_mgr->esd_handle();
		dpu_pr_debug("dp_panel_mgr->esd_handle %d!\n", ret);
	}
	return ret;
}

static int dpu_comp_ctrl_esd(struct dpu_composer *dpu_comp, struct comp_online_present *present)
{
	int ret = 0;
	struct dpu_panel_ops *pops = NULL;
	struct dp_private *dp_priv = NULL;
	struct dp_ctrl *dptx = NULL;
	struct dpu_connector * connector = NULL;
	struct composer_manager *comp_mgr = NULL;

	/* skip esd check, goto esd_recovery */
	if (g_enable_esd_recovery != 0) {
        return 1;
    }

	dpu_check_and_return(!dpu_comp, 0, err, "dpu_comp is NULL\n");
	comp_mgr = dpu_comp->comp_mgr;
	dpu_comp_status_debug(&comp_mgr->power_status);
	connector = get_primary_connector(dpu_comp->conn_info);
	dpu_check_and_return(!connector, 0, err, "connector is NULL\n");

	down(&comp_mgr->power_sem);
	if (!composer_check_power_status(dpu_comp)) {
		dpu_pr_warn("composer %d, panel power off!\n", dpu_comp->comp.index);
		up(&comp_mgr->power_sem);
		return ret;
	}

	dpu_pr_debug("dpu_comp->comp.index %d, dpu_comp->conn_info->base.id %d",
		dpu_comp->comp.index, dpu_comp->conn_info->base.id);

	if (is_dp_primary_panel(&dpu_comp->comp.base)) {
		dp_priv = to_dp_private(connector->conn_info);
		if (!dp_priv) {
			dpu_pr_err("dp_priv is null\n");
			up(&comp_mgr->power_sem);
			return 0;
		}
		dptx = &dp_priv->dp[MASTER_DPTX_IDX];
		if (!dptx || !dptx->dp_panel_mgr) {
			dpu_pr_warn("dp_panel_mgr is null\n");
			up(&comp_mgr->power_sem);
			return ret;
		}
	} else {
		pops = get_panel_ops(dpu_comp->conn_info->base.id);
		if (!pops || !pops->esd_handle) {
			dpu_pr_warn("esd_handle is null\n");
			up(&comp_mgr->power_sem);
			return ret;
		}
	}

	dpu_comp_active_vsync(dpu_comp);
	atomic_set(&dpu_comp->esd_ctrl.esd_check_is_doing, 1);
	if (present->dfr_ctrl.cur_frm_rate != present->dfr_ctrl.pre_frm_rate &&
		present->dfr_ctrl.cur_frm_rate != present->dfr_ctrl.active_frm_rate) {
		dpu_pr_warn("can not do esd check when frame rate is changing\n");
		goto err_out;
	}

	if (is_ppc_support(&(dpu_comp->conn_info->base)) &&
		(dpu_comp->conn_info->ppc_config_id_record != dpu_comp->conn_info->ppc_config_id_active)) {
		dpu_pr_warn("can not do esd check when glmode is changing");
		goto err_out;
	}

	/* wait when mipi_resource is available */
	if (wait_flag_in_range(&connector->post_info[connector->active_idx]->mipi.dsi_bit_clk_upt_flag, 0, 1000, ESD_WAIT_MIPI_AVAILABLE_TIMEOUT) != 0) {
		dpu_pr_warn("wait mipi_available timeout");
		goto err_out;
	}

	/* wait for vsync coming */
	if (wait_flag_in_range(&dpu_comp->esd_ctrl.is_vsync_comming, 1, 1000, ESD_WAIT_VSYNC_TIME_COUNT) == 0)
		usleep_range(ESD_CHECK_AFTER_VSYNC_TIME_PERIOD, ESD_CHECK_AFTER_VSYNC_TIME_PERIOD + 100);
	else
		dpu_pr_warn("wait vsync timeout");

	dpu_esd_timing_ctrl(dpu_comp, present);

	if (is_dp_primary_panel(&dpu_comp->comp.base)) {
		ret = dpu_comp_dp_esd_handle(dptx);
	} else if (pops->esd_handle) {
		ret = pops->esd_handle();
		dpu_pr_debug("pops->esd_handle %d!\n", ret);
	}

err_out:
	dpu_comp_deactive_vsync(dpu_comp);
	atomic_set(&dpu_comp->esd_ctrl.esd_check_is_doing, 0);
	up(&comp_mgr->power_sem);

	return ret;
}

static void dpu_comp_esd_ctrl_check_and_recover(struct dpu_composer *dpu_comp,
	struct comp_online_present *present, uint32_t *recover_count)
{
	struct dkmd_connector_info *conn_info = NULL;
	struct dpu_connector * connector = NULL;
	struct dpu_esd_ctrl *esd_ctrl = NULL;
	struct esd_panel_info *esd_info = NULL;
	uint32_t esd_check_count = 0;
	uint32_t max_recover_count = 0;
	uint32_t max_check_count = 0;

	conn_info = dpu_comp->conn_info;
	dpu_check_and_no_retval(!conn_info, err, "conn_info is NULL\n");
	connector = get_primary_connector(conn_info);
	dpu_check_and_no_retval(!connector, err, "connector is NULL\n");
	esd_ctrl = &dpu_comp->esd_ctrl;

	esd_info = &connector->esd_info;
	max_recover_count = esd_info->esd_recovery_max_count != 0 ?
		esd_info->esd_recovery_max_count : ESD_RECOVERY_MAX_COUNT;
	max_check_count = esd_info->esd_check_max_count != 0 ?
		esd_info->esd_check_max_count : DPU_ESD_CHECK_MAX_COUNT;
	dpu_pr_debug("esd_recovery_max_count: %u, esd_check_max_count: %u, \
		esd_timing_ctrl: %u, esd_check_time_period: %u, esd_first_check_delay: %u",
		esd_info->esd_recovery_max_count, esd_info->esd_check_max_count, esd_info->esd_timing_ctrl,
		esd_info->esd_check_time_period, esd_info->esd_first_check_delay);
	while (*recover_count < max_recover_count) {
		if (esd_check_count < max_check_count) {
			if (is_tui_running(dpu_comp, TUI_LEVEL_0)) {
				dpu_pr_debug("tui level0 not do esd recovery");
				break;
			}

			atomic_set(&esd_ctrl->is_vsync_comming, 0);
			if ((dpu_comp_ctrl_esd(dpu_comp, present) != 0) || (esd_ctrl->esd_recover_state == ESD_RECOVER_STATE_START)) {
				esd_check_count++;
				atomic_set(&esd_ctrl->esd_happened, 1);
				dpu_pr_debug("esd check abnormal, esd_check_count:%u!\n", esd_check_count);
			} else {
				if (atomic_read(&esd_ctrl->esd_happened) == 1)
					esd_send_event(&dpu_comp->comp, ESD_EVENT_END);

				atomic_set(&esd_ctrl->esd_happened, 0);
				dpu_pr_debug("esd not happen, max_check_count:%u!\n", max_check_count);
				break;
			}
		}

		if (esd_check_count >= max_check_count || esd_ctrl->esd_recover_state == ESD_RECOVER_STATE_START) {
			dpu_pr_debug("esd recover panel, recover_count:%u max_recover_count:%u!", *recover_count, max_recover_count);
			esd_send_event(&dpu_comp->comp, ESD_EVENT_START);
			down(&esd_ctrl->esd_recover_sem);
			dpu_comp_esd_recover(dpu_comp, present);
			up(&esd_ctrl->esd_recover_sem);
			esd_check_count = 0;
			esd_ctrl->esd_recover_state = ESD_RECOVER_STATE_COMPLETE;
			(*recover_count)++;
		}
	}
}

static void dpu_comp_esd_check_wq_handler(struct work_struct *work)
{
	struct dpu_composer *dpu_comp = NULL;
	struct dkmd_connector_info *conn_info = NULL;
	struct dpu_connector * connector = NULL;
	struct comp_online_present *present = NULL;
	struct dpu_esd_ctrl *esd_ctrl = NULL;
	struct esd_panel_info *esd_info = NULL;
	uint32_t recover_count = 0;
	uint32_t max_recover_count = 0;

	esd_ctrl = container_of(work, struct dpu_esd_ctrl, esd_check_work);
	dpu_check_and_no_retval(!esd_ctrl, err, "esd_ctrl is NULL\n");

	dpu_comp = esd_ctrl->dpu_comp;
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL\n");

	present = esd_ctrl->present;
	dpu_check_and_no_retval(!present, err, "present is NULL\n");

	conn_info = dpu_comp->conn_info;
	dpu_check_and_no_retval(!conn_info, err, "conn_info is NULL\n");

	if ((conn_info->esd_enable == 0) || (g_esd_recover_disable != 0)) {
		if ((g_esd_recover_disable != 0))
			dpu_pr_debug("esd_enable=%hhu, g_esd_recover_disable=%d\n",
				conn_info->esd_enable, g_esd_recover_disable);
		return;
	}

	dpu_comp_esd_ctrl_check_and_recover(dpu_comp, present, &recover_count);

	/* recover count equate 5, we disable esd check function */
	connector = get_primary_connector(conn_info);
	esd_info = &connector->esd_info;
	max_recover_count = esd_info->esd_recovery_max_count != 0 ? esd_info->esd_recovery_max_count : ESD_RECOVERY_MAX_COUNT;
	if (recover_count >= max_recover_count) {
		dpu_pr_warn("esd recover %u count, disable esd function\n", max_recover_count);
		hrtimer_cancel(&esd_ctrl->esd_hrtimer);
		conn_info->esd_enable = 0;
	}
}

static enum hrtimer_restart dpu_comp_esd_hrtimer_fnc(struct hrtimer *timer)
{
	struct dpu_composer *dpu_comp = NULL;
	struct dkmd_connector_info *conn_info = NULL;
	struct dpu_connector *connector = NULL;
	struct dpu_esd_ctrl *esd_ctrl = NULL;
	struct esd_panel_info *esd_info = NULL;

	esd_ctrl = container_of(timer, struct dpu_esd_ctrl, esd_hrtimer);
	dpu_check_and_return(!esd_ctrl, HRTIMER_NORESTART, err, "esd_ctrl is NULL\n");

	dpu_comp = esd_ctrl->dpu_comp;
	dpu_check_and_return(!dpu_comp, HRTIMER_NORESTART, err, "dpu_comp is NULL\n");

	conn_info = dpu_comp->conn_info;
	dpu_check_and_return(!conn_info, HRTIMER_NORESTART, err, "conn_info is NULL\n");

	connector = get_primary_connector(conn_info);
	dpu_check_and_return(!connector, HRTIMER_NORESTART, err, "connector is NULL\n");
	esd_info = &connector->esd_info;

	dpu_pr_debug("conn_info->esd_enable %hhu", conn_info->esd_enable);
	if (conn_info->esd_enable) {
		if (esd_ctrl->esd_check_wq != NULL)
			queue_work(esd_ctrl->esd_check_wq, &esd_ctrl->esd_check_work);
	}

	if (esd_info->esd_check_time_period) {
		hrtimer_start(&esd_ctrl->esd_hrtimer, ktime_set(esd_info->esd_check_time_period / 1000,
			(esd_info->esd_check_time_period % 1000) * 1000000), HRTIMER_MODE_REL);
	} else {
		hrtimer_start(&esd_ctrl->esd_hrtimer, ktime_set(ESD_CHECK_TIME_PERIOD / 1000,
			(ESD_CHECK_TIME_PERIOD % 1000) * 1000000), HRTIMER_MODE_REL);
	}
	return HRTIMER_NORESTART;
}

void dpu_comp_esd_register(struct dpu_composer *dpu_comp, struct comp_online_present *present)
{
	struct dpu_esd_ctrl *esd_ctrl = NULL;
	struct dkmd_connector_info *conn_info = NULL;
	struct dpu_connector *connector = NULL;
	struct esd_panel_info *esd_info = NULL;

	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL\n");
	dpu_check_and_no_retval(!present, err, "present is NULL\n");

	esd_ctrl = &(dpu_comp->esd_ctrl);
	dpu_check_and_no_retval(!esd_ctrl, err, "esd_ctrl is NULL\n");
	if (esd_ctrl->esd_inited)
		return;

	conn_info = dpu_comp->conn_info;
	dpu_check_and_no_retval(!conn_info, err, "conn_info is NULL\n");

	if (conn_info->esd_enable) {
		dpu_pr_info("this panel support esd");
		connector = get_primary_connector(conn_info);
		dpu_check_and_no_retval(!connector, err, "connector is NULL\n");
		esd_info = &connector->esd_info;

		atomic_set(&esd_ctrl->esd_happened, 0);

		esd_ctrl->dpu_comp = dpu_comp;
		esd_ctrl->present = present;

		esd_ctrl->esd_check_wq = create_singlethread_workqueue("esd_check");
		dpu_check_and_no_retval(!esd_ctrl->esd_check_wq, err, "create esd_check_wq failed\n");

		INIT_WORK(&esd_ctrl->esd_check_work, dpu_comp_esd_check_wq_handler);
		sema_init(&esd_ctrl->esd_recover_sem, 1);
		// /* hrtimer for ESD timing */
		hrtimer_init(&esd_ctrl->esd_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
		esd_ctrl->esd_hrtimer.function = dpu_comp_esd_hrtimer_fnc;
		if (esd_info->esd_check_time_period) {
			hrtimer_start(&esd_ctrl->esd_hrtimer, ktime_set(esd_info->esd_check_time_period / 1000,
				(esd_info->esd_check_time_period % 1000) * 1000000), HRTIMER_MODE_REL);
		} else if (esd_info->esd_first_check_delay) {
			hrtimer_start(&esd_ctrl->esd_hrtimer, ktime_set(esd_info->esd_first_check_delay / 1000,
				(esd_info->esd_first_check_delay % 1000) * 1000000), HRTIMER_MODE_REL);
		} else {
			hrtimer_start(&esd_ctrl->esd_hrtimer, ktime_set(ESD_CHECK_TIME_PERIOD / 1000,
				(ESD_CHECK_TIME_PERIOD % 1000) * 1000000), HRTIMER_MODE_REL);
		}
		esd_ctrl->esd_inited = 1;
	}
}

void dpu_comp_esd_unregister(struct dpu_composer *dpu_comp)
{
	struct dpu_esd_ctrl *esd_ctrl = NULL;
	struct dkmd_connector_info *conn_info = NULL;

	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL\n");
	esd_ctrl = &(dpu_comp->esd_ctrl);
	dpu_check_and_no_retval(!esd_ctrl, err, "esd_ctrl is NULL\n");
	conn_info = dpu_comp->conn_info;
	dpu_check_and_no_retval(!conn_info, err, "conn_info is NULL\n");
	if (esd_ctrl->esd_inited == 0)
		return;
	if (conn_info->esd_enable != 0)
		hrtimer_cancel(&esd_ctrl->esd_hrtimer);
}

int dpu_comp_esd_recover_disable(int value)
{
	dpu_pr_info("esd_recover_disable=%d\n", value);
	g_esd_recover_disable = (unsigned int)value;
	return 0;
}

void restart_esd_timer(struct dpu_composer *dpu_comp)
{
	struct dpu_connector *connector = NULL;
	struct dpu_esd_ctrl *esd_ctrl = NULL;

	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL\n");

	esd_ctrl = &dpu_comp->esd_ctrl;
	if ((dpu_comp->conn_info->esd_enable != 0) && (esd_ctrl->esd_inited != 0)) {
		connector = get_primary_connector(dpu_comp->conn_info);
		if (connector != NULL) {
			dpu_pr_debug("esd timer restart!");
			hrtimer_restart(&esd_ctrl->esd_hrtimer);
		}
	}
}

void start_esd_timer(struct dpu_composer *dpu_comp)
{
	struct dpu_connector *connector = NULL;
	struct dpu_esd_ctrl *esd_ctrl = NULL;
	struct esd_panel_info *esd_info = NULL;

	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL\n");

	if ((dpu_comp->comp.index != DEVICE_COMP_PRIMARY_ID) && (dpu_comp->comp.index != DEVICE_COMP_BUILTIN_ID))
		return;
	esd_ctrl = &dpu_comp->esd_ctrl;
	if ((dpu_comp->conn_info->esd_enable != 0) && (esd_ctrl->esd_inited != 0)) {
		connector = get_primary_connector(dpu_comp->conn_info);
		esd_info = &connector->esd_info;
		dpu_pr_debug("esd timer start!");
		if (esd_info->esd_check_time_period) {
			hrtimer_start(&esd_ctrl->esd_hrtimer, ktime_set(esd_info->esd_check_time_period / 1000,
				(esd_info->esd_check_time_period % 1000) * 1000000), HRTIMER_MODE_REL);
		} else {
			hrtimer_start(&esd_ctrl->esd_hrtimer, ktime_set(ESD_CHECK_TIME_PERIOD / 1000,
				(ESD_CHECK_TIME_PERIOD % 1000) * 1000000), HRTIMER_MODE_REL);
		}
	}
}

void cancel_esd_timer(struct dpu_composer *dpu_comp)
{
	struct dpu_esd_ctrl *esd_ctrl = NULL;

	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL\n");
	esd_ctrl = &dpu_comp->esd_ctrl;
	if ((dpu_comp->comp.index != DEVICE_COMP_PRIMARY_ID) && (dpu_comp->comp.index != DEVICE_COMP_BUILTIN_ID))
		return;
	if ((dpu_comp->conn_info->esd_enable != 0) && (esd_ctrl->esd_inited != 0)) {
		dpu_pr_debug("esd timer cancel!");
		hrtimer_cancel(&esd_ctrl->esd_hrtimer);
	}
}

void cancel_start_esd_timer(struct dpu_composer *dpu_comp)
{
	struct dpu_connector *connector = NULL;
	struct dpu_esd_ctrl *esd_ctrl = NULL;
	struct esd_panel_info *esd_info = NULL;
 
	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL\n");
	if ((dpu_comp->comp.index != DEVICE_COMP_PRIMARY_ID) && (dpu_comp->comp.index != DEVICE_COMP_BUILTIN_ID))
		return;
	esd_ctrl = &dpu_comp->esd_ctrl;
	if ((dpu_comp->conn_info->esd_enable != 0) && (esd_ctrl->esd_inited != 0)) {
		connector = get_primary_connector(dpu_comp->conn_info);
		esd_info = &connector->esd_info;

		if (hrtimer_active(&esd_ctrl->esd_hrtimer)) {
			dpu_pr_info("cancel esd timer then start");
			hrtimer_cancel(&esd_ctrl->esd_hrtimer);
			if (esd_info->esd_check_time_period) {
				hrtimer_start(&esd_ctrl->esd_hrtimer, ktime_set(esd_info->esd_check_time_period / 1000,
					(esd_info->esd_check_time_period % 1000) * 1000000), HRTIMER_MODE_REL);
			} else {
				hrtimer_start(&esd_ctrl->esd_hrtimer, ktime_set(ESD_CHECK_TIME_PERIOD / 1000,
					(ESD_CHECK_TIME_PERIOD % 1000) * 1000000), HRTIMER_MODE_REL);
			}
		}
	}
}

void esd_handle_vsync(struct dpu_composer *dpu_comp)
{
	atomic_set(&dpu_comp->esd_ctrl.is_vsync_comming, 1);
}

void dpu_comp_esd_recovery_manually(struct dpu_composer *dpu_comp)
{
    void_unused(dpu_comp);
}
