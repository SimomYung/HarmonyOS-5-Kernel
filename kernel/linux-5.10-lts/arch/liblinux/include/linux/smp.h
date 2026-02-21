/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LIBLINUX_LINUX_SMP_H__
#define __LIBLINUX_LINUX_SMP_H__

#include <linux/version.h>

#include_next <linux/smp.h>

extern void arch_set_cpuhp_task(unsigned int cpu, struct task_struct *task);

#endif /* __LIBLINUX_LINUX_SMP_H__ */
