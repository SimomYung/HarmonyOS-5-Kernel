/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_CGROUP_H__
#define __LIBLINUX_CGROUP_H__

#define task_dfl_cgroup __task_dfl_cgroup
#include_next <linux/cgroup.h>
#undef task_dfl_cgroup

#ifdef CONFIG_CGROUPS
static inline struct cgroup *task_dfl_cgroup(struct task_struct *task)
{
	if (!task->cgroups)
		RCU_INIT_POINTER(task->cgroups, &init_css_set);
	return task_css_set(task)->dfl_cgrp;
}
#endif

#endif	/* __LIBLINUX_CGROUP_H__ */
