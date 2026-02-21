/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Definition of vtimer
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 16 14:04:30 2020
 */

#ifndef MAPI_HMKERNEL_VTIMER_H
#define MAPI_HMKERNEL_VTIMER_H

#include <hmkernel/types.h>
#include <hmkernel/interrupt.h>
#include <hmkernel/tcb/struct.h>

struct ptimedev_s;
/*
 * TIMERDEV_STATE_STOP: All timers of the current cpu are turned off.
 * TIMERDEV_STATE_START: tick_timer of the current cpu are turned on.
 * TIMERDEV_STATE_TICKOFF: tick_timer of the current cpu are turned off.
 * Scheduling and feeding watchdog depend on the tick_timer of each cpu.
 */
enum timerdev_state {
	TIMERDEV_STATE_STOP,
	TIMERDEV_STATE_START,
	TIMERDEV_STATE_TICKOFF,
	TIMERDEV_STATE_INVAL,
};

enum timerdev_mode {
	TIMERDEV_MODE_PERIODIC,
	TIMERDEV_MODE_ONESHOT,
};

struct pcp_timer {
	struct ptimedev_s *pdev;
	enum timerdev_mode mode;
	unsigned int cpu;

	/*
	 * @start:	Start all timer function
	 * @cancel:	Cancel all timer function
	 * @reprogram:	Reprogram timer function
	 */
	void (*start)(struct pcp_timer *timer);
	int (*cancel)(struct pcp_timer *timer);
	int (*reprogram)(struct pcp_timer *timer, u64 value);
	void (*dump)(const struct pcp_timer *timer);

	void *tock_args;
	unsigned long (*tock_callback)(struct pcp_timer *timer, u64 current_tock, void *arg);

	u64 next_tock_at;
	bool has_next_tock;
	bool is_inited;
	u64 interval;

	struct tcb_s *preempted_tcb;
	enum timerdev_state state;
};

#ifdef __HOST_LLT__
/* for LLT usage */
void timer_cpu_hotplug_handler(struct knotifier_obj *knobj, int value, void *private);
#endif

struct pcp_timer *vtimer_of_cpu(unsigned int cpu);
void vtimer_start_percpu(unsigned int cpu);
void vtimer_cancel_percpu(unsigned int cpu);
int vtimer_migrate_timers(unsigned int src_cpu, unsigned int dst_cpu);

/* tcb tickless interfaces */
void vtimer_cancel_tick(unsigned int cpu);
/* This func should be called on current cpu (Paramter cpu should be current cpu). */
void vtimer_restart_tick(unsigned int cpu);

enum timerdev_state vtimer_state_of(unsigned int cpu);

static inline bool vtimer_tick_is_on(unsigned int cpu)
{
	return vtimer_state_of(cpu) == TIMERDEV_STATE_START;
}

#endif
