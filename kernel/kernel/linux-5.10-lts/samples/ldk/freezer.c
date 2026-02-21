/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Define interface of thread
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 25 08:30:28 UTC 2024
 */

#include <linux/freezer.h>
#include <linux/sched.h>
#include <linux/types.h>

#include <liblinux/pal.h>

#include "freezer.h"

static atomic_t proc_freeze_atomic = ATOMIC_INIT(0);

void suspend_system_proc_freeze(bool freeze)
{
	if (freeze) {
		atomic_inc(&proc_freeze_atomic);
	} else {
		atomic_set(&proc_freeze_atomic, 0);
	}
}

bool try_to_freeze_unsafe(void)
{
	if (atomic_read(&proc_freeze_atomic) == 0) {
		return false;
	}
	if (unlikely(!(current->flags & PF_KTHREAD))) {
		return false;
	}

	return (liblinux_pal_thread_try_to_freeze() > 0);
}
EXPORT_SYMBOL(try_to_freeze_unsafe);

bool try_to_freeze(void)
{
	return try_to_freeze_unsafe();
}
EXPORT_SYMBOL(try_to_freeze);

bool set_freezable(void)
{
	if (unlikely(!(current->flags & PF_KTHREAD))) {
		return false;
	}

	return (liblinux_pal_thread_set_freezable() >= 0);
}
EXPORT_SYMBOL(set_freezable);

bool freezing(struct task_struct *p)
{
	if (atomic_read(&proc_freeze_atomic) == 0) {
		return false;
	}
	return liblinux_pal_thread_freezing();
}
EXPORT_SYMBOL(freezing);