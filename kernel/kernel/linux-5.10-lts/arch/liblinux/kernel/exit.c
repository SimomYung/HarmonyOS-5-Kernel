// SPDX-License-Identifier: GPL-2.0-only
#include <linux/rcuwait.h>

int rcuwait_wake_up(struct rcuwait *w)
{
	int ret = 0;
	struct task_struct *task;

	rcu_read_lock();

	/*
	 * Order condition vs @task, such that everything prior to the load
	 * of @task is visible. This is the condition as to why the user called
	 * rcuwait_wake() in the first place. Pairs with set_current_state()
	 * barrier (A) in rcuwait_wait_event().
	 *
	 *    WAIT		WAKE
	 *    [S] tsk = current   [S] cond = true
	 *	MB (A)	      MB (B)
	 *    [L] cond	    [L] tsk
	 */
	smp_mb(); /* (B) */

	task = rcu_dereference(w->task);
	if (task)
		ret = wake_up_process(task);
	rcu_read_unlock();

	return ret;
}
EXPORT_SYMBOL_GPL(rcuwait_wake_up);
