// SPDX-License-Identifier: GPL-2.0
/*
 * lcd_kit_hybrid_control.c
 *
 * source file for hybrid switch control
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
#include "lcd_kit_hybrid_control.h"

#include <linux/mutex.h>
#include <linux/time.h>
#include <platform_include/smart/linux/iomcu_config.h>
#include "securec.h"
#include "lcd_kit_hybrid_swctrl.h"
#include "lcd_kit_hybrid_aod.h"
#include "lcd_kit_hybrid_force.h"
#include "lcd_kit_hybrid_core.h"
#include "huawei_ts_kit_hybrid_core.h"
#include "ext_sensorhub_inner_cmd.h"
#include "lcd_kit_adapt.h"
#include "lcd_kit_hybrid_trace.h"
#ifdef CONFIG_EXT_INPUTHUB_MATTING_ALSC
#include "matting_algo_task.h"
#endif
#if !defined(CONFIG_FACTORY_MODE) && defined(CONFIG_DFX_ZEROHUNG)
#include "wp_screen.h"
#endif

enum display_ddic_id {
	UNKNOWN_DDIC_ID = 0,
	VXN_DDIC_ID = 1,
	OTHER_DDIC_ID = 2,
};

#define INNER_CMD_COMMU_SID 0x90
#define INNER_CMD_COMMU_CID 0x0D
#define HYBRID_SUB_COMMAND_RES_ID 0x0C
#define HYBRID_WAIT_DISP_ON_TIMES 30

static bool block_brightness_flag;
struct timespec64 record_time;

bool hybrid_check_power_on(struct display_hybrid_ctrl *display_ctrl)
{
	if (!display_ctrl || !display_ctrl->dpu_composer)
		return false;

	return display_ctrl->dpu_composer->comp.power_on;
}

void hybrid_dss_on(struct display_hybrid_ctrl *display_ctrl, uint8_t on_mode)
{
	if (!display_ctrl || !display_ctrl->dpu_composer || !display_ctrl->composer_on)
		return;

	if (!hybrid_control_check()) {
		LCD_KIT_WARNING("do not hold !");
		return;
	}

	display_ctrl->skip_panel_onoff = true;
	display_ctrl->composer_on(&display_ctrl->dpu_composer->comp, on_mode);
	display_ctrl->skip_panel_onoff = false;

#ifdef CONFIG_EXT_INPUTHUB_MATTING_ALSC
	notify_lcd_state_to_matting_algo_task(MATTING_ALGO_AP_SW, MATTING_ALGO_AP_POWER_NORMAL);
#endif
}

void hybrid_dss_off(struct display_hybrid_ctrl *display_ctrl)
{
	if (!display_ctrl || !display_ctrl->dpu_composer || !display_ctrl->composer_off)
		return;

	if (!hybrid_control_check()) {
		LCD_KIT_WARNING("do not hold !");
		return;
	}

	display_ctrl->skip_panel_onoff = true;
	display_ctrl->composer_off(&display_ctrl->dpu_composer->comp, COMPOSER_OFF_MODE_FAKE);
	display_ctrl->skip_panel_onoff = false;
#ifdef CONFIG_EXT_INPUTHUB_MATTING_ALSC
	notify_lcd_state_to_matting_algo_task(0, 0);
#endif
}

int hybrid_te_on(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl)
		return -EINVAL;

	if (!hybrid_control_check()) {
		LCD_KIT_WARNING("do not hold !\n");
		return -EACCES;
	}

	if (!hybrid_check_power_on(display_ctrl)) {
		LCD_KIT_ERR("dss is power off, return\n");
		return -EACCES;
	}

	ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.te_on_cmds);
	if (ret != LCD_KIT_OK)
		LCD_KIT_ERR("send te off cmds error\n");

	return ret;
}

int hybrid_te_off(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl)
		return -EINVAL;

	if (!hybrid_control_check()) {
		LCD_KIT_WARNING("do not hold !\n");
		return -EACCES;
	}

	if (!hybrid_check_power_on(display_ctrl)) {
		LCD_KIT_ERR("dss is power off, return\n");
		return -EACCES;
	}

	ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.te_off_cmds);
	if (ret != LCD_KIT_OK)
		LCD_KIT_ERR("send te off cmds error\n");

	return ret;
}

static bool lcd_kit_check_vxn_panel(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	u8 read_value = 0;
	int panel_id = lcd_kit_get_active_panel_id();
	static enum display_ddic_id ddic_id = UNKNOWN_DDIC_ID;

	LCD_KIT_INFO("get in\n");

	if (!display_ctrl)
		return false;

	if (ddic_id == VXN_DDIC_ID)
		return true;

	if (ddic_id)
		return false;

	ret = lcd_kit_cmds_rx(panel_id, NULL, &read_value, sizeof(read_value),
				  &display_ctrl->hybrid_info.ddic_id_cmds);
	if (ret) {
		LCD_KIT_WARNING("mipi rx failed!\n");
		return false;
	}

	LCD_KIT_INFO("ddic_id 0x%x\n", read_value);

	if (read_value == display_ctrl->hybrid_info.vxn_id) {
		ddic_id = VXN_DDIC_ID;
		return true;
	}

	ddic_id = OTHER_DDIC_ID;
	return false;
}

static int hybrid_lcd_try_off(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	u8 read_value = 0;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl)
		return -EINVAL;

	ret = lcd_kit_cmds_rx(panel_id, NULL, &read_value, sizeof(read_value),
		&display_ctrl->hybrid_info.status_cmds);
	if (ret)
		LCD_KIT_WARNING("mipi rx failed!\n");

	LCD_KIT_INFO("get LCD status:%02x!\n", read_value);
	if (read_value == STATUS_IDLE || read_value == STATUS_NORMAL) {
		if (get_hybrid_common_ops()->panel_off_lp)
			ret = get_hybrid_common_ops()->panel_off_lp(panel_id, NULL);
	}

	return ret;
}

static bool lcd_kit_is_deepsleep(void)
{
	struct iomcu_quick_on_config *quick_on_config =
		iomcu_config_get(IOMCU_CONFIG_USR_IOMCU_QUICK_ON,
		sizeof(struct iomcu_quick_on_config));

	if (!quick_on_config)
		return false;

	LCD_KIT_INFO("lcd of mcu deepsleep: %d\n", quick_on_config->lcd_is_deepsleep);
	return quick_on_config->lcd_is_deepsleep;
}

static void lcd_kit_try_reset_hw(void)
{
	if (lcd_kit_is_deepsleep()) {
		set_current_gesture_status(get_gesture_switch());
		if (get_current_gesture_status()) {
			LCD_KIT_INFO("lcd hw reset");
			lcd_kit_hybrid_hw_reset();
		}
	}
}

static int lcd_kit_set_deepsleep(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	u8 read_value = 0;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl)
		return -EINVAL;

	/* switch from mcu deepsleep to ap off */
	if (lcd_kit_is_deepsleep()) {
		LCD_KIT_INFO("mcu deepsleep -> ap deepsleep, do nothing\n");
		if (display_ctrl->display_sw_ops &&
			display_ctrl->display_sw_ops->send_hybrid_state) {
			ret = display_ctrl->display_sw_ops->send_hybrid_state(
				display_ctrl->display_sw_ops, AP_DISPLAY_DEEP_SLEEP);
			if (ret < 0)
				LCD_KIT_WARNING("AP_DISPLAY_DEEP_SLEEP to mcu, ret: %d\n", ret);
		}
		lcd_kit_hybrid_esd_off();
		alpm_set_state_flag(ALPM_STATE_OUT);
		return LCD_KIT_OK;
	}

	/* power on */
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->power_control)
		display_ctrl->display_sw_ops->power_control(display_ctrl->display_sw_ops, 1);
	lcd_kit_hybrid_esd_off();
	hybrid_dss_on(display_ctrl, COMPOSER_ON_MODE_UNBLANK);

	ret = lcd_kit_cmds_rx(panel_id, NULL, &read_value, sizeof(read_value),
		&display_ctrl->hybrid_info.status_cmds);
	if (ret)
		LCD_KIT_WARNING("mipi rx failed!\n");

	LCD_KIT_INFO("get LCD status:%02x!\n", read_value);

	if (read_value != STATUS_POWERON && read_value != STATUS_NOPOWER) {
		ret = lcd_kit_enter_deep_sleep(display_ctrl);
		if (ret)
			LCD_KIT_WARNING("fail to deep sleep!\n");

		if (display_ctrl->display_sw_ops &&
			display_ctrl->display_sw_ops->send_hybrid_state) {
			ret = display_ctrl->display_sw_ops->send_hybrid_state(
				display_ctrl->display_sw_ops, AP_DISPLAY_DEEP_SLEEP);
			if (ret < 0)
				LCD_KIT_WARNING("AP_DISPLAY_DEEP_SLEEP to mcu, ret: %d\n", ret);
		}
	}

	hybrid_dss_off(display_ctrl);
	alpm_set_state_flag(ALPM_STATE_OUT);
	return ret;
}

