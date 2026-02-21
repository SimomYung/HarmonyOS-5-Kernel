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

#ifndef _INPUT_MT_WRAPPER_H_
#define _INPUT_MT_WRAPPER_H_

#include "huawei_thp.h"
#include "bus_adapter/huawei_bus_adapter.h"
#include "bus_adapter/huawei_spi_bus_platform.h"
#include "power_adapter/huawei_tdev_power_func.h"

#if defined(CONFIG_HUAWEI_DSM)
#include <dsm/dsm_pub.h>
#endif

#define INPUT_MT_WRAPPER_MAX_FINGERS 10
#define THP_MT_WRAPPER_MAX_FINGERS 10
#define THP_MT_WRAPPER_MAX_X 1079
#define THP_MT_WRAPPER_MAX_Y 1919
#define THP_MT_WRAPPER_MAX_Z 100
#define THP_MT_WRAPPER_MAX_MAJOR 1
#define THP_MT_WRAPPER_MAX_MINOR 1
#define THP_MT_WRAPPER_MIN_ORIENTATION (-90)
#define THP_MT_WRAPPER_MAX_ORIENTATION 90
#define THP_MT_WRAPPER_TOOL_TYPE_MAX 1
#define THP_MT_WRAPPER_TOOL_TYPE_STYLUS 1
#define THP_MT_WRAPPER_MAX_ORIENTATION 90
#define THP_MT_WRAPPER_TOOL_TYPE_MAX 1
#define THP_MT_WRAPPER_TOOL_TYPE_STYLUS 1
#define THP_PEN_WRAPPER_TILT_MAX_X 90

#define TS_MAX_PEN_BUTTON 2
#define TS_PEN_BUTTON_NONE 0
#define TS_PEN_BUTTON_RELEASE (1 << 5)
#define TS_PEN_BUTTON_PRESS (1 << 6)
#define TS_PEN_KEY_NONE 0
#define STYLUS3_CONNECTED_MASK (1 << 1)
#define STYLUS3_PLAM_SUPPRESSION_MASK (1 << 4)

#define THP_INPUT_DEV_COMPATIBLE "huawei,thp_input"
#define THP_PEN_INPUT_DEV_COMPATIBLE "huawei,thp_pen_input"
#define THP_EXTRA_KEY_INPUT_DEV_COMPATIBLE "huawei,thp_extra_key_input_dev"

#define DEFAULT_MAGNIFICATION 1

#define THP_DATA_ARRAY_SIZE 2
#define THP_PROJECTID_LEN 9
#define THP_PROJECTID_PRODUCT_NAME_LEN 4
#define THP_PROJECTID_IC_NAME_LEN 2
#define THP_PROJECTID_VENDOR_ID_LEN 2

enum input_mt_wrapper_state {
	INPUT_MT_WRAPPER_STATE_DEFAULT,
	INPUT_MT_WRAPPER_STATE_FIRST_TOUCH = 1,
	INPUT_MT_WRAPPER_STATE_LAST_TOUCH = 2,
	INPUT_MT_WRAPPER_STATE_SAME_REPORT,
	INPUT_MT_WRAPPER_STATE_SAME_ZERO_REPORT,
};

#define THP_INPUT_DEVICE_NAME "input_mt_wrapper"
#define TS_PEN_DEV_NAME "huawei,ts_pen"
#define TS_EXTRA_KEY_DEV_NAME "huawei,ts_extra_key"

#define THP_EVENT_ANTI_TOUCH_DETECTED (1 << 24)
#define THP_EVENT_ANTI_TOUCH_RELEASE (1 << 25)

#define THP_EVENT_DE_TOUCH_DETECTED (1 << 27)
#define THP_EVENT_DE_TOUCH_RELEASE (1 << 28)

#define ABS_HAND_FEATURE 0x27
#define ABS_HAND_OP_GESTURE 0x22 // Reserved stauts
#define ABS_LEFT_HAND_POSITION 0x23 // Reserved stauts
#define ABS_RIGHT_HAND_POSITION 0x24 // Reserved stauts
#define ABS_DELTA_TIME 0x29
#define ABS_T_SEC 0x2A
#define ABS_T_USEC 0x2B
#define ABS_MT_TWIST 0x2C

struct thp_hand_status {
	int hand_feature;
	int feature_1; // Reserved stauts
	int feature_2; // Reserved stauts
	int feature_3; // Reserved stauts
};

struct thp_input_dev_config {
	int abs_max_x;
	int abs_max_y;
	int abs_max_z;
	int tracking_id_max;
	int major_max;
	int minor_max;
	int orientation_min;
	int orientation_max;
	int tool_type_max;
};

