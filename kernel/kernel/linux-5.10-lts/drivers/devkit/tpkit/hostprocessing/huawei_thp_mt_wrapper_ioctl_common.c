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
#include <linux/init.h>

#ifdef CONFIG_HUAWEI_PS_SENSOR
#include "ps_sensor.h"
#endif

#if (!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK)
#include "ps/ps_channel.h"
#endif

#ifdef CONFIG_HUAWEI_SENSORS_2_0
#include "sensor_scp.h"
#endif

#ifdef CONFIG_HUAWEI_THP_QCOM
#include "ap_sensor_route.h"
#endif

#if ((!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK))
unsigned int get_boot_into_recovery_flag(void);
#endif
static unsigned int get_recovery_flag(void);

#define MAX_MAGNIFICATION 16

#ifdef CONFIG_HUAWEI_THP_QCOM
static bool thp_cmdline_is_recovery_mode(void)
{
	if (strstr(saved_command_line, "enter_recovery=1"))
		return true;

	return false;
}
static bool thp_cmdline_is_erecovery_mode(void)
{
	if (strstr(saved_command_line, "enter_erecovery=1"))
		return true;

	return false;
}

static bool get_boot_into_recovery_flag_qcom(void)
{
	bool recovery_mode = false;

	if (thp_cmdline_is_recovery_mode() | thp_cmdline_is_erecovery_mode())
		recovery_mode = true;
	else
		recovery_mode = false;
	thp_log_info("%s recovery_mode = %u\n", __func__, recovery_mode);
	return recovery_mode;
}
#endif

static unsigned int get_recovery_flag(void)
{
	unsigned int recovery_flag = 0;

#if ((!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK))
	recovery_flag = get_boot_into_recovery_flag();
#endif
#ifdef CONFIG_HUAWEI_THP_QCOM
	recovery_flag = get_boot_into_recovery_flag_qcom();
#endif
	return recovery_flag;
}

unsigned int thp_get_finger_resolution_magnification(u32 panel_id)
{
	unsigned int ret;
	unsigned int recovery_flag;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	recovery_flag = get_recovery_flag();
	if ((cd->finger_resolution_magnification > MAX_MAGNIFICATION) ||
		(cd->finger_resolution_magnification == 0) ||
		(recovery_flag != 0))
		ret = DEFAULT_MAGNIFICATION;
	else
		ret = cd->finger_resolution_magnification;
	thp_log_info("%s:recovery_flag = %u, ret = %u\n",
		__func__, recovery_flag, ret);
	return ret;
}

int thp_mt_ioctl_read_finger_resolution_magnification(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	unsigned int magnification;

	if (argp == NULL) {
		thp_log_err("%s:argp is NULL\n", __func__);
		return -EINVAL;
	}
	magnification = thp_get_finger_resolution_magnification(panel_id);
	if (copy_to_user(argp, (void *)&magnification, sizeof(magnification))) {
		thp_log_err("%s: Failed to copy_to_user()\n", __func__);
		return -EFAULT;
	}
	return NO_ERR;
}

int thp_mt_wrapper_ioctl_read_status(unsigned long arg, u32 panel_id)
{
	int __user *status = (int *)(uintptr_t)arg;
	u32 thp_status = thp_get_status_all(panel_id);
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	thp_log_info("%s:status = 0x%x\n", __func__, thp_status);

	if (!status) {
		thp_log_err("%s:input null\n", __func__);
		return -EINVAL;
	}

	if (copy_to_user(status, &thp_status, sizeof(u32))) {
		thp_log_err("%s:copy status failed\n", __func__);
		return -EFAULT;
	}

	if (atomic_read(&cd->thp_mt_wrapper->status_updated) != 0)
		atomic_dec(&cd->thp_mt_wrapper->status_updated);

	return NO_ERR;
}

