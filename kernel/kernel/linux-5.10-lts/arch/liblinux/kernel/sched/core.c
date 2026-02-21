// SPDX-License-Identifier: GPL-2.0-only
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/sched/wake_q.h>
#include <linux/kern_levels.h>
#include <linux/blkdev.h>
#include <linux/cpumask.h>
#include <linux/taskstats.h>
#include <linux/delayacct.h>
#include <linux/kernel_stat.h>
#include <linux/rcupdate_wait.h>
#include <uapi/linux/sched/types.h> /* for SCHED_RR */

#include <liblinux/sched.h>
#include <liblinux/pal.h>
#include <liblinux/kapi.h>
#include <linux/atomic.h>
#include <linux/kallsyms.h>
#include <linux/tracepoint.h>
#include <linux/preempt.h>

#include "core.h"

#ifdef CONFIG_DETECT_HUNG_TASK
#define IGNORE_STAGE_TIME	60

enum {
	TASK_TYPE_KTHREAD = 1,
	TASK_TYPE_ACTV,
};

static bool ignore_stage_done = false;
static struct timer_list ignore_stage_timer;

static struct {
	const char *name;
	int type;
} ignore_whitelist[] = {};

static inline bool __check_whitelist(char *name, int type)
{
	for (int i = 0; i < ARRAY_SIZE(ignore_whitelist); i++) {
		if (type == ignore_whitelist[i].type &&
		    !strncmp(name, ignore_whitelist[i].name, TASK_COMM_LEN - 1))
			return true;
	}
	return false;
}

static bool __check_task_state(struct task_struct *tsk)
{
	if (!(tsk->flags & PF_KTHREAD))
		return true;
	if (tsk->thread_info.owner == 0)
		return false;
	if (tsk->flags & PF_FROZEN)
		return false;
	if (!ignore_stage_done) {
		tsk->last_switch_time = TASK_STATE_NO_HUNG_DETECT;
		return false;
	}
	if (tsk->last_switch_time == TASK_STATE_INITIAL ||
	    tsk->last_switch_time == TASK_STATE_NO_HUNG_DETECT)
		return false;
	if (tsk->last_switch_time == TASK_STATE_WAKEUP) {
		if (__check_whitelist(tsk->comm, TASK_TYPE_KTHREAD)) {
			tsk->last_switch_time = TASK_STATE_NO_HUNG_DETECT;
			return false;
		} else {
			tsk->last_switch_time = TASK_STATE_WHITELIST_CHECKED;
		}
	}
	return true;
}

static inline void __update_task_state(struct task_struct *tsk)
{
	if (tsk->flags & PF_KTHREAD) {
		if (!ignore_stage_done || tsk->last_switch_time == TASK_STATE_INITIAL)
			tsk->last_switch_time = TASK_STATE_WAKEUP;
	}
}

static void ignore_stage_update(struct timer_list *unused)
{
	ignore_stage_done = true;
}

static int __init ignore_stage_init(void)
{
	timer_setup(&ignore_stage_timer, ignore_stage_update, 0);
	ignore_stage_timer.expires = jiffies + IGNORE_STAGE_TIME * HZ;
	add_timer(&ignore_stage_timer);
	return 0;
}
late_initcall(ignore_stage_init);
#endif

static inline bool __get_wchan_info(struct task_struct *tsk, struct liblinux_wchan_info *info)
{
	int ret;
	unsigned long wchan;
	char mod_name[MODULE_NAME_LEN];

	if (unlikely(!trace_enable(sched_blocked_reason)))
		return false;

	wchan = get_wchan(tsk);
	if (!wchan)
		return false;

	ret = lookup_symbol_attrs(wchan, &info->size, &info->offset, mod_name, info->name);
	if (ret)
		return false;

	return true;
}

