/*
 * Huawei Touchscreen Driver
 *
 * Copyright (c) 2012-2024 Huawei Technologies Co., Ltd.
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
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/uaccess.h>
#include <linux/of.h>
#include <huawei_platform/log/hw_log.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <linux/gpio.h>

#if defined(CONFIG_FB)
#include <linux/notifier.h>
#include <linux/fb.h>
#endif

#define DEVICE_NAME "input_mt_wrapper"
#define INPUT_DEV_LOCATION_NAME "input_tp_device"

static const char *mt_wrapper_misc_device_name[TOTAL_PANEL_NUM] = {
	"input_mt_wrapper0",
	"input_mt_wrapper1"
};

struct thp_vendor_name {
	const char *vendor_id;
	const char *compatible_name;
};

/*
 * use 2 bits vendor_id in project_id to distinguish LCD IC.
 * 09: SDC, 13: BOE, 15: EDO, 29: Visionox
 */
static struct thp_vendor_name thp_input_compatible_table[] = {
	{ "09", "huawei,thp_input_09" },
	{ "13", "huawei,thp_input_13" },
	{ "15", "huawei,thp_input_15" },
	{ "29", "huawei,thp_input_29" },
};

void thp_inputkey_report(unsigned int gesture_wakeup_value, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	input_report_key(cd->thp_mt_wrapper->input_dev, gesture_wakeup_value, 1);
	input_sync(cd->thp_mt_wrapper->input_dev);
	input_report_key(cd->thp_mt_wrapper->input_dev, gesture_wakeup_value, 0);
	input_sync(cd->thp_mt_wrapper->input_dev);
	thp_log_info("%s ->done\n", __func__);
}

#if (defined(CONFIG_HUAWEI_THP_MTK) || defined(CONFIG_HUAWEI_THP_QCOM))
void thp_aod_click_report(struct thp_udfp_data udfp_data, u32 panel_id)
{
	unsigned int x;
	unsigned int y;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *input_dev = cd->thp_mt_wrapper->input_dev;

	if ((input_dev == NULL) || (!cd->aod_touch_status) ||
		(!cd->aod_window_ready_status)) {
		thp_log_err("%s:aod_touch_status:%u, aod_window_ready_status = %u\n",
			__func__, cd->aod_touch_status, cd->aod_window_ready_status);
		return;
	}
	x = udfp_data.tpud_data.tp_x;
	y = udfp_data.tpud_data.tp_y;
	input_report_key(input_dev, TS_SINGLE_CLICK, THP_KEY_DOWN);
	input_sync(input_dev);
	input_report_key(input_dev, TS_SINGLE_CLICK, THP_KEY_UP);
	input_sync(input_dev);
	thp_log_info("%s ->done, (%u, %u)\n", __func__, x, y);
}

void thp_udfp_event_to_aod(struct thp_udfp_data udfp_data, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *input_dev = cd->thp_mt_wrapper->input_dev;

	if (input_dev == NULL) {
		thp_log_err("%s: have null ptr\n", __func__);
		return;
	}
	if (udfp_data.tpud_data.tp_event == TP_EVENT_FINGER_DOWN) {
		thp_log_info("%s FINGER_DOWN\n", __func__);
		input_report_key(input_dev, KEY_FINGER_DOWN, THP_KEY_DOWN);
		input_sync(input_dev);
		input_report_key(input_dev, KEY_FINGER_DOWN, THP_KEY_UP);
		input_sync(input_dev);
	}
	if (udfp_data.tpud_data.tp_event == TP_EVENT_FINGER_UP) {
		thp_log_info("%s FINGER_UP\n", __func__);
		input_report_key(input_dev, KEY_FINGER_UP, THP_KEY_DOWN);
		input_sync(input_dev);
		input_report_key(input_dev, KEY_FINGER_UP, THP_KEY_UP);
		input_sync(input_dev);
	}
}
#endif

void thp_input_pen_report(unsigned int pen_event_value, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (cd->thp_mt_wrapper->pen_dev == NULL) {
		thp_log_info("%s pen device is null\n", __func__);
		return;
	}

	input_report_key(cd->thp_mt_wrapper->pen_dev, pen_event_value, 1);
	input_sync(cd->thp_mt_wrapper->pen_dev);
	input_report_key(cd->thp_mt_wrapper->pen_dev, pen_event_value, 0);
	input_sync(cd->thp_mt_wrapper->pen_dev);
	thp_log_info("%s:done\n", __func__);
}

void thp_clean_fingers(u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *input_dev = cd->thp_mt_wrapper->input_dev;

	input_mt_sync(input_dev);
	input_sync(input_dev);

	input_report_key(input_dev, BTN_TOUCH, 0);
	input_sync(input_dev);
}