static int lcd_kit_set_off(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	u8 read_value = 0;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl)
		return -EINVAL;

	if (!hybrid_control_check()) {
		LCD_KIT_WARNING("do not hold !\n");
		return 0;
	}
	/* power on */
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->power_control)
		display_ctrl->display_sw_ops->power_control(display_ctrl->display_sw_ops, 1);
	lcd_kit_hybrid_esd_off();
	hybrid_dss_on(display_ctrl, COMPOSER_ON_MODE_UNBLANK);

	ret = lcd_kit_cmds_rx(panel_id, NULL, &read_value, sizeof(read_value),
				  &display_ctrl->hybrid_info.status_cmds);
	if (ret)
		LCD_KIT_WARNING("mipi rx failed!\n");
	LCD_KIT_INFO("get LCD status:%02x!\n", read_value);

	ret = 0;
	if (read_value != STATUS_POWERON) {
		if (get_hybrid_common_ops()->panel_off_lp)
			ret = get_hybrid_common_ops()->panel_off_lp(panel_id, NULL);
	}

	hybrid_dss_off(display_ctrl);
	lcd_kit_hybrid_esd_on();
	alpm_set_state_flag(ALPM_STATE_OUT);
	/* power off */
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->power_control)
		display_ctrl->display_sw_ops->power_control(display_ctrl->display_sw_ops, 0);

	return ret;
}

