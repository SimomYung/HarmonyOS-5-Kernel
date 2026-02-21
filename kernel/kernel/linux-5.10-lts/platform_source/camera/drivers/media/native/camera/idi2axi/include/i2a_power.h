/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi common power interface
 *
 * This file is released under the GPLv2.
 */
#ifndef I2A_POWER_H
#define I2A_POWER_H
#include <linux/types.h>
#include <linux/device.h>

struct power_setting;
struct power_setting_group {
	struct power_setting *settings;
	int nsettings;
};

int get_power_setting(struct device *dev, struct power_setting_group *grp);
int enable_power_setting(struct power_setting_group *grp);
void disable_power_setting(struct power_setting_group *grp);
#endif /* end of include guard: I2A_POWER_H */