int thp_mt_wrapper_esd_event(unsigned int status, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *input_dev = cd->thp_mt_wrapper->input_dev;

	if (!input_dev) {
		thp_log_err("%s:cd or input_dev is null\n", __func__);
		return -EINVAL;
	}
	
	(void)status;
	input_report_key(input_dev, KEY_F26, 1);
	input_sync(input_dev);
	input_report_key(input_dev, KEY_F26, 0);
	input_sync(input_dev);
	thp_log_info("%s:ESD EVENT\n", __func__);

	return NO_ERR;
}

static int thp_mt_wrapper_open(struct inode *inode, struct file *file)
{
	struct thp_core_data *cd = container_of((struct miscdevice *)file->private_data,
		struct thp_core_data, thp_mt_wrapper_misc_device);

	thp_log_info("%s: called panel %u\n", __func__, cd->panel_id);
	return NO_ERR;
}

static int thp_mt_wrapper_release(struct inode *inode, struct file *file)
{
	struct thp_core_data *cd = container_of((struct miscdevice *)file->private_data,
		struct thp_core_data, thp_mt_wrapper_misc_device);

	thp_log_info("%s: panel %u release\n", __func__, cd->panel_id);
	return NO_ERR;
}

struct thp_mt_wrapper_ioctl_group g_thp_mt_wrapper_ioctl_table[] = {
	{ INPUT_MT_WRAPPER_IOCTL_CMD_SET_COORDINATES_AND_TIME,
		thp_mt_wrapper_ioctl_cmd_set_coordinates_and_time },
	{ INPUT_MT_WRAPPER_IOCTL_CMD_SET_COORDINATES,
		thp_mt_wrapper_ioctl_cmd_set_coordinates },
	{ INPUT_MT_WRAPPER_IOCTL_CMD_REPORT_PEN,
		thp_mt_wrapper_ioctl_report_pen },
	{ INPUT_MT_WRAPPER_IOCTL_CMD_REPORT_PEN_AND_EXTENSION,
		thp_mt_wrapper_ioctl_report_pen_and_extension },
	{ INPUT_MT_WRAPPER_IOCTL_READ_STATUS,
		thp_mt_wrapper_ioctl_read_status },
	{ INPUT_MT_WRAPPER_IOCTL_READ_INPUT_CONFIG,
		thp_mt_ioctl_read_input_config },
	{ INPUT_MT_WRAPPER_IOCTL_READ_FINGER_RESOLUTION_MAGNIFICATION,
		thp_mt_ioctl_read_finger_resolution_magnification },
	{ INPUT_MT_WRAPPER_IOCTL_READ_SCENE_INFO,
		thp_mt_wrapper_ioctl_read_scene_info },
	{ INPUT_MT_WRAPPER_IOCTL_GET_WINDOW_INFO,
		thp_mt_wrapper_ioctl_get_window_info },
	{ INPUT_MT_WRAPPER_IOCTL_GET_PROJECT_ID,
		thp_mt_wrapper_ioctl_get_projectid },
	{ INPUT_MT_WRAPPER_IOCTL_CMD_SET_EVENTS,
		thp_mt_wrapper_ioctl_set_events },
	{ INPUT_MT_WRAPPER_IOCTL_CMD_GET_EVENTS,
		thp_mt_wrapper_ioctl_get_events },
	{ INPUT_MT_WRAPPER_IOCTL_SET_ROI_DATA,
		thp_mt_wrapper_ioctl_set_roi_data },
	{ INPUT_MT_WRAPPER_IOCTL_CMD_REPORT_KEYEVENT,
		thp_mt_ioctl_report_keyevent },
	{ INPUT_MT_WRAPPER_IOCTL_REPORT_SYSTEM_KEYEVENT,
		thp_mt_ioctl_report_system_keyevent },
	{ INPUT_MT_WRAPPER_IOCTL_GET_PLATFORM_TYPE,
		thp_mt_wrapper_ioctl_get_platform_type },
#if ((defined CONFIG_HUAWEI_SHB_THP) || (defined CONFIG_HUAWEI_THP_MTK) || (defined CONFIG_HUAWEI_THP_QCOM))
	{ INPUT_MT_WRAPPER_IOCTL_CMD_SHB_EVENT,
		thp_mt_ioctl_cmd_shb_event },
#endif
	{ INPUT_MT_WRAPPER_IOCTL_GET_VOMLUME_SIDE,
		thp_ioctl_get_volume_side },
	{ INPUT_MT_WRAPPER_IOCTL_GET_POWER_SWITCH,
		thp_ioctl_get_power_switch },
	{ INPUT_MT_WRAPPER_IOCTL_GET_APP_INFO,
		ioctl_get_app_info },
	{ INPUT_MT_IOCTL_CMD_GET_STYLUS3_CONNECT_STATUS,
		thp_wrapper_ioctl_get_stylus3_connect_status },
	{ INPUT_MT_IOCTRL_CMD_SET_STYLUS3_CONNECT_STATUS,
		thp_wrapper_ioctl_set_stylus3_connect_status },
	{ INPUT_MT_IOCTL_CMD_GET_CALLBACK_EVENTS,
		thp_ioctl_get_callback_events },
	{ INPUT_MT_IOCTL_CMD_SET_CALLBACK_EVENTS,
		thp_ioctl_set_callback_events },
	{ INPUT_MT_IOCTL_CMD_SET_DAEMON_INIT_PROTECT,
		daemon_init_protect },
	{ INPUT_MT_IOCTL_CMD_SET_DAEMON_POWER_RESET,
		thp_daemon_power_reset },
	{ INPUT_MT_IOCTL_CMD_SET_STYLUS3_PLAM_SUPPRESSION_STATUS,
		thp_ioctl_set_stylus3_plam_suppression },
	{ INPUT_MT_IOCTL_CMD_SET_STYLUS_ADSORPTION_STATUS,
		thp_ioctl_set_stylus_adsorption_status },
	{ INPUT_MT_IOCTL_CMD_SET_STYLUS3_WORK_MODE,
		thp_wrapper_ioctl_set_stylus3_work_mode },
	{ INPUT_MT_IOCTL_CMD_SET_EXTERNAL_CLK,
		thp_wrapper_ioctl_set_external_clk },
	{ INPUT_MT_IOCTL_CMD_GET_EXTERNAL_CLK,
		thp_wrapper_ioctl_get_external_clk },
	{ INPUT_MT_WRAPPER_IOCTL_CMD_SET_TP_BROKEN_INFO,
		thp_ioctl_set_tp_broken_info },
	{ INPUT_MT_WRAPPER_IOCTL_CMD_SET_SCREEN_STATUS,
		thp_ioctl_set_screen_status },
	{ INPUT_MT_WRAPPER_IOCTL_CMD_FORCE_WAKEUP,
		thp_wrapper_ioctl_force_wakeup },
	{ INPUT_MT_IOCTL_CMD_GET_POWER_STATUS,
		thp_wrapper_ioctl_get_power_status },
	{ INPUT_MT_IOCTL_CMD_TP_RECOVERY,
		thp_wrapper_ioctl_tp_recovery },
	{ INPUT_MT_IOCTL_CMD_SET_STYLUS_HIGH_HOVER,
		thp_ioctl_set_stylus_high_hover },
	{ INPUT_MT_IOCTL_CMD_SET_STYLUS_LITE_STATUS,
		thp_ioctl_set_stylus_lite_status },
	{ INPUT_MT_WRAPPER_IOCTL_CMD_REPORT_HAND_STATUS,
		thp_mt_wrapper_ioctl_report_hand_status },
};