struct thp_input_pen_dev_config {
	int max_x;
	int max_y;
	int pressure;
	int max_tilt_x;
	int max_tilt_y;
};

struct thp_mt_wrapper_data {
	struct input_dev *input_dev;
	struct input_dev *key_input_dev;
	struct input_dev *pen_dev;
	struct input_dev *extra_key_dev;
	struct input_dev *hand_status_dev;
	struct thp_input_dev_config input_dev_config;
	struct thp_input_pen_dev_config input_pen_dev_config;
	wait_queue_head_t wait;
	atomic_t status_updated;
};

struct thp_key_info {
	uint16_t key;
	uint8_t action;
};

struct thp_volumn_info {
	unsigned char data[THP_DATA_ARRAY_SIZE];
};

struct input_mt_wrapper_touch_data {
	unsigned char down;
	unsigned char valid; /* 0:invalid !=0:valid */
	int x;
	int y;
	int pressure;
	int tracking_id;
	int hand_side;
	int major;
	int minor;
	int orientation;
	unsigned int tool_type;
};

struct thp_mt_wrapper_ioctl_touch_data {
	struct input_mt_wrapper_touch_data touch[INPUT_MT_WRAPPER_MAX_FINGERS];
	enum input_mt_wrapper_state state;
	int t_num;
	int down_num;
};

struct thp_ioctl_touch_and_time_data {
	struct input_mt_wrapper_touch_data touch[INPUT_MT_WRAPPER_MAX_FINGERS];
	enum input_mt_wrapper_state state;
	int t_num;
	int down_num;
	unsigned int delta_time;
	unsigned int t_sec;
	unsigned int t_usec;
};

struct thp_tool {
	int pen_inrange_status;
	int tip_status;
	int x;
	int y;
	int pressure;
	signed char tilt_x;
	signed char tilt_y;
	int tool_type;
};

struct thp_button {
	int status;
	int key;
};

struct thp_mt_wrapper_ioctl_pen_data {
	struct thp_tool tool;
	struct thp_button buttons[TS_MAX_PEN_BUTTON];
};

struct thp_ioctl_pen_and_extension_data {
	struct thp_tool tool;
	struct thp_button buttons[TS_MAX_PEN_BUTTON];
	int confidence;
	int barrel;
	int azimuth;
	int invert;
	int eraser;
	int twist;
	int stylusonscreenoverheadangle;
};

#define KEY_F26 196

enum input_mt_wrapper_keyevent {
	INPUT_MT_WRAPPER_KEYEVENT_NULL = 0,
	INPUT_MT_WRAPPER_KEYEVENT_ESD = 1 << 0,
	INPUT_MT_WRAPPER_KEYEVENT_APPROACH = 1 << 1,
	INPUT_MT_WRAPPER_KEYEVENT_AWAY = 1 << 2,
};

struct thp_app_info {
	unsigned int len;
	char __user *buf;
};

#define TP_FAIL_NV_NUM 457
#define FAIL_NUM 4
#pragma pack(push, 1)
struct tp_fail_data {
	uint64_t time;
	uint16_t brk_sensor;
	uint8_t rect[4]; /* brk_sensor area: x1, y1, x2, y2 */
	uint16_t raw;
};
#pragma pack(pop)

struct tp_nv_data {
	int index;
	struct tp_fail_data tp_info[FAIL_NUM];
};

struct thp_mt_wrapper_ioctl_group {
	unsigned int cmd;
	int (*ioctl_func)(unsigned long arg, u32 panel_id);
};

#define PROX_VALUE_LEN 3
#define PROX_EVENT_LEN 12
#define APPROCH_EVENT_VALUE 0
#define AWAY_EVENT_VALUE 1

/* commands */
#define INPUT_MT_WRAPPER_IO_TYPE 0xB9
#define INPUT_MT_GET_IO_TYPE 0xBA
#define INPUT_MT_SET_IO_TYPE 0xBB
#define INPUT_MT_WRAPPER_IOCTL_CMD_SET_COORDINATES \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x01, \
		struct thp_mt_wrapper_ioctl_touch_data)
#define INPUT_MT_WRAPPER_IOCTL_READ_STATUS \
	_IOR(INPUT_MT_WRAPPER_IO_TYPE, 0x02, u32)
#define INPUT_MT_WRAPPER_IOCTL_READ_INPUT_CONFIG \
	_IOR(INPUT_MT_WRAPPER_IO_TYPE, 0x03, struct thp_input_dev_config)
#define INPUT_MT_WRAPPER_IOCTL_READ_SCENE_INFO \
	_IOR(INPUT_MT_WRAPPER_IO_TYPE, 0x04, struct thp_scene_info)