static inline int *_task_futex_addr(struct task_struct *p)
{
	int *futex = NULL;
#if defined(__BIG_ENDIAN) && BITS_PER_LONG == 64
	futex = (int *)&p->state + 1;
#else
	futex = (int *)&p->state;
#endif
	return futex;
}

static void __schedule_timeout(unsigned long long tv64)
{
	unsigned long flags = 0;
	unsigned long irq_flags = 0;
	unsigned long state;
	int *futex = NULL;
	int ret;
	struct task_struct *tsk = current;
	char name[KSYM_NAME_LEN];
	struct liblinux_wchan_info wchan_info = { .name = name, .name_len = KSYM_NAME_LEN, };
	struct liblinux_wchan_info *info = &wchan_info;

	local_irq_disable();
	rcu_note_context_switch(false);
	local_irq_enable();
	sched_submit_work(tsk);

	/* use `pi_lock` to ensure visibility of `state` with `__do_try_wake_up` */
	raw_spin_lock_irqsave(&tsk->pi_lock, irq_flags);
#ifdef CONFIG_LIBLINUX_THREAD_BLOCK
	if (!(tsk->flags & PF_KTHREAD)) {
		if (task_thread_info(tsk)->thread_handle.tcb_cref !=
		    liblinux_pal_thread_sched_cref()) {
			panic("actv task tcb_cref abnormal\n");
		}
	}
#endif
	/* This pairs with smp_mb() (B) in try_to_wake_up() that the wake_up thread does. */
	smp_mb();
	state = READ_ONCE(tsk->state);
	raw_spin_unlock_irqrestore(&tsk->pi_lock, irq_flags);

	if (state == TASK_PARKED) {
		flags |= LIBLINUX_PAL_FUTEX_UNINTERRUPTIBLE;
	} else if (unlikely(state & ~(TASK_NORMAL | TASK_NOLOAD | TASK_WAKEKILL))) {
		panic("liblinux: task [%s] unknown state 0x%lx (%c)\n",
		      tsk->comm, tsk->state, task_state_to_char(tsk));
	}
	if (state & TASK_UNINTERRUPTIBLE) {
		if (state & TASK_WAKEKILL)
			flags |= LIBLINUX_PAL_FUTEX_KILLABLE;
		else
			flags |= LIBLINUX_PAL_FUTEX_UNINTERRUPTIBLE;
	}
#ifdef CONFIG_DETECT_HUNG_TASK
	if (state == TASK_UNINTERRUPTIBLE || state == TASK_KILLABLE) {
		if (__check_task_state(tsk))
			flags |= LIBLINUX_PAL_FUTEX_TIMEDOUT_DETECT;
	}
#endif
	if (tsk->thread_info.owner)
		flags |= LIBLINUX_PAL_FUTEX_LOCK_IN_LDK;

	futex = _task_futex_addr(tsk);
	if (state != TASK_RUNNING) {
		if (__get_wchan_info(tsk, info)) {
			if (tsk->in_iowait)
				flags |= LIBLINUX_PAL_FUTEX_IOWAIT;
		} else {
			info = NULL;
		}
#ifdef CONFIG_LIBLINUX_THREAD_BLOCK
		ret = liblinux_pal_thread_block(futex, (int)state, tv64, flags, info);
#else
		ret = liblinux_pal_futex_timedwait_ex(futex, (int)state, tv64, flags, info);
#endif
		if (ret == -EINTR) {
			/* temporarily only set signal for actv task */
			if (!(tsk->flags & PF_KTHREAD)) {
				set_tsk_thread_flag(tsk, TIF_SIGPENDING);
				if (flags & LIBLINUX_PAL_FUTEX_KILLABLE)
					sigaddset(&tsk->pending.signal, SIGKILL);
			}
		}
#ifdef CONFIG_DETECT_HUNG_TASK
		__update_task_state(tsk);
#endif
	} else {
		yield();
	}
	sched_update_worker(tsk);
}

