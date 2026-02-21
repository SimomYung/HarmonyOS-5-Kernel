/*
 * Huawei Touchscreen Driver
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

#include "huawei_thp_mt_wrapper.h"
#include <linux/input.h>

#ifdef CONFIG_INPUTHUB_20
#include "contexthub_recovery.h"
#endif

#if defined(CONFIG_HUAWEI_TS_KIT_3_0)
#include "../3_0/trace-events-touch.h"
#else
#define trace_touch(x...)
#endif

#define SECOND 1000000
#define NO_DELAY 0
#define DELAY_LIMIT_FRAME_COUNT 5

#if ((!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK))
static void thp_adjustment_coordinate(u32 panel_id)
{
	unsigned int delta_time;
	unsigned int delay_time;
	unsigned int time_interval;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (!cd->frame_count)
		time_interval = cd->time_interval;
	else if (cd->frame_count < DELAY_LIMIT_FRAME_COUNT)
		time_interval = cd->time_min_interval;
	else
		time_interval = NO_DELAY;
	do_timetransfer(&cd->report_cur_time);
	delta_time = ((cd->report_cur_time.tv_sec -
		cd->report_pre_time.tv_sec) * SECOND) +
		(cd->report_cur_time.tv_nsec - cd->report_pre_time.tv_nsec);

	if (delta_time < time_interval) {
		delay_time = time_interval - delta_time;
		udelay(delay_time);
		do_timetransfer(&cd->report_cur_time);
	}
	cd->report_pre_time = cd->report_cur_time;
}
#endif

static void thp_report_sync_time(struct thp_mt_wrapper_ioctl_touch_data *data,
	u32 panel_id, unsigned int have_time_data)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *input_dev = cd->thp_mt_wrapper->input_dev;
	struct thp_ioctl_touch_and_time_data *touch_and_time_data = NULL;

	if (data->t_num != 0 && cd->tp_time_sync_support && have_time_data) {
		touch_and_time_data = (struct thp_ioctl_touch_and_time_data *)data;
		input_report_abs(input_dev, ABS_DELTA_TIME,
			touch_and_time_data->delta_time);
		input_report_abs(input_dev, ABS_T_SEC,
			touch_and_time_data->t_sec);
		input_report_abs(input_dev, ABS_T_USEC,
			touch_and_time_data->t_usec);
		thp_log_debug("%s:delta_time = %u, t_sec = %u, t_usec = %u\n",
			__func__, touch_and_time_data->delta_time,
			touch_and_time_data->t_sec, touch_and_time_data->t_usec);
	}
}

static void thp_report_abs(struct thp_mt_wrapper_ioctl_touch_data *data,
	struct input_dev *input_dev)
{
	unsigned int i;

	for (i = 0; i < INPUT_MT_WRAPPER_MAX_FINGERS; i++) {
#ifdef TYPE_B_PROTOCOL
		input_mt_slot(input_dev, i);
		input_mt_report_slot_state(input_dev,
			data->touch[i].tool_type, data->touch[i].valid != 0);
#endif
		if (data->touch[i].valid != 0) {
			input_report_abs(input_dev, ABS_MT_POSITION_X,
						data->touch[i].x);
			input_report_abs(input_dev, ABS_MT_POSITION_Y,
						data->touch[i].y);
			input_report_abs(input_dev, ABS_MT_PRESSURE,
						data->touch[i].pressure);
			input_report_abs(input_dev, ABS_MT_TRACKING_ID,
						data->touch[i].tracking_id);
			input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR,
						data->touch[i].major);
			input_report_abs(input_dev, ABS_MT_TOUCH_MINOR,
						data->touch[i].minor);
			input_report_abs(input_dev, ABS_MT_ORIENTATION,
						data->touch[i].orientation);
			input_report_abs(input_dev, ABS_MT_TOOL_TYPE,
						data->touch[i].tool_type);
			input_report_abs(input_dev, ABS_MT_BLOB_ID,
						data->touch[i].hand_side);
#ifndef TYPE_B_PROTOCOL
			input_mt_sync(input_dev);
#endif
		}
	}
}

static void thp_coordinate_report(struct thp_mt_wrapper_ioctl_touch_data *data,
	unsigned int have_time_data, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *input_dev = cd->thp_mt_wrapper->input_dev;

	thp_report_sync_time(data, cd->panel_id, have_time_data);

	thp_report_abs(data, input_dev);

	/* BTN_TOUCH DOWN */
	if (data->t_num > 0)
		input_report_key(input_dev, BTN_TOUCH, 1);
	/* BTN_TOUCH UP */
	if (data->t_num == 0) {
#ifndef TYPE_B_PROTOCOL
		input_mt_sync(input_dev);
#endif
		input_report_key(input_dev, BTN_TOUCH, 0);
	}
	input_sync(input_dev);
}

