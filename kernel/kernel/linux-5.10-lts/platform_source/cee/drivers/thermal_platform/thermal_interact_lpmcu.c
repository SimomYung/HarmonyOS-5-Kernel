// SPDX-License-Identifier: GPL-2.0
/*
 * thermal_interact_lpmcu.c
 *
 * thermal lpmcu AP interact
 *
 * Copyright (C) 2025 Huawei Technologies Co., Ltd.
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

#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sched/task.h>
#include <linux/of_platform.h>
#include <uapi/linux/sched/types.h>
#include <platform_include/basicplatform/linux/ipc_rproc.h>
#include <platform_include/basicplatform/linux/ipc_msg.h>
#include <platform_include/cee/linux/thermal_interact_lpmcu.h>

#define AP_ID			0x00
#define LPM3_ID			0x08
#define INTERACT_CMD		0x04
#define INTERACT_TYPE0		0x5B
#define INTERACT_TYPE1		0x5C

#define DDR_THERMAL_HEAD ((LPM3_ID << 24) | (AP_ID << 16) | (INTERACT_CMD << 8) | INTERACT_TYPE0)
#define LOWTEMP_THERMAL_HEAD ((LPM3_ID << 24) | (AP_ID << 16) | (INTERACT_CMD << 8) | INTERACT_TYPE1)

static struct notifier_block g_isp_nb;

LIST_HEAD(g_thermal_lpmcu_ops_list);
DEFINE_MUTEX(g_thermal_lpmcu_ops_list_lock);

int thermal_lpmcu_ops_register(struct thermal_lpmcu_ops *thermal_ops)
{
	struct thermal_lpmcu_ops *ops = NULL;

	if (thermal_ops == NULL) {
		pr_err("%s thermal_ops is invalid\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&g_thermal_lpmcu_ops_list_lock);
	list_for_each_entry(ops, &g_thermal_lpmcu_ops_list, ops_list) {
		if (strncmp(ops->name, thermal_ops->name, MAX_LEN) == 0) {
			mutex_unlock(&g_thermal_lpmcu_ops_list_lock);
			pr_err("%s thermal_ops has registered\n", thermal_ops->name);
			return -EINVAL;
		}
	}
	list_add_tail(&thermal_ops->ops_list, &g_thermal_lpmcu_ops_list);
	mutex_unlock(&g_thermal_lpmcu_ops_list_lock);

	return 0;
}

void thermal_lpmcu_ops_unregister(struct thermal_lpmcu_ops *thermal_ops)
{
	struct thermal_lpmcu_ops *ops = NULL;

	if (thermal_ops == NULL) {
		pr_err("%s thermal_ops is invalid\n", __func__);
		return;
	}

	mutex_lock(&g_thermal_lpmcu_ops_list_lock);
	list_for_each_entry(ops, &g_thermal_lpmcu_ops_list, ops_list) {
		if (strncmp(thermal_ops->name, ops->name, MAX_LEN) == 0) {
			list_del_init(&ops->ops_list);
			break;
		}
	}
	mutex_unlock(&g_thermal_lpmcu_ops_list_lock);
	pr_err("%s:%s unregister\n", __func__, thermal_ops->name);
}

int (*g_ddr_to_isp_handler)(unsigned int) = NULL;

void ddr_to_isp_callback_register(int (*ddr_to_isp_handler)(unsigned int))
{
	if (ddr_to_isp_handler == NULL) {
		pr_err("ddr to isp callback register failed!\n");
		return;
	}
	g_ddr_to_isp_handler = ddr_to_isp_handler;
}

void ddr_to_isp_callback_unregister(void)
{
	g_ddr_to_isp_handler = NULL;
}

static int ddr_thermal_handler(unsigned int level)
{
	pr_info("%s: ddr_thermal_handler, level:%d\n", __func__, level);
	if (g_ddr_to_isp_handler == NULL) {
		pr_err("%s: invalid callback\n", __func__);
		return 0;
	}
	g_ddr_to_isp_handler(level);
	return 0;
}

static void lowtemp_protect_handler(void *mbox_msg)
{
	union ipc_data *msg = (union ipc_data *)mbox_msg;
	struct thermal_lpmcu_ops *ops = NULL;
	int status;

	pr_info("%s: lowtemp_protect_handler, level:%d\n", __func__, msg->data[1]);

	list_for_each_entry(ops, &g_thermal_lpmcu_ops_list, ops_list) {
		if (ops->lowtemp_thermal_handler != NULL) {
			if (ops->lowtemp_thermal_handler(msg->data[1]) != 0) {
				pr_err("%s lowtemp thermal fail\n", ops->name);
				continue;
			}
		} else {
			pr_err("%s lowtemp_thermal_handler is unregister\n", ops->name);
		}
	}
}

static int thermal_lpmcu_ipc_handler(struct notifier_block *nb, unsigned long len,
	void *mbox_msg)
{
	union ipc_data *msg = (union ipc_data *)mbox_msg;

	switch (msg->data[0]) {
		case DDR_THERMAL_HEAD:
			ddr_thermal_handler(msg->data[1]);
			break;
		case LOWTEMP_THERMAL_HEAD:
			lowtemp_protect_handler(mbox_msg);
			break;
		default:
			break;
	}
	return 0;
}

int thermal_isp_mbox_init(void)
{
	int ret;

	g_isp_nb.next = NULL;
	g_isp_nb.notifier_call = thermal_lpmcu_ipc_handler;

	/* register the rx notify callback */
	ret = RPROC_MONITOR_REGISTER(IPC_LPM3_ACPU_MBX_7, &g_isp_nb);
	if (ret)
		pr_err("rproc monitor register failed");

	return ret;
}
