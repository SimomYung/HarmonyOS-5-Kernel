// SPDX-License-Identifier: GPL-2.0
#include <linux/task_work.h>

#include <trace/hooks/liblinux.h>

static void hook_task_work_check(void *args, struct task_struct *task,
				     enum task_work_notify_mode notify)
{
	if (task != current) {
		panic("liblinux: UN-IMPL task_work_check to !current, current=%s, task=%s\n",
		      current->comm, task->comm);
	} else if (notify == TWA_SIGNAL) {
		panic("liblinux: UN-IMPL task_work_check TWA_SIGNAL notify, task=%s\n",
		      task->comm);
	}
}
INIT_VENDOR_HOOK(ldk_rvh_task_work_check, hook_task_work_check);
