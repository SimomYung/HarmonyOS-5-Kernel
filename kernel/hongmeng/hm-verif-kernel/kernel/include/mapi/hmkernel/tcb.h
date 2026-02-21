/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Export kernel tcb.h to mapi, and declare functions for mapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 10 10:19:14 2019
 */
#ifndef MAPI_HMKERNEL_TCB_H
#define MAPI_HMKERNEL_TCB_H

#include <uapi/hmkernel/tcb.h>
#include <uapi/hmkernel/tcb_stat.h>
#include <uapi/hmkernel/tcb_sched.h>
#include <uapi/hmkernel/rq.h>
#include <hmkernel/types.h>
#include <hmkernel/tcb/struct.h>
#include <hmkernel/cpuset.h>
#include <hmkernel/capability.h>
#include <mapi/hmkernel/syscall.h>
#include <hmkernel/klog_level.h>
#include <hmkernel/debug/dumpstack_user.h>

struct tcb_s;
struct scheduler_s;
struct trigger_listener_s;
struct CNode_s;
struct actv_s;

#define tcb_sched_dump_on(cond, tcb, klog_level) \
do {					\
	if (cond) {		\
		__tcb_sched_dump(tcb, __func__, __LINE__, klog_level); \
	}				\
} while (0)

#define tcb_user_stack_dump_on(cond, tcb) \
do {					\
	if (cond) {		\
		dumpstack_user_callchain_lastword(tcb); \
	}				\
} while (0)

#define tcb_sched_dump(tcb) \
	__tcb_sched_dump(tcb, __func__, __LINE__, KLOG_INFO)

#define TCB_INFO_ON(cond, tcb) \
	tcb_sched_dump_on(cond, tcb, KLOG_INFO)

