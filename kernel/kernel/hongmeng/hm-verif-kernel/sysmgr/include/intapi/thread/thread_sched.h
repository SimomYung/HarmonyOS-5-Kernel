/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Thread interface related to scheduling attributes
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 11 10:44:11 2020
 */
#ifndef H_SYSMGR_INCLUDE_THREAD_SCHED_H
#define H_SYSMGR_INCLUDE_THREAD_SCHED_H

#include <hmkernel/cpuset.h>
#include <hmkernel/sched_module.h>
#include <hmkernel/sched/priority.h>
#include <intapi/thread/thread_ops.h>
#include <intapi/process/process.h>
#include <intapi/thread/thread.h>
#include <time.h>
#include <hmsched.h>

#define SCHED_RT_EX 0xff

#define SCHED_ATTR_SIZE_VER0 48
#define SCHED_ATTR_SIZE_VER1 56

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define INVALID_UID ((uid_t)-1)

struct __thread_sched_stat_s {
	unsigned int cpu;
	uint64_t utime;
	uint64_t stime;
	uint64_t cutime;
	uint64_t cstime;
#ifdef CONFIG_PROCESS_SCHEDSTAT
	/* time spent on the cpu */
	uint64_t exec_time;
	/* time spent waiting on a runqueue */
	uint64_t run_delay;
	/* Total online counts of the thread */
	uint64_t pcount;
#endif
	uint32_t tcb_state;
	int rt_priority;
	int priority;
	int sched_policy;
	int nice;
};

struct thread_sched_ops_s {
	int (*setparam)(struct process_s *process, struct thread_s *thread, int prio);
	int (*setpriority)(struct thread_s *thread, struct process_s *process, int nice);
	int (*setscheduler)(struct thread_s *thread, struct process_s *process, int prio, int policy);

	int (*setaffinity_weaksec)(struct thread_s *thread, __cpuset_t cpuset);
	int (*getaffinity)(struct thread_s *thread, __cpuset_t *cpuset);

	int (*amend)(struct process_s *process,
		     struct thread_s *parent,
		     struct thread_s *new_thread,
		     const __cpuset_t *cpuset);
	int (*setaffinity_direct)(struct thread_s *thread, __cpuset_t cpuset);
	int (*set_lt_conf)(struct thread_s *thread, unsigned int flag,
			   struct sched_lt_config_s *configs,
			   struct deadline_args_s *dl_args);
};

struct sched_param_info_s {
	uid_t uid;
	int nice;
	int ret;
	bool is_uspinhdlr;
	unsigned long credential;
};

extern const thread_sched_ops_t klt_sched_ops;

int thread_sched_setparam(struct thread_s *thread, int prio);
int thread_sched_getparam(struct thread_s *thread);
int thread_setpriority(unsigned long long sender, struct thread_s *thread, int nice);
int thread_getpriority(struct thread_s *thread);
int thread_set_runqueue(struct thread_s *thread, unsigned int rq);
int thread_sched_setscheduler(unsigned long long sender, struct thread_s *thread,
			      int prio, int policy);
int thread_sched_setscheduler_nochk(unsigned long long sender, struct thread_s *thread,
			      int prio, int policy);
int thread_sched_getscheduler(unsigned long long sender, struct thread_s *thread);
int thread_sched_getscheduler_nochk(unsigned long long sender, struct thread_s *thread);
int thread_sched_get_avail_prio_max(struct thread_s *thread);
int thread_affinity_check(__cpuset_t *hmcpuset, unsigned int affinity);
int thread_sched_setaffinity(struct thread_s *thread, __cpuset_t cpuset);
int thread_sched_setaffinity_weaksec(struct thread_s *thread, __cpuset_t cpuset);
int thread_sched_getaffinity(struct thread_s *thread, __cpuset_t *cpuset);
int thread_sched_getaffinity_no_chk(struct thread_s *thread, __cpuset_t *cpuset);
int thread_sched_get_interval(const struct thread_s *thread, int policy, struct timespec *ts);
int thread_sched_setattr(unsigned long long sender, struct thread_s *thread,
			 struct sched_attr *attr);
int thread_sched_setattr_nochk(unsigned long long sender, struct thread_s *thread,
			       struct sched_attr *attr);
int thread_sched_setattr_uclamp(struct thread_s *thread, const struct sched_attr *attr);
int thread_sched_getattr(unsigned long long sender, struct thread_s *thread, struct sched_attr *attr);
int thread_check_sched_setscheduler(int policy, int *new_policy);
int thread_sched_amend(struct process_s *process,
		       struct thread_s *parent,
		       struct thread_s *new_thread,
		       const __cpuset_t *cpuset);