static long thp_mt_wrapper_ioctl(struct file *file, unsigned int cmd,
	unsigned long arg)
{
	int i;
	struct thp_core_data *cd = container_of((struct miscdevice *)file->private_data,
		struct thp_core_data, thp_mt_wrapper_misc_device);

	for (i = 0; i < ARRAY_SIZE(g_thp_mt_wrapper_ioctl_table); ++i)
		if (cmd == g_thp_mt_wrapper_ioctl_table[i].cmd)
			return g_thp_mt_wrapper_ioctl_table[i].ioctl_func(arg, cd->panel_id);
	thp_log_err("%s: cmd unknown, cmd = 0x%x\n", __func__, cmd);
	return -EINVAL;
}

int thp_mt_wrapper_wakeup_poll(u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (!cd->thp_mt_wrapper) {
		thp_log_err("%s: wrapper not init\n", __func__);
		return -ENODEV;
	}
	atomic_inc(&cd->thp_mt_wrapper->status_updated);
	wake_up_interruptible(&cd->thp_mt_wrapper->wait);
	return NO_ERR;
}

static unsigned int thp_mt_wrapper_poll(struct file *file, poll_table *wait)
{
	unsigned int mask = 0;
	struct thp_core_data *cd = container_of((struct miscdevice *)file->private_data,
		struct thp_core_data, thp_mt_wrapper_misc_device);

	thp_log_debug("%s:poll call in\n", __func__);
	poll_wait(file, &cd->thp_mt_wrapper->wait, wait);
	if (atomic_read(&cd->thp_mt_wrapper->status_updated) > 0)
		mask |= POLLIN | POLLRDNORM;

	thp_log_debug("%s:poll call out, mask = 0x%x\n", __func__, mask);
	return mask;
}