int lcd_kit_set_normal(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	u8 read_value = 0;
	bool vxn_panel = false;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl)
		return -EINVAL;

	if (!hybrid_control_check()) {
		LCD_KIT_WARNING("do not hold !\n");
		return 0;
	}

	ret = lcd_kit_cmds_rx(panel_id, NULL, &read_value, sizeof(read_value),
				  &display_ctrl->hybrid_info.status_cmds);
	if (ret)
		LCD_KIT_WARNING("mipi rx failed!\n");

	LCD_KIT_INFO("get LCD status:%02x!\n", read_value);
	ret = 0;
	switch (read_value) {
	case STATUS_NORMAL:
		LCD_KIT_INFO("get normal status\n");
		ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.te_on_cmds);
		break;
	case STATUS_IDLE:
		LCD_KIT_INFO("get idle status\n");
		ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.alpm_exit_cmds);
		if (ret)
			break;
		/* te on */
		ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.te_on_cmds);
		break;
	case STATUS_NOPOWER:
		if (get_hybrid_common_ops()->panel_power_on)
			ret = get_hybrid_common_ops()->panel_power_on(panel_id, NULL);
		/* fall-through */
	default:
		vxn_panel = lcd_kit_check_vxn_panel(display_ctrl);
		LCD_KIT_INFO("end initial code, vxn_panel:%d\n", vxn_panel);
		lcd_kit_set_aod_panel_state(panel_id, 0);
		if (get_hybrid_common_ops()->panel_on_lp)
			ret = get_hybrid_common_ops()->panel_on_lp(panel_id, NULL);
		lcd_kit_set_aod_panel_state(panel_id, 1);
	}

	alpm_set_state(display_ctrl);

	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->send_hybrid_state) {
		ret = display_ctrl->display_sw_ops->send_hybrid_state(display_ctrl->display_sw_ops,
			AP_DISPLAY_ON);
		if (ret < 0)
			LCD_KIT_INFO("send AP_DISPLAY_ON to mcu, ret: %d\n", ret);
	}

	return ret;
}

