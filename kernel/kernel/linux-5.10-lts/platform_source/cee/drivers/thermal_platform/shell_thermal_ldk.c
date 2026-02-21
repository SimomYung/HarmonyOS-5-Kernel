// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 * Description:
 * Shell thermal ldk, send shell_frame_temp to kernel.
 */

#include <linux/thermal.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <liblinux/pal.h>

static struct delayed_work g_thermal_work;

static unsigned int get_shell_thermal_temp(void)
{
	int temp;
	struct thermal_zone_device *shell_tz = NULL;

	shell_tz = thermal_zone_get_zone_by_name("shell_frame");
	if (IS_ERR(shell_tz)) {
		pr_err("get shell zone error\n");
		return 0;
	}
	if (thermal_zone_get_temp(shell_tz, &temp) != 0) {
		pr_err("get shell temp error\n");
		return 0;
	}

	if (temp < 0)
		return 0;
	return (unsigned int)temp;
}

static void shell_thermal_work(struct work_struct *work)
{
	int ret;
	unsigned int temp;

	temp = get_shell_thermal_temp();
	ret = liblinux_pal_cooling_freq_limit((unsigned int)temp);
	if (ret != 0)
		pr_err("set shell temp fail %d\n", ret);

	mod_delayed_work(system_freezable_power_efficient_wq,
			 &g_thermal_work,
			 msecs_to_jiffies(1000));
}

static int shell_thermal_ldk_init(void)
{
	INIT_DEFERRABLE_WORK(&g_thermal_work, shell_thermal_work);

	mod_delayed_work(system_freezable_power_efficient_wq,
			 &g_thermal_work,
			 msecs_to_jiffies(1000));
	return 0;
}

static void shell_thermal_ldk_exit(void)
{
	cancel_delayed_work(&g_thermal_work);
}

late_initcall(shell_thermal_ldk_init);
module_exit(shell_thermal_ldk_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("shell_thermal_ldk module driver");
