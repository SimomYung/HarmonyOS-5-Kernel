/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of timer-related operations
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 08 09:43:26 2020
 */
#ifndef MAPI_HMKERNEL_TIMER_H
#define MAPI_HMKERNEL_TIMER_H

#include <hmkernel/module_proto.h>
#include <hmkernel/timer/vtimekeep.h>

#define KERN_TIMER_ABSTIME		1
#define BROADCAST_TIMER_EXPIRE_MAX	(~0ULL)
#define TIMER_EXPIRE_MAX		(BROADCAST_TIMER_EXPIRE_MAX - 1ULL)

#define TIMER_STATE_FREE		0U
#define	TIMER_STATE_ACTIVE		1U
#define	TIMER_STATE_HANDLE		2U
#define CPU_RELAX_MAX_ROUND 512
#define TIMER_NO_RESTART (struct timer_ret_s) { false }

enum rbt_timer_mode {
	RBTIMER_MODE_REL,
	RBTIMER_MODE_ABS,
};

enum rbt_clock_base_type {
	CLOCK_TYPE_REALTIME,
	CLOCK_TYPE_MONOTONIC,
	CLOCK_TYPE_BOOTTIME,
	CLOCK_TYPE_MAX,
};

#ifdef CONFIG_MODULE_TIMER_RBTREE
/*
 * After the CPU is powered off and on again, the expire of vtimer of this cpu
 * needs to be reprogrammed.
 */
void vtimer_reprogram_current_cpu(void);
/* After adjusting the system time, reset the clock of each CPU */
extern int reset_pcp_clock(void);
/* get the timeout interval of the given CPU */
extern u64 timer_rbt_first_timeout(unsigned int cpuid);
extern u64 timer_rbt_next_timeout(unsigned int cpuid);
extern void rbtree_timer_dump_last_timers(void);
extern void rbtree_timer_dump_percpu_first_timer(void);
extern void timer_slack_unihb_maybe_active_other_cpus(u64 curr_tock);
extern void rbtree_timer_dump_timer_status(void);
#else
static inline void rbtree_timer_dump_percpu_first_timer(void)
{
	return;
}
static inline void rbtree_timer_dump_timer_status(void)
{
}
static inline void rbtree_timer_dump_last_timers(void)
{
}
#endif

struct timer_ret_s {
	bool timer_restart;
};

enum timer_type {
	TIMER_TYPE_CAP = 1,
	TIMER_TYPE_VM,
	TIMER_TYPE_IRQSC,
	TIMER_TYPE_EPOLL,
	TIMER_TYPE_EVENTPOLL, /* = 5 */
	TIMER_TYPE_PPOLL,
	TIMER_TYPE_SELECT,
	TIMER_TYPE_FUTEX,
	TIMER_TYPE_RTFUTEX,
	TIMER_TYPE_TCB, /* = 10 */
	TIMER_TYPE_TICKDEFER,
	TIMER_TYPE_VTIMER,
	TIMER_TYPE_KVIC,
	TIMER_TYPE_HMLT,
	TIMER_TYPE_SCHED, /* = 15 */
	TIMER_TYPE_RT,
	TIMER_TYPE_ALARM,
	TIMER_TYPE_ISOCTL,
	TIMER_TYPE_SLEEP,
	TIMER_TYPE_UADDR, /* = 20 */
	TIMER_TYPE_PERF_NG,
	TIMER_USAGE_THREAD_CPUTIME,
	TIMER_USAGE_PROCESS_CPUTIME,
	TIMER_USAGE_ULWT,
	TIMER_USAGE_SMMU, /* = 25 */
	TIMER_USAGE_CPUFREQ,
};

/*
 * @callback should be called when timer interrupt
 * happens at the @expire point in the future. And
 * no timer_lock is hold when @callback is called.
 */
struct timer_s;
typedef struct timer_ret_s (*timer_callback_t)(struct timer_s *timer, u64 curr_time);
typedef struct timer_s {
	struct timer_protodata_s proto;
	enum timer_type type;
	char clk_ori_id;
	char clk_id;
	char flags;
	unsigned char state;
	u64 soft_expire;
	u64 expire;
	timer_callback_t callback;
	unsigned int cpu;
} __timer_s __kit_state(timer);

static inline int vtimekeep_clk_to_rbt_clk(unsigned long clk_id)
{
	int ret_clk_id;

	if (clk_id == VTIMEKEEP_CLOCK_MONOTONIC) {
		ret_clk_id = CLOCK_TYPE_MONOTONIC;
	} else if (clk_id == VTIMEKEEP_CLOCK_BOOTTIME) {
		ret_clk_id = CLOCK_TYPE_BOOTTIME;
	} else {
		ret_clk_id = CLOCK_TYPE_REALTIME;
	}

	return ret_clk_id;
}

