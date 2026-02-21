// SPDX-License-Identifier: GPL-2.0
/*
 * hotplug_thermal_gov.c
 *
 * thermal hotplug governer
 *
 * Copyright (C) 2017-2024 Huawei Technologies Co., Ltd.
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

#include "hotplug_thermal.h"
#include <platform_include/cee/linux/thermal_interact_lpmcu.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sched/task.h>
#include <linux/of_platform.h>
#include <uapi/linux/sched/types.h>

LIST_HEAD(g_hotplug_ops_list);
DEFINE_MUTEX(g_hotplug_ops_list_lock);

#define DEFAULT_POLL_DELAY	200 /* unit is ms */

int hotplug_ops_register(struct hotplug_ops *hp_ops)
{
	struct hotplug_ops *ops = NULL;

	if (hp_ops == NULL) {
		pr_err("%s hp_ops is invalid\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&g_hotplug_ops_list_lock);
	list_for_each_entry(ops, &g_hotplug_ops_list, ops_list) {
		if (strncmp(hp_ops->name, ops->name, MAX_LEN) == 0) {
			mutex_unlock(&g_hotplug_ops_list_lock);
			pr_err("%s hp_ops has registered\n", hp_ops->name);
			return -EINVAL;
		}
	}
	list_add_tail(&hp_ops->ops_list, &g_hotplug_ops_list);
	mutex_unlock(&g_hotplug_ops_list_lock);

	return 0;
}

void hotplug_ops_unregister(struct hotplug_ops *hp_ops)
{
	struct hotplug_ops *ops = NULL;

	if (hp_ops == NULL) {
		pr_err("%s hp_ops is invalid\n", __func__);
		return;
	}

	mutex_lock(&g_hotplug_ops_list_lock);
	list_for_each_entry(ops, &g_hotplug_ops_list, ops_list) {
		if (strncmp(hp_ops->name, ops->name, MAX_LEN) == 0) {
			list_del_init(&ops->ops_list);
			break;
		}
	}
	mutex_unlock(&g_hotplug_ops_list_lock);
	pr_err("%s:%s unregister\n", __func__, hp_ops->name);
}

#ifdef CONFIG_HOTPLUG_EMULATION
struct hotplug_ops *search_hotplug_ops(const char *name)
{
	struct hotplug_ops *ops = NULL;
	struct hotplug_ops *tmp = NULL;

	list_for_each_entry(tmp, &g_hotplug_ops_list, ops_list) {
		if (strncmp(name, tmp->name, MAX_LEN) == 0) {
			ops = tmp;
			break;
		}
	}

	return ops;
}
#endif

static ssize_t hotplug_mode_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	if (dev == NULL || attr == NULL || buf == NULL)
		return -EINVAL;

	return snprintf_s(buf, (unsigned long)MAX_LEN, (unsigned long)(MAX_LEN - 1), "%s\n",
			  g_thermal_hotplug.hotplug.hotplug_enabled ? "enabled" : "disabled");
}

static ssize_t hotplug_mode_store(struct device *dev, struct device_attribute *attr,
			  const char *buf, size_t count)
{
	if (dev == NULL || attr == NULL || buf == NULL)
		return -EINVAL;

	if (strncmp(buf, "enabled", sizeof("enabled") - 1) == 0)
		g_thermal_hotplug.hotplug.hotplug_enabled = true;
	else if (strncmp(buf, "disabled", sizeof("disabled") - 1) == 0)
		g_thermal_hotplug.hotplug.hotplug_enabled = false;
	else
		return -EINVAL;

	return (ssize_t)count;
}

static DEVICE_ATTR_RW(hotplug_mode);

typedef int (*fsm_fp)(struct hotplug_ops *ops);

static int normal_fsm(struct hotplug_ops *ops);
static int first_fsm(struct hotplug_ops *ops);
static int second_fsm(struct hotplug_ops *ops);

static fsm_fp g_fsm_array[HOTPLUG_STATUS_NR] = {
	normal_fsm,
	first_fsm,
	second_fsm,
};

static int normal_fsm(struct hotplug_ops *ops)
{
	if (ops->current_temp > ops->critical_down_threshold)
		return HOTPLUG_CRITICAL;

	if (ops->current_temp > ops->down_threshold)
		return HOTPLUG_NORMAL;

	return HOTPLUG_NONE;
}

static int first_fsm(struct hotplug_ops *ops)
{
	if (ops->current_temp < ops->up_threshold)
		return HOTPLUG_NONE;

	if (ops->current_temp > ops->critical_down_threshold)
		return HOTPLUG_CRITICAL;

	return HOTPLUG_NORMAL;
}

static int second_fsm(struct hotplug_ops *ops)
{
	if (ops->current_temp < ops->up_threshold)
		return HOTPLUG_NONE;

	if (ops->current_temp < ops->critical_up_threshold)
		return HOTPLUG_NORMAL;

	return HOTPLUG_CRITICAL;
}

static void hotplug_poll_work(struct work_struct *poll_work)
{
	bool poll_passive = false;
	enum HOTPLUG_STATUS status;
	struct hotplug_ops *ops = NULL;
	struct hotplug_t *hotplug = &g_thermal_hotplug.hotplug;
	struct delayed_work *delayed_work = to_delayed_work(poll_work);
	u32 polling_delay = hotplug->polling_delay;

	if (!hotplug->hotplug_enabled)
		goto mod_work;

	list_for_each_entry(ops, &g_hotplug_ops_list, ops_list) {
		if (ops->get_temp(&ops->current_temp) != 0) {
			pr_err("%s get_temp fail\n", ops->name);
			continue;
		}

#ifdef CONFIG_HOTPLUG_EMULATION
		if (ops->emul_temp != 0)
			ops->current_temp = ops->emul_temp;
#endif

		status = g_fsm_array[ops->status](ops);
		if (status != ops->status) {
			pr_err("%s hotplug:temp=%d, status=%d\n", ops->name, ops->current_temp, status);
			ops->status = status;
			kthread_queue_work(&hotplug->worker, &hotplug->work);
		}

		if (ops->status > HOTPLUG_NONE)
			poll_passive = true;
	}

	if (poll_passive)
		polling_delay = hotplug->polling_delay_passive;
	else
		polling_delay = hotplug->polling_delay;
mod_work:
	mod_delayed_work(system_freezable_power_efficient_wq, delayed_work,
			 msecs_to_jiffies(polling_delay));
}

static void thermal_hotplug_action(struct kthread_work *work)
{
	struct hotplug_ops *ops = NULL;
	int status;

	list_for_each_entry(ops, &g_hotplug_ops_list, ops_list) {
		status = ops->status;
		if (ops->physical_status != status) {
			ops->fsm_callback(status);
			ops->physical_status = status;
		}
	}
}

static int thermal_hotplug_task_create(struct hotplug_t *hotplug)
{
	struct task_struct *thread = NULL;
	struct sched_param param = { .sched_priority = MAX_RT_PRIO - 1 };

	kthread_init_work(&hotplug->work, thermal_hotplug_action);
	kthread_init_worker(&hotplug->worker);
	thread = kthread_create(kthread_worker_fn, &hotplug->worker, "thermal_hotplug");
	if (IS_ERR(thread)) {
		pr_err("%s: thermal hotplug task create fail\n", __func__);
		return -EINVAL;
	}

	sched_setscheduler_nocheck(thread, SCHED_FIFO, &param);
	hotplug->thread = thread;
	wake_up_process(hotplug->thread);

	return 0;
}

int ipa_get_dev_temp(const char *name, int *val)
{
	int id, ret;

	id = ipa_get_tsensor_id(name);
	if (id < 0) {
		pr_err("%s:tsensor %s is not exist.\n", __func__, name);
		return -ENODEV;
	}

	ret = ipa_get_sensor_value((u32)id, val);
	return ret;
}

static int thermal_hotplug_ip_init(struct hotplug_t *hotplug)
{
	int ret;
	struct device_node *hotplug_np = NULL;
	struct hotplug_ops *ops = NULL;
	struct hotplug_ops *tmp = NULL;

	hotplug_np = of_find_node_by_name(NULL, "cpu_temp_threshold");
	if (hotplug_np == NULL) {
		pr_err("cpu_temp_threshold node not found\n");
		return -ENODEV;
	}

	ret = of_property_read_u32(hotplug_np, "ithermal,polling_delay", &hotplug->polling_delay);
	if (ret != 0) {
		pr_err("%s hotplug polling_delay use default value\n", __func__);
		hotplug->polling_delay = DEFAULT_POLL_DELAY;
	}

	ret = of_property_read_u32(hotplug_np, "ithermal,polling_delay_passive",
				   &hotplug->polling_delay_passive);
	if (ret != 0) {
		pr_err("%s hotplug polling_delay_passive use default value\n", __func__);
		hotplug->polling_delay_passive = DEFAULT_POLL_DELAY;
	}

	list_for_each_entry_safe(ops, tmp, &g_hotplug_ops_list, ops_list) {
		if (ops->init(hotplug_np, hotplug->device) != 0) {
			ops->exit(hotplug->device);
			hotplug_ops_unregister(ops);
		}
	}

	of_node_put(hotplug_np);
	return 0;
}

static void thermal_hotplug_ip_exit(struct device *dev)
{
	struct hotplug_ops *ops = NULL;
	struct hotplug_ops *tmp = NULL;

	list_for_each_entry_safe(ops, tmp, &g_hotplug_ops_list, ops_list) {
		ops->exit(dev);
		hotplug_ops_unregister(ops);
	}
}

int thermal_hotplugs_init(void)
{
	int ret;
	struct hotplug_t *hotplug = &g_thermal_hotplug.hotplug;

	hotplug->device = device_create(g_thermal_hotplug.thermal_class,
					NULL, THERMAL_HOTPLUG_DEVT, NULL, "hotplug");
	if (IS_ERR(hotplug->device)) {
		pr_err("Hotplug device create error\n");
		ret = (int)PTR_ERR(hotplug->device);
		goto device_destroy;
	}

	ret = device_create_file(hotplug->device, &dev_attr_hotplug_mode);
	if (ret != 0) {
		pr_err("Hotplug mode create error\n");
		goto device_remove_file;
	}

	ret = thermal_hotplug_ip_init(hotplug);
	if (ret != 0) {
		pr_err("thermal hotplug init error\n");
		goto hotplug_ip_exit;
	}

	ret = thermal_hotplug_task_create(hotplug);
	if (ret != 0) {
		pr_err("%s thermal_hotplug_task_create err\n", __func__);
		goto hotplug_ip_exit;
	}

	INIT_DEFERRABLE_WORK(&hotplug->poll_work, hotplug_poll_work);
	mod_delayed_work(system_freezable_power_efficient_wq, &hotplug->poll_work,
			 msecs_to_jiffies(DEFAULT_POLL_DELAY));

	hotplug->hotplug_enabled = true;

	ret = thermal_isp_mbox_init();
	if (ret != 0) {
		pr_err("%s thermal_isp_mbox_init failed\n", __func__);
	}

	return 0;

hotplug_ip_exit:
	thermal_hotplug_ip_exit(hotplug->device);
device_remove_file:
	device_remove_file(hotplug->device, &dev_attr_hotplug_mode);
device_destroy:
	device_destroy(g_thermal_hotplug.thermal_class, THERMAL_HOTPLUG_DEVT);
	hotplug->device = NULL;

	return ret;
}
EXPORT_SYMBOL(thermal_hotplugs_init);

void thermal_hotplugs_exit(void)
{
	struct hotplug_t *hotplug = &g_thermal_hotplug.hotplug;

	cancel_delayed_work(&hotplug->poll_work);

	kthread_flush_worker(&hotplug->worker);
	kthread_stop(hotplug->thread);

	thermal_hotplug_ip_exit(hotplug->device);

	device_remove_file(hotplug->device, &dev_attr_hotplug_mode);

	if (g_thermal_hotplug.thermal_class) {
		device_destroy(g_thermal_hotplug.thermal_class, THERMAL_HOTPLUG_DEVT);
		hotplug->device = NULL;
	}
}
EXPORT_SYMBOL(thermal_hotplugs_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("thermal hotplug module driver");
