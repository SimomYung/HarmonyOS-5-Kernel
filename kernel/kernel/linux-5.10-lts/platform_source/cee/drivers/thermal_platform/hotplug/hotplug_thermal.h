/* SPDX-License-Identifier: GPL-2.0
 * hotplug_thermal.h
 *
 * header of thermal hotplug
 *
 * Copyright (C) 2020-2022 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __THERMAL_HOTPLUG_H
#define __THERMAL_HOTPLUG_H
#include <linux/thermal.h>
#include <linux/kthread.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/of_platform.h>
#include <linux/workqueue.h>
#include <platform_include/cee/linux/lpm_thermal.h>
#include <securec.h>

#ifdef CONFIG_THERMAL_HOTPLUG
#define MAX_LEN	15
#ifdef CONFIG_THERMAL_PERCORE
#define MAX_TDP_LEVEL 2
#endif
enum HOTPLUG_STATUS {
	HOTPLUG_NONE = 0,
	HOTPLUG_NORMAL,
	HOTPLUG_CRITICAL,
	HOTPLUG_STATUS_NR,
};

struct hotplug_ops {
	const char name[MAX_LEN];
	int physical_status;
	int status;

	int down_threshold;
	int up_threshold;
	int critical_down_threshold;
	int critical_up_threshold;

	int (*get_temp)(int *val);
	int current_temp;
#ifdef CONFIG_HOTPLUG_EMULATION
	int emul_temp;
#endif

	void (*fsm_callback)(enum HOTPLUG_STATUS);
	u32 control_val[HOTPLUG_STATUS_NR];

	int (*init)(struct device_node *np, struct device *dev);
	void (*exit)(struct device *dev);

	struct list_head ops_list;
};

struct hotplug_t {
	struct device *device;

	struct kthread_work work;
	struct kthread_worker worker;
	struct task_struct *thread;

	struct delayed_work poll_work;
	u32 polling_delay;
	u32 polling_delay_passive;

	bool hotplug_enabled;
};

struct thermal_hotplug {
	struct class *thermal_class;
	struct hotplug_t hotplug;
};

int ipa_get_dev_temp(const char *name, int *val);
int hotplug_ops_register(struct hotplug_ops *hp_ops);
void hotplug_ops_unregister(struct hotplug_ops *hp_ops);

#ifdef CONFIG_HOTPLUG_EMULATION
#define DECIMAL	10

struct hotplug_ops *search_hotplug_ops(const char *name);

#define show_hotplug_attr(ip, mode)				\
static ssize_t show_ip##_##mode					\
(struct device *dev, struct device_attribute *attr, char *buf)	\
{								\
	struct hotplug_ops *ops = NULL;				\
								\
	if (dev == NULL || attr == NULL || buf == NULL)		\
		return 0;					\
	ops = search_hotplug_ops(#ip);				\
	if (ops == NULL)					\
		return 0;					\
	return snprintf_s(buf, MAX_LEN,  MAX_LEN - 1,		\
			  "%d\n", ops->mode);			\
}

#define store_hotplug_attr(ip, mode)			\
static ssize_t store_ip##_##mode			\
(struct device *dev, struct device_attribute *attr,	\
	const char *buf, size_t count)			\
{							\
	struct hotplug_ops *ops = NULL;			\
	int value = 0;					\
							\
	if (dev == NULL || attr == NULL || buf == NULL)	\
		return 0;				\
	ops = search_hotplug_ops(#ip);			\
	if (ops == NULL)				\
		return 0;				\
	if (kstrtoint(buf, DECIMAL, &value))		\
		return -EINVAL;				\
	ops->mode = value;				\
	pr_err("%s set %s %d\n", #ip, #mode, value);	\
	return (ssize_t)count;				\
}

#define hotplug_attr_rw(name, ip, mode)		\
static DEVICE_ATTR(name, 0644,		\
		   show_ip##_##mode, store_ip##_##mode)

#endif

extern struct thermal_hotplug g_thermal_hotplug;

int thermal_hotplugs_init(void);
void thermal_hotplugs_exit(void);
#endif
#endif