int lcd_kit_set_idle(struct display_hybrid_ctrl *display_ctrl)
{
	int ret = 0;
	u8 read_value = 0;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl)
		return -EINVAL;

	if (!hybrid_control_check()) {
		LCD_KIT_WARNING("do not hold !\n");
		return -EACCES;
	}

	ret = hybrid_get_lcd_status(display_ctrl, &read_value);
	if (ret)
		LCD_KIT_WARNING("fail to get lcd status!\n");

	ret = 0;
	switch (read_value) {
	case STATUS_NORMAL:
		LCD_KIT_INFO("get normal status\n");
		ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.alpm_on_nl_cmds);
		if (ret)
			break;
		ret = hybrid_te_on(display_ctrl);
		break;
	case STATUS_IDLE:
		LCD_KIT_INFO("get idle status\n");
		ret = hybrid_te_on(display_ctrl);
		break;
	case STATUS_NOPOWER:
		if (get_hybrid_common_ops()->panel_power_on)
			ret = get_hybrid_common_ops()->panel_power_on(panel_id, NULL);
		if (ret)
			break;
		/* fall-through */
	default:
		lcd_kit_set_aod_panel_state(panel_id, 0);
		if (get_hybrid_common_ops()->panel_on_lp)
			ret = get_hybrid_common_ops()->panel_on_lp(panel_id, NULL);

		ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.alpm_on_nl_cmds);
		if (ret)
			break;

		ret = hybrid_te_on(display_ctrl);
		lcd_kit_set_aod_panel_state(panel_id, 1);
	}

	alpm_set_state(display_ctrl);

	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->send_hybrid_state)
		display_ctrl->display_sw_ops->send_hybrid_state(display_ctrl->display_sw_ops,
			AP_DISPLAY_AOD);

	if (ret)
		LCD_KIT_ERR("err out: ret: %d", ret);
	return ret;
}

static int hybrid_set_full_dirty_region(struct display_hybrid_ctrl *display_ctrl)
{
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl)
		return -EINVAL;

	LCD_KIT_INFO("get in");
	if (common_info->dirty_region.support)
		return lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.dirty_region_cmds);

	return LCD_KIT_OK;
}

static void hybrid_wait_display_on(struct display_hybrid_ctrl *display_ctrl)
{
	bool display_on_ongoing = false;
	int retry = 0;

	if (!display_ctrl)
		return;

	while (retry <= HYBRID_WAIT_DISP_ON_TIMES) {
		spin_lock(&display_ctrl->comp_on_spin);
		display_on_ongoing = display_ctrl->composer_on_ongoing;
		spin_unlock(&display_ctrl->comp_on_spin);
		if (display_on_ongoing) {
			LCD_KIT_INFO("waiting display on");
			/* 10ms */
			usleep_range(10000, 10001);
			retry++;
		} else {
			break;
		}
	}
}

static int hybrid_on_switch_mcu_with_aim_state(struct display_hybrid_ctrl *display_ctrl,
	u8 aim_state)
{
	int ret;

	LCD_KIT_INFO("get in\n");

	if (!display_ctrl)
		return -EINVAL;

	mutex_lock(&display_ctrl->hybrid_lock);
	display_ctrl->hybrid_control_switching = true;

	lcd_hybrid_trace_begin(__func__);
	if (!hybrid_control_check()) {
		LCD_KIT_INFO("do not hold display control, do nothing\n");
		lcd_hybrid_trace_end(__func__);
		display_ctrl->hybrid_control_switching = false;
		mutex_unlock(&display_ctrl->hybrid_lock);
		return -EACCES;
	}

	hybrid_wait_display_on(display_ctrl);
	lcd_kit_hybrid_gesture_qon_reset();
	if (hybrid_check_power_on(display_ctrl)) {
		/* set dirty region with full size */
		ret = hybrid_set_full_dirty_region(display_ctrl);
		if (ret != LCD_KIT_OK)
			LCD_KIT_ERR("set full dirty region error\n");
	}
	/* send ts event */
	send_hybrid_ts_cmd(TS_KIT_HYBRID_NORMAL_TO_MCU);
	/* real dss off */
	hybrid_dss_off(display_ctrl);
	/* send on state to mcu */
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->send_hybrid_state) {
		ret = display_ctrl->display_sw_ops->send_hybrid_state(display_ctrl->display_sw_ops,
			aim_state);
		if (ret != 0) {
			LCD_KIT_ERR("send state to mcu failed, abort switching, ret=%d\n", ret);
			goto out;
		}
	}

	/* request control to mcu */
	ret = hybrid_request_display(0);
	if (ret != 0)
		LCD_KIT_ERR("control ap->mcu failed, ret: %d\n", ret);

out:
	display_ctrl->hybrid_control_switching = false;
	lcd_hybrid_trace_end(__func__);
	mutex_unlock(&display_ctrl->hybrid_lock);
	/* need display off by hybrid FWK */
	return ret;
}