void __sched schedule(void)
{
	__schedule_timeout(LIBLINUX_PAL_FUTEX_MAX_TIMEOUT);
}
EXPORT_SYMBOL(schedule);

void __sched yield(void)
{
	liblinux_pal_thread_yield();
}
EXPORT_SYMBOL(yield);

static void __do_try_wake_up(struct task_struct *p, int wake_flags)
{
	int *futex = _task_futex_addr(p);

#ifdef CONFIG_LIBLINUX_THREAD_BLOCK
	(void)liblinux_pal_thread_unblock(task_thread_info(p)->thread_handle.tcb_cref,
					  futex, wake_flags & WF_SYNC);
#else
	(void)liblinux_pal_futex_wake_ex(futex, wake_flags & WF_SYNC);
#endif
}

static int inline __check_state_wakeup(struct task_struct *p, int wake_flags, unsigned long state)
{
	int ret = 0;

	if (state & (TASK_NORMAL | TASK_PARKED)) {
		WRITE_ONCE(p->state, TASK_RUNNING);
		/*
		 * Pairs with the smp_mb() in __schedule_timeout().
		 * Make sure update to p->state is visible now.
		 */
		smp_mb(); /* B */
		__do_try_wake_up(p, wake_flags);
		ret = 1;
	} else if ((state != TASK_RUNNING) && (state != TASK_DEAD)) {
		printk(KERN_WARNING "%s: unexpected state detected: state=%lu\n", __func__, state);
		ret = 0;
	}

	return ret;
}

int try_to_wake_up(struct task_struct *p, unsigned int _state, int wake_flags)
{
	unsigned long state;
	unsigned long flags;
	int suc = 0;

	/* CAUSION: for some testcase, this api is called with `p == NULL` */
	if (p == NULL) {
		printk(KERN_WARNING "%s: NULL task_struct detected\n", __func__);
		return 0;
	}

	/*
	 * If we are going to wake up a thread waiting for CONDITION we
	 * need to ensure that CONDITION=1 done by the caller can not be
	 * reordered with p->state check below. This pairs with smp_store_mb()
	 * in set_current_state() that the waiting thread does.
	 */
	raw_spin_lock_irqsave(&p->pi_lock, flags); /* serialize against concurrent self */
	smp_mb__after_spinlock();
	state = p->state;
	if (!(state & _state))
		goto unlock;

	suc = __check_state_wakeup(p, wake_flags, state);

unlock:
	raw_spin_unlock_irqrestore(&p->pi_lock, flags);
	return suc;
}

/**
 * schedule_preempt_disabled - called with preemption disabled
 *
 * Returns with preemption disabled. Note: preempt_count must be 1
 */
void __sched schedule_preempt_disabled(void)
{
	sched_preempt_enable_no_resched();
	schedule();
	preempt_disable();
}

int wake_up_state(struct task_struct *p, unsigned int state)
{
	return try_to_wake_up(p, state, 0);
}

int sched_cpu_activate(unsigned int cpu)
{
	set_cpu_active(cpu, true);
	return 0;
}

int __weak sched_cpu_deactivate(unsigned int cpu)
{
	set_cpu_active(cpu, false);
	/*
	 * We've cleared cpu_active_mask, wait for all preempt-disabled and RCU
	 * users of this state to go away such that all new such users will
	 * observe it.
	 *
	 * Do sync before park smpboot threads to take care the rcu boost case.
	 */
	synchronize_rcu();

	return 0;
}

int sched_cpu_dying(unsigned int cpu)
{
	liblinux_pal_process_set_slv(cpu, PROCESS_SCHED_LEVEL_MAX);
	return 0;
}

