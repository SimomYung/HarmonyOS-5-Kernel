/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Definition of alarm timer related operations
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 02 20:43:52 2023
 */
#ifndef MAPI_HMKERNEL_ALARM_TIMER_H
#define MAPI_HMKERNEL_ALARM_TIMER_H

#include <hmkernel/module_proto.h>
#include <hmkernel/time.h>
#include <hmkernel/timer/timer.h>
#include <hmkernel/timer/vtimekeep.h>

/* alarm micro */
#define ALARM_TIMER_MODE 0x0100

/* The minimum time for system suspend */
#define MIN_SUSPEND_SECOND 2

/* The minimum time between last alarm to current suspend is 100ms */
#define MIN_ALARM_SUSPEND_PREGAP_MSEC 100U
#define MIN_ALARM_SUSPEND_PREGAP_TOCK ((u64)vtimekeep_freq() * MIN_ALARM_SUSPEND_PREGAP_MSEC / __MSEC_PER_SEC)

enum alarm_clock_type {
	ALARM_CLOCK_REALTIME,
	ALARM_CLOCK_BOOTTIME,
	ALARM_CLOCK_MAX,
};

enum alarm_timer_state {
	ALARM_TIMER_STATE_FREE,
	ALARM_TIMER_STATE_ONTREE,
	ALARM_TIMER_STATE_HANDLE,
};

/*
 * struct alarm_timer_s is alarm timer structure
 * @proto: node for adding to the rbtree
 * @callback: callback function executed when the timer expired
 * @timer: timer is uniformly scheduled
 * @type: alarm timer type
 * @state: current state of alarm timer
 */
typedef struct timer_ret_s (*alarm_timer_callback_t)(struct timer_s *alarm_timer, u64 curr_time);
struct alarm_timer_s {
	struct timer_s timer;
	struct timer_protodata_s proto;
	alarm_timer_callback_t callback;
	enum alarm_clock_type type;
	enum alarm_timer_state state;
};

#ifdef CONFIG_RTC
/* init a alarm timer */
extern void alarm_timer_init(struct alarm_timer_s *alarm_timer,
			     struct timer_ret_s (*callback)(struct timer_s *timer, u64 curr_time),
			     unsigned int clk_id);

/*
 * Add an alarm timer to rbtree, it has two types of return:
 * 1. E_HM_BUSY, the alarm timer to be added is in state ALARM_TIMER_STATE_HANDLE
 * or ALARM_TIMER_STATE_ACTIVE,
 * 2. E_HM_OK, the alarm timer to be added is in state ALARM_TIMER_STATE_FREE.
 */
extern int alarm_timer_add(struct alarm_timer_s *alarm_timer, u64 expire);

/*
 * alarm_timer_remove() will remove an alarm timer until its state changes to
 * ALARM_TIMER_STATE_FREE, it has two types of return values:
 * 1. E_HM_NOOBJ, the alarm timer to be removed is always in state ALARM_TIMER_STATE_FREE,
 * 2. E_HM_OK, the alarm timer to be removed is in state ALARM_TIMER_STATE_HANDLE or
 *    ALARM_TIMER_STATE_ACTIVE.
 */
extern int alarm_timer_remove(struct alarm_timer_s *alarm_timer);

/*
 * alarm_timer_try_remove() will try to remove an alarm timer, it has three
 * types of return values:
 * 1. E_HM_NOOBJ, the alarm timer to be removed is always in state
 *    ALARM_TIMER_STATE_FREE,
 * 2. E_HM_BUSY, the alarm timer to be removed is in state ALARM_TIMER_STATE_HANDLE,
 * 3. E_HM_OK, the alarm timer to be removed is in state ALARM_TIMER_STATE_ACTIVE.
 */
extern int alarm_timer_try_remove(struct alarm_timer_s *alarm_timer);

extern void alarm_timer_next_pid_and_name(int *pid_rp, char *name_rp);

/* record suspend and resume rato tock */
extern int alarm_timer_curr_suspend_time(void);
#else
/* init a alarm timer */
static inline void alarm_timer_init(struct alarm_timer_s *alarm_timer,
			     struct timer_ret_s (*callback)(struct timer_s *timer, u64 curr_time),
			     unsigned int clk_id)
{
	return;
}

/* add an alarm timer */
static inline int alarm_timer_add(struct alarm_timer_s *alarm_timer, u64 expire)
{
	return E_HM_OK;
}

/* remove the alarm timer */
static inline int alarm_timer_remove(struct alarm_timer_s *alarm_timer)
{
	return E_HM_OK;
}

static inline int alarm_timer_try_remove(struct alarm_timer_s *alarm_timer)
{
	return E_HM_OK;
}

static inline void alarm_timer_next_pid_and_name(int *pid_rp, char *name_rp)
{
	return;
}

static inline int alarm_timer_curr_suspend_time(void)
{
	return E_HM_OK;
}
#endif

/* suspend system with the alarm timer */
extern int alarm_timer_suspend(void);

/* resume system by removing the alarm timer */
extern int alarm_timer_resume(void);

/* register suspend & resume for pm */
extern void alarm_timer_pm_dev_register(void);

#ifdef __HOST_LLT__
extern int alarm_timer_base_init(void);
#endif

#endif