static const struct file_operations g_thp_mt_wrapper_fops = {
	.owner = THIS_MODULE,
	.open = thp_mt_wrapper_open,
	.release = thp_mt_wrapper_release,
	.unlocked_ioctl = thp_mt_wrapper_ioctl,
	.compat_ioctl = thp_mt_wrapper_ioctl,
	.poll = thp_mt_wrapper_poll,
};

static void set_default_input_config(struct thp_input_dev_config *input_config)
{
	input_config->abs_max_x = THP_MT_WRAPPER_MAX_X;
	input_config->abs_max_y = THP_MT_WRAPPER_MAX_Y;
	input_config->abs_max_z = THP_MT_WRAPPER_MAX_Z;
	input_config->major_max = THP_MT_WRAPPER_MAX_MAJOR;
	input_config->minor_max = THP_MT_WRAPPER_MAX_MINOR;
	input_config->tool_type_max = THP_MT_WRAPPER_TOOL_TYPE_MAX;
	input_config->tracking_id_max = THP_MT_WRAPPER_MAX_FINGERS;
	input_config->orientation_min = THP_MT_WRAPPER_MIN_ORIENTATION;
	input_config->orientation_max = THP_MT_WRAPPER_MAX_ORIENTATION;
}

static int thp_projectid_to_vender_id(const char *project_id,
	unsigned int project_id_len, char *temp_buf, unsigned int len)
{
	if ((!project_id) || (!temp_buf)) {
		thp_log_err("%s: project id or temp buffer null\n", __func__);
		return -EINVAL;
	}
	if ((strlen(project_id) > project_id_len) ||
		(len < THP_PROJECTID_VENDOR_ID_LEN)) {
		thp_log_err("%s:project_id or temp_buf has a wrong length\n", __func__);
		return -EINVAL;
	}
	strncpy(temp_buf, project_id + THP_PROJECTID_PRODUCT_NAME_LEN +
		THP_PROJECTID_IC_NAME_LEN, THP_PROJECTID_VENDOR_ID_LEN);

	return NO_ERR;
}

static struct device_node* get_thp_dev_node(u32 panel_id)
{
	int ret;
	unsigned int i;
	char temp_buf[THP_PROJECTID_VENDOR_ID_LEN + 1] = {0};
	struct device_node *thp_dev_node = NULL;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (cd->support_diff_resolution) {
		thp_log_info("%s: use different resolution\n", __func__);
		ret = thp_projectid_to_vender_id(cd->project_id,
			THP_PROJECT_ID_LEN + 1, temp_buf, sizeof(temp_buf));
		if (ret < 0) {
			thp_log_err("%s: get vendor id failed\n", __func__);
			return NULL;
		}
		for (i = 0; i < ARRAY_SIZE(thp_input_compatible_table); i++) {
			if (!strncmp(thp_input_compatible_table[i].vendor_id,
				(const char *)temp_buf,
				strlen(thp_input_compatible_table[i].vendor_id))) {
				thp_dev_node = of_find_compatible_node(NULL, NULL,
					thp_input_compatible_table[i].compatible_name);
				break;
			}
		}
		/* if no compatible id-name pair in table, use default */
		if (i == ARRAY_SIZE(thp_input_compatible_table)) {
			thp_log_err("%s:vendor id:%s not in id_table\n", __func__, temp_buf);
			thp_dev_node = of_find_compatible_node(NULL, NULL,
				THP_INPUT_DEV_COMPATIBLE);
		}
	} else {
		thp_dev_node = of_find_compatible_node(NULL, NULL,
			THP_INPUT_DEV_COMPATIBLE);
		if (panel_id == 1)
			thp_dev_node = of_find_compatible_node(NULL, NULL, "huawei,thp_input_sub_screen");
	}
	return thp_dev_node;
}

static void thp_read_u32_from_dts(struct device_node *thp_dev_node,
	const char *property, u32 *value, u32 default_value)
{
	int rc;

	rc = of_property_read_u32(thp_dev_node, property, value);
	if (rc) {
		thp_log_err("%s: %s not config, use deault\n", __func__, property);
		*value = default_value;
	}
}