static inline int __normal_prio(int policy, int rt_prio, int nice)
{
	int prio;

	if (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	else
		prio = NICE_TO_PRIO(nice);

	return prio;
}

static inline int normal_prio(struct task_struct *p)
{
	return __normal_prio(p->policy, p->rt_priority, PRIO_TO_NICE(p->static_prio));
}

static int effective_prio(struct task_struct *p)
{
	p->normal_prio = normal_prio(p);

	if (!rt_prio(p->prio))
		return p->normal_prio;
	return p->prio;
}

static void __setscheduler_params(struct task_struct *p, const struct sched_attr *attr)
{
	int policy = attr->sched_policy;
	int nice;

	p->policy = policy;

	if (fair_policy(policy)) {
		nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);
		p->static_prio = NICE_TO_PRIO(nice);
	}

	p->rt_priority = attr->sched_priority;
	p->normal_prio = normal_prio(p);
}

static void __setscheduler_prio(struct task_struct *p, int prio)
{
	p->prio = prio;
}

static int __sched_setscheduler(struct task_struct *p,
				const struct sched_attr *attr,
				bool user, bool pi)
{
	int ret = 0;
	int policy = attr->sched_policy;
	int newprio;

	if (!(p->flags & PF_KTHREAD)) {
		pr_warn("%s: actv task pid %d may not be valid\n", __func__, p->pid);
		return -EPERM;
	}

	if (!(fair_policy(policy) || rt_policy(policy)))
		return -EINVAL;

	if (rt_policy(policy) && attr->sched_priority > MAX_RT_PRIO - 1)
		return -EINVAL;

	if (fair_policy(policy) && attr->sched_priority != 0)
		return -EINVAL;

	if (fair_policy(policy) && (attr->sched_nice < MIN_NICE || attr->sched_nice > MAX_NICE))
		return -EINVAL;

	if (policy == p->policy) {
		if (fair_policy(policy) && attr->sched_nice == task_nice(p))
			goto out;
		if (rt_policy(policy) && attr->sched_priority == p->rt_priority)
			goto out;
	}

	newprio = __normal_prio(policy, attr->sched_priority, attr->sched_nice);

	if (fair_policy(policy)) {
		if (rt_policy(p->policy)) {
			ret = liblinux_pal_thread_setscheduler(&task_thread_info(p)->thread_handle,
							       0, policy);
			if (!ret && attr->sched_nice != 0)
				ret = liblinux_pal_set_user_nice(p->pid, attr->sched_nice);
		} else {
			ret = liblinux_pal_set_user_nice(p->pid, attr->sched_nice);
		}
	} else if (rt_policy(policy)) {
		ret = liblinux_pal_thread_setscheduler(&task_thread_info(p)->thread_handle,
						       attr->sched_priority, policy);
	}
	if (ret < 0) {
		pr_warn("%s: task=%s tid=%d setscheduler failed, policy=%d priority=%d nice=%d, ret=%d\n",
			__func__, p->comm, p->pid, attr->sched_policy, attr->sched_priority, attr->sched_nice, ret);
		goto out;
	}

	if (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
		__setscheduler_params(p, attr);
		__setscheduler_prio(p, newprio);
	}
out:
	return ret;
}

static int _sched_setscheduler(struct task_struct *p, int policy,
			       const struct sched_param *param, bool check)
{
	struct sched_attr attr = {
		.sched_policy   = policy,
		.sched_priority = param->sched_priority,
		.sched_nice	= PRIO_TO_NICE(p->static_prio),
	};

	return __sched_setscheduler(p, &attr, check, true);
}

void sched_set_stop_task(int cpu, struct task_struct *stop)
{
	struct sched_param sp = { .sched_priority = MAX_RT_PRIO - 1 };
	liblinux_pal_thread_set_slv(&task_thread_info(stop)->thread_handle, THREAD_SCHED_LEVEL_MAX);
	(void)_sched_setscheduler(stop, SCHED_FIFO, &sp, false);
}

void arch_set_cpuhp_task(unsigned int cpu, struct task_struct *task)
{
	liblinux_pal_thread_set_slv(&task_thread_info(task)->thread_handle, THREAD_SCHED_LEVEL_MAX);
}