int hybrid_on_switch_mcu(struct display_hybrid_ctrl *display_ctrl)
{
	if (!display_ctrl)
		return -EINVAL;

	return hybrid_on_switch_mcu_with_aim_state(display_ctrl, DISPLAY_ON);
}

int hybrid_str_switch_mcu(struct display_hybrid_ctrl *display_ctrl)
{
	if (!display_ctrl)
		return -EINVAL;

	return hybrid_on_switch_mcu_with_aim_state(display_ctrl, DISPLAY_ON_STR);
}

int hybrid_idle_switch_mcu(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;

	LCD_KIT_INFO("get in\n");

	if (!display_ctrl)
		return -EINVAL;

	mutex_lock(&display_ctrl->hybrid_lock);
	display_ctrl->hybrid_control_switching = true;
	if (!hybrid_control_check()) {
		LCD_KIT_INFO("do not hold display control, do nothing\n");
		display_ctrl->hybrid_control_switching = false;
		mutex_unlock(&display_ctrl->hybrid_lock);
		return -EACCES;
	}

	hybrid_wait_display_on(display_ctrl);
	lcd_kit_hybrid_gesture_qon_reset();
	hybrid_dss_on(display_ctrl, COMPOSER_ON_MODE_UNBLANK);
	if (hybrid_check_power_on(display_ctrl)) {
		/* set dirty region with full size */
		ret = hybrid_set_full_dirty_region(display_ctrl);
		if (ret != LCD_KIT_OK)
			LCD_KIT_ERR("set full dirty region error\n");
	}

	/* real dss off */
	hybrid_dss_off(display_ctrl);
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->send_hybrid_state) {
		ret = display_ctrl->display_sw_ops->send_hybrid_state(display_ctrl->display_sw_ops,
			DISPLAY_AOD);
		if (ret != 0) {
			LCD_KIT_ERR("send state to mcu failed, abort switching, ret=%d\n", ret);
			goto out;
		}
	}

	ret = hybrid_request_display(0);
	if (!ret)
		send_hybrid_ts_cmd(TS_KIT_HYBRID_IDLE_TO_MCU);
	else
		LCD_KIT_ERR("control ap->mcu failed, ret=%d\n", ret);

out:
	display_ctrl->hybrid_control_switching = false;
	mutex_unlock(&display_ctrl->hybrid_lock);
	return ret;
}

int hybrid_off_switch_mcu(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;

	LCD_KIT_INFO("get in\n");

	if (!display_ctrl)
		return -EINVAL;

	mutex_lock(&display_ctrl->hybrid_lock);
	display_ctrl->hybrid_control_switching = true;
	if (!hybrid_control_check()) {
		LCD_KIT_INFO("do not hold display control, do nothing\n");
		display_ctrl->hybrid_control_switching = false;
		mutex_unlock(&display_ctrl->hybrid_lock);
		return -EACCES;
	}

	hybrid_wait_display_on(display_ctrl);
	lcd_kit_hybrid_gesture_qon_reset();
	/* send ts event */
	if (lcd_kit_is_deepsleep())
		send_hybrid_ts_cmd(TS_KIT_HYBRID_IDLE_TO_MCU);
	else
		send_hybrid_ts_cmd(TS_KIT_HYBRID_SLEEP_TO_MCU);

	if (hybrid_check_power_on(display_ctrl)) {
		/* set dirty region with full size */
		ret = hybrid_set_full_dirty_region(display_ctrl);
		if (ret != LCD_KIT_OK)
			LCD_KIT_ERR("set full dirty region error\n");

		hybrid_lcd_try_off(display_ctrl);
	}

	/* real dss off */
	hybrid_dss_off(display_ctrl);
	/* send off state to mcu */
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->send_hybrid_state) {
		ret = display_ctrl->display_sw_ops->send_hybrid_state(display_ctrl->display_sw_ops,
			DISPLAY_OFF);
		if (ret != 0) {
			LCD_KIT_ERR("send state to mcu failed, abort switching, ret=%d\n", ret);
			goto out;
		}
	}
	/* request control to mcu */
	ret = hybrid_request_display(0);
	if (ret != 0)
		LCD_KIT_ERR("control ap->mcu failed, ret=%d\n", ret);

out:
	display_ctrl->hybrid_control_switching = false;
	mutex_unlock(&display_ctrl->hybrid_lock);
	return ret;
}