static int thp_parse_input_config(struct thp_input_dev_config *config, u32 panel_id)
{
	struct device_node *thp_dev_node = NULL;
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	cd->finger_resolution_magnification = DEFAULT_MAGNIFICATION;

	thp_dev_node = get_thp_dev_node(panel_id);
	if (!thp_dev_node) {
		thp_log_info("%s:found node fail, use defatle config\n", __func__);
		goto use_default;
	}
	thp_read_u32_from_dts(thp_dev_node, "abs_max_x",
		&config->abs_max_x, THP_MT_WRAPPER_MAX_X);
	thp_read_u32_from_dts(thp_dev_node, "abs_max_y",
		&config->abs_max_y, THP_MT_WRAPPER_MAX_Y);
	thp_read_u32_from_dts(thp_dev_node, "abs_max_z",
		&config->abs_max_z, THP_MT_WRAPPER_MAX_Z);
	thp_read_u32_from_dts(thp_dev_node, "tracking_id_max",
		&config->tracking_id_max, THP_MT_WRAPPER_MAX_FINGERS);
	thp_read_u32_from_dts(thp_dev_node, "major_max",
		&config->major_max, THP_MT_WRAPPER_MAX_MAJOR);
	thp_read_u32_from_dts(thp_dev_node, "minor_max",
		&config->minor_max, THP_MT_WRAPPER_MAX_MINOR);
	thp_read_u32_from_dts(thp_dev_node, "orientation_min",
		&config->orientation_min, THP_MT_WRAPPER_MIN_ORIENTATION);
	thp_read_u32_from_dts(thp_dev_node, "orientation_max",
		&config->orientation_max, THP_MT_WRAPPER_MAX_ORIENTATION);
	thp_read_u32_from_dts(thp_dev_node, "tool_type_max",
		&config->tool_type_max, THP_MT_WRAPPER_TOOL_TYPE_MAX);
	thp_read_u32_from_dts(thp_dev_node, "magnification_of_finger_resolution",
		&cd->finger_resolution_magnification, DEFAULT_MAGNIFICATION);

	return NO_ERR;
use_default:
	set_default_input_config(config);
	return NO_ERR;
}

static int thp_parse_pen_input_config(struct thp_input_pen_dev_config *config, u32 panel_id)
{
	int rc = -EINVAL;
	struct device_node *thp_dev_node = NULL;

	if (config == NULL) {
		thp_log_err("%s: config is null\n", __func__);
		goto err;
	}
	thp_dev_node = of_find_compatible_node(NULL, NULL,
		THP_PEN_INPUT_DEV_COMPATIBLE);
	if (panel_id == 1)
		thp_dev_node = of_find_compatible_node(NULL, NULL,
			"huawei,thp_pen_input_sub_screen");
	if (!thp_dev_node) {
		thp_log_info("%s:thp_dev_node not found\n", __func__);
		goto err;
	}

	rc = of_property_read_u32(thp_dev_node, "max_x",
		&config->max_x);
	if (rc) {
		thp_log_err("%s:max_x not config\n", __func__);
		goto err;
	}

	rc = of_property_read_u32(thp_dev_node, "max_y",
		&config->max_y);
	if (rc) {
		thp_log_err("%s:max_y not config\n", __func__);
		goto err;
	}

	thp_read_u32_from_dts(thp_dev_node, "max_pressure",
		&config->pressure, THP_MT_WRAPPER_MAX_Z);
	thp_read_u32_from_dts(thp_dev_node, "max_tilt_x",
		&config->max_tilt_x, THP_PEN_WRAPPER_TILT_MAX_X);
	thp_read_u32_from_dts(thp_dev_node, "max_tilt_y",
		&config->max_tilt_y, THP_PEN_WRAPPER_TILT_MAX_X);
err:
	return rc;
}

static int thp_set_pen_input_config(struct input_dev *pen_dev, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if ((cd == NULL) || (pen_dev == NULL)) {
		thp_log_err("%s:input null ptr\n", __func__);
		return -EINVAL;
	}

	pen_dev->evbit[0] |= BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	__set_bit(ABS_X, pen_dev->absbit);
	__set_bit(ABS_Y, pen_dev->absbit);
	__set_bit(ABS_TILT_X, pen_dev->absbit);
	__set_bit(ABS_TILT_Y, pen_dev->absbit);
	__set_bit(BTN_STYLUS, pen_dev->keybit);
	__set_bit(BTN_TOUCH, pen_dev->keybit);
	__set_bit(BTN_TOOL_PEN, pen_dev->keybit);
	__set_bit(INPUT_PROP_DIRECT, pen_dev->propbit);
	input_set_abs_params(pen_dev, ABS_X, 0,
		cd->thp_mt_wrapper->input_pen_dev_config.max_x, 0, 0);
	input_set_abs_params(pen_dev, ABS_Y, 0,
		cd->thp_mt_wrapper->input_pen_dev_config.max_y, 0, 0);
	input_set_abs_params(pen_dev, ABS_PRESSURE, 0,
		cd->thp_mt_wrapper->input_pen_dev_config.pressure, 0, 0);
	input_set_abs_params(pen_dev, ABS_TILT_X,
		-1 * cd->thp_mt_wrapper->input_pen_dev_config.max_tilt_x,
		cd->thp_mt_wrapper->input_pen_dev_config.max_tilt_x, 0, 0);
	input_set_abs_params(pen_dev, ABS_TILT_Y,
		-1 * cd->thp_mt_wrapper->input_pen_dev_config.max_tilt_y,
		cd->thp_mt_wrapper->input_pen_dev_config.max_tilt_y, 0, 0);
	if (cd->support_stylus_extension)
		input_set_abs_params(pen_dev, ABS_MT_TWIST, -65536, 65535, 0, 0);
	__set_bit(TS_STYLUS_WAKEUP_TO_MEMO, pen_dev->keybit);
	__set_bit(TS_STYLUS_WAKEUP_SCREEN_ON, pen_dev->keybit);
	return NO_ERR;
}

