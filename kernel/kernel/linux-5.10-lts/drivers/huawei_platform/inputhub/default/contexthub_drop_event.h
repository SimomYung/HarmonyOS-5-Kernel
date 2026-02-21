/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 * Description: notifier interface for drop module
 *
 * This software is licensed under the terms of the GNU General Public
 * License, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __DROP_EVENT_H__
#define __DROP_EVENT_H__

#include <linux/notifier.h>
#include <linux/time.h>
#include <linux/version.h>

enum drop_event {
	SENSOR_DROP = 0
};

struct sensor_drop_data {
	int type;
	int initial_speed;
	int height;
	int angle_pitch;
	int angle_roll;
	int impact;
	int posture;
};

 int drop_register_atomic_notifier(struct notifier_block *nb);
 int drop_unregister_atomic_notifier(struct notifier_block *nb);
 int call_drop_atomic_notifiers(unsigned long val, void *v);
 int drop_register_blocking_notifier(struct notifier_block *nb);
 int drop_unregister_blocking_notifier(struct notifier_block *nb);
 int call_drop_blocking_notifiers(unsigned long val, void *v);
#endif