#define INPUT_MT_WRAPPER_IOCTL_CMD_SET_EVENTS \
	_IOR(INPUT_MT_WRAPPER_IO_TYPE, 0x05, uint32_t)
#define INPUT_MT_WRAPPER_IOCTL_CMD_GET_EVENTS \
	_IOR(INPUT_MT_WRAPPER_IO_TYPE, 0x06, uint32_t)
#define INPUT_MT_WRAPPER_IOCTL_CMD_REPORT_KEYEVENT \
	_IOR(INPUT_MT_WRAPPER_IO_TYPE, 0x07, u32)
#define INPUT_MT_WRAPPER_IOCTL_GET_PROJECT_ID \
	_IOR(INPUT_MT_WRAPPER_IO_TYPE, 0x08, uint32_t)
#define INPUT_MT_WRAPPER_IOCTL_SET_ROI_DATA \
	_IOW(INPUT_MT_WRAPPER_IO_TYPE, 0x09, uint32_t)
#define INPUT_MT_WRAPPER_IOCTL_GET_WINDOW_INFO \
	_IOR(INPUT_MT_WRAPPER_IO_TYPE, 0x0a, struct thp_window_info)
#define INPUT_MT_WRAPPER_IOCTL_REPORT_SYSTEM_KEYEVENT \
	_IOR(INPUT_MT_WRAPPER_IO_TYPE, 0x0b, struct thp_key_info)
#define INPUT_MT_WRAPPER_IOCTL_CMD_REPORT_PEN \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x0c, \
		struct thp_mt_wrapper_ioctl_pen_data)
#if ((defined CONFIG_HUAWEI_SHB_THP) || (defined CONFIG_HUAWEI_THP_MTK) || (defined CONFIG_HUAWEI_THP_QCOM))
#define INPUT_MT_WRAPPER_IOCTL_CMD_SHB_EVENT \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x0d, struct thp_shb_info)
#endif
#define INPUT_MT_WRAPPER_IOCTL_GET_PLATFORM_TYPE \
	_IOR(INPUT_MT_WRAPPER_IO_TYPE, 0x0e, uint32_t)
#define INPUT_MT_WRAPPER_IOCTL_GET_VOMLUME_SIDE \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x0f, uint32_t)
#define INPUT_MT_WRAPPER_IOCTL_GET_POWER_SWITCH \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x10, uint32_t)
#define INPUT_MT_WRAPPER_IOCTL_GET_APP_INFO \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x11, struct thp_app_info)
#define INPUT_MT_WRAPPER_IOCTL_READ_FINGER_RESOLUTION_MAGNIFICATION \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x12, uint32_t)
#define INPUT_MT_WRAPPER_IOCTL_CMD_SET_COORDINATES_AND_TIME \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x13, \
		struct thp_ioctl_touch_and_time_data)
#define INPUT_MT_WRAPPER_IOCTL_CMD_SET_TP_BROKEN_INFO \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x14, struct tp_fail_data)
#define INPUT_MT_WRAPPER_IOCTL_CMD_SET_SCREEN_STATUS \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x15, uint32_t)
#define INPUT_MT_WRAPPER_IOCTL_CMD_FORCE_WAKEUP \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x16, uint32_t)
#define INPUT_MT_WRAPPER_IOCTL_CMD_REPORT_HAND_STATUS \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x18, \
		struct thp_hand_status)
#define INPUT_MT_WRAPPER_IOCTL_CMD_REPORT_PEN_AND_EXTENSION \
	_IOWR(INPUT_MT_WRAPPER_IO_TYPE, 0x19, \
		struct thp_ioctl_pen_and_extension_data)

#define INPUT_MT_IOCTL_CMD_GET_STYLUS3_CONNECT_STATUS \
	_IOWR(INPUT_MT_GET_IO_TYPE, 0x05, unsigned int)
#define INPUT_MT_IOCTRL_CMD_SET_STYLUS3_CONNECT_STATUS \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x08, unsigned int)
#define INPUT_MT_IOCTL_CMD_GET_CALLBACK_EVENTS \
	_IOWR(INPUT_MT_GET_IO_TYPE, 0x07, struct tp_callback_event)