static int thp_set_extra_key_input_config(
	struct input_dev *extra_key_dev)
{
	if (extra_key_dev == NULL) {
		thp_log_err("%s:input null ptr\n", __func__);
		return -EINVAL;
	}
	__set_bit(EV_SYN, extra_key_dev->evbit);
	__set_bit(EV_KEY, extra_key_dev->evbit);
	__set_bit(KEY_VOLUME_UP, extra_key_dev->keybit);
	__set_bit(KEY_VOLUME_DOWN, extra_key_dev->keybit);
	__set_bit(KEY_POWER, extra_key_dev->keybit);
	__set_bit(KEY_VOLUME_MUTE, extra_key_dev->keybit);
	__set_bit(KEY_VOLUME_TRIG, extra_key_dev->keybit);

	return NO_ERR;
}

static int thp_input_pen_device_register(u32 panel_id)
{
	int rc;
	struct thp_input_pen_dev_config *pen_config = NULL;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *pen_dev = input_allocate_device();

	if (pen_dev == NULL) {
		thp_log_err("%s:failed to allocate memory\n", __func__);
		rc = -ENOMEM;
		goto err_out;
	}

	pen_dev->name = TS_PEN_DEV_NAME;
	cd->thp_mt_wrapper->pen_dev = pen_dev;
	pen_config = &cd->thp_mt_wrapper->input_pen_dev_config;
	rc = thp_parse_pen_input_config(pen_config, cd->panel_id);
	if (rc)
		thp_log_err("%s: parse pen input config failed: %d\n",
			__func__, rc);

	rc = thp_set_pen_input_config(pen_dev, cd->panel_id);
	if (rc) {
		thp_log_err("%s:set input config failed : %d\n",
			__func__, rc);
		goto err_free_dev;
	}
	rc = input_register_device(pen_dev);
	if (rc) {
		thp_log_err("%s:input dev register failed : %d\n",
			__func__, rc);
		goto err_free_dev;
	}
	return rc;
err_free_dev:
	input_free_device(pen_dev);
err_out:
	return rc;
}

static int thp_input_extra_key_register(u32 panel_id)
{
	int rc;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *extra_key = input_allocate_device();

	if (extra_key == NULL) {
		thp_log_err("%s:failed to allocate memory\n", __func__);
		rc = -ENOMEM;
		goto err_out;
	}

	extra_key->name = TS_EXTRA_KEY_DEV_NAME;
	cd->thp_mt_wrapper->extra_key_dev = extra_key;

	rc = thp_set_extra_key_input_config(extra_key);
	if (rc) {
		thp_log_err("%s:set input config failed : %d\n",
			__func__, rc);
		goto err_free_dev;
	}
	rc = input_register_device(extra_key);
	if (rc) {
		thp_log_err("%s:input dev register failed : %d\n",
			__func__, rc);
		goto err_free_dev;
	}
	return rc;
err_free_dev:
	input_free_device(extra_key);
err_out:
	return rc;
}

static int thp_input_hand_status_register(u32 panel_id)
{
	int rc;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct input_dev *hand_status_dev = input_allocate_device();

	if (hand_status_dev == NULL) {
		thp_log_err("%s:failed to allocate memory\n", __func__);
		rc = -ENOMEM;
		goto err_out;
	}

	hand_status_dev->name = "hand_status_dev";
	cd->thp_mt_wrapper->hand_status_dev = hand_status_dev;

	__set_bit(ABS_HAND_FEATURE, hand_status_dev->absbit);
	__set_bit(INPUT_PROP_DIRECT, hand_status_dev->propbit);
	input_set_abs_params(hand_status_dev, ABS_HAND_FEATURE, 0,
		0xffffffff, 0, 0);

	rc = input_register_device(hand_status_dev);
	if (rc) {
		thp_log_err("%s:input dev register failed : %d\n",
			__func__, rc);
		goto err_free_dev;
	}
	return rc;
err_free_dev:
	input_free_device(hand_status_dev);
err_out:
	return rc;
}

