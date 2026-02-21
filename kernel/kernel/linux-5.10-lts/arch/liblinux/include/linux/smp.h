/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LIBLINUX_LINUX_SMP_H__
#define __LIBLINUX_LINUX_SMP_H__

#include <linux/version.h>

#include_next <linux/smp.h>

extern void arch_set_cpuhp_task(unsigned int cpu, struct task_struct *task);
extern int smp_call_function_single_force(int cpuid, smp_call_func_t func,
			     void *info, int wait);

#endif /* __LIBLINUX_LINUX_SMP_H__ */
