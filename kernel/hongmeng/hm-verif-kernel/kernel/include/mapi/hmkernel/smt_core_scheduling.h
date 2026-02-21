/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Definition of module operations
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 30 10:00:31 2024
 */

#ifndef MAPI_HMKERNEL_SMT_CORE_SCHEDULING_H
#define MAPI_HMKERNEL_SMT_CORE_SCHEDULING_H

#include <hmkernel/strict.h>
#include <hmkernel/tcb.h>
#include <hmkernel/sched/sched.h>

enum core_state {
	CORE_IDLE = 0,
	THREAD_IDLE,
	FORCE_IDLE,
	SMT,
	NO_IDLE,
};

enum cookie_match_state {
	MATCH,
	NOT_MATCH,
	NEVER_MATCH,
};
#ifdef CONFIG_CORE_SCHEDULING
struct core_sched_s {
	struct rec_spinlock_s curr_lock;
	struct rec_spinlock_s state_lock;
	cpuset_t need_resched_mask;
	enum core_state stat;
};

void core_lock(unsigned int cpuid);
void core_unlock(unsigned int cpuid);
void core_state_lock(unsigned int cpuid);
void core_state_unlock(unsigned int cpuid);
bool do_smt_expeller_trans(struct tcb_s *src_tcb, struct tcb_s *des_tcb,
			   bool restore, enum sched_trans_method method);
void sched_core_prefer_cookie_timeout_init(void);
bool core_sched_enable(void);
struct tcb_s *core_sched_pick_next_hook(unsigned int cpu);
int sched_coresched_set_conf(struct capcall_info_s *callinfo,
			     const void __user *args, size_t args_size,
			     void __user *result, size_t result_size);
enum cookie_match_state core_sched_check_cookie_match(const struct tcb_s *tcb, unsigned int cpu, bool need_mark);
void mark_cpu_forceidle(unsigned int cpuid, bool is_forceidle);
bool cpu_is_forceidle(unsigned int cpuid);
cpuset_t core_sched_get_idle_cpuset(const struct tcb_s *tcb);
unsigned int task_smt_expeller(const struct tcb_s *task);
enum core_state sched_get_core_state(unsigned int cpu);
enum core_state sched_get_core_state_ahead(unsigned int cpu);
int core_sched_task_abs_prio(const struct tcb_s *task);
void enqueue_prefer_cookie(struct tcb_s *tcb);
void prefer_cookie_sched_switch(struct tcb_s *prev_tcb, struct tcb_s *next_tcb, unsigned int cpu);
bool match_prefer_cookie(u64 prefer_cookie, unsigned int cpu, bool strict);
cpuset_t core_sched_get_prefer_cookie_wanted_cpus(struct tcb_s *tcb, cpuset_t prefer_mask, cpuset_t allowed_mask);
bool core_sched_prefer_cookie_allowed(struct tcb_s *tcb, unsigned int cpu);
int smt_set_expeller_util_threshold(unsigned int util_threshold);
int smt_get_expeller_util_threshold(void);

static inline u64 tcb_secure_cookie_of(const struct tcb_s *tcb)
{
	return tcb->secure_cookie;
}

static inline u64 tcb_prefer_cookie_of(const struct tcb_s *tcb)
{
	return tcb->prefer_cookie;
}

void tcb_clear_force_expeller(struct tcb_s *tcb);
void force_smt_expeller_prepare(void);
#else
static inline void core_lock(unsigned int cpuid)
{
	UNUSED(cpuid);
}

static inline void core_unlock(unsigned int cpuid)
{
	UNUSED(cpuid);
}

static inline void core_state_lock(unsigned int cpuid)
{
	UNUSED(cpuid);
}

static inline void core_state_unlock(unsigned int cpuid)
{
	UNUSED(cpuid);
}

static inline bool do_smt_expeller_trans(struct tcb_s *src_tcb, struct tcb_s *des_tcb,
				         bool restore, enum sched_trans_method method)
{
	UNUSED(src_tcb, des_tcb, restore, method);
	return false;
}

static inline enum cookie_match_state core_sched_check_cookie_match(const struct tcb_s *tcb,
                                                                    unsigned int cpu, bool need_mark)
{
	UNUSED(tcb, cpu, need_mark);
	return MATCH;
}

static inline void mark_cpu_forceidle(unsigned int cpuid, bool is_forceidle)
{
	UNUSED(cpuid, is_forceidle);
	return;
}

static inline bool cpu_is_forceidle(unsigned int cpuid)
{
	UNUSED(cpuid);
	return false;
}

cpuset_t get_idle_cpuset(void);
static inline cpuset_t core_sched_get_idle_cpuset(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return get_idle_cpuset();
}

static inline unsigned int task_smt_expeller(struct tcb_s *task)
{
	UNUSED(task);
	return 0;
}

static inline enum core_state sched_get_core_state(unsigned int cpu)
{
	UNUSED(cpu);
	return FORCE_IDLE;
}

static inline enum core_state sched_get_core_state_ahead(unsigned int cpu)
{
	UNUSED(cpu);
	return FORCE_IDLE;
}

static inline int core_sched_task_abs_prio(const struct tcb_s *tcb)
{
	return tcb ? sched_tcb_get_prio(tcb) : (-1);
}

static inline void enqueue_prefer_cookie(struct tcb_s *tcb)
{
	UNUSED(tcb);
}

static inline void prefer_cookie_sched_switch(struct tcb_s *prev_tcb,
					      struct tcb_s *next_tcb, unsigned int cpu)
{
	UNUSED(prev_tcb, next_tcb, cpu);
}

static inline bool match_prefer_cookie(u64 prefer_cookie, unsigned int cpu, bool strict)
{
	UNUSED(prefer_cookie, cpu, strict);
	return false;
}

static inline cpuset_t core_sched_get_prefer_cookie_wanted_cpus(struct tcb_s *tcb,
							        cpuset_t prefer_mask, cpuset_t allowed_mask)
{
	UNUSED(tcb, allowed_mask);
	return prefer_mask;
}

static inline bool core_sched_prefer_cookie_allowed(struct tcb_s *tcb, unsigned int cpu)
{
	UNUSED(tcb, cpu);
	return true;
}

static inline u64 tcb_secure_cookie_of(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return -1;
}

static inline u64 tcb_prefer_cookie_of(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return -1;
}

static inline void tcb_clear_force_expeller(struct tcb_s *tcb)
{
	UNUSED(tcb);
	return;
}

static inline void force_smt_expeller_prepare(void)
{
	return;
}

#endif

#endif
