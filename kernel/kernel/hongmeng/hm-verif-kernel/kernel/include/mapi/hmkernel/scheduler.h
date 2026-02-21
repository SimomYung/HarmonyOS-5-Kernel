/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Export scheduler interface to kernel mapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 10 09:59:21 2019
 */
#ifndef MAPI_HMKERNEL_SCHEDULER_H
#define MAPI_HMKERNEL_SCHEDULER_H

#include <hmkernel/module_proto.h>
#include <hmkernel/tcb.h>
#include <uapi/hmkernel/scheduler.h>

struct scheduler_s;

/* scheduler low level API */
extern void sched_wakeup(struct tcb_s *tcb);
extern void sched_append(struct tcb_s *tcb);
extern void sched_enqueue(struct tcb_s *tcb);
extern void sched_dequeue(struct tcb_s *tcb);

extern void
sched_actv_online(struct actv_s *actv, bool is_sysmgr_irq_preempt);
extern void
sched_actv_offline(struct actv_s *curr_actv, struct actv_s *next_actv, bool is_pop);
extern bool
sched_actv_fastpath_possible(struct actv_s *caller_actv, struct actv_s *callee_actv, bool is_pop);

extern bool
sched_is_tcb_preempted_by_cnode(struct tcb_s *tcb, struct CNode_s *cnode);
extern int
sched_set_tcb_preempt_conf(struct tcb_s *tcb, unsigned int type);

extern struct scheduler_s *
sched_current_scheduler_of_cpu(unsigned int cpu);

extern struct scheduler_s *
scheduler_parent_of(const struct scheduler_s *scheduler);

extern cref_t
scheduler_cref_of(const struct scheduler_s *scheduler);

/* target cpu should do schedule to consume those flags */
#define IPI_FLAG_RESCHED_RANGE       	BIT_RANGE(0, 7)
#define IPI_FLAG_RESCHED		BIT(0)
#define IPI_FLAG_REPICK			BIT(1)
#define IPI_FLAG_SCHED_FUNC_RANGE	BIT_RANGE(8, 15)
#define IPI_FLAG_RELOAD_BANDWIDTH	BIT(8)
#define IPI_FLAG_DISABLE_BANDWIDTH	BIT(9)
#define IPI_FLAG_TIMER_SLACK_UNIHB	BIT(10)
#define IPI_FLAG_ASYNC_SEND_REPICK	BIT(11)
#define IPI_FLAG_DOWNGRADE		BIT(12)
#define IPI_FLAG_RECOVER		BIT(13)

extern void consume_ipi_sched_flags(struct tcb_s *tcb);
extern int force_notify_cpu_with_flag(unsigned int cpu, unsigned int flags);
extern int notify_cpu_with_flag(unsigned int cpu, unsigned int flags);
extern int notify_cpuset_with_flag(cpuset_t cpuset, unsigned int flags);

static inline int notify_cpu(unsigned int cpu)
{
	return notify_cpu_with_flag(cpu, 0U);
}

static inline int notify_cpu_resched(unsigned int cpu)
{
	return notify_cpu_with_flag(cpu, IPI_FLAG_RESCHED);
}

static inline int notify_cpu_of_tcb(struct tcb_s *tcb)
{
	int err = E_HM_OK;
	struct tcb_s *sched_tcb = tcb_schedule_tcb_of(tcb);

	if (tcb_is_running(sched_tcb)) {
		err = notify_cpu(tcb_cpu_of(sched_tcb));
	}

	return err;
}

JAM_EXTERN_INLINE u32
scheduler_task_nr_of(struct scheduler_s *scheduler);

extern bool kernel_resched_enabled(void);

/* schedule forecly */
extern struct tcb_s *schedule(struct tcb_s *curr);

#ifdef CONFIG_SCHED_INFO_DEBUG
extern void update_pcp_sched_time(unsigned int flag);
#else
static inline void update_pcp_sched_time(unsigned int flag)
{
	UNUSED(flag);
}
#endif

/* if curr is marked with resched, then re-schedule */
#define try_schedule(__curr) \
({					\
	struct tcb_s *__next;		\
	if (tcb_schedflags_tst_resched(__curr)) {	\
		update_pcp_sched_time(0);	\
		__next = schedule(__curr);	\
	} else {			\
		update_pcp_sched_time(1);	\
		__next = __curr;		\
	}				\
	__next;				\
})

extern struct tcb_s *irq_try_schedule(struct tcb_s *tcb);
void resched_curr(struct tcb_s *tcb, u32 flags);

#ifdef __HOST_LLT__
#ifdef CONFIG_KEV_TRACE_SCHED_SWITCH
extern void
llt_kev_trace_sched_switch(struct tcb_s *prev_tcb, struct tcb_s *next_tcb);
#endif
#endif

#endif