static void thp_input_dev_set_bit(struct input_dev *input_dev)
{
	__set_bit(EV_SYN, input_dev->evbit);
	__set_bit(EV_KEY, input_dev->evbit);
	__set_bit(EV_ABS, input_dev->evbit);
	__set_bit(BTN_TOUCH, input_dev->keybit);
	__set_bit(BTN_TOOL_FINGER, input_dev->keybit);
	__set_bit(INPUT_PROP_DIRECT, input_dev->propbit);
	__set_bit(KEY_F26, input_dev->keybit);
	__set_bit(TS_DOUBLE_CLICK, input_dev->keybit);
	__set_bit(KEY_FINGER_DOWN, input_dev->keybit);
	__set_bit(KEY_FINGER_UP, input_dev->keybit);
	__set_bit(TS_STYLUS_WAKEUP_TO_MEMO, input_dev->keybit);
	__set_bit(KEY_VOLUME_UP, input_dev->keybit);
	__set_bit(KEY_VOLUME_DOWN, input_dev->keybit);
	__set_bit(KEY_POWER, input_dev->keybit);
	__set_bit(TS_SINGLE_CLICK, input_dev->keybit);
#ifdef CONFIG_LIBLINUX
	__set_bit(AOD_SLIDE_UNLOCK, input_dev->keybit);
	/* Key_F4 is reused as the single click screen-on key event */
	__set_bit(KEY_F4, input_dev->keybit);
#endif
}

static void thp_input_dev_set_params(struct input_dev *input_dev,
	struct thp_mt_wrapper_data *mt_wrapper, u32 panel_id)
{
	unsigned int magnification;

	thp_input_dev_set_bit(input_dev);

	magnification = thp_get_finger_resolution_magnification(panel_id);
	input_set_abs_params(input_dev, ABS_X, 0,
		(mt_wrapper->input_dev_config.abs_max_x * magnification) - 1,
		0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0,
		(mt_wrapper->input_dev_config.abs_max_y * magnification) - 1,
		0, 0);
	input_set_abs_params(input_dev, ABS_PRESSURE,
		0, mt_wrapper->input_dev_config.abs_max_z, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0,
		(mt_wrapper->input_dev_config.abs_max_x * magnification) - 1,
		0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0,
		(mt_wrapper->input_dev_config.abs_max_y * magnification) - 1,
		0, 0);
	input_set_abs_params(input_dev, ABS_MT_PRESSURE,
			0, mt_wrapper->input_dev_config.abs_max_z, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TRACKING_ID, 0,
			mt_wrapper->input_dev_config.tracking_id_max - 1, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR,
			0, mt_wrapper->input_dev_config.major_max, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MINOR,
			0, mt_wrapper->input_dev_config.minor_max, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_ORIENTATION,
			mt_wrapper->input_dev_config.orientation_min,
			mt_wrapper->input_dev_config.orientation_max, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_BLOB_ID, 0,
			INPUT_MT_WRAPPER_MAX_FINGERS, 0, 0);
	input_set_abs_params(input_dev, ABS_DELTA_TIME, 0,
		0xffffffff, 0, 0);
	input_set_abs_params(input_dev, ABS_T_SEC, 0,
		0xffffffff, 0, 0);
	input_set_abs_params(input_dev, ABS_T_USEC, 0,
		0xffffffff, 0, 0);
#ifdef TYPE_B_PROTOCOL
	input_mt_init_slots(input_dev, THP_MT_WRAPPER_MAX_FINGERS);
#endif
}

static int thp_input_dev_register(struct input_dev *input_dev,
	struct thp_mt_wrapper_data *mt_wrapper, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	int rc;

	input_dev->name = THP_INPUT_DEVICE_NAME;
	if (panel_id == 1)
		 input_dev->name = "input_mt_wrapper1";
	if (cd->support_input_location &&
		(cd->multi_panel_index != SINGLE_TOUCH_PANEL)) {
		rc = snprintf(cd->input_dev_location, MULTI_PANEL_NODE_BUF_LEN,
			"%s%d", INPUT_DEV_LOCATION_NAME, cd->multi_panel_index);
		if (rc < 0) {
			thp_log_err("%s: snprintf err\n", __func__);
			return rc;
		}

		input_dev->phys = (const char *)(cd->input_dev_location);
		thp_log_info("%s location name is :%s\n", __func__,
			input_dev->phys);
	}

	thp_input_dev_set_params(input_dev, mt_wrapper, panel_id);
	rc = input_register_device(input_dev);
	if (rc) {
		thp_log_err("%s:failed to register input device\n", __func__);
		return rc;
	}
	return NO_ERR;
}

