/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
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

#ifndef UKMD_ISR_H
#define UKMD_ISR_H

#include <linux/types.h>
#include <linux/notifier.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/atomic.h>

#include "ukmd_listener.h"

#define ISR_LISTENER_LIST_COUNT   32
#define ISR_NAME_SIZE   64

enum {
	UKMD_ISR_REQUEST = 1,
	UKMD_ISR_RELEASE,
	UKMD_ISR_DISABLE,
	UKMD_ISR_ENABLE,
	UKMD_ISR_DISABLE_NO_REF,
	UKMD_ISR_ENABLE_NO_REF
};

struct ukmd_isr_listener_node {
	struct list_head list_node;

	struct ukmd_listener_data listener_data;
	uint32_t listen_bit;
	struct raw_notifier_head irq_nofitier;
};

struct ukmd_isr {
	int32_t irq_no;
	char irq_name[ISR_NAME_SIZE];
	void *parent;
	uint32_t unmask;
	atomic_t refcount;
	struct list_head list_node;

	void (*handle_func)(struct ukmd_isr *isr_ctrl, uint32_t handle_event);
	irqreturn_t (*isr_fnc)(int32_t irq, void *ptr);
	struct list_head isr_listener_list[ISR_LISTENER_LIST_COUNT];
};

void ukmd_isr_setup(struct ukmd_isr *isr_ctrl);
int32_t ukmd_isr_notify_listener(struct ukmd_isr *isr_ctrl, uint32_t listen_bit);
int32_t ukmd_isr_unregister_listener(struct ukmd_isr *isr_ctrl, struct notifier_block *nb, uint32_t listen_bit);
int32_t ukmd_isr_register_listener(struct ukmd_isr *isr_ctrl,
	struct notifier_block *nb, uint32_t listen_bit, void *listener_data);

#endif