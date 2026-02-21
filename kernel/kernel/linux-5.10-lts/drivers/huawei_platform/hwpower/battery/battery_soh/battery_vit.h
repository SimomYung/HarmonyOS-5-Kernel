/* SPDX-License-Identifier: GPL-2.0 */
/*
 * battery_vit.h
 *
 * driver adapter for vit.
 *
 * Copyright (c) 2025-2025 Huawei Technologies Co., Ltd.
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

#ifndef _BATTERY_VIT_H_
#define _BATTERY_VIT_H_

#include <linux/list.h>
#include <linux/notifier.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <chipset_common/hwpower/battery/battery_soh.h>
#include <chipset_common/hwpower/battery/battery_soh.h>

#define VIT_MAX_BATT_NUM                        5

struct vit_batt_data {
	u8 batt_index;
	long time;
	int volt;
	int cur;
	int temp;
	u32 cycle;
	int cc;
	u8 ufsoc;
	u8 msoc;
	u8 uisoc;
	u8 shell_temp;
	u16 event_mask;
};

struct vit_data_queue_node {
	struct vit_batt_data *batt;
	struct list_head list;
};

struct vit_data_queue {
	struct list_head head;
	spinlock_t lock;
	unsigned int count;
};

static struct vit_device_info {
	int batt_num;
	int enable_discharge_monitor;
	int enable_charge_monitor;
	int charge_monitor_interval;
	int discharge_monitor_interval;
	int update_interval;
	int is_charging;
	int is_charge_done;
	int is_plugged;
	int ocv_updated[VIT_MAX_BATT_NUM];
	int monitor_cnt;
	u32 last_update_time;
	struct vit_data_queue *batt_queues;
	struct delayed_work vit_monitor_work;
	struct notifier_block vit_update_notify;
};

#endif /* _BATTERY_VIT_H_ */
