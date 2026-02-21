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
#include "huawei_pen_clk.h"

#define SUPPORT_PEN_PROTOCOL_CLASS 2
#define SUPPORT_PEN_PROTOCOL_CODE 4

static long thp_ioctl_get_stylus3_connect_status(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = NULL;
	void __user *argp = NULL;
	int ret;

	if (arg == 0) {
		thp_log_err("arg == 0\n");
		return -EINVAL;
	}
	argp = (void __user *)(uintptr_t)arg;
	cd = thp_get_core_data(panel_id);
	if (!cd) {
		thp_log_err("%s:have null ptr\n", __func__);
		return -EINVAL;
	}
	ret = wait_for_completion_interruptible(&cd->stylus3_status_flag);
	if (ret) {
		thp_log_err(" Failed to get_connect_status\n");
		return ret;
	}
	if (copy_to_user(argp, &cd->last_stylus3_status,
		sizeof(cd->last_stylus3_status))) {
		thp_log_err("%s: Failed to copy_to_user()\n",
			__func__);
		return -EFAULT;
	}
	return NO_ERR;
}

int thp_wrapper_ioctl_get_stylus3_connect_status(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (!cd) {
		thp_log_err("%s:have null ptr\n", __func__);
		return -EINVAL;
	}
	if ((cd->pen_supported) && (cd->pen_mt_enable_flag))
		return thp_ioctl_get_stylus3_connect_status(arg, panel_id);
	return NO_ERR;
}

