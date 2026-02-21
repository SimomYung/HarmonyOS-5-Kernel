// SPDX-License-Identifier: GPL-2.0-only
#include <linux/tick.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/sched/wake_q.h>
#include <linux/kern_levels.h>
#include <linux/blkdev.h>
#include <linux/taskstats.h>
#include <linux/delayacct.h>
#include <linux/kernel_stat.h>
#include <linux/rcupdate_wait.h>
#include "../../../../kernel/time/tick-internal.h"

#include <liblinux/sched.h>

long io_schedule_timeout(long timeout)
{
	int token;
#ifndef CONFIG_LIBLINUX
	struct rq *rq;
#endif
	long ret;

	token = io_schedule_prepare();

	delayacct_blkio_start();
#ifndef CONFIG_LIBLINUX
	rq = raw_rq();
	atomic_inc(&rq->nr_iowait);
#endif
	ret = schedule_timeout(timeout);
	io_schedule_finish(token);
#ifndef CONFIG_LIBLINUX
	atomic_dec(&rq->nr_iowait);
#endif
	delayacct_blkio_end(current);

	return ret;
}
EXPORT_SYMBOL(io_schedule_timeout);

void __sched io_schedule(void)
{
	io_schedule_timeout(MAX_SCHEDULE_TIMEOUT);
}
EXPORT_SYMBOL(io_schedule);

extern int try_to_wake_up(struct task_struct *p, unsigned int state, int wake_flags);

int wake_up_process(struct task_struct *p)
{
	return try_to_wake_up(p, TASK_NORMAL, 0);
}

EXPORT_SYMBOL(wake_up_process);

int default_wake_function(wait_queue_entry_t *curr, unsigned mode, int wake_flags,
			  void *key)
{
	return try_to_wake_up(curr->private, mode, wake_flags);
}
EXPORT_SYMBOL(default_wake_function);

static bool __wake_q_add(struct wake_q_head *head, struct task_struct *task)
{
	struct wake_q_node *node = &task->wake_q;

	/*
	 * Atomically grab the task, if ->wake_q is !nil already it means
	 * its already queued (either by us or someone else) and will get the
	 * wakeup due to that.
	 *
	 * In order to ensure that a pending wakeup will observe our pending
	 * state, even in the failed case, an explicit smp_mb() must be used.
	 */
	smp_mb__before_atomic();
	if (unlikely(cmpxchg_relaxed(&node->next, NULL, WAKE_Q_TAIL)))
		return false;

	/*
	 * The head is context local, there can be no concurrency.
	 */
	*head->lastp = node;
	head->lastp = &node->next;
	return true;
}

void wake_q_add(struct wake_q_head *head, struct task_struct *task)
{
	if (__wake_q_add(head, task))
		get_task_struct(task);
}

void wake_q_add_safe(struct wake_q_head *head, struct task_struct *task)
{
	if (!__wake_q_add(head, task))
		put_task_struct(task);
}

void wake_up_q(struct wake_q_head *head)
{
	struct wake_q_node *node = head->first;

	while (node != WAKE_Q_TAIL) {
		struct task_struct *task;

		task = container_of(node, struct task_struct, wake_q);
		BUG_ON(!task);
		/* task can safely be re-inserted now */
		node = node->next;
		task->wake_q.next = NULL;

		/*
		 * wake_up_process() implies a wmb() to pair with the queueing
		 * in wake_q_add() so as not to miss wakeups.
		 */
		wake_up_process(task);
		put_task_struct(task);
	}
}

#if defined(CONFIG_SMP) && defined(CONFIG_NO_HZ_COMMON)
#ifdef CONFIG_LIBLINUX_NO_HZ
int get_nohz_timer_target(void)
{
	return 0;
}

void wake_up_nohz_cpu(int cpu)
{
	if (cpu && cpu_online(cpu))
		tick_nohz_full_kick_cpu(cpu);
}
#else
int get_nohz_timer_target(void)
{
	int cur_cpu = smp_processor_id();
	int cpu;

	for_each_online_cpu(cpu) {
		if (!idle_cpu(cpu)) {
			return cpu;
		}
	}
	return cur_cpu;
}

static void __wake_up_work(void *info)
{
#ifdef CONFIG_NO_HZ_IDLE
	int cpu = smp_processor_id();
	struct clock_event_device *dev;
	dev = __this_cpu_read(tick_cpu_device.evtdev);
	if (cpu != 0 && tick_nohz_tick_stopped() &&
	    !clockevent_state_detached(dev)) {
		tick_nohz_idle_restart_tick();
	}
#endif
	return;
}

void wake_up_nohz_cpu(int cpu)
{
	smp_call_function_single_force(cpu, __wake_up_work, NULL, false);
}
#endif
#endif

#ifdef CONFIG_SMP
DEFINE_PER_CPU(struct kernel_stat, kstat);
DEFINE_PER_CPU(struct kernel_cpustat, kernel_cpustat);

EXPORT_PER_CPU_SYMBOL(kstat);
EXPORT_PER_CPU_SYMBOL(kernel_cpustat);
#endif

unsigned long nr_iowait_cpu(int cpu)
{
	return 0;
}

int idle_cpu(int cpu)
{
	return tick_nohz_tick_stopped_cpu(cpu);
}

/* pid.c */
int pid_max = PID_MAX_DEFAULT;
#define RESERVED_PIDS	300
static  __cacheline_aligned_in_smp DEFINE_SPINLOCK(pidmap_lock);

struct task_struct *find_task_by_pid_ns(pid_t nr, struct pid_namespace *ns)
{
	RCU_LOCKDEP_WARN(!rcu_read_lock_held(),
			 "find_task_by_pid_ns() needs rcu_read_lock() protection");
	return (struct task_struct *)idr_find(&ns->idr, nr);
}
EXPORT_SYMBOL(find_task_by_pid_ns);

pid_t __alloc_pid(struct task_struct *p, pid_t assigned_pid)
{
	pid_t nr;

	idr_preload(GFP_KERNEL);
	spin_lock_irq(&pidmap_lock);
	nr = idr_alloc(&init_pid_ns.idr, p, assigned_pid, assigned_pid + 1, GFP_ATOMIC);
	spin_unlock_irq(&pidmap_lock);
	idr_preload_end();

	return nr;
}

void __free_pid(struct task_struct *p)
{
	spin_lock_irq(&pidmap_lock);
	idr_remove(&init_pid_ns.idr, p->pid);
	spin_unlock_irq(&pidmap_lock);
}

unsigned long long __weak sched_clock(void)
{
	return (unsigned long long)(jiffies - INITIAL_JIFFIES)
		* (NSEC_PER_SEC / HZ);
}
EXPORT_SYMBOL_GPL(sched_clock);

u64 sched_clock_cpu(int cpu)
{
	return sched_clock();
}
EXPORT_SYMBOL_GPL(sched_clock_cpu);

#ifdef CONFIG_OPT_TIMER_ENERGY_EFFICIENCY
u64 relative_slack_ns(ktime_t req_time, struct task_struct *tsk, u64 default_slack)
{
	return default_slack;
}
#endif