#define INPUT_MT_IOCTL_CMD_SET_CALLBACK_EVENTS \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x09, struct tp_callback_event)
#define INPUT_MT_IOCTL_CMD_SET_DAEMON_INIT_PROTECT \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x0a, uint32_t)
#define INPUT_MT_IOCTL_CMD_SET_DAEMON_POWER_RESET \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x0b, uint32_t)
#define INPUT_MT_IOCTL_CMD_SET_STYLUS3_PLAM_SUPPRESSION_STATUS \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x0c, unsigned int)
#define INPUT_MT_IOCTL_CMD_SET_STYLUS_ADSORPTION_STATUS \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x0d, unsigned int)
#define INPUT_MT_IOCTL_CMD_SET_STYLUS3_WORK_MODE \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x0e, unsigned int)
#define INPUT_MT_IOCTL_CMD_SET_EXTERNAL_CLK \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x0f, unsigned int)
#define INPUT_MT_IOCTL_CMD_GET_EXTERNAL_CLK \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x10, unsigned int)
/* 0x11:Reserved event, which is not implemented currently */
#define INPUT_MT_IOCTL_CMD_POWER_CONTROL \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x11, unsigned int)
#define INPUT_MT_IOCTL_CMD_GET_POWER_STATUS \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x12, unsigned int)
#define INPUT_MT_IOCTL_CMD_TP_RECOVERY \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x13, unsigned int)
#define INPUT_MT_IOCTL_CMD_SET_STYLUS_HIGH_HOVER \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x14, unsigned int)
#define INPUT_MT_IOCTL_CMD_SET_STYLUS_LITE_STATUS \
	_IOWR(INPUT_MT_SET_IO_TYPE, 0x15, unsigned int)

int thp_mt_wrapper_init(u32 panel_id);
void thp_mt_wrapper_exit(u32 panel_id);
int thp_mt_wrapper_wakeup_poll(u32 panel_id);
void thp_clean_fingers(u32 panel_id);
int thp_mt_wrapper_esd_event(unsigned int status, u32 panel_id);
unsigned int thp_get_finger_resolution_magnification(u32 panel_id);

int thp_mt_wrapper_ioctl_cmd_set_coordinates_and_time(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_cmd_set_coordinates(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_report_pen(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_report_pen_and_extension(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_read_status(unsigned long arg, u32 panel_id);
int thp_mt_ioctl_read_input_config(unsigned long arg, u32 panel_id);
int thp_mt_ioctl_read_finger_resolution_magnification(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_read_scene_info(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_get_window_info(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_get_projectid(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_set_events(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_get_events(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_set_roi_data(unsigned long arg, u32 panel_id);
int thp_mt_ioctl_report_keyevent(unsigned long arg, u32 panel_id);
int thp_mt_ioctl_report_system_keyevent(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_report_hand_status(unsigned long arg, u32 panel_id);
int thp_mt_wrapper_ioctl_get_platform_type(unsigned long arg, u32 panel_id);
#if ((defined CONFIG_HUAWEI_SHB_THP) || (defined CONFIG_HUAWEI_THP_MTK) || (defined CONFIG_HUAWEI_THP_QCOM))
int thp_mt_ioctl_cmd_shb_event(unsigned long arg, u32 panel_id);
#endif
int thp_ioctl_get_volume_side(unsigned long arg, u32 panel_id);
int thp_ioctl_get_power_switch(unsigned long arg, u32 panel_id);
int ioctl_get_app_info(unsigned long arg, u32 panel_id);
int thp_wrapper_ioctl_get_stylus3_connect_status(unsigned long arg, u32 panel_id);
int thp_wrapper_ioctl_set_stylus3_connect_status(unsigned long arg, u32 panel_id);
int thp_ioctl_get_callback_events(unsigned long arg, u32 panel_id);
int thp_ioctl_set_callback_events(unsigned long arg, u32 panel_id);
int daemon_init_protect(unsigned long arg, u32 panel_id);
int thp_daemon_power_reset(unsigned long arg, u32 panel_id);
int thp_ioctl_set_stylus3_plam_suppression(unsigned long arg, u32 panel_id);
int thp_ioctl_set_stylus_adsorption_status(unsigned long arg, u32 panel_id);
int thp_wrapper_ioctl_set_stylus3_work_mode(unsigned long arg, u32 panel_id);
int thp_wrapper_ioctl_set_external_clk(unsigned long arg, u32 panel_id);
int thp_wrapper_ioctl_get_external_clk(unsigned long arg, u32 panel_id);
int thp_ioctl_set_tp_broken_info(unsigned long arg, u32 panel_id);
int thp_ioctl_set_screen_status(unsigned long arg, u32 panel_id);
int thp_wrapper_ioctl_force_wakeup(unsigned long arg, u32 panel_id);
int thp_wrapper_ioctl_get_power_status(unsigned long arg, u32 panel_id);
int thp_wrapper_ioctl_tp_recovery(unsigned long arg, u32 panel_id);
int thp_ioctl_set_stylus_high_hover(unsigned long arg, u32 panel_id);
int thp_ioctl_set_stylus_lite_status(unsigned long arg, u32 panel_id);
#endif /* _INPUT_MT_WRAPPER_H_ */