/* kernel/sched/core.c */
#ifdef CONFIG_SMP
#include <linux/bitmap.h>
#include <linux/cpumask.h>
#include <linux/sched.h>

#define __RETRY_CNT_MAX		20

void __weak liblinux_get_cpus_allowed(struct cpumask *allowed_mask,
				      const struct cpumask *new_mask)
{
	cpumask_copy(allowed_mask, new_mask);
}

static int __do_set_cpus_allowed(struct task_struct *p, const struct cpumask *__new_mask)
{
	int ret = 0;
	int i;
	struct cpumask new_mask;
	/* thread_handle should be set to NULL in actv context */
	liblinux_pal_thread_handle_t *thread_handle = NULL;

	liblinux_get_cpus_allowed(&new_mask, __new_mask);

	if (cpumask_equal(&p->cpus_mask, &new_mask) || cpumask_empty(&new_mask) ||
	    !cpumask_intersects(&new_mask, cpu_online_mask)) {
		return -EINVAL;
	}

	if (p->flags & PF_KTHREAD)
		thread_handle = &task_thread_info(p)->thread_handle;

	/*
	 * If config_liblinux_preempt is enabled, set affinity will fail when
	 * the thread is running with preempt disabled, so retry it more times.
	 * It can be fixed untill LDK support wait_task_inactive.
	 */
	for (i = 0; i < __RETRY_CNT_MAX; i++) {
		ret = liblinux_pal_thread_setaffinity(thread_handle,
				*((unsigned long *)cpumask_bits(&new_mask)));
		if (ret >= 0) {
			break;
		}
	}

	if (i == __RETRY_CNT_MAX) {
		if (!cpumask_intersects(&new_mask, cpu_online_mask)) {
			printk(KERN_ERR "set affinity 0x%lx for thread [%s] failed since cpus offline\n",
			       *cpumask_bits(&new_mask), p->comm);
			return -EINVAL;
		} else {
			panic("set affinity 0x%lx for thread [%s] failed, ret=%d\n",
			      *cpumask_bits(&new_mask), p->comm, ret);
		}
	} else if (i != 0) {
		printk(KERN_WARNING "set affinity 0x%lx for thread [%s] retry times=%d\n",
		       *cpumask_bits(&new_mask), p->comm, i);
	}

	/* TODO: check it
	task_thread_info(p)->cpu = cpumask_first(&new_mask);
	*/
	cpumask_copy(&p->cpus_mask, &new_mask);
	p->nr_cpus_allowed = cpumask_weight(&new_mask);

	return ret;
}

void do_set_cpus_allowed(struct task_struct *p, const struct cpumask *__new_mask) {
	__do_set_cpus_allowed(p, __new_mask);
}

int set_cpus_allowed_ptr(struct task_struct *p, const struct cpumask *new_mask)
{
	return __do_set_cpus_allowed(p, new_mask);
}
EXPORT_SYMBOL_GPL(set_cpus_allowed_ptr);

#endif

int io_schedule_prepare(void)
{
	int old_iowait = current->in_iowait;

	current->in_iowait = 1;
	blk_schedule_flush_plug(current);
	liblinux_pal_set_iowait();

	return old_iowait;
}

void io_schedule_finish(int token)
{
	current->in_iowait = token;
	if (token == 0) {
		liblinux_pal_clr_iowait();
	}
}

int sched_setscheduler(struct task_struct *p, int policy,
		       const struct sched_param *param)
{
	return _sched_setscheduler(p, policy, param, true);
}
EXPORT_SYMBOL_GPL(sched_setscheduler);

int sched_setscheduler_nocheck(struct task_struct *p, int policy,
			       const struct sched_param *param)
{
	return _sched_setscheduler(p, policy, param, false);
}

long sched_setaffinity(pid_t pid, const struct cpumask *in_mask)
{
	return liblinux_pal_sched_setaffinity(pid,
		*((unsigned long *)cpumask_bits(in_mask)));
}