static inline int vtimekeep_alarm_clk_to_rbt_clk(unsigned long clk_id)
{
	int ret_clk_id = CLOCK_TYPE_REALTIME;

	if (clk_id == VTIMEKEEP_CLOCK_BOOTTIME_ALARM) {
		ret_clk_id = CLOCK_TYPE_BOOTTIME;
	}

	return ret_clk_id;
}

/*
 * Current cpu_relax_round is used only in timer_remove function,
 * the cpu_relax_round function is called by function which is a very tight loop
 * and may starve another CPU's LL/SC atomic operation due to cache locality.
 * And cpu_relax_round can solve it by gradually increase the number of spins to mitigate this issue.
 */
static inline void cpu_relax_round(int *cpu_relax_curr_round)
{
	if (*cpu_relax_curr_round < CPU_RELAX_MAX_ROUND) {
		(*cpu_relax_curr_round)++;
	}
	for (int i = 0; i < *cpu_relax_curr_round; ++i) {
		cpu_relax();
	}
	return;
}

/* called by tcb_init or cap_timer_create */
extern void timer_init(struct timer_s *timer,
		       struct timer_ret_s (*callback)(struct timer_s *timer,
						      u64 curr_time),
		       enum timer_type type);

/* add a timer on the current cpu */
extern int timer_add(struct timer_s *timer, u64 expire, u64 slack);
extern int timer_add_maybe_slack(struct tcb_s *tcb, struct timer_s *timer, u64 expire);
extern int timer_add_nolock(struct timer_s *timer, u64 expire, u64 slack);

#ifdef CONFIG_TIMEWHEEL_TIMER
/* add a timer to timewheel slot head (by default add to tail) */
extern int timer_add_head(struct timer_s *timer, u64 expire);
#endif

/* whether the timer is pending or not */
extern bool timer_is_pending(struct timer_s *timer);

/* remove the pending timer */
extern int timer_remove(struct timer_s *timer);
extern int timer_remove_nolock(struct timer_s *timer);
extern int timer_try_remove(struct timer_s *timer);

/* migrate a timer to another cpu */
extern void timer_migrate(struct timer_s *timer, unsigned int target_cpu);

/* return the remaining time of the removed timer */
extern u64 timer_rmtime(const struct timer_s *timer);

/* migrate all the timers from @src_cpu to @dst_cpu */
extern int migrate_timers(unsigned int src_cpu, unsigned int dst_cpu);

static inline u64 timer_unify_by_expire(int clk_id, u64 timer_expire, int flag)
{
	u64 expire;
	if (clk_id == (int)CLOCK_TYPE_REALTIME) {
		expire = vtimekeep_tock_real_to_mono(timer_expire);
	} else if (clk_id == (int)CLOCK_TYPE_BOOTTIME) {
		if (((unsigned char)flag & (unsigned char)KERN_TIMER_ABSTIME) == (unsigned char)RBTIMER_MODE_ABS) {
			/* In abstime's insertion, the rato_mono_diff_tock(tock_start) is not added */
			expire = vtimekeep_tock_boot_to_rato(timer_expire);
		} else {
			/* In reltime's insertion, the rato_mono_diff_tock(tock_start) is added */
			expire = vtimekeep_tock_boot_to_mono(timer_expire);
		}

		/*
		* When tock_start(rato_mono_diff) is larger than rawtock, unify_expire
		* becomes negative, which leads to repeatedly setting expired alarm timer.
		*/
		if ((s64)expire < 0) {
			expire = 0ULL;
		}
	} else {
		expire = timer_expire;
	}

	return expire;
}

static inline u64 unify_timer_expire(const struct timer_s *timer)
{
	return timer_unify_by_expire(timer->clk_id, timer->expire, timer->flags);
}

static inline u64 unify_timer_soft_expire(const struct timer_s *timer)
{
	return timer_unify_by_expire(timer->clk_id, timer->soft_expire, timer->flags);
}

static inline void timer_set_cpuid(struct timer_s *timer, unsigned int cpu)
{
	timer->cpu = cpu;
}

#ifdef __HOST_LLT__
void timer_rbt_init(void);
u64 llt_unify_timer_expire(const struct timer_s *timer);
#endif

void timer_hdlr_illegal_output(void);
#endif
