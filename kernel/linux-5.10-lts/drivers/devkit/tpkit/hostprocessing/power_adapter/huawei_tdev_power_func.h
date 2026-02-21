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
 
#ifndef __TDEV_POWER_FUNC__
#define __TDEV_POWER_FUNC__
#include "huawei_thp.h"

/* power adapter ops */
struct thp_power_ops {
	int (*init)(struct thp_device *tdev);
	int (*release)(struct thp_device *tdev);
	int (*poweron)(struct thp_device *tdev, struct thp_poweron_timing *g_poweron_timing);
	int (*poweroff)(struct thp_device *tdev, struct thp_poweroff_timing *g_poweroff_timing);
};

/* common power control ops */
struct common_power_control {
	u32 power_on_flag;
	int (*power_on)(struct thp_device *tdev);
	int (*power_off)(struct thp_device *tdev);
};

/* independent power call function */
int independent_power_init(struct thp_device *tdev);
int independent_power_release(struct thp_device *tdev);
int use_independent_power_on(struct thp_device *tdev, struct thp_poweron_timing *g_poweron_timing);
int use_independent_power_off(struct thp_device *tdev, struct thp_poweroff_timing *g_poweroff_timing);
int thp_power_on(struct thp_poweron_timing *poweron_timing, struct thp_device *tdev);
int thp_power_off(struct thp_poweroff_timing *poweroff_timing, struct thp_device *tdev);
void thp_set_power_on_timing(struct thp_device *tdev, struct thp_poweron_timing *poweron_timing);
void thp_set_power_off_timing(struct thp_device *tdev, struct thp_poweroff_timing *poweroff_timing);
int thp_set_timing_value(struct thp_device *tdev, struct thp_poweron_timing *poweron_timing,
	struct thp_poweroff_timing *poweroff_timing);

/* common power call function */
int common_power_init(struct thp_device *tdev);
int common_power_release(struct thp_device *tdev);
int multi_panel_use_common_power_on(struct thp_device *tdev,
	struct thp_poweron_timing *g_poweron_timing);
int multi_panel_use_common_power_off(struct thp_device *tdev,
	struct thp_poweroff_timing *g_poweroff_timing);
int touch_driver_power_on_for_seq0(struct thp_device *tdev);
int touch_driver_power_off_for_seq0(struct thp_device *tdev);
int thp_common_power_on(struct thp_device *tdev, struct thp_poweron_timing *g_poweron_timing);
struct common_power_control *get_common_power_control(u8 power_sequence);
u32 thp_get_panel_for_common_power_ctrl(struct thp_core_data *cd);
void thp_common_power_init(struct thp_device *tdev);
void thp_common_power_supply_ctrl(struct thp_poweron_timing *poweron_timing,
	u32 panel_id);
int thp_common_power_off(struct thp_device *tdev, struct thp_poweroff_timing *g_poweroff_timing);
void thp_common_poweroff_supply_ctrl(struct thp_poweroff_timing *poweroff_timing,
	u32 panel_id);

/* tddi power call function */
int tp_use_tddi_power_on(struct thp_device *tdev, struct thp_poweron_timing *g_poweron_timing);
int tp_use_tddi_power_off(struct thp_device *tdev, struct thp_poweroff_timing *g_poweroff_timing);
int thp_parse_power_config(struct device_node *thp_node,
	struct thp_core_data *cd);
#endif