long sched_getaffinity(pid_t pid, struct cpumask *mask)
{
	return liblinux_pal_sched_getaffinity(pid,
		((unsigned long *)cpumask_bits(mask)));
}

#ifndef CONFIG_PREEMPTION
int __sched _cond_resched(void)
{
	if (should_resched(0)) {
		//preempt_schedule_common();
		schedule();
		return 1;
	}
	rcu_all_qs();
	return 0;
}
EXPORT_SYMBOL(_cond_resched);
#endif

int sched_setattr_nocheck(struct task_struct *p, const struct sched_attr *attr)
{
	return __sched_setscheduler(p, attr, false, true);
}
EXPORT_SYMBOL_GPL(sched_setattr_nocheck);

void sched_set_fifo(struct task_struct *p)
{
	struct sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_SYMBOL_GPL(sched_set_fifo);

void sched_set_fifo_low(struct task_struct *p)
{
	struct sched_param sp = { .sched_priority = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_SYMBOL_GPL(sched_set_fifo_low);

void sched_set_normal(struct task_struct *p, int nice)
{
	struct sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_SYMBOL_GPL(sched_set_normal);

static void resched_cpu_fn(struct work_struct *work)
{
#ifdef CONFIG_NO_HZ_IDLE
	preempt_disable();
	tick_nohz_idle_restart_tick();
	preempt_enable();
#endif
	kfree(work);
	/* After kworker blocks, rcu_qs will be called. */
}

/* Called by rcu_sched */
void resched_cpu(int cpu)
{
	struct work_struct *work = kmalloc(sizeof(*work), GFP_ATOMIC);
	if (work == NULL) {
		pr_warn_once("%s: no memory\n", __func__);
		return;
	}
	INIT_WORK(work, resched_cpu_fn);
	/* Use normal queue to avoid preempting high-prio tasks. */
	queue_work_on(cpu, system_wq, work);
}

void set_user_nice(struct task_struct *p, long nice)
{
	int ret = 0;

	/* exit when actv task, because pid of actv's task_struct may not be valid */
	if (!(p->flags & PF_KTHREAD)) {
		pr_warn("%s: actv task pid %d may not be valid\n", __func__, p->pid);
		return;
	}

	if (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		return;

	if (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_TO_PRIO(nice);
		return;
	}

	ret = liblinux_pal_set_user_nice(p->pid, nice);
	if (ret == 0) {
		p->static_prio = NICE_TO_PRIO(nice);
		p->prio = effective_prio(p);
	} else {
		pr_warn("%s: failed %d\n", __func__, ret);
	};
}
EXPORT_SYMBOL(set_user_nice);

LIBLINUX_KAPI_DIFF
void kick_process(struct task_struct *p)
{
	(void)p;
}
EXPORT_SYMBOL_GPL(kick_process);

LIBLINUX_KAPI_DIFF
void scheduler_tick(void)
{
}

int in_sched_functions(unsigned long addr)
{
	return in_lock_functions(addr) ||
		(addr >= (unsigned long)__sched_text_start &&
		addr < (unsigned long)__sched_text_end);
}

#ifdef CONFIG_SCHED_DFX_OHOS
#define LONG_PREEMPT_OFF_THRE 1
static long __preempt_disabled_ts[NR_CPUS] = { 0 };

void preempt_latency_start(u32 cpu)
{
	__preempt_disabled_ts[cpu] = jiffies;
}
EXPORT_SYMBOL(preempt_latency_start);

void preempt_latency_stop(u32 cpu)
{
	long delta = jiffies - __preempt_disabled_ts[cpu];
	if (delta > LONG_PREEMPT_OFF_THRE) {
		pr_err("long preempt off at cpu%d, delta=%ld\n", cpu, delta);
		dump_stack();
	}
}
EXPORT_SYMBOL(preempt_latency_stop);
#endif