static int set_stylus3_change_protocol(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = NULL;
	unsigned int stylus_status;
	void __user *argp = (void __user *)(uintptr_t)arg;

	if (arg == 0) {
		thp_log_err("arg == 0\n");
		return -EINVAL;
	}
	if (copy_from_user(&stylus_status, argp, sizeof(stylus_status))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	/* bt close,do not need handle */
	if (stylus_status == 0) {
		thp_log_info("do not change pen protocol\n");
		return NO_ERR;
	}
	cd = thp_get_core_data(panel_id);
	if (!cd) {
		thp_log_err("%s:have null ptr\n", __func__);
		return -EINVAL;
	}
	/* change pen protocol to 2.2 */
	cd->stylus3_callback_event.event_class = SUPPORT_PEN_PROTOCOL_CLASS;
	cd->stylus3_callback_event.event_code = SUPPORT_PEN_PROTOCOL_CODE;
	thp_log_info("%s: to pen\n", __func__);
	atomic_set(&cd->callback_event_flag, true);
	complete(&cd->stylus3_callback_flag);
	return NO_ERR;
}

static int thp_ioctl_set_stylus3_connect_status(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = NULL;
	void __user *argp = NULL;
	unsigned int stylus3_status;

	if (arg == 0) {
		thp_log_err("arg == 0\n");
		return -EINVAL;
	}
	argp = (void __user *)(uintptr_t)arg;
	cd = thp_get_core_data(panel_id);
	if (!cd) {
		thp_log_err("%s:have null ptr\n", __func__);
		return -EINVAL;
	}
	if (copy_from_user(&stylus3_status, argp, sizeof(unsigned int))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	thp_log_info("%s:stylus3_status = %d\n", __func__, stylus3_status);
	thp_set_status(THP_STATUS_STYLUS3,
		(stylus3_status & STYLUS3_CONNECTED_MASK), cd->panel_id);
	cd->suspend_stylus3_status = thp_get_status(THP_STATUS_STYLUS3, cd->panel_id);
	atomic_set(&cd->last_stylus3_status, stylus3_status);
	complete(&cd->stylus3_status_flag);
	if ((cd->send_bt_status_to_fw) &&
		(cd->thp_dev->ops->bt_handler))
		if (cd->thp_dev->ops->bt_handler(cd->thp_dev, false))
			thp_log_err("send bt status to fw fail\n");

	return NO_ERR;
}

int thp_wrapper_ioctl_set_stylus3_connect_status(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (!cd) {
		thp_log_err("%s:have null ptr\n", __func__);
		return -EINVAL;
	}
	if (cd->pen_change_protocol)
		return set_stylus3_change_protocol(arg, panel_id);
	if ((cd->pen_supported) && (cd->pen_mt_enable_flag))
		return thp_ioctl_set_stylus3_connect_status(arg, panel_id);
	return NO_ERR;
}

int thp_ioctl_get_callback_events(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = NULL;
	int ret;

	cd = thp_get_core_data(panel_id);
	if (!cd) {
		thp_log_err("%s:have null ptr\n", __func__);
		return -EINVAL;
	}
	if (!(cd->pen_supported) || !(cd->pen_mt_enable_flag)) {
		thp_log_err("%s: Not support stylus3\n", __func__);
		return -EINVAL;
	}
	if (arg == 0) {
		thp_log_err("arg == 0\n");
		return -EINVAL;
	}
	ret = wait_for_completion_interruptible(&cd->stylus3_callback_flag);
	if (ret) {
		thp_log_info(" Failed to get stylus3_callback_flag\n");
	} else {
		if (copy_to_user(argp, &cd->stylus3_callback_event,
			sizeof(cd->stylus3_callback_event))) {
			thp_log_err("%s: Failed to copy_to_user()\n", __func__);
			return -EFAULT;
		}
		thp_log_info("%s, eventClass=%d, eventCode=%d\n",
			__func__, cd->stylus3_callback_event.event_class,
			cd->stylus3_callback_event.event_code);
	}
	atomic_set(&cd->callback_event_flag, false);
	return NO_ERR;
}

int thp_ioctl_set_callback_events(unsigned long arg, u32 panel_id)
{
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = NULL;

	cd = thp_get_core_data(panel_id);
	if (!cd) {
		thp_log_err("%s:have null ptr\n", __func__);
		return -EINVAL;
	}
	if (!(cd->pen_supported) || !(cd->pen_mt_enable_flag)) {
		thp_log_err("%s: Not support stylus3\n", __func__);
		return -EINVAL;
	}
	if (arg == 0) {
		thp_log_err("arg == 0\n");
		return -EINVAL;
	}
	if (atomic_read(&cd->callback_event_flag) != false) {
		thp_log_err("%s,callback event not handle, need retry\n",
			__func__);
		return -EBUSY;
	}
	if (copy_from_user(&cd->stylus3_callback_event,
		argp, sizeof(cd->stylus3_callback_event))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	thp_log_info("%s, eventClass=%d, eventCode=%d\n",
		__func__,
		cd->stylus3_callback_event.event_class,
		cd->stylus3_callback_event.event_code);
	atomic_set(&cd->callback_event_flag, true);
	complete(&cd->stylus3_callback_flag);
	return NO_ERR;
}

int thp_ioctl_set_stylus3_plam_suppression(unsigned long arg, u32 panel_id)
{
	unsigned int suppression_type;
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (!cd) {
		thp_log_err("%s have null ptr\n", __func__);
		return -EINVAL;
	}
	if ((!cd->pen_supported) || (!cd->pen_mt_enable_flag) ||
		(!cd->support_stylus3_plam_suppression)) {
		thp_log_err("%s unsupported stylus3 plam suppression\n", __func__);
		return  -EINVAL;
	}
	if (arg == 0) {
		thp_log_err("arg == 0\n");
		return -EINVAL;
	}
	if (copy_from_user(&suppression_type, argp, sizeof(unsigned int))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	thp_log_info("%s suppression_type = %u\n",
		__func__, suppression_type);
	thp_set_status(THP_STATUS_STYLUS3_PLAM_SUPPRESSION,
		(suppression_type & STYLUS3_PLAM_SUPPRESSION_MASK), cd->panel_id);
	return NO_ERR;
}

int thp_ioctl_set_stylus_adsorption_status(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = NULL;
	void __user *argp = NULL;
	unsigned int stylus_adsorption_status;

	if (arg == 0) {
		thp_log_err("%s: arg == 0\n", __func__);
		return -EINVAL;
	}
	argp = (void __user *)(uintptr_t)arg;
	cd = thp_get_core_data(panel_id);
	if (!cd) {
		thp_log_err("%s: have null ptr\n", __func__);
		return -EINVAL;
	}
	if ((!cd->pen_supported) || (!cd->pen_mt_enable_flag) ||
		(!cd->send_adsorption_status_to_fw)) {
		thp_log_err("%s: unsupported send stylus adsorption state\n", __func__);
		return  -EINVAL;
	}
	if (copy_from_user(&stylus_adsorption_status, argp, sizeof(unsigned int))) {
		thp_log_err("%s: Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	thp_log_info("%s: stylus_adsorption_status = %d\n", __func__,
		stylus_adsorption_status);
	atomic_set(&cd->last_stylus_adsorption_status, stylus_adsorption_status);
	if (cd->thp_dev->ops->bt_handler && cd->thp_dev->ops->bt_handler(cd->thp_dev, false))
		thp_log_err("%s: send adsorption status to fw fail\n", __func__);

	return NO_ERR;
}

static int thp_ioctl_set_stylus3_work_mode(struct thp_core_data *cd,
	unsigned long arg)
{
	void __user *argp = NULL;
	unsigned int current_stylus3_work_mode;

	if (arg == 0) {
		thp_log_err("arg == 0\n");
		return -EINVAL;
	}
	argp = (void __user *)(uintptr_t)arg;
	if (copy_from_user(&current_stylus3_work_mode, argp, sizeof(unsigned int))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	thp_log_info("%s:current_stylus3_work_mode = %u\n", __func__,
		current_stylus3_work_mode);
	atomic_set(&cd->last_stylus3_work_mode, current_stylus3_work_mode);
	if (cd->thp_dev->ops->bt_handler && cd->thp_dev->ops->bt_handler(cd->thp_dev, false))
		thp_log_err("send work mode status to fw fail\n");
	return NO_ERR;
}

int thp_wrapper_ioctl_set_stylus3_work_mode(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd =  thp_get_core_data(panel_id);
	bool stylus3_workmode = false;

	if (!cd) {
		thp_log_err("%s:have null ptr\n", __func__);
		return -EINVAL;
	}
	stylus3_workmode = (!cd->pen_supported) ||
		(!cd->pen_mt_enable_flag) || (!cd->send_stylus3_workmode_to_fw);
	if (!stylus3_workmode)
		return thp_ioctl_set_stylus3_work_mode(cd, arg);
	return NO_ERR;
}

static void thp_pen_clk_control(uint32_t status, u32 panel_id)
{
	if (status)
		thp_pen_clk_enable(panel_id);
	else
		thp_pen_clk_disable(panel_id);
}

int thp_wrapper_ioctl_set_external_clk(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = NULL;
	void __user *argp = NULL;
	u32 enable;

	if (arg == 0) {
		thp_log_err("arg == 0\n");
		return -EINVAL;
	}
	argp = (void __user *)(uintptr_t)arg;
	cd = thp_get_core_data(panel_id);
	if (copy_from_user(&enable, argp, sizeof(u32))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	if (cd->need_extra_system_clk && (panel_id == get_active_panel()))
		thp_pen_clk_control(enable, panel_id);
	return NO_ERR;
}

int thp_wrapper_ioctl_get_external_clk(unsigned long arg, u32 panel_id)
{
	struct thp_core_data *cd = NULL;
	void __user *argp = NULL;

	if (arg == 0) {
		thp_log_err("arg == 0\n");
		return -EINVAL;
	}
	argp = (void __user *)(uintptr_t)arg;
	cd = thp_get_core_data(panel_id);
	if (copy_to_user(argp, &cd->need_extra_system_clk, sizeof(u32))) {
		thp_log_err("%s Failed to copy_to_user\n", __func__);
		return -EFAULT;
	}
	return NO_ERR;
}

int thp_ioctl_set_stylus_high_hover(unsigned long arg, u32 panel_id)
{
	unsigned int stylus_high_hover;
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	bool high_hover_flag;

	high_hover_flag = cd->pen_supported &&
		cd->pen_mt_enable_flag && cd->support_stylus_high_hover;
	if (!high_hover_flag) {
		thp_log_err("%s unsupported stylus high hover\n", __func__);
		return  -EINVAL;
	}
	if (arg == 0) {
		thp_log_err("arg == 0\n");
		return -EINVAL;
	}
	if (copy_from_user(&stylus_high_hover, argp, sizeof(unsigned int))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	thp_log_info("%s stylus_high_hover = %u\n", __func__, stylus_high_hover);
	thp_set_status(THP_STATUS_HIGH_HOVER, !!stylus_high_hover, cd->panel_id);
	return NO_ERR;
}

int thp_ioctl_set_stylus_lite_status(unsigned long arg, u32 panel_id)
{
	unsigned int stylus_lite_status;
	void __user *argp = (void __user *)(uintptr_t)arg;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (arg == 0) {
		thp_log_err("arg == 0\n");
		return -EINVAL;
	}
	if (copy_from_user(&stylus_lite_status, argp, sizeof(unsigned int))) {
		thp_log_err("%s Failed to copy_from_user\n", __func__);
		return -EFAULT;
	}
	thp_log_info("%s stylus_lite_status = %u\n", __func__, stylus_lite_status);
	atomic_set(&cd->last_stylus_lite_status, stylus_lite_status);
	if (cd->thp_dev->ops->bt_handler)
		if (cd->thp_dev->ops->bt_handler(cd->thp_dev, false))
			thp_log_err("send work mode status to fw fail\n");

	return NO_ERR;
}