void hybrid_set_block_brightness_flag(bool flag)
{
	if (flag)
		ktime_get_real_ts64(&record_time);
	block_brightness_flag = flag;
}

bool hybrid_get_block_brightness_flag(struct display_hybrid_ctrl *display_ctrl)
{
	struct timespec64 current_time;
	struct timespec64 ts_delta;
	uint8_t normal_brightness = 0;
	uint8_t hbm_brightness = 0;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!block_brightness_flag)
		return false;

	ktime_get_real_ts64(&current_time);
	set_normalized_timespec64(&ts_delta, current_time.tv_sec - record_time.tv_sec,
		current_time.tv_nsec - record_time.tv_nsec);
	/* wait inner cmd callbck timeout 200ms */
	if (ts_delta.tv_sec > 0 || ts_delta.tv_nsec > 200 * 1000 * 1000) {
		block_brightness_flag = false;
		LCD_KIT_INFO("ts_delta.tv_sec %ld ts_delta.tv_nsec %ld",
			ts_delta.tv_sec, ts_delta.tv_nsec);
		return false;
	}

	/* when brightness err no need block brightness */
	lcd_kit_cmds_rx(panel_id, NULL, &hbm_brightness, sizeof(hbm_brightness),
			&display_ctrl->hybrid_info.hbm_brightness_cmds);
	lcd_kit_cmds_rx(panel_id, NULL, &normal_brightness, sizeof(normal_brightness),
			&display_ctrl->hybrid_info.normal_brightness_cmds);
	if (hbm_brightness == 0 && normal_brightness == 0)
		block_brightness_flag = false;

	return block_brightness_flag;
}

static int lcd_kit_hybrid_inner_cmd_callback(unsigned char sid, unsigned char cid,
	unsigned char *data, int data_len)
{
	if (sid != INNER_CMD_COMMU_SID || cid != INNER_CMD_COMMU_CID || data == NULL ||
		data_len < 2 || data[0] != HYBRID_SUB_COMMAND_RES_ID)
		return 0;

	/* data1: autoMode, data2 : brightness */
	LCD_KIT_INFO("autoMode: %d, brightness: %d", data[1], data[2]);
	hybrid_set_block_brightness_flag(false);

	return 0;
}

void lcd_kit_hybrid_register_inner_cmd(void)
{
	unsigned char sid = INNER_CMD_COMMU_SID;
	unsigned char cid = INNER_CMD_COMMU_CID;

	LCD_KIT_INFO("get in");
	register_inner_cmd(DISPLAY_CHANNEL, &sid, &cid, 1, lcd_kit_hybrid_inner_cmd_callback);
}

static void hybrid_on_notify_brightness(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	u8 offset = 0;
	u8 hbm_mode = 0;
	u8 brightness = 0;
	unsigned char data[3] = {0}; /* 3 for msg len: sub command id + brightness + hbm_mode */
	int panel_id = lcd_kit_get_active_panel_id();

	ret = lcd_kit_cmds_rx(panel_id, NULL, &hbm_mode, sizeof(hbm_mode),
			&display_ctrl->hybrid_info.hbm_mode_cmds);
	if (ret)
		return;

	if (hbm_mode)
		lcd_kit_cmds_rx(panel_id, NULL, &brightness, sizeof(brightness),
			&display_ctrl->hybrid_info.hbm_brightness_cmds);
	else
		lcd_kit_cmds_rx(panel_id, NULL, &brightness, sizeof(brightness),
			&display_ctrl->hybrid_info.normal_brightness_cmds);

	LCD_KIT_INFO("brightness:%u, hbm mode:%u\n", brightness, hbm_mode);
	if (brightness != 0) {
		/* update kernel last brightness */
		if (display_ctrl->dpu_composer) {
			/* current panel already set */
			display_ctrl->dpu_composer->bl_ctrl.bl_updated = 1;
			display_ctrl->dpu_composer->bl_ctrl.bl_level = brightness;
			display_ctrl->dpu_composer->bl_ctrl.bl_level_old = brightness;
			display_ctrl->dpu_composer->bl_ctrl.bl_timestamp = ktime_get();
		}

		data[offset++] = 0xc; /* 0xc: hybrid brightness sub command id */
		data[offset++] = brightness;
		data[offset++] = hbm_mode;

#if !defined(CONFIG_FACTORY_MODE) && defined(CONFIG_DFX_ZEROHUNG)
		hung_wp_screen_setbl(brightness);
		LCD_KIT_INFO("hung_wp_screen_setbl %d\n", brightness);
#endif /* !CONFIG_FACTORY_MODE && CONFIG_DFX_ZEROHUNG */

		/* 0x01 device msg service id, 0x9f lcd msg command id */
		hybrid_set_block_brightness_flag(true);
		send_inner_command(0x01, 0x9f, data, ARRAY_SIZE(data));
	}
}

