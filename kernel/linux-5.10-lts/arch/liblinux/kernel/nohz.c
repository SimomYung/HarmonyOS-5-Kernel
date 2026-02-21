// SPDX-License-Identifier: GPL-2.0-only
#include <linux/sched/nohz.h>
#include <linux/module.h>
#include <linux/cpu.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <asm/smp.h>
#include <linux/rcutree.h>
#include <linux/clockchips.h>
#include "../../../kernel/time/tick-internal.h"

#include <liblinux/pal.h>
#include <liblinux/sched.h>
#include <trace/hooks/liblinux.h>

static bool in_suspend_process = false;
struct rcu_tick_work {
	struct work_struct work;
	bool init;
};

static DEFINE_PER_CPU(bool, rcu_needs_qs) = false;
static DEFINE_PER_CPU(struct rcu_tick_work, restart_work) = { .init = false };

bool check_do_timer_none()
{
	return (tick_do_timer_cpu == TICK_DO_TIMER_NONE);
}

static void __try_stop_tick(int cpu)
{
	int ret;

	tick_nohz_idle_enter();
	local_irq_disable();
	tick_nohz_idle_stop_tick();
	ret = tick_nohz_tick_stopped();
	local_irq_enable();

	if (ret == 0) {
		tick_nohz_idle_exit();
	}
}

void liblinux_try_stop_tick(int cpu)
{
	struct tick_sched *ts;
	struct clock_event_device *dev;
	int this_cpu;

	/* kevtd may have been migrated to another cpu, check before continue */
	this_cpu = get_cpu();
	if (this_cpu != cpu || this_cpu == 0) {
		goto out;
	}

	ts = tick_get_tick_sched(cpu);
	dev = __this_cpu_read(tick_cpu_device.evtdev);

	/* 
	 * Still in idle, but tick is restarted somewhere.
	 * Do the last part of exiting idle status here.
	 */
	if (ts->inidle) {
		tick_nohz_idle_exit();
		if (clockevent_state_oneshot_stopped(dev)) {
			pr_warn("kevtd/%d: dev state is still oneshot_stopped, tick_stopped=%d\n",
				cpu, ts->tick_stopped);
		}
		goto out;
	}

	/* Stop tick when there is only kevtd running */
	if (need_resched()) {
		goto out;
	}
	if (unlikely(local_softirq_pending())) {
		goto out;
	}

	/*
	 * Keep tick to accelerate suspend process
	 */
	if (READ_ONCE(in_suspend_process)) {
		goto out;
	}

	if (per_cpu(rcu_needs_qs, this_cpu)) {
		goto out;
	}

	__try_stop_tick(cpu);

out:
	put_cpu();
}

static void hook_tick_irq_exit(void *data, int cpu)
{
	if (!tick_nohz_tick_stopped() || in_irq()) {
		return;
	}
	if (cpu != 0 && check_do_timer_none()) {
		tick_nohz_idle_restart_tick();
	}
}
INIT_VENDOR_HOOK(ldk_rvh_tick_irq_exit, hook_tick_irq_exit);

static void hook_rcu_restart_tick(void *data, int wake_cpu)
{
	int cpu;
	if (wake_cpu < nr_cpu_ids) {
		if (wake_cpu != 0 && tick_nohz_tick_stopped_cpu(wake_cpu)) {
			wake_up_nohz_cpu(wake_cpu);
		}
		return;
	}

	/* Restart tick on all cpus if wake_cpu == nr_cpu_ids */
	for_each_online_cpu(cpu) {
		if (cpu != 0 && tick_nohz_tick_stopped_cpu(cpu)) {
			wake_up_nohz_cpu(cpu);
		}
	}
}
INIT_VENDOR_HOOK(ldk_rvh_rcu_restart_tick, hook_rcu_restart_tick);

static void restart_fn(struct work_struct *work)
{
	preempt_disable();
	tick_nohz_idle_restart_tick();
	preempt_enable();
}

static void hook_rcu_notify_qs(void *data, int cpu, bool need_qs)
{
	struct rcu_tick_work *rcu_work;

	rcu_work = per_cpu_ptr(&restart_work, cpu);
	if (unlikely(!(rcu_work->init))) {
		/*
		 * This init process will be called in rcu_init(), which
		 * is in the very early booting stage. Putting this init
		 * in initcalls will be too late.
		 */
		INIT_WORK(&(rcu_work->work), restart_fn);
		rcu_work->init = true;
	}

	per_cpu(rcu_needs_qs, cpu) = need_qs;
	if (need_qs) {
		queue_work_on(cpu, system_wq, &(rcu_work->work));
	}
}
INIT_VENDOR_HOOK(ldk_rvh_rcu_notify_qs, hook_rcu_notify_qs);

void liblinux_inform_suspend(void)
{
	int cpu;
	WRITE_ONCE(in_suspend_process, true);
	for_each_online_cpu(cpu) {
		wake_up_nohz_cpu(cpu);
	}
}

void liblinux_inform_resume(void)
{
	WRITE_ONCE(in_suspend_process, false);
}

#define TASKLET_MASK (1UL << TASKLET_SOFTIRQ)
static void hook_wakeup_softirqd(void *data, int unused)
{
	unsigned long pending = local_softirq_pending();
	if ((pending & TASKLET_MASK) != 0) {
		tick_nohz_idle_restart_tick();
	}
}
INIT_VENDOR_HOOK(ldk_rvh_wakeup_softirqd, hook_wakeup_softirqd);