static int thp_input_device_init(struct input_dev **input_dev,
	struct thp_mt_wrapper_data *mt_wrapper, u32 panel_id)
{
	int rc;

	*input_dev = input_allocate_device();
	if (!(*input_dev)) {
		thp_log_err("%s:Unable to allocated input device\n", __func__);
		return -ENODEV;
	}

	rc = thp_parse_input_config(&mt_wrapper->input_dev_config, panel_id);
	if (rc)
		thp_log_err("%s: parse config fail\n", __func__);

	rc = thp_input_dev_register(*input_dev, mt_wrapper, panel_id);
	if (rc) {
		thp_log_err("%s: input dev register fail\n", __func__);
		return rc;
	}
	return NO_ERR;
}

static int thp_misc_device_init(struct thp_core_data *cd,
	struct input_dev *input_dev, struct thp_mt_wrapper_data *mt_wrapper)
{
	int rc;

	cd->thp_mt_wrapper_misc_device.minor = MISC_DYNAMIC_MINOR;
	cd->thp_mt_wrapper_misc_device.name = DEVICE_NAME;
	cd->thp_mt_wrapper_misc_device.fops = &g_thp_mt_wrapper_fops;
	if (cd->multi_panel_index != SINGLE_TOUCH_PANEL) {
		cd->thp_mt_wrapper_misc_device.name = mt_wrapper_misc_device_name[cd->panel_id];
		thp_log_info("%s misc name is :%s\n", __func__,
			cd->thp_mt_wrapper_misc_device.name);
	}
	rc = misc_register(&cd->thp_mt_wrapper_misc_device);
	if (rc) {
		thp_log_err("%s:failed to register misc device\n", __func__);
		return rc;
	}

	mt_wrapper->input_dev = input_dev;
	cd->thp_mt_wrapper = mt_wrapper;

	return NO_ERR;
}

static void thp_extra_input_device_init(struct thp_core_data *cd)
{
	int rc;

	if (cd->pen_supported) {
		rc = thp_input_pen_device_register(cd->panel_id);
		if (rc)
			thp_log_err("%s:pen register failed\n", __func__);
	}
	if (cd->support_extra_key_event_input) {
		rc = thp_input_extra_key_register(cd->panel_id);
		if (rc)
			thp_log_err("%s:ring key register failed\n", __func__);
	}
	if (cd->support_hand_status_dev) {
		rc = thp_input_hand_status_register(cd->panel_id);
		if (rc)
			thp_log_err("%s:hand status dev register failed\n", __func__);
	}
	atomic_set(&cd->thp_mt_wrapper->status_updated, 0);
	if ((cd->pen_supported) && (cd->pen_mt_enable_flag)) {
		atomic_set(&cd->last_stylus3_status, 0);
		atomic_set(&cd->callback_event_flag, false);
		init_completion(&cd->stylus3_status_flag);
		init_completion(&cd->stylus3_callback_flag);
	}
}

int thp_mt_wrapper_init(u32 panel_id)
{
	struct input_dev *input_dev = NULL;
	struct thp_mt_wrapper_data *mt_wrapper;
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	int rc;

	if (cd->thp_mt_wrapper) {
		thp_log_err("%s:thp_mt_wrapper have inited, exit\n", __func__);
		return NO_ERR;
	}

	mt_wrapper = kzalloc(sizeof(struct thp_mt_wrapper_data), GFP_KERNEL);
	if (!mt_wrapper) {
		thp_log_err("%s:out of memory\n", __func__);
		return -ENOMEM;
	}
	init_waitqueue_head(&mt_wrapper->wait);

	rc = thp_input_device_init(&input_dev, mt_wrapper, panel_id);
	if (rc)
		goto input_dev_reg_err;

	rc = thp_misc_device_init(cd, input_dev, mt_wrapper);
	if (rc)
		goto misc_dev_reg_err;

	thp_extra_input_device_init(cd);

	return NO_ERR;

misc_dev_reg_err:
	input_unregister_device(input_dev);
input_dev_reg_err:
	kfree(mt_wrapper);

	return rc;
}
EXPORT_SYMBOL(thp_mt_wrapper_init);

void thp_mt_wrapper_exit(u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);

	if (!cd->thp_mt_wrapper)
		return;

	input_unregister_device(cd->thp_mt_wrapper->input_dev);
	if (cd->pen_supported)
		input_unregister_device(cd->thp_mt_wrapper->pen_dev);
	if (cd->support_hand_status_dev)
		input_unregister_device(cd->thp_mt_wrapper->hand_status_dev);
	misc_deregister(&cd->thp_mt_wrapper_misc_device);
	kfree(cd->thp_mt_wrapper);
	cd->thp_mt_wrapper = NULL;
}
EXPORT_SYMBOL(thp_mt_wrapper_exit);

