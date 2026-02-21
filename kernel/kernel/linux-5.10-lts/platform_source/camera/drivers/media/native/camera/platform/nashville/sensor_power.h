/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: common power interface
 *
 * This file is released under the GPLv2.
 */
#ifndef SENSOR_POWER_H
#define SENSOR_POWER_H
#include <linux/device.h>
#include <linux/types.h>

struct power_setting_group;

enum power_setting_type {
	PS_SENSOR,
	PS_VCM,
	PS_PUBLIC,
	PS_MATCHID,
	PS_MAX
};

struct power_setting_group *parse_power_setting(struct device *dev);
int sensor_enable_power_setting(struct power_setting_group *grp, enum power_setting_type ps_type);
void sensor_disable_power_setting(struct power_setting_group *grp, enum power_setting_type ps_type);
int sensor_ext_vcm_power_on_off(struct power_setting_group *grpm, bool on);
int sensor_release_hold(struct power_setting_group *grp, bool release);
int sensor_mipi_switch(struct power_setting_group *grp);
int sensor_btb_check(struct power_setting_group *grp);
int sensor_fsin_check(struct power_setting_group *grp);
bool pset_group_invalid(struct power_setting_group *grp,
	enum power_setting_type ps_type);
#endif /* end of include guard: SENSOR_POWER_H */