int thp_mt_wrapper_ioctl_read_scene_info(unsigned long arg, u32 panel_id)
{
	struct thp_scene_info __user *config =
		(struct thp_scene_info *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct thp_scene_info *scene_info = NULL;

	if (!config || !cd) {
		thp_log_err("%s:null ptr\n", __func__);
		return -EINVAL;
	}
	scene_info = &(cd->scene_info);

	thp_log_info("%s:%d,%d,%d\n", __func__,
		scene_info->type, scene_info->status, scene_info->parameter);

	if (copy_to_user(config, scene_info, sizeof(struct thp_scene_info))) {
		thp_log_err("%s:copy scene_info failed\n", __func__);
		return -EFAULT;
	}

	return NO_ERR;
}

int thp_mt_wrapper_ioctl_get_window_info(unsigned long arg, u32 panel_id)
{
	struct thp_window_info __user *window_info =
		(struct thp_window_info *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if ((!cd) || (!window_info)) {
		thp_log_err("%s:args error\n", __func__);
		return -EINVAL;
	}

	thp_log_info("%s:x0=%d,y0=%d,x1=%d,y1=%d\n", __func__,
		cd->window.x0, cd->window.y0, cd->window.x1, cd->window.y1);

	if (copy_to_user(window_info, &cd->window,
		sizeof(struct thp_window_info))) {
		thp_log_err("%s:copy window_info failed\n", __func__);
		return -EFAULT;
	}

	return NO_ERR;
}

int thp_mt_wrapper_ioctl_get_projectid(unsigned long arg, u32 panel_id)
{
	char __user *project_id = (char __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if ((!cd) || (!project_id)) {
		thp_log_err("%s:args error\n", __func__);
		return -EINVAL;
	}

	thp_log_info("%s:project id:%s\n", __func__, cd->project_id);

	if (copy_to_user(project_id, cd->project_id, sizeof(cd->project_id))) {
		thp_log_err("%s:copy project_id failed\n", __func__);
		return -EFAULT;
	}

	return NO_ERR;
}

#define ANTI_TOUCH_DETECTED 1
#define ANTI_TOUCH_RELEASE 0
#ifndef CONFIG_MODULE_KO_TP
__attribute__((weak)) int tp_event_to_ultrasonic(uint8_t event)
{
	hwlog_info("%s:not listen to the function properply, in weak function now\n",
			__func__);
	return NO_ERR;
}
#endif

static void thp_send_event_to_ultrasonic(unsigned int value, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (!cd->tp_assisted_ultrasonic_algo)
		return;

	if (value & THP_EVENT_ANTI_TOUCH_DETECTED) {
		tp_event_to_ultrasonic(ANTI_TOUCH_DETECTED);
		thp_log_info("%s:send tp_event to sensor :0x%x\n",
			__func__, value);
	} else if (value & THP_EVENT_ANTI_TOUCH_RELEASE) {
		tp_event_to_ultrasonic(ANTI_TOUCH_RELEASE);
		thp_log_info("%s:send tp_event to sensor :0x%x\n",
			 __func__, value);
	}
}

#ifndef CONFIG_MODULE_KO_TP
#if ((defined CONFIG_LCD_KIT_DRIVER) || (defined CONFIG_HW_EDP_DRIVER))

#define TOUCH_DETECTED 1
#define TOUCH_RELEASE 0

static void thp_send_event_to_de(unsigned int value, u32 panel_id)
{
#ifdef CONFIG_LCD_KIT_DRIVER
	struct lcd_kit_ops *ops = lcd_kit_get_ops();
#else
	struct edp_tp_safe_rate_ops *ops = edp_tp_safe_rate_get_ops();
#endif
	if (!ops)
		return;

	if (value & THP_EVENT_DE_TOUCH_DETECTED) {
		ops->send_de_event(TOUCH_DETECTED, panel_id);
		thp_log_info("%s:send tp_event to de :0x%x\n",
			__func__, value);
	} else if (value & THP_EVENT_DE_TOUCH_RELEASE) {
		ops->send_de_event(TOUCH_RELEASE, panel_id);
		thp_log_info("%s:send tp_event to de :0x%x\n",
			 __func__, value);
	}
}
#endif
#endif

int thp_mt_wrapper_ioctl_set_events(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	void __user *argp = (void __user *)(uintptr_t)arg;
	int val;

	if (arg == 0) {
		thp_log_err("%s:arg is null\n", __func__);
		return -EINVAL;
	}
	if (copy_from_user(&val, argp, sizeof(int))) {
		thp_log_err("Failed to copy_from_user()\n");
		return -EFAULT;
	}
	thp_log_info("thp_send, write: 0x%x\n", val);
	cd->event_flag = true;
	cd->event = val;
	thp_send_event_to_ultrasonic(val, panel_id);
#ifndef CONFIG_MODULE_KO_TP
#if ((defined CONFIG_LCD_KIT_DRIVER) || (defined CONFIG_HW_EDP_DRIVER))
	thp_send_event_to_de(val, panel_id);
#endif
#endif
	if (cd->event_flag) {
		cd->thp_event_waitq_flag = WAITQ_WAKEUP;
		wake_up_interruptible(&cd->thp_event_waitq);
		thp_log_info("%d: wake_up\n", cd->event);
	}

	return NO_ERR;
}

int thp_mt_wrapper_ioctl_get_events(unsigned long event, u32 panel_id)
{
	int t;
	int __user *events = (int *)(uintptr_t)event;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if ((!cd) || (!events)) {
		thp_log_info("%s: input null\n", __func__);
		return -ENODEV;
	}

	thp_log_info("%d: cd->event_flag\n", cd->event_flag);
	if (cd->event_flag) {
		if (copy_to_user(events, &cd->event, sizeof(cd->event))) {
			thp_log_err("%s:copy events failed\n", __func__);
			return -EFAULT;
		}

		cd->event_flag = false;
	} else {
		cd->thp_event_waitq_flag = WAITQ_WAIT;
		t = wait_event_interruptible(cd->thp_event_waitq,
			(cd->thp_event_waitq_flag == WAITQ_WAKEUP));
		thp_log_info("%s: set wait finish :%d\n", __func__, t);
	}

	return NO_ERR;
}

int thp_mt_wrapper_ioctl_set_roi_data(unsigned long arg, u32 panel_id)
{
	short __user *roi_data = (short __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if ((!cd) || (!roi_data)) {
		thp_log_err("%s:args error\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(cd->roi_data, roi_data, sizeof(cd->roi_data))) {
		thp_log_err("%s:copy roi data failed\n", __func__);
		return -EFAULT;
	}

	return NO_ERR;
}

int thp_mt_wrapper_ioctl_get_platform_type(unsigned long arg, u32 panel_id)
{
	int __user *platform_type = (int __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if ((!cd) || (!platform_type)) {
		thp_log_info("%s: input null\n", __func__);
		return -ENODEV;
	}

	thp_log_info("%s: cd->platform_type %d\n", __func__, cd->platform_type);

	if (copy_to_user(platform_type, &cd->platform_type, sizeof(cd->platform_type))) {
		thp_log_err("%s:copy platform_type failed\n", __func__);
		return -EFAULT;
	}

	return NO_ERR;
}

#if (defined(CONFIG_HUAWEI_THP_MTK) || defined(CONFIG_HUAWEI_THP_QCOM))
static int thp_notify_fp_event(struct thp_shb_info info)
{
	int ret;
	struct ud_fp_ops *fp_ops = NULL;
	struct tp_to_udfp_data fp_data = {0};
	struct thp_core_data *cd = thp_get_core_data(MAIN_TOUCH_PANEL);

	if (!cd || !cd->use_ap_gesture) {
		thp_log_info("%s: not support ap handle udfp\n", __func__);
		return -EINVAL;
	}
	if (info.cmd_type != THP_FINGER_PRINT_EVENT) {
		thp_log_info("%s: only handle fp event, return\n", __func__);
		return NO_ERR;
	}

	thp_log_info("%s: called, sizeof(tp_to_udfp_data):%u, cmd_len:%u\n",
		__func__, sizeof(struct tp_to_udfp_data), info.cmd_len);
	fp_ops = fp_get_ops();
	if (!fp_ops || !fp_ops->fp_irq_notify) {
		thp_log_err("%s: point is NULL!\n", __func__);
		return -EINVAL;
	}

	if (info.cmd_len > sizeof(struct tp_to_udfp_data)) {
		thp_log_err("%s: cmd_len:%u is illegal\n", __func__,
			info.cmd_len);
		return -EINVAL;
	}

	if (memcpy_s(&fp_data, sizeof(fp_data), info.cmd_addr, info.cmd_len)) {
		thp_log_err("%s: memcpy fail\n", __func__);
		return -EINVAL;
	}
	fp_data.tp_source = UDFP_DATA_SOURCE_THP;

	ret = fp_ops->fp_irq_notify(&fp_data, sizeof(struct tp_to_udfp_data));
	if (ret)
		thp_log_err("%s: fp_irq_notify fail, ret %d\n", __func__, ret);
	return ret;
}
#endif

#ifdef CONFIG_HUAWEI_SHB_THP
static int thp_event_info_dispatch(struct thp_shb_info info)
{
	int ret;
	unsigned int cmd_type = info.cmd_type;
	uint8_t cmd;

	switch (cmd_type) {
	case THP_FINGER_PRINT_EVENT:
		cmd = ST_CMD_TYPE_FINGERPRINT_EVENT;
		ret = send_thp_ap_event(info.cmd_len, info.cmd_addr, cmd);
		break;
	case THP_RING_EVENT:
		cmd = ST_CMD_TYPE_RING_EVENT;
		ret = send_thp_ap_event(info.cmd_len, info.cmd_addr, cmd);
		break;
	case THP_ALGO_SCREEN_OFF_INFO:
		ret = send_thp_algo_sync_event(info.cmd_len, info.cmd_addr);
		break;
	case THP_AUXILIARY_DATA:
		ret = send_thp_auxiliary_data(info.cmd_len, info.cmd_addr);
		break;
	default:
		thp_log_err("%s: thp_shb_info is null\n", __func__);
		ret = -EFAULT;
	}
	return ret;
}
#endif

#if ((defined CONFIG_HUAWEI_SHB_THP) || (defined CONFIG_HUAWEI_THP_MTK) || (defined CONFIG_HUAWEI_THP_QCOM))
static int thp_get_shb_info(unsigned long arg, struct thp_shb_info *data)
{
	void __user *argp = (void __user *)(uintptr_t)arg;

	if (arg == 0) {
		thp_log_err("%s:arg is null.\n", __func__);
		return -EINVAL;
	}
	if (copy_from_user(data, argp, sizeof(struct thp_shb_info))) {
		thp_log_err("%s:copy info failed\n", __func__);
		return -EFAULT;
	}
	if ((data->cmd_len > MAX_THP_CMD_INFO_LEN) || (data->cmd_len == 0)) {
		thp_log_err("%s:cmd_len:%u is illegal\n", __func__,
			data->cmd_len);
		return -EINVAL;
	}
	return NO_ERR;
}

static int thp_get_shb_cmd_data(char **cmd_data, struct thp_shb_info data)
{
	*cmd_data = kzalloc(data.cmd_len, GFP_KERNEL);
	if ((*cmd_data) == NULL) {
		thp_log_err("%s:cmd buffer kzalloc failed\n", __func__);
		return -EFAULT;
	}
	if (copy_from_user(*cmd_data, data.cmd_addr, data.cmd_len)) {
		thp_log_err("%s:copy cmd data failed\n", __func__);
		kfree(*cmd_data);
		return -EFAULT;
	}
	return NO_ERR;
}

#ifdef CONFIG_HUAWEI_SHB_THP
#define OUT 1
static int thp_send_shb_udfp_event(struct thp_shb_info data, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	int ret;

	if (cd->tsa_event_to_udfp) {
		ret = send_tp_ap_event(data.cmd_len, data.cmd_addr,
			ST_CMD_TYPE_FINGERPRINT_EVENT);
		if (ret < 0)
			thp_log_err("%s:tsa_event notify fp err %d\n",
				__func__, ret);
		return OUT;
	}
	ret = thp_event_info_dispatch(data);
	if (ret < 0)
		thp_log_err("%s:thp event info dispatch failed\n", __func__);
	return NO_ERR;
}
#endif

int thp_mt_ioctl_cmd_shb_event(unsigned long arg, u32 panel_id)
{
	int ret;
	struct thp_shb_info data;
	char *cmd_data = NULL;

	ret = thp_get_shb_info(arg, &data);
	if (ret) {
		thp_log_err("%s:get shb info fail\n", __func__);
		return ret;
	}

	ret = thp_get_shb_cmd_data(&cmd_data, data);
	if (ret) {
		thp_log_err("%s:get shb data fail\n", __func__);
		return ret;
	}
	data.cmd_addr = cmd_data;
#ifdef CONFIG_HUAWEI_SHB_THP
	if (thp_send_shb_udfp_event(data, panel_id)) {
		kfree(cmd_data);
		return NO_ERR;
	}
#endif
#if (defined(CONFIG_HUAWEI_THP_MTK) || defined(CONFIG_HUAWEI_THP_QCOM))
	ret = thp_notify_fp_event(data);
	if (ret < 0)
		thp_log_err("%s:tpud event notify fp err %d\n", __func__, ret);
#endif
	kfree(cmd_data);
	return NO_ERR;
}
#endif

int thp_ioctl_get_volume_side(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	void __user *status = (void __user *)(uintptr_t)arg;

	if (cd == NULL) {
		thp_log_err("%s: null ptr\n", __func__);
		return -EINVAL;
	}
	if (status == NULL) {
		thp_log_err("%s: input parameter null\n", __func__);
		return -EINVAL;
	}

	if (copy_to_user(status, (void *)&cd->volume_side_status,
		sizeof(cd->volume_side_status))) {
		thp_log_err("%s: get volume side failed\n", __func__);
		return -EFAULT;
	}

	return NO_ERR;
}

int thp_ioctl_get_power_switch(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	void __user *status = (void __user *)(uintptr_t)arg;

	if ((cd == NULL) || (status == NULL)) {
		thp_log_err("%s: null ptr\n", __func__);
		return -EINVAL;
	}

	if (copy_to_user(status, (void *)&cd->power_switch,
		sizeof(cd->power_switch))) {
		thp_log_err("%s: get power_switch failed\n", __func__);
		return -EFAULT;
	}
	return NO_ERR;
}

int ioctl_get_app_info(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct thp_app_info app_info;

	if (argp == NULL) {
		thp_log_err("%s: null ptr\n", __func__);
		return -EINVAL;
	}
	if (memset_s(&app_info, sizeof(app_info), 0, sizeof(app_info))) {
		thp_log_err("%s: memset fail\n", __func__);
		return -EINVAL;
	}
	if (copy_from_user(&app_info, argp, sizeof(app_info))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	if ((app_info.len > MAX_APP_INFO_LEN) || (app_info.len == 0) ||
		(app_info.buf == NULL)) {
		thp_log_err("%s invalid data\n", __func__);
		return -EINVAL;
	}
	if (copy_to_user(app_info.buf, cd->app_info, app_info.len)) {
		thp_log_err("%s Failed to copy_to_user\n", __func__);
		return -EINVAL;
	}
	return NO_ERR;
}

int daemon_init_protect(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	u32 daemon_flag;

	if (argp == NULL) {
		thp_log_err("%s: null ptr\n", __func__);
		return -EINVAL;
	}

	if (!cd->support_daemon_init_protect) {
		thp_log_err("%s: not support daemon init protect\n", __func__);
		return NO_ERR;
	}

	if (copy_from_user(&daemon_flag, argp, sizeof(daemon_flag))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}

	thp_log_info("%s called,daemon_flag = %u\n", __func__, daemon_flag);
	if (get_recovery_flag()) {
		mutex_lock(&cd->daemon_init_mutex);
		thp_log_info("in recovery mode, need lock!");
	}
	if (daemon_flag) {
		atomic_set(&(cd->fw_update_protect), 1);
	} else {
		atomic_set(&(cd->fw_update_protect), 0);
		if (atomic_read(&(cd->resend_suspend_after_fw_update)) == 1) {
			thp_log_info("%s: fw update complete, need resend suspend cmd\n",
				__func__);
			atomic_set(&(cd->resend_suspend_after_fw_update), 0);
			thp_set_status(THP_STATUS_POWER, THP_SUSPEND, cd->panel_id);
			mdelay(5); /* delay 5ms to wait for daemon reading status */
#if defined(CONFIG_LCD_KIT_DRIVER)
			thp_power_control_notify(TS_BEFORE_SUSPEND, 0);
#endif
		}
	}
	if (get_recovery_flag()) {
		mutex_unlock(&cd->daemon_init_mutex);
		thp_log_info("in recovery mode,unlock daemon_init_mutex!");
	}
	return NO_ERR;
}

int thp_daemon_power_reset(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	u32 daemon_flag;
#if defined(CONFIG_LCD_KIT_DRIVER)
	int err;
#endif

	if (argp == NULL) {
		thp_log_err("%s: null ptr\n", __func__);
		return -EINVAL;
	}

	if (cd->multi_panel_index == SINGLE_TOUCH_PANEL) {
		thp_log_err("%s: not support daemon power reset\n", __func__);
		return NO_ERR;
	}

	if (copy_from_user(&daemon_flag, argp, sizeof(daemon_flag))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}

	thp_log_info("%s called,daemon_flag = %u\n", __func__, daemon_flag);
	if ((daemon_flag != MAIN_TOUCH_PANEL) &&
		(daemon_flag != SUB_TOUCH_PANEL)) {
		thp_log_err("%s invalid arg\n", __func__);
		return -EINVAL;
	}
#if defined(CONFIG_LCD_KIT_DRIVER)
	err = thp_multi_power_control_notify(TS_EARLY_SUSPEND,
		SHORT_SYNC_TIMEOUT, daemon_flag);
	if (err)
		thp_log_err("%s: TS_EARLY_SUSPEND fail\n", __func__);
	msleep(200); /* delay 200ms to wait ic suspend done */
	err = thp_multi_power_control_notify(TS_RESUME_DEVICE,
		SHORT_SYNC_TIMEOUT, daemon_flag);
	if (err)
		thp_log_err("%s: TS_EARLY_SUSPEND fail\n", __func__);
#endif
	return NO_ERR;
}

static int read_tp_nv_info(struct thp_core_data *cd, int nv_num, int valid_size, char *nv_name)
{
	int ret;

	if (!nv_name) {
		thp_log_err("%s: invalid nv name\n", __func__);
		return -EINVAL;
	}

	if (memset_s(&cd->nv_info, sizeof(cd->nv_info), 0, sizeof(cd->nv_info))) {
		thp_log_err("%s: memset fail\n", __func__);
		return -EINVAL;
	}
	cd->nv_info.nv_operation = NV_READ;
	cd->nv_info.nv_number = nv_num;
	cd->nv_info.valid_size = valid_size;
	if (strncpy_s(cd->nv_info.nv_name, sizeof(cd->nv_info.nv_name),
		nv_name, sizeof(cd->nv_info.nv_name))) {
		thp_log_err("%s: strncpy fail\n", __func__);
		return -EINVAL;
	}
	cd->nv_info.nv_name[sizeof(cd->nv_info.nv_name) - 1] = '\0';
#ifdef CONFIG_HUAWEI_THP_QCOM
	ret = hw_nve_direct_access(&cd->nv_info);
#else
	ret = nve_direct_access_interface(&cd->nv_info);
#endif
	if (ret != 0) {
		thp_log_err("%s: read tp info from nv %d fail\n", __func__, nv_num);
		return -1;
	}
	return NO_ERR;
}

static int write_tp_nv_info(struct thp_core_data *cd, int nv_num,
	unsigned int valid_size, char *nv_name, char *info)
{
	int ret;
	unsigned int len;

	if (!nv_name || !info) {
		thp_log_err("%s: invalid arg\n", __func__);
		return -EINVAL;
	}

	if (memset_s(&cd->nv_info, sizeof(cd->nv_info), 0, sizeof(cd->nv_info))) {
		thp_log_err("%s: memset fail\n", __func__);
		return -EINVAL;
	}
	cd->nv_info.nv_operation = NV_WRITE;
	cd->nv_info.nv_number = nv_num;
	cd->nv_info.valid_size = valid_size;
	if (strncpy_s(cd->nv_info.nv_name, sizeof(cd->nv_info.nv_name),
		nv_name, sizeof(cd->nv_info.nv_name))) {
		thp_log_err("%s: strncpy fail\n", __func__);
		return -EINVAL;
	}
	cd->nv_info.nv_name[sizeof(cd->nv_info.nv_name) - 1] = '\0';

	len = sizeof(cd->nv_info.nv_data) < valid_size ? sizeof(cd->nv_info.nv_data) : valid_size;
	if (memcpy_s(cd->nv_info.nv_data, sizeof(cd->nv_info.nv_data), info, len)) {
		thp_log_err("%s: memcpy fail\n", __func__);
		return -EINVAL;
	}
#ifdef CONFIG_HUAWEI_THP_QCOM
	ret = hw_nve_direct_access(&cd->nv_info);
#else
	ret = nve_direct_access_interface(&cd->nv_info);
#endif
	if (ret != 0) {
		thp_log_err("%s: write tp info to nv %d fail\n", __func__, nv_num);
		return -1;
	}
	return NO_ERR;
}

static int thp_set_tp_nv_info(struct thp_core_data *cd,
	struct tp_nv_data *info, struct tp_fail_data tp_fail_info)
{
	if (memcpy_s(info, sizeof(*info), cd->nv_info.nv_data, sizeof(*info))) {
		thp_log_err("%s: memcpy fail\n", __func__);
		return -EINVAL;
	}
	if ((info->index < 0) || (info->index >= FAIL_NUM)) {
		thp_log_err("%s: invalid index %d\n", __func__, info->index);
		return -EINVAL;
	}
	if (memcpy_s(&info->tp_info[info->index], sizeof(info->tp_info[info->index]),
		&tp_fail_info, sizeof(tp_fail_info))) {
		thp_log_err("%s: memcpy fail\n", __func__);
		return -EINVAL;
	}
	info->index = (info->index + 1) % 4; /* index value range:0,1,2,3 */
	return NO_ERR;
}

int thp_ioctl_set_tp_broken_info(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct tp_fail_data tp_fail_info;
	struct tp_nv_data tp_nv_info;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (argp == NULL) {
		thp_log_err("%s: null ptr\n", __func__);
		return -EINVAL;
	}

	if (memset_s(&tp_fail_info, sizeof(tp_fail_info), 0, sizeof(tp_fail_info))) {
		thp_log_err("%s: memset fail\n", __func__);
		return -EINVAL;
	}
	if (copy_from_user(&tp_fail_info, argp, sizeof(tp_fail_info))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	thp_log_info("%s:Time:%llu BrkSensor:%u X1:%u Y1:%u X2:%u Y2:%u Raw:%u\n", __func__,
			tp_fail_info.time, tp_fail_info.brk_sensor,
			tp_fail_info.rect[0], tp_fail_info.rect[1],
			tp_fail_info.rect[2], tp_fail_info.rect[3],
			tp_fail_info.raw);
	if (read_tp_nv_info(cd, TP_FAIL_NV_NUM, sizeof(tp_nv_info), "TP_FAIL")) {
		thp_log_err("%s: read nv info fail\n", __func__);
		return -1;
	}

	if (thp_set_tp_nv_info(cd, &tp_nv_info, tp_fail_info)) {
		thp_log_err("%s: set_tp_nv_info fail\n", __func__);
		return -EINVAL;
	}

	if (write_tp_nv_info(cd, TP_FAIL_NV_NUM, sizeof(tp_nv_info), "TP_FAIL", (char *)&tp_nv_info)) {
		thp_log_err("%s: write nv info fail\n", __func__);
		return -1;
	}
	return NO_ERR;
}

int thp_ioctl_set_screen_status(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	u32 screen_status;

	if (argp == NULL) {
		thp_log_err("%s: null ptr\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(&screen_status, argp, sizeof(u32))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	if (screen_status) {
		thp_log_info("%s: panel %u active\n", __func__, cd->panel_id);
		cd->screen_waitq_flag = WAITQ_WAKEUP;
		wake_up_interruptible(&(cd->screen_waitq));
	} else {
		thp_log_info("%s: panel %u suspend\n", __func__, cd->panel_id);
		cd->screen_waitq_flag = WAITQ_WAIT;
	}
	return NO_ERR;
}

int thp_wrapper_ioctl_force_wakeup(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	u32 force_wakeup;

	if (argp == NULL) {
		thp_log_err("%s: null ptr\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(&force_wakeup, argp, sizeof(u32))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	if (force_wakeup) {
		thp_mt_wrapper_wakeup_poll(cd->panel_id);
		thp_wake_up_frame_waitq(cd);
		thp_log_info("%s: out\n", __func__);
	}
	return NO_ERR;
}

static u32 thp_get_power_status(struct thp_core_data *cd)
{
	struct thp_power_supply *power = NULL;
	u32 power_status = 1; /* power on: 1 */
	int i;
	u32 ret;

	for (i = 0; i < THP_POWER_ID_MAX; i++) {
		power = &cd->thp_powers[i];
		if (!power->use_count)
			continue;
		switch (power->type) {
		case THP_POWER_GPIO:
			ret = gpio_get_value(power->gpio);
			thp_log_info("%s:%s = %u\n", __func__, thp_power_id2name(i), ret);
			power_status &= ret;
			break;
		default:
			thp_log_err("%s: invalid power type %d\n",
				__func__, power->type);
			break;
		}
	}

	return power_status;
}

int thp_wrapper_ioctl_get_power_status(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	u32 i;
	u32 power_status;

	if (argp == NULL) {
		thp_log_err("%s: null ptr\n", __func__);
		return -EINVAL;
	}

	for (i = 0; i <= cd->dual_thp; i++) {
		if (thp_get_core_data(i)->power_cfg.power_supply_mode &&
			thp_get_core_data(i)->power_cfg.common_power_flag) {
			cd = thp_get_core_data(i);
			break;
		}
	}

	power_status = thp_get_power_status(cd);
#if defined(CONFIG_HUAWEI_DSM)
	if (!power_status)
		thp_dmd_report(DSM_TP_FREEZE_ERROR_NO,
			"%s, abnormal power-off\n", __func__);
#endif
	if (copy_to_user(argp, (void *)&power_status, sizeof(power_status))) {
		thp_log_err("%s: Failed to copy_to_user()\n", __func__);
		return -EFAULT;
	}
	return NO_ERR;
}

static void thp_multi_panel_ic_recovery(uint32_t panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	msleep(250); /* need wait 250ms after power on */
	thp_easy_wake_mode_change(cd);
	if (cd->thp_dev)
		cd->thp_dev->ops->suspend(cd->thp_dev);
	thp_easy_wake_mode_revert(cd);
}

/* recover ic status when power recovery */
static void thp_ic_status_recovery(struct thp_core_data *cd)
{
	if (cd->dual_thp) {
		thp_request_vote_of_polymeric_chip(cd);
		if (get_active_panel() == MAIN_TOUCH_PANEL)
			thp_multi_panel_ic_recovery(SUB_TOUCH_PANEL);
		else
			thp_multi_panel_ic_recovery(MAIN_TOUCH_PANEL);
		thp_release_vote_of_polymeric_chip(cd);
	}
}

static int thp_power_recovery(struct thp_core_data *cd)
{
	u32 i;
	struct common_power_control *power_control = NULL;

	thp_log_info("%s enter\n", __func__);
	thp_power_lock();
	if (cd->power_cfg.power_supply_mode == MULTI_PANEL_USE_SINGEL_POWER) {
		power_control = get_common_power_control(cd->power_cfg.power_sequence);
		for (i = 0; i <= cd->dual_thp; i++)
			power_control->power_off(thp_get_core_data(i)->thp_dev);
		mdelay(cd->thp_dev->timing_config.recovery_power_delay_ms);
		for (i = 0; i <= cd->dual_thp; i++)
			power_control->power_on(thp_get_core_data(i)->thp_dev);
		thp_ic_status_recovery(cd);
	}
	thp_power_unlock();
	if (cd->support_ic_recovery)
		cd->tp_recovery_count++;
	return NO_ERR;
}

static int thp_spi_revocery(struct thp_core_data *cd)
{
#ifdef CONFIG_CONTEXTHUB_IO_DIE_STS
	int rc = NO_ERR;
#endif

	if (!cd) {
		thp_log_err("%s:cd is NULL\n", __func__);
		return -EINVAL;
	}
#ifdef CONFIG_CONTEXTHUB_IO_DIE_STS
	if (cd->support_polymeric_chip) {
		thp_log_info("%s:panel %u spi recovery\n", __func__, cd->panel_id);
		rc = thp_bus_lock(cd->thp_dev);
		if (rc) {
			thp_log_err("%s: get lock failed\n", __func__);
			return rc;
		}
		sts_spi_rst(STS_USER_TP, STS_ESD_ERR);
		thp_bus_unlock(cd->thp_dev);
	}
#endif
	return NO_ERR;
}

#define POWER_RECOVERY 1
#define SPI_RECOVERY 2
int thp_wrapper_ioctl_tp_recovery(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	u32 recovery_flag;

	if (argp == NULL) {
		thp_log_err("%s: null ptr\n", __func__);
		return -EINVAL;
	}

	if (copy_from_user(&recovery_flag, argp, sizeof(u32))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	thp_log_info("%s recovery_flag = %u\n", __func__, recovery_flag);
	switch (recovery_flag) {
	case POWER_RECOVERY:
		return thp_power_recovery(cd);
	case SPI_RECOVERY:
		return thp_spi_revocery(cd);
	default:
		thp_log_info("%s: invalid recovery_flag\n");
		break;
	}
	return NO_ERR;
}