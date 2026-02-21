/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef __LIBLINUX_LINUX_SCHED_H__
#define __LIBLINUX_LINUX_SCHED_H__

#define task_tgid_vnr __task_tgid_vnr
#include_next <linux/sched.h>

#undef __set_current_state
#undef set_current_state
#undef set_special_state
#undef task_tgid_vnr

/*
 * liblinux only support the task state TASK_NORMAL, TASK_PARKED, TASK_DEAD and TASK_RUNNING.
 * however there other states may occur like TASK_NOLOAD and TASK_WAKEKILL, we need
 * to handle this for compatiable. there are two reasons why we do it globally:
 *
 * 1. if we do the state changes inside the __schedule_timeout, it may trigger concurrency issues
 * 2. if we just left it unchanged inside the __schedule_timeout, there will be a mismatch
 *    between state(liblinux supported) and `current->state`, this will case futex_timedwait
 *    return immediately, thus a busyloop until we really waked.
 */
#define LIBLINUX_TASK_STATE(state) ((state) & (TASK_NORMAL | TASK_PARKED | TASK_RUNNING | TASK_DEAD))

#ifdef CONFIG_DEBUG_ATOMIC_SLEEP

#define __set_current_state(state_value)			\
	do {							\
		WARN_ON_ONCE(is_special_task_state(state_value));\
		current->task_state_change = _THIS_IP_;		\
		current->state = LIBLINUX_TASK_STATE(state_value);\
	} while (0)

#define set_current_state(state_value)				\
	do {							\
		WARN_ON_ONCE(is_special_task_state(state_value));\
		current->task_state_change = _THIS_IP_;		\
		smp_store_mb(current->state, LIBLINUX_TASK_STATE(state_value));	\
	} while (0)

#define set_special_state(state_value)					\
	do {								\
		unsigned long flags; /* may shadow */			\
		WARN_ON_ONCE(!is_special_task_state(state_value));	\
		raw_spin_lock_irqsave(&current->pi_lock, flags);	\
		current->task_state_change = _THIS_IP_;			\
		current->state = LIBLINUX_TASK_STATE(state_value);	\
		raw_spin_unlock_irqrestore(&current->pi_lock, flags);	\
	} while (0)

#else /* !CONFIG_DEBUG_ATOMIC_SLEEP */

#define __set_current_state(state_value)				\
	current->state = LIBLINUX_TASK_STATE(state_value)

#define set_current_state(state_value)					\
	smp_store_mb(current->state, LIBLINUX_TASK_STATE(state_value))

#define set_special_state(state_value)					\
	do {								\
		unsigned long flags; /* may shadow */			\
		raw_spin_lock_irqsave(&current->pi_lock, flags);	\
		current->state = LIBLINUX_TASK_STATE(state_value);	\
		raw_spin_unlock_irqrestore(&current->pi_lock, flags);	\
	} while (0)

#endif /* !CONFIG_DEBUG_ATOMIC_SLEEP */

static inline pid_t task_tgid_vnr(struct task_struct *tsk)
{
	return task_thread_info(tsk)->vtgid;
}

#endif /* __LIBLINUX_LINUX_SCHED_H__ */
