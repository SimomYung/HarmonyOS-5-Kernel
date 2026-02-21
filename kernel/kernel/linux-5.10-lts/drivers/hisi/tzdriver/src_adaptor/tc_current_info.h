/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef TC_CURRENT_INFO_H
#define TC_CURRENT_INFO_H

#include <linux/sched.h>
#include <linux/version.h>
#if (KERNEL_VERSION(4, 14, 0) <= LINUX_VERSION_CODE)
#include <linux/sched/task.h>
#endif

#ifdef CONFIG_LIBLINUX
#define HM_SIGNAL_BIT_KILL  1
#define HM_SIGNAL_BIT_POSIX 10
#endif

#ifdef CURRENT_PID
#undef CURRENT_PID
#endif
#define CURRENT_PID (current->pid)

#ifdef CURRENT_COMM
#undef CURRENT_COMM
#endif
#define CURRENT_COMM (current->comm)

#define CURRENT_MM (current->mm)
#define CURRENT_TGID (current->tgid)
#define CURRENT_FLAGS (current->flags)

#ifndef CONFIG_CONFIDENTIAL_CONTAINER
#define PROC_PID_INIT_INO 0x00EFFFFFFCU
#endif

static inline void get_current_struct(struct task_struct **task)
{
	get_task_struct(current);
	*task = current;
}

static inline void put_current_struct(struct task_struct *task)
{
	put_task_struct(task);
}

static inline bool set_preempt_call_permission(void)
{
	/* linux & ldk do not need set it, just return true */
	return true;
}

char *get_proc_dpath(char *path, int path_len);
bool current_sigkill_pending(void);
bool is_current_pending_signal(void);
int get_pid_name(pid_t pid, char *comm, size_t size);
void clear_freezable_wait_pending(int rc);
#endif