#define TCB_BUG_ON(cond, tcb) \
do {							\
	bool _c = (cond);				\
	tcb_sched_dump_on(_c, tcb, KLOG_PANIC);		\
	BUG_ON(_c, #cond); 				\
} while (0)

#define TCB_PREEMPT_BUG_ON(cond, tcb) \
do {								\
	bool _c = ((cond) && !kernel_resched_enabled());	\
	tcb_user_stack_dump_on(_c, tcb);			\
	tcb_sched_dump_on(_c, tcb, KLOG_PANIC); 		\
	BUG_ON(_c, #cond); 					\
} while (0)

#ifdef CONFIG_DEBUG_BUILD
#define tcb_current_assert(__tcb, __irq) \
do {												\
	struct tcb_s *__curr = tcb_current();							\
	BUG_ON(((__tcb) != NULL) && ((__tcb) != __curr),					\
	       "current tcb is switched to %s cpu %u after irq %d, expect curr %s cpu %u\n",	\
	       tcb_name_of(__curr), tcb_cpu_of(__curr), __irq,			\
	       tcb_name_of(__tcb), tcb_cpu_of(__tcb));						\
} while (0)
#else
#define tcb_current_assert(__tcb, __irq)
#endif

void __tcb_sched_dump(const struct tcb_s *tcb, const char *fn, int ln,
		      enum klog_level lv);

extern struct scheduler_s *tcb_scheduler_of(const struct tcb_s *tcb);

#ifdef CONFIG_MODULE_SCHED_EER
extern void
tcb_set_irq(struct tcb_s *tcb, bool flag);
extern bool
tcb_is_irq(const struct tcb_s *tcb);
extern void
tcb_set_pin_preempted_tcb_flag(struct tcb_s *tcb, bool flag);
extern bool
tcb_is_pin_preempted_tcb(const struct tcb_s *tcb);
#else
static inline void tcb_set_irq(struct tcb_s *tcb, bool flag)
{
	UNUSED(tcb, flag);
}
static inline bool tcb_is_irq(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return false;
}
static inline void tcb_set_pin_preempted_tcb_flag(struct tcb_s *tcb, bool flag)
{
	UNUSED(tcb, flag);
}
static inline bool tcb_is_pin_preempted_tcb(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return false;
}
#endif
extern unsigned int
tcb_cpu_of(const struct tcb_s *tcb);
extern unsigned int
tcb_rq_of(const struct tcb_s *tcb);
extern int
tcb_resume(struct tcb_s *tcb);
extern int
tcb_suspend(struct tcb_s *tcb);
extern int
tcb_switch_rq(struct tcb_s *tcb, unsigned int newrq, const void __user *args, size_t args_size);
extern int
tcb_switch_user_rq(struct tcb_s *tcb);
extern int
set_user_rq(struct tcb_s *tcb, unsigned int newrq);
extern int
tcb_switch_rq_nocheck(struct tcb_s *tcb, unsigned int newrq);
extern int
tcb_switch_rq_nocheck_nolock(struct tcb_s *tcb, unsigned int newrq, const void *args, size_t args_size);
extern enum tcb_state_e
tcb_state_of(const struct tcb_s *tcb);
extern void
tcb_set_cpu_nocheck(struct tcb_s *tcb, unsigned int cpu);
extern void
tcb_set_cpu(struct tcb_s *tcb, unsigned int cpu);
extern struct trigger_listener_s *
tcb_curr_listener_of(struct tcb_s *tcb);
extern int
tcb_switch_scheduler(struct tcb_s *tcb, struct scheduler_s *scheduler);
extern const char *
tcb_name_of(const struct tcb_s *tcb);
extern void
tcb_intr(struct tcb_s *tcb, int retval);
extern bool
tcb_is_idle(const struct tcb_s *tcb);
extern void
tcb_block_timer_init(struct tcb_s *tcb);
extern struct timer_s *
tcb_block_timer_of(struct tcb_s *tcb);
extern struct tcb_s *
tcb_of_block_timer(const struct timer_s *timer);
extern struct CNode_s *
tcb_base_cnode_of(const struct tcb_s *tcb);
extern struct CNode_s *
tcb_curr_cnode_of(const struct tcb_s *tcb);
extern cref_t
tcb_cref_of(const struct tcb_s *tcb);
extern struct trigger_source_s *
tcb_exit_trigger_source_of(struct tcb_s *tcb);
extern struct tcb_s *
tcb_of_block_at(const void *block_at);
extern void tcb_unblock(struct tcb_s *tcb, enum tcb_block_reason block_reason, int retval);
JAM_INLINE bool
tcb_is_active(const struct tcb_s *tcb);
JAM_INLINE struct arch_uctx *
tcb_curr_uctx_of(struct tcb_s *tcb);
JAM_INLINE struct tcb_s *
tcb_current(void);
JAM_INLINE unsigned int
tcb_user_rq_of(const struct tcb_s *tcb);
JAM_INLINE void
tcb_set_retval_ul(struct tcb_s *tcb, unsigned long retval);
JAM_INLINE void
tcb_set_retval_l(struct tcb_s *tcb, long retval);
JAM_EXTERN_INLINE bool
tcb_is_base_actv(struct tcb_s *tcb, const struct actv_s *actv);
JAM_EXTERN_INLINE bool
tcb_is_prime_actv(struct tcb_s *tcb, const struct actv_s *actv);
JAM_EXTERN_INLINE bool
tcb_is_effective_actv(struct tcb_s *tcb, const struct actv_s *actv);
JAM_INLINE struct actv_s *
tcb_curr_raw_actv_of(const struct tcb_s *tcb);
JAM_INLINE struct actv_s *
tcb_curr_actv_of_nolock(const struct tcb_s *tcb);
#ifdef __HOST_LLT__
#define tcb_curr_actv_of(tcb) tcb_curr_actv_of_nolock(tcb)
#endif
JAM_INLINE struct actv_s *
tcb_acquire_curr_actv(const struct tcb_s *tcb);
JAM_INLINE void
tcb_release_curr_actv(void);
JAM_INLINE void
tcb_set_curr_actv(struct tcb_s *tcb, struct actv_s *actv);
JAM_INLINE struct tcb_s *
tcb_schedule_tcb_of(struct tcb_s *tcb);
JAM_INLINE u64
tcb_sctime_of(const struct tcb_s *tcb);
extern int switch_kern_scheduler_migrate_to(struct tcb_s *tcb, unsigned int rq, unsigned int cpu);
extern int switch_user_scheduler_migrate_to(struct tcb_s *tcb, unsigned int cpu);

extern bool tcb_trylock(struct tcb_s *tcb);
extern void tcb_lock(struct tcb_s *tcb);
extern void tcb_unlock(struct tcb_s *tcb);
extern void double_tcb_lock(struct tcb_s *src_tcb, struct tcb_s *des_tcb);
extern void double_tcb_unlock(struct tcb_s *src_tcb, struct tcb_s *des_tcb);
extern void tcb_pi_lock(const struct tcb_s *tcb);
extern void tcb_pi_unlock(const struct tcb_s *tcb);

#ifdef CONFIG_SMP
/*
 * As we remove the thread lock before schedule, when schedule, we don't know
 * which tcb picked yet, so only holds the rq lock to pick the tcb, then online
 * it by holding the tcb_sched lock.
 *
 * There is a gap when a tcb is not on queue and in the READY state after pick &
 * dequeue, and clearly we should treat it as RUNNING state, not the READY, e.g.,
 * When concurrently do schedule and do tcb_switch_rq like following:
 *
 *   schedule             tcb_swith_rq
 *
 *    /======\              /======\
 *    | CPU0 |              | CPU1 |
 *    \======/              \======/
 *
 *  lock_rq
 *    pick & dequeue
 *  unlock_rq
 *       |
 *       +-----------------> observing
 *       |                   |
 *  tcb_lock           READY & not on queue
 *    online set RUNNING
 *  tcb_unlock
 *
 * So in this way, make `tcb_is_on_queue` serve as a synchronization state.
 *
 * ```
 * tcb_lock
 *   if tcb ready
 *     lock_rq
 *     double check the on queue state
 *     ...
 *     unlock_rq
 * tcb_unlock
 * ```
 *
 * In one word, the assumption that a tcb in the READY state must be on the queue
 * no longer holds.
 */
/* Must be called while rq lock is held */
static inline bool tcb_is_on_queue(const struct tcb_s *tcb)
{
	return tcb->tcb_on_queue;
}

/* Must be called while rq lock is held */
static inline void tcb_set_on_queue(struct tcb_s *tcb)
{
	tcb->tcb_on_queue = true;
}

/* Must be called while rq lock is held */
static inline void tcb_clr_on_queue(struct tcb_s *tcb)
{
	tcb->tcb_on_queue = false;
}
#else
/* Check if the READY tcb picked by schedule or not, when SMP not enabled
 * the READY tcb always on queue */
static inline bool tcb_is_on_queue(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return true;
}

static inline void tcb_set_on_queue(struct tcb_s *tcb)
{
	UNUSED(tcb);
}

static inline void tcb_clr_on_queue(struct tcb_s *tcb)
{
	UNUSED(tcb);
}
#endif

#ifdef CONFIG_TIMER_SLACK
static inline void tcb_set_in_restart_block(struct tcb_s *tcb, bool in_restart_block)
{
	tcb->in_restart_block = in_restart_block;
}
#else
static inline void tcb_set_in_restart_block(struct tcb_s *tcb, bool in_restart_block)
{
	UNUSED(tcb, in_restart_block);
}
#endif

extern struct krec_ticketlock_s* tcb_rq_lock_of(const struct tcb_s *tcb);
extern void tcb_rq_lock(struct tcb_s *tcb);
extern void tcb_rq_unlock(struct tcb_s *tcb);

static inline void tcb_set_retval(struct tcb_s *tcb, int retval)
{
	struct arch_uctx *uctx = tcb_curr_uctx_of(tcb);
	arch_uctx_setret(uctx, retval);
}

static inline bool tcb_is_ready(const struct tcb_s *tcb)
{
	return tcb_state_of(tcb) == TCB_STATE_READY;
}

static inline bool tcb_is_dead(struct tcb_s *tcb)
{
	return (tcb_state_of(tcb) == TCB_STATE_DEAD);
}

static inline bool tcb_is_running(const struct tcb_s *tcb)
{
	return tcb_state_of(tcb) == TCB_STATE_RUNNING;
}

static inline bool tcb_is_blocked(const struct tcb_s *tcb)
{
	return tcb_state_of(tcb) == TCB_STATE_BLOCKED;
}

static inline bool
tcb_is_prev_idle(const struct tcb_s *tcb)
{
	return tcb_user_rq_of(tcb) == HM_RUNQUEUE_PREV_IDLE;
}

static inline bool tcb_is_rt(const struct tcb_s *tcb)
{
	return tcb_rq_of(tcb) == HM_RUNQUEUE_MODULE_RT;
}

extern void tcb_offline(struct tcb_s *tcb, enum tcb_state_e state);

void tcb_schedflags_clr(struct tcb_s *tcb, u32 sched_flags);
void tcb_schedflags_set(struct tcb_s *tcb, u32 sched_flags);
void tcb_schedflags_set_remote(struct tcb_s *tcb, u32 sched_flags);
bool tcb_schedflags_tst(const struct tcb_s *tcb, u32 sched_flags);
unsigned int tcb_schedflags_of(const struct tcb_s *tcb);

static inline void tcb_schedflags_set_repick(struct tcb_s *tcb)
{
#ifdef CONFIG_SMP
	if (tcb == tcb_current()) {
		tcb_schedflags_set(tcb, SCHED_FLAG_REPICK);
	} else {
		tcb_schedflags_set_remote(tcb, SCHED_FLAG_REPICK);
	}
#else
	tcb_schedflags_set(tcb, SCHED_FLAG_REPICK);
#endif
}

#ifdef CONFIG_ATOMIC_CONTEXT_BLOCK_CHECK
bool atomic_context_block_check_is_enable(void);
void tcb_set_as_seq(struct tcb_s *tcb, int seq);
#else
static inline bool
atomic_context_block_check_is_enable(void)
{
	return false;
}
static inline void tcb_set_as_seq(struct tcb_s *tcb, int seq)
{
	UNUSED(tcb, seq);
}
#endif

static inline bool tcb_schedflags_tst_resched(struct tcb_s *curr)
{
	return tcb_schedflags_tst(curr, SCHED_FLAG_RESCHED);
}
#endif