static long thp_mt_wrapper_ioctl_set_coordinate(unsigned long arg,
	unsigned int have_time_data, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_mt_wrapper_ioctl_touch_data data;
	struct thp_ioctl_touch_and_time_data touch_and_time_data;
#if (!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK)
	struct thp_core_data *cd = thp_get_core_data(panel_id);
#endif

	trace_touch(TOUCH_TRACE_ALGO_SET_EVENT, TOUCH_TRACE_FUNC_IN, "thp");
	if (arg == 0) {
		thp_log_err("%s:arg is null\n", __func__);
		return -EINVAL;
	}

	if (!have_time_data) {
		if (copy_from_user(&data, argp, sizeof(data))) {
			thp_log_err("Failed to copy_from_user()\n");
			return -EFAULT;
		}
	} else {
		if (copy_from_user(&touch_and_time_data, argp, sizeof(touch_and_time_data))) {
			thp_log_err("Failed to copy_from_user()\n");
			return -EFAULT;
		}
	}
	trace_touch(TOUCH_TRACE_ALGO_SET_EVENT, TOUCH_TRACE_FUNC_OUT, "thp");

	trace_touch(TOUCH_TRACE_INPUT, TOUCH_TRACE_FUNC_IN, "thp");
#if ((!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK))
	if ((cd->support_interval_adjustment) && (cd->time_adjustment_switch))
		thp_adjustment_coordinate(panel_id);
#endif
	if (!have_time_data)
		thp_coordinate_report(&data, have_time_data, panel_id);
	else
		thp_coordinate_report((struct thp_mt_wrapper_ioctl_touch_data *)&touch_and_time_data,
			have_time_data, panel_id);
	trace_touch(TOUCH_TRACE_INPUT, TOUCH_TRACE_FUNC_OUT, "thp");
	return NO_ERR;
}

int thp_mt_wrapper_ioctl_cmd_set_coordinates(unsigned long arg, u32 panel_id)
{
	return thp_mt_wrapper_ioctl_set_coordinate(arg, 0, panel_id);
}

int thp_mt_wrapper_ioctl_cmd_set_coordinates_and_time(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (!cd) {
		thp_log_err("%s:thp_core_data is NULL\n", __func__);
		return -EINVAL;
	}

	if (cd->tp_time_sync_support)
		return thp_mt_wrapper_ioctl_set_coordinate(arg, 1, panel_id);
	else
		return -EINVAL;
}

static void thp_report_pen_button(struct input_dev *input,
	struct thp_mt_wrapper_ioctl_pen_data *pens)
{
	int i;
	int key_value;

	for (i = 0; i < TS_MAX_PEN_BUTTON; i++) {
		if (pens->buttons[i].status == 0)
			continue;
		else if (pens->buttons[i].status == TS_PEN_BUTTON_PRESS)
			key_value = 1; /* key down */
		else
			key_value = 0; /* key up */
		if (pens->buttons[i].key != 0) {
			thp_log_err("pen index is %d\n", i);
			input_report_key(input, pens->buttons[i].key, key_value);
		}
	}
}

static void thp_report_pen_event(struct input_dev *input, struct thp_tool tool)
{
	thp_log_debug("%s:tool.tip_status:%d, tool_type:%d, tool_value:%d\n",
		__func__, tool.tip_status, tool.tool_type, tool.pen_inrange_status);
	input_report_abs(input, ABS_X, tool.x);
	input_report_abs(input, ABS_Y, tool.y);
	input_report_abs(input, ABS_PRESSURE, tool.pressure);
	input_report_abs(input, ABS_TILT_X, tool.tilt_x);
	input_report_abs(input, ABS_TILT_Y, tool.tilt_y);
	input_report_key(input, BTN_TOUCH, tool.tip_status);
	input_report_key(input, tool.tool_type, tool.pen_inrange_status);
	input_sync(input);
}

static void thp_report_pen_extension(struct thp_mt_wrapper_ioctl_pen_data *data,
	u32 panel_id, unsigned int have_pen_extension)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct thp_ioctl_pen_and_extension_data *touch_and_time_data = NULL;
	struct input_dev *input = cd->thp_mt_wrapper->pen_dev;

	if (have_pen_extension) {
		touch_and_time_data = (struct thp_ioctl_pen_and_extension_data *)data;
		thp_log_debug("%s:touch_and_time_data->twist = %d\n",
			__func__, touch_and_time_data->twist);
		input_report_abs(input, ABS_MT_TWIST, touch_and_time_data->twist);
	}
}