int hybrid_on_switch_ap(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	bool on_without_switching = false;

	LCD_KIT_INFO("get in\n");

	if (!display_ctrl)
		return -EINVAL;

	mutex_lock(&display_ctrl->hybrid_lock);
	lcd_hybrid_trace_begin(__func__);
	display_ctrl->hybrid_control_switching = true;

	if (hybrid_control_check()) {
		on_without_switching = true;
		if (display_ctrl->current_state == HYBRID_OFF_SWITCH_STR &&
			hybrid_control_check_mcu()) {
			on_without_switching = false;
			LCD_KIT_WARNING("Abnormal!! Both ap and mcu hold control, dss_on[%d]",
				hybrid_check_power_on(display_ctrl));
		}
	}

	/* request control to ap */
	if (!on_without_switching) {
		ret = hybrid_request_display(1);
		if (ret != LCD_KIT_OK) {
			LCD_KIT_INFO("control mcu->ap failed, abort switching, ret: %d\n", ret);
			goto out;
		}
	}

	/* power on */
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->power_control) {
		ret = display_ctrl->display_sw_ops->power_control(display_ctrl->display_sw_ops, 1);
		if (ret != LCD_KIT_OK) {
			LCD_KIT_INFO("power on control failed, abort switching, ret: %d\n", ret);
			goto out;
		}
	}

	lcd_kit_hybrid_esd_off();
	/* Reset hw when switch from mcu deepsleep to ap on */
	lcd_kit_try_reset_hw();
	hybrid_dss_on(display_ctrl, COMPOSER_ON_MODE_UNBLANK);

	/* initial code */
	ret = lcd_kit_set_normal(display_ctrl);
	if (ret != LCD_KIT_OK)
		LCD_KIT_INFO("set notmal failed: %d\n", ret);
	send_hybrid_ts_cmd(TS_KIT_HYBRID_NORMAL_TO_AP);
	hybrid_on_notify_brightness(display_ctrl);
	lcd_kit_hybrid_esd_on();
	hybrid_lcd_status_check(display_ctrl);

out:
	display_ctrl->hybrid_control_switching = false;
	lcd_hybrid_trace_end(__func__);
	if (ret == LCD_KIT_OK && on_without_switching)
		ret = -EACCES;
	mutex_unlock(&display_ctrl->hybrid_lock);
	return ret;
}

int hybrid_off_switch_ap(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	u8 gesture_status;

	LCD_KIT_INFO("get in\n");

	if (!display_ctrl)
		return -EINVAL;

	mutex_lock(&display_ctrl->hybrid_lock);
	if (hybrid_control_check()) {
		LCD_KIT_INFO("already hold display control, do nothing\n");
		mutex_unlock(&display_ctrl->hybrid_lock);
		return -EACCES;
	}

	display_ctrl->hybrid_control_switching = true;
	set_current_gesture_status(get_gesture_switch());
	gesture_status = get_current_gesture_status();
	if (gesture_status == 1)
		send_hybrid_ts_cmd(TS_KIT_HYBRID_IDLE_TO_AP);
	else
		send_hybrid_ts_cmd(TS_KIT_HYBRID_SLEEP_TO_AP);
	/* request control to ap */
	ret = hybrid_request_display(1);
	if (ret != 0) {
		LCD_KIT_INFO("control mcu->ap failed, abort switching, ret: %d\n", ret);
		goto out;
	}

	if (gesture_status)
		ret = lcd_kit_set_deepsleep(display_ctrl);
	else
		ret = lcd_kit_set_off(display_ctrl);

	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->send_hybrid_state) {
		ret = display_ctrl->display_sw_ops->send_hybrid_state(display_ctrl->display_sw_ops,
			AP_DISPLAY_OFF);
		if (ret < 0)
			LCD_KIT_WARNING("send AP_DISPLAY_OFF status to mcu, ret: %d\n", ret);
	}

	if (ret != 0) {
		LCD_KIT_INFO("lcd_kit_set_off failed, ret: %d, gesture: %d\n", ret, gesture_status);
		ret = -EIO;
	}

