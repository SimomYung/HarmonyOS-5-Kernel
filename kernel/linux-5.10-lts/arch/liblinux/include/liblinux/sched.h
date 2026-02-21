/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_SCHED_H__
#define __LIBLINUX_SCHED_H__

#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/sched/wake_q.h>
#include <linux/kern_levels.h>
#include <linux/blkdev.h>
#include <linux/taskstats.h>
#include <linux/delayacct.h>
#include <linux/kernel_stat.h>
#include "../../kernel/workqueue_internal.h"
#include "../../kernel/sched/sched.h"
#include "../../fs/io-wq.h"

#ifdef CONFIG_LIBLINUX
static inline void sched_update_worker(struct task_struct *tsk)
{
	if (tsk->flags & (PF_WQ_WORKER | PF_IO_WORKER)) {
		if (tsk->flags & PF_WQ_WORKER)
			wq_worker_running(tsk);
		else
			io_wq_worker_running(tsk);
	}
}

static inline void sched_submit_work(struct task_struct *tsk)
{
	unsigned int task_flags;

	if (tsk->state == TASK_RUNNING)
		return;

	task_flags = tsk->flags;

	if (task_flags & (PF_WQ_WORKER | PF_IO_WORKER)) {
		if (tsk->flags & PF_WQ_WORKER)
			wq_worker_sleeping(tsk);
		else
			io_wq_worker_sleeping(tsk);
	}

	if (tsk_is_pi_blocked(tsk))
		return;

	if (blk_needs_flush_plug(tsk)) {
		blk_schedule_flush_plug(tsk);
	}
}

/*
 * mark the task as uninterruptible because liblinux cannot
 * interrupt hm futex.
 * Update workqueue context because the worker may be blocked by hm
 */
static void liblinux_ctx_leave(void)
{
	__set_current_state(TASK_UNINTERRUPTIBLE);
	sched_submit_work(current);
}
/* the task must be running when enter liblinux ctx */
static void liblinux_ctx_enter(void)
{
	sched_update_worker(current);
	__set_current_state(TASK_RUNNING);
}

#endif /* CONFIG_LIBLINUX */

#endif /* __LIBLINUX_SCHED_H__ */