static void thp_pen_event_report(struct thp_mt_wrapper_ioctl_pen_data *data,
	unsigned int have_pen_extension, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *input = cd->thp_mt_wrapper->pen_dev;

	/* report pen extension feature */
	thp_report_pen_extension(data, cd->panel_id, have_pen_extension);

	/* report pen basic single button */
	thp_report_pen_button(input, data);

	/* pen or rubber report point */
	thp_report_pen_event(input, data->tool);
}

static long thp_mt_wrapper_ioctl_set_pen_event(unsigned long arg,
	unsigned int have_pen_extension, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_mt_wrapper_ioctl_pen_data pen;
	struct thp_ioctl_pen_and_extension_data pen_and_extension;

	if (arg == 0) {
		thp_log_err("%s:arg is null\n", __func__);
		return -EINVAL;
	}

	if (!have_pen_extension) {
		if (copy_from_user(&pen, argp, sizeof(pen))) {
			thp_log_err("Failed to copy_from_user()\n");
			return -EFAULT;
		}
		thp_pen_event_report(&pen, have_pen_extension, panel_id);
	} else {
		if (copy_from_user(&pen_and_extension, argp, sizeof(pen_and_extension))) {
			thp_log_err("Failed to copy_from_user()\n");
			return -EFAULT;
		}
		thp_pen_event_report((struct thp_mt_wrapper_ioctl_pen_data *)&pen_and_extension,
			have_pen_extension, panel_id);
	}

	return NO_ERR;
}

int thp_mt_wrapper_ioctl_report_pen(unsigned long arg, u32 panel_id)
{
	return thp_mt_wrapper_ioctl_set_pen_event(arg, 0, panel_id);
}

int thp_mt_wrapper_ioctl_report_pen_and_extension(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (!cd) {
		thp_log_err("%s:thp_core_data is NULL\n", __func__);
		return -EINVAL;
	}

	if (cd->support_stylus_extension)
		return thp_mt_wrapper_ioctl_set_pen_event(arg, 1, panel_id);
	else
		return -EINVAL;
}

int thp_mt_ioctl_read_input_config(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct thp_input_dev_config __user *config =
		(struct thp_input_dev_config *)(uintptr_t)arg;
	struct thp_input_dev_config *input_config =
			&cd->thp_mt_wrapper->input_dev_config;

	if (!config) {
		thp_log_err("%s:input null\n", __func__);
		return -EINVAL;
	}

	if (copy_to_user(config, input_config,
			sizeof(struct thp_input_dev_config))) {
		thp_log_err("%s:copy input config failed\n", __func__);
		return -EFAULT;
	}

	return NO_ERR;
}

int thp_mt_ioctl_report_keyevent(unsigned long arg, u32 panel_id)
{
	int report_value[PROX_VALUE_LEN] = {0};
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *input_dev = cd->thp_mt_wrapper->input_dev;
	void __user *argp = (void __user *)(uintptr_t)arg;
	enum input_mt_wrapper_keyevent keyevent;

	if (arg == 0) {
		thp_log_err("%s:arg is null\n", __func__);
		return -EINVAL;
	}
	if (copy_from_user(&keyevent, argp,
			sizeof(enum input_mt_wrapper_keyevent))) {
		thp_log_err("Failed to copy_from_user()\n");
		return -EFAULT;
	}

	if (keyevent == INPUT_MT_WRAPPER_KEYEVENT_ESD) {
		input_report_key(input_dev, KEY_F26, 1);
		input_sync(input_dev);
		input_report_key(input_dev, KEY_F26, 0);
		input_sync(input_dev);
#if defined(CONFIG_HUAWEI_DSM)
		thp_dmd_report(DSM_TP_ESD_ERROR_NO,
			"%s, KEYEVENT ESD\n", __func__);
#endif
	} else if (keyevent == INPUT_MT_WRAPPER_KEYEVENT_APPROACH) {
		thp_log_info("[Proximity_feature] %s: report [near] event!\n",
			__func__);
		report_value[0] = APPROCH_EVENT_VALUE;
#if ((defined CONFIG_INPUTHUB_20) || (defined CONFIG_HUAWEI_PS_SENSOR) || \
	(defined CONFIG_HUAWEI_SENSORS_2_0))
		thp_prox_event_report(report_value, PROX_EVENT_LEN);
#endif
	} else if (keyevent == INPUT_MT_WRAPPER_KEYEVENT_AWAY) {
		thp_log_info("[Proximity_feature] %s: report [far] event!\n",
			__func__);
		report_value[0] = AWAY_EVENT_VALUE;
#if ((defined CONFIG_INPUTHUB_20) || (defined CONFIG_HUAWEI_PS_SENSOR) || \
	(defined CONFIG_HUAWEI_SENSORS_2_0))
		thp_prox_event_report(report_value, PROX_EVENT_LEN);
