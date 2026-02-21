/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_LOCK_H
#define HVGR_LOCK_H

#include <linux/spinlock.h>
#include <linux/ktime.h>
#include <linux/delay.h>

struct hvgr_device;

struct hvgr_spin_lock {
	spinlock_t spin_lock;
	unsigned long flags;
	ktime_t lock_time;
	const char *name;
	const char *owner;
};

/**
 * hvgr_spin_lock_init - Init hvgr spin_lock.
 *
 * @spin_lock: Spin lock pointer
 * @lock_name: Spin lock name
 *
 */
void hvgr_spin_lock_init(struct hvgr_spin_lock *lock, const char * lock_name);

/**
 * hvgr_spin_lock_irqsave - Lock hvgr spin_lock.
 *
 * @gdev: Device pointer
 * @spin_lock: Spin lock pointer
 * @func_name: spin lock owner
 *
 */
#define hvgr_spin_lock_irqsave_macro(gdev, lock) hvgr_spin_lock_irqsave(gdev, lock, __func__)

void hvgr_spin_lock_irqsave(struct hvgr_device * const gdev, struct hvgr_spin_lock *lock,
	const char *func_name);

/**
 * hvgr_spin_unlock_irqrestore - Unlock hvgr spin_lock.
 *
 * @gdev: Device pointer
 * @spin_lock: Spin lock pointer
 *
 */
void hvgr_spin_unlock_irqrestore(struct hvgr_device * const gdev, struct hvgr_spin_lock *lock);

#endif