out:
	display_ctrl->hybrid_control_switching = false;
	mutex_unlock(&display_ctrl->hybrid_lock);
	return ret;
}

int hybrid_idle_switch_ap(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;

	LCD_KIT_INFO("get in\n");

	if (!display_ctrl)
		return -EINVAL;

	mutex_lock(&display_ctrl->hybrid_lock);
	if (hybrid_control_check()) {
		LCD_KIT_INFO("already hold display control, do nothing\n");
		mutex_unlock(&display_ctrl->hybrid_lock);
		return -EACCES;
	}

	display_ctrl->hybrid_control_switching = true;
	/* request control to ap */
	ret = hybrid_request_display(1);
	if (ret != 0) {
		LCD_KIT_INFO("control mcu->ap failed, abort switching, ret: %d\n", ret);
		goto out;
	}
	send_hybrid_ts_cmd(TS_KIT_HYBRID_IDLE_TO_AP);

	/* power on */
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->power_control) {
		ret = display_ctrl->display_sw_ops->power_control(display_ctrl->display_sw_ops, 1);
		if (ret != 0) {
			LCD_KIT_INFO("power on control failed, abort switching, ret: %d\n", ret);
			goto out;
		}
	}
	lcd_kit_hybrid_esd_off();
	hybrid_dss_on(display_ctrl, COMPOSER_ON_MODE_DOZE);

	ret = lcd_kit_set_idle(display_ctrl);
	hybrid_lcd_status_check(display_ctrl);

out:
	display_ctrl->hybrid_control_switching = false;
	mutex_unlock(&display_ctrl->hybrid_lock);
	return ret;
}

int hybrid_lcd_status_check(struct display_hybrid_ctrl *display_ctrl)
{
	u8 read_value0a = 0;
	u8 read_value52 = 0;
	u8 read_value54 = 0;
	u8 read_value0d = 0;
	u8 read_value0e = 0;
	u8 read_value0f = 0;
	u8 all_pixel_on = 0x10; /* register value 0x10 for all pixel on */

	int panel_id = lcd_kit_get_active_panel_id();

	if (!hybrid_control_check()) {
		LCD_KIT_INFO("control not at\n");
		return 0;
	}

	if (!display_ctrl)
		return -EINVAL;

	lcd_kit_cmds_rx(panel_id, NULL, &read_value0a, sizeof(read_value0a),
		&display_ctrl->hybrid_info.status_cmds);
	lcd_kit_cmds_rx(panel_id, NULL, &read_value52, sizeof(read_value52),
		&display_ctrl->hybrid_info.status_cmds_52);
	lcd_kit_cmds_rx(panel_id, NULL, &read_value54, sizeof(read_value54),
		&display_ctrl->hybrid_info.status_cmds_54);
	lcd_kit_cmds_rx(panel_id, NULL, &read_value0d, sizeof(read_value0d),
		&display_ctrl->hybrid_info.status_cmds_0d);
	lcd_kit_cmds_rx(panel_id, NULL, &read_value0e, sizeof(read_value0e),
		&display_ctrl->hybrid_info.status_cmds_0e);
	lcd_kit_cmds_rx(panel_id, NULL, &read_value0f, sizeof(read_value0f),
		&display_ctrl->hybrid_info.status_cmds_0f);

	LCD_KIT_INFO("read value, 0x52:0x%02x, 0x54:0x%02x, 0x0d:0x%02x, 0x0e:0x%02x, 0x0f:0x%02x, 0x0a:0x%02x\n",
		read_value52, read_value54, read_value0d, read_value0e, read_value0f, read_value0a);

	if ((read_value0d & all_pixel_on) != 0x00) {
		LCD_KIT_ERR("0x0D reg is all pixel on\n");
		return -EFAULT;
	}

	return 0;
}

int hybrid_get_lcd_status(struct display_hybrid_ctrl *display_ctrl, u8 *status)
{
	int ret;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl || !status)
		return LCD_KIT_FAIL;

	ret = lcd_kit_cmds_rx(panel_id, NULL, status, sizeof(*status),
		&display_ctrl->hybrid_info.status_cmds);

	return ret;
}
