/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef __COMM_UTILS_H__
#define __COMM_UTILS_H__

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/device.h>
#include <securec.h>
#include <platform_include/basicplatform/linux/switch.h>
#include "dp_ctrl.h"

enum dptx_work_queue_event {
	DPTX_GPIO_PLUG_EVENT,
	DPTX_PSR2_UEVENT_EVENT,
	DPTX_WORK_QUEUE_EVENT_MAX,
};

void switch_notification_event_unchange_state(struct switch_dev *sdev, uint32_t state);

void dptx_create_workqueue(void);
void dptx_destory_workqueue(void);
void dptx_work_queue_handle(struct dp_ctrl *dptx, enum dptx_work_queue_event event_type);
#endif