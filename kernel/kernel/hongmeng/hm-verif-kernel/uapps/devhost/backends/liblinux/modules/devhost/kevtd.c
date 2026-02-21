/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implement clockevent device based on futex
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 9 17:54:35 2020
 */
#include <devhost/backend.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/securec.h>
#include <linux/clockchips.h>
#include <linux/async.h>
#include <linux/irq.h>
#include <linux/hardirq.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
#include <linux/cpuhotplug.h>
#endif
#include <lnxbase/lnxbase.h>

#include "devhost.h"
#define KEVTD_THREAD_NAME_SIZE 16
#define KVIC_DYING_TIMEOUT	(-2)

struct vtimer_event_device {
	struct clock_event_device evt;
	struct task_struct *tsk;
	int kt_next;
	bool online;
};
static DEFINE_PER_CPU(struct vtimer_event_device, vtimer_evt);

static void log_dying_timer(int state)
{
	if (state == KVIC_DYING_TIMEOUT) {
		dump_stack();
		panic("kevtd: set prev DYING to another state\n");
	}
}

static void vtimer_event_handler(unsigned int cpu)
{
	struct vtimer_event_device *vt_evt = per_cpu_ptr(&vtimer_evt, cpu);
	struct pt_regs regs = { 0 };
	struct pt_regs *old_regs = NULL;

	if (vt_evt->online) {
		local_irq_disable();
		old_regs = set_irq_regs(&regs);
		irq_enter();
		vt_evt->evt.event_handler(&vt_evt->evt);
		irq_exit();
		set_irq_regs(old_regs);
		local_irq_enable();
	}
}

static int vtimer_thread_init(unsigned int cpu)
{
	struct vtimer_event_device *vt_evt = per_cpu_ptr(&vtimer_evt, cpu);
	clockevents_config_and_register(&vt_evt->evt, NSEC_PER_SEC, 1, INT_MAX);
	return 0;
}

static int vtimer_set_oneshot(struct clock_event_device *evt)
{
	struct vtimer_event_device *vt_evt =
			container_of(evt, struct vtimer_event_device, evt);
	BUG_ON(vt_evt->kt_next == KVIC_DYING_TIMEOUT);
	vt_evt->online = true;
	return 0;
}

static int vtimer_set_next_event(unsigned long expires_ns, struct clock_event_device *evt)
{
	struct vtimer_event_device *vt_evt =
			container_of(evt, struct vtimer_event_device, evt);
	int ret;

	if (!vt_evt->online) {
		pr_err("vtimer is offline, can't set event\n");
		return -ENODEV;
	}

	vt_evt->kt_next = (int)expires_ns;
	ret = lnxbase_vtimer_set_next_event(vt_evt->kt_next);
	if (ret < 0) {
		vt_evt->kt_next = 0;
	}
	BUG_ON(!vt_evt->online);

	return ret;
}

static int vtimer_set_state_oneshot_stopped(struct clock_event_device *evt)
{
	struct vtimer_event_device *vt_evt =
			container_of(evt, struct vtimer_event_device, evt);
	int ret;

	if (!vt_evt->online) {
		log_dying_timer(vt_evt->kt_next);
	}
	vt_evt->online = false;
	vt_evt->kt_next = -1;
	ret = lnxbase_vtimer_set_next_event(vt_evt->kt_next);
	if (ret < 0) {
		vt_evt->kt_next = 0;
	}

	return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
static int vtimer_starting_cpu(unsigned int cpu)
{
	struct vtimer_event_device *vt_evt = per_cpu_ptr(&vtimer_evt, cpu);

	if (!vt_evt->online) {
		vt_evt->online = true;
		clockevents_config_and_register(&vt_evt->evt, NSEC_PER_SEC, 1, INT_MAX);
		vt_evt->kt_next = -1;
	}
	pr_info("vtimer cpu %u start\n", cpu);

	return 0;
}

static int vtimer_dying_cpu(unsigned int cpu)
{
	int ret = 0;
	struct vtimer_event_device *vt_evt = per_cpu_ptr(&vtimer_evt, cpu);

	vt_evt->online = false;
	vt_evt->kt_next = KVIC_DYING_TIMEOUT;
	vt_evt->evt.state_use_accessors = CLOCK_EVT_STATE_DETACHED;
	ret = lnxbase_vtimer_set_next_event(vt_evt->kt_next);
	pr_info("vtimer cpu %u die\n", cpu);

	return ret < 0 ? ret : 0;
}
#endif

bool is_vtimer_dying(unsigned int cpu)
{
	struct vtimer_event_device *vt_evt = per_cpu_ptr(&vtimer_evt, cpu);

	return vt_evt->kt_next == KVIC_DYING_TIMEOUT;
}

int __init vtimer_kevt_init(void)
{
	int cpu;
	int ret;
	uint32_t cpu_mask = 0U;

	/* setup oneshort vtimer */
	for_each_online_cpu(cpu) {
		struct vtimer_event_device *vt_evt = per_cpu_ptr(&vtimer_evt, cpu);
		vt_evt->tsk = NULL;
		vt_evt->kt_next = -1;
		vt_evt->online = true;

		ret = memset_s(&vt_evt->evt, sizeof(vt_evt->evt), 0, sizeof(vt_evt->evt));
		BUG_ON(ret != 0);
		vt_evt->evt.name = "kevt";
		vt_evt->evt.cpumask = cpumask_of(cpu);
		vt_evt->evt.features = CLOCK_EVT_FEAT_ONESHOT;
		vt_evt->evt.set_state_oneshot = vtimer_set_oneshot;
		vt_evt->evt.set_state_oneshot_stopped = vtimer_set_state_oneshot_stopped;
		vt_evt->evt.set_next_event = vtimer_set_next_event;
		vt_evt->evt.set_state_shutdown = vtimer_set_state_oneshot_stopped;

		cpu_mask |= (1U << cpu);
	}

	ret = lnxbase_vtimer_init(vtimer_event_handler, vtimer_thread_init, cpu_mask);
	if (ret != 0) {
		return ret;
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
	ret = cpuhp_setup_state(CPUHP_AP_DUMMY_TIMER_STARTING,
				"clockevents/lnxbase:starting",
				vtimer_starting_cpu, vtimer_dying_cpu);
#endif

	return ret;
}
