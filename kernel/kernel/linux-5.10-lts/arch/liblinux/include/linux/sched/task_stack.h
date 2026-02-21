/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LIBLINUX_LINUX_TASK_STACK_H__
#define __LIBLINUX_LINUX_TASK_STACK_H__

#define object_is_on_stack __object_is_on_stack
#include_next <linux/sched/task_stack.h>
#undef object_is_on_stack

static inline int object_is_on_stack(const void *obj)
{
	unsigned long sp = current_stack_pointer;
	unsigned long stack_end = read_sysreg(tpidr_el0);
	return ((unsigned long)obj >= sp) && ((unsigned long)obj < stack_end);
}

#endif /* __LIBLINUX_LINUX_TASK_STACK_H__ */