int thread_sched_borrow_getaffinity(const struct process_s *borrower, __cpuset_t *cpuset);
int thread_sched_borrow_setaffinity(const struct process_s *borrower, __cpuset_t cpuset);
int thread_sched_setaffinity_direct(struct thread_s *thread, __cpuset_t cpuset);
int thread_sched_trans(struct thread_s *thread, cref_t src_cref, cref_t dst_cref,
		       bool restore);

int thread_sched_set_lt_conf(struct thread_s *thread, unsigned int flag,
			     struct sched_lt_config_s *configs);
int thread_sched_set_lt_group_conf(unsigned int flag, struct sched_lt_config_s *configs);
int thread_sched_set_lt_deadline(struct thread_s *thread, struct deadline_args_s *args);
int query_thread_stat(struct thread_s *thread, struct __thread_sched_stat_s *thread_sched_stat);
int thread_proc_stat_of(struct thread_s *thread, struct sysarg_thread_stat *stat);
int thread_sched_stat_transfer(struct process_s *process, struct proc_sched_stat_s *sched_stat, int num);
int thread_all_thread_sched_stat(struct proc_sched_stat_s *thread_sched, int num);
bool check_service_nonself_ops(unsigned long cred, const struct thread_s *target_thread);
bool check_service_self_ops(unsigned long cred, const struct thread_s *target_thread);
int sched_attr_size_check(char *uattr, const struct sched_attr *attr);
int thread_setpriority_by_tid(unsigned long credential, int tid, int nice);
int thread_getpriority_by_tid(int tid);
int thread_sched_setparam_by_tid(unsigned long credential, int tid, int prio);
int thread_sched_getparam_by_tid(int tid);
int thread_sched_setscheduler_by_tid(unsigned long credential, int tid, int prio, int policy);
int thread_sched_getscheduler_by_tid(int tid);
int thread_sched_setattr_by_tid(unsigned long long sender, unsigned long credential,
				int tid, struct sched_attr *attr);
int thread_sched_getattr_by_tid(unsigned long long sender, int tid, struct sched_attr *attr);
int thread_update_scheduler(struct thread_s *thread, int policy, int prio);
int thread_sched_extend_rt_prio(struct thread_s *thread, int prio,
				bool is_heavy_weight, bool is_restricted_rt);
int thread_sched_get_restricted_rt_state(struct thread_s *thread, int *res_rt_state);
void thread_sched_update_ex_attr(struct thread_s *thread, int *policy, int *prio, bool is_vip);
void thread_sched_save_rtprio(struct thread_s *thread, int rt_prio);
int thread_setpriority_entry(unsigned long credential, int which, int tid, int nice, bool is_uspinhdlr);
int thread_getpriority_entry(unsigned long credential, int which, int tid, bool is_uspinhdlr);
void thread_sched_update_fork_attr(struct thread_s *thread,
				   struct thread_s *parent,
				   const struct thread_attr *attr,
				   struct sysarg_thread_config *config);

static inline int
real_policy(int policy)
{
	return (int)((unsigned int)policy & (~__U(SCHED_RESET_ON_FORK)));
}
int thread_set_min_util(struct thread_s *thread, unsigned int util);

int thread_set_load_trans(struct thread_s *thread, unsigned int load_trans);

#ifdef CONFIG_THREAD_FINE_LOAD
int thread_sched_set_domain(struct thread_s *thread, int domain);
int thread_sched_get_domain(struct thread_s *thread, int *domain);
#else
static inline int thread_sched_set_domain(struct thread_s *thread, int domain)
{
	UNUSED(thread, domain);
	return E_HM_NOSYS;
}

static inline int thread_sched_get_domain(struct thread_s *thread, int *domain)
{
	UNUSED(thread, domain);
	return E_HM_NOSYS;
}
#endif
int thread_sched_set_boost(struct thread_s *thread, int boost);

#ifdef CONFIG_CORE_SCHEDULING
int thread_sched_set_smt_type(struct thread_s *thread, unsigned int type);
int thread_sched_get_smt_type(struct thread_s *thread, unsigned int *type);
#else
static inline int thread_sched_get_smt_type(struct thread_s *thread, unsigned int *type)
{
	UNUSED(thread, type);
	return 0;
}
#endif

#ifdef CONFIG_HISI_PERF_CTRL
unsigned int cpu_max_capacity_level(void);
__cpuset_t cpumask_of_capacity_level(unsigned int level);
#else
static inline unsigned int cpu_max_capacity_level(void)
{
	return 0;
}
static inline __cpuset_t cpumask_of_capacity_level(unsigned int level)
{
	UNUSED(level);
	__cpuset_t ret = {0};
	return ret;
}
#endif

#endif