#endif
	}

	return NO_ERR;
}

static int thp_report_system_event(struct thp_key_info *key_info, u32 panel_id)
{
	struct input_dev *input_dev = NULL;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	unsigned int is_valid_key;
	unsigned int is_valid_action;

	if ((cd == NULL) || (!cd->support_extra_key_event_input) ||
		(cd->thp_mt_wrapper->extra_key_dev == NULL)) {
		thp_log_err("%s:input is invalid\n", __func__);
		return -EINVAL;
	}
	input_dev = cd->thp_mt_wrapper->extra_key_dev;
	thp_log_info("%s Ring-Vibrate : key: %d, value: %d\n",
		__func__, key_info->key, key_info->action);
	is_valid_key = (key_info->key != KEY_VOLUME_UP) &&
		(key_info->key != KEY_VOLUME_DOWN) &&
		(key_info->key != KEY_POWER) &&
		(key_info->key != KEY_VOLUME_MUTE) &&
		(key_info->key != KEY_VOLUME_TRIG);
	if (is_valid_key) {
		thp_log_err("%s:key is invalid\n", __func__);
		return -EINVAL;
	}
	is_valid_action = (key_info->action != THP_KEY_UP) &&
		(key_info->action != THP_KEY_DOWN);
	if (is_valid_action) {
		thp_log_err("%s:action is invalid\n", __func__);
		return -EINVAL;
	}

	input_report_key(input_dev, key_info->key,
		key_info->action);
	input_sync(input_dev);
	return NO_ERR;
}

#ifdef CONFIG_HUAWEI_SHB_THP
int thp_send_volumn_to_drv(const char *head)
{
	struct thp_volumn_info *rx = (struct thp_volumn_info *)head;
	struct thp_key_info key_info;
	struct thp_core_data *cd = thp_get_core_data(MAIN_TOUCH_PANEL);

	if ((rx == NULL) || (cd == NULL)) {
		thp_log_err("%s:rx or cd is null\n", __func__);
		return -EINVAL;
	}
	if (!atomic_read(&cd->register_flag)) {
		thp_log_err("%s: thp have not be registered\n", __func__);
		return -ENODEV;
	}
	__pm_wakeup_event(cd->thp_wake_lock, jiffies_to_msecs(HZ));
	thp_log_info("%s:key:%ud, action:%ud\n", __func__,
		rx->data[0], rx->data[1]);
	key_info.key = rx->data[0];
	key_info.action = rx->data[1];
	return thp_report_system_event(&key_info, cd->panel_id);
}
#endif

int thp_mt_ioctl_report_system_keyevent(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_key_info key_info;

	if (arg == 0) {
		thp_log_err("%s:arg is null\n", __func__);
		return -EINVAL;
	}
	if (memset_s(&key_info, sizeof(key_info), 0, sizeof(key_info))) {
		thp_log_err("%s: memset fail\n", __func__);
		return -EINVAL;
	}
	if (copy_from_user(&key_info, argp, sizeof(key_info))) {
		thp_log_err("Failed to copy_from_user()\n");
		return -EFAULT;
	}
	return thp_report_system_event(&key_info, panel_id);
}

int thp_mt_wrapper_ioctl_report_hand_status(unsigned long arg, u32 panel_id)
{
	struct thp_hand_status hand_status;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *input = cd->thp_mt_wrapper->hand_status_dev;
	void __user *argp = NULL;

	if ((arg == 0) || (input == NULL)) {
		thp_log_err("%s:have null ptr\n", __func__);
		return -EINVAL;
	}
	argp = (void __user *)(uintptr_t)arg;
	if (copy_from_user(&hand_status, argp, sizeof(hand_status))) {
		thp_log_err("Failed to copy_from_user\n");
		return -EFAULT;
	}

	input_report_abs(input, ABS_HAND_FEATURE, hand_status.hand_feature);
	input_sync(input);
	return NO_ERR;
}