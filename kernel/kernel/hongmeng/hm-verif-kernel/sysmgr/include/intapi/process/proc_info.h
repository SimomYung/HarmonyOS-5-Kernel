/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition for information of process
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 10 20:17:55 2020
 */
#ifndef SYSMGR_INTAPI_PROCESS_PROCINFO_H
#define SYSMGR_INTAPI_PROCESS_PROCINFO_H

#ifndef SYSMGR_INTAPI_PROCESS_PROCESS_H
# error "Not for directly included by C file. Include intapi/process/process.h instead."
#endif

#include <libhmactv/actv.h>
#include <intapi/process/proc_core.h>
#include <intapi/nsmgr/ns_pid.h>

struct process_stat_s {
	uint64_t minflt;
	uint64_t majflt;
	uint64_t utime;
	uint64_t stime;
	uint64_t cutime;
	uint64_t cstime;
	int rt_priority;
	int sched_policy;
	unsigned int processor;
	int nice;
#ifdef CONFIG_PROCESS_SCHEDSTAT
	uint64_t exec_time;
	uint64_t run_delay;
	uint64_t pcount;
#endif
};

static inline char *
process_name_of(const struct process_s *process)
{
	return process->loader.target.name;
}

static inline struct raw_rwlock *
process_name_lock_of(struct process_s *process)
{
	return &process->pname_lock;
}

/*
 * 1. process_pid_of(): Get a global PID of process in root pid ns.
 * 2. process_vpid_of(): Get a virtual PID of process in the ns where the process is located.
 * 3. process_pid_in_pidns(): Get a PID of process in a specified ns.
 */
static inline pid_t
process_pid_of(const struct process_s *process)
{
	struct pid_s *pid = process->pid_link.pid[PIDTYPE_PID];
	return pid == NULL ? PID_NONE : pid_s_nr_of_level(pid, root_pid_ns()->level);
}

static inline pid_t
process_vpid_of(const struct process_s *process)
{
	struct pid_s *pid = process->pid_link.pid[PIDTYPE_PID];
	return pid == NULL ? PID_NONE : pid_s_nr_of(pid);
}

static inline pid_t pid_in_pidns(struct pid_s *pid, struct pid_ns *ns)
{
	unsigned int level = pid_ns_level_of(ns);
	pid_t ret = PID_NONE;

	if (pid != NULL) {
		if (level <= pid_s_level_of(pid) &&
		    tid_ns_of_level(&pid->tid, level) == ns) {
			ret = tid_s_nr_of_level(&pid->tid, level);
		}
	}
	return ret;
}

static inline pid_t
process_pid_in_pidns(const struct process_s *process, struct pid_ns *ns)
{
	struct pid_s *pid = process->pid_link.pid[PIDTYPE_PID];

	return pid_in_pidns(pid, ns);
}

static inline pid_t
process_pgid_of(const struct process_s *process)
{
	struct pid_s *pgid = process->pid_link.pid[PIDTYPE_PGID];
	return pgid == NULL ? PID_NONE : pid_s_nr_of(pgid);
}

static inline pid_t
process_pgid_in_pidns(const struct process_s *process, struct pid_ns *ns)
{
	pid_t ret;
	struct pid_s *pgid = process->pid_link.pid[PIDTYPE_PGID];

	ret = pid_in_pidns(pgid, ns);
	if (ret == PID_NONE) {
		ret = 0;
	}

	return ret;
}

static inline pid_t
process_pgid_in_pidns_with_lock(struct process_s *process, struct pid_ns *ns)
{
	RAW_MUTEX_GUARD(_, &process->pid_link.mutex);
	return process_pgid_in_pidns(process, ns);
}

static inline pid_t
process_sid_of(const struct process_s *process)
{
	struct pid_s *sid = process->pid_link.pid[PIDTYPE_SID];
	return sid == NULL ? PID_NONE : pid_s_nr_of(sid);
}

static inline pid_t
process_sid_in_pidns(const struct process_s *process, struct pid_ns *ns)
{
	pid_t ret;
	struct pid_s *sid = process->pid_link.pid[PIDTYPE_SID];

	ret = pid_in_pidns(sid, ns);
	if (ret == PID_NONE) {
		ret = 0;
	}

	return ret;
}

static inline pid_t
process_sid_in_pidns_with_lock(struct process_s *process, struct pid_ns *ns)
{
	RAW_MUTEX_GUARD(_, &process->pid_link.mutex);
	return process_sid_in_pidns(process, ns);
}

static inline struct pid_s *
process_pid_s_of(const struct process_s *process, enum pid_type type)
{
	return process->pid_link.pid[type];
}

static inline rref_t
process_module_sched_rref_of(const struct process_s *process)
{
	return process->module_sched_rref;
}

static inline struct pid_link_s *
process_pid_link_of(struct process_s *process)
{
	return &process->pid_link;
}

static inline struct raw_mutex *
process_pid_link_mutex(struct process_s *process)
{
	return &process->pid_link.mutex;
}

static inline struct raw_mutex *
process_sid_mutex(const struct process_s *process)
{
	return &process->pid_link.pid[PIDTYPE_SID]->mutex;
}

static inline struct tid_s *
process_tid_s_of(const struct process_s *process)
{
	return &process->pid_link.pid[PIDTYPE_PID]->tid;
}

static inline bool
process_is_in_same_session(struct process_s *proc1, struct process_s *proc2)
{
	bool res = false;
	struct raw_mutex *mutex1 = process_pid_link_mutex(proc1);
	struct raw_mutex *mutex2 = process_pid_link_mutex(proc2);
	raw_mutex_lock_pair(mutex1, mutex2);
	res = ((uintptr_t)process_pid_s_of(proc1, PIDTYPE_SID) ==
	       (uintptr_t)process_pid_s_of(proc2, PIDTYPE_SID));
	raw_mutex_unlock_pair(mutex1, mutex2);
	return res;
}

static inline int process_exit_status_of(const struct process_s *process)
{
	return process->threads.exit_status;
}

static inline struct process_resource* process_resource_of(struct process_s *process)
{
	return (process == NULL) ? NULL : &process->resource;
}

static inline struct raw_refcnt* process_refcnt_of(struct process_s *process)
{
	return &process->refcnt;
}

static inline struct raw_refcnt_nr *
process_resource_refcnt_of(struct process_resource *resource)
{
	return &resource->refcnt_nr;
}

static inline struct wait_queue_head* process_waiter_of(struct process_s *process)
{
	return &process->wait_list;
}

static inline struct process_s* refcnt_to_process(struct raw_refcnt *ref)
{
	return container_of(ref, struct process_s, refcnt);
}

static inline struct process_resource *
refcnt_to_process_resource(struct raw_refcnt_nr *refcnt_nr)
{
	return container_of(refcnt_nr, struct process_resource, refcnt_nr);
}

static inline void process_set_vspace(struct process_s *process,
				      struct vspace_s *vspace)
{
	process->resource.vspace = vspace;
}

#ifdef CONFIG_ACTV_BINDER
static inline struct process_actv_binder_s *
process_actv_binder_of(struct process_s *process)
{
	return &process->resource.actv_binder;
}
#endif /* CONFIG_ACTV_BINDER */

#ifdef CONFIG_HYPERVISOR
static inline bool process_has_vm(struct process_s *process)
{
	bool ret = false;

	raw_mutex_lock(&(process->mutex));
	ret = (process->resource.vm != NULL);
	raw_mutex_unlock(&(process->mutex));

	return ret;
}

static inline void process_delete_vm(struct process_s *process)
{
	struct process_vm_s *vm = NULL;

	raw_mutex_lock(&process->mutex);
	if (process->resource.vm != NULL) {
		vm = process->resource.vm;

		/* detach vm from process.resource */
		process->resource.vm = NULL;
	}
	raw_mutex_unlock(&process->mutex);

	/* put the raw refcnt of vm */
	process_put_vm(vm);
}
#else
static inline bool process_has_vm(struct process_s *process)
{
	UNUSED(process);
	return false;
}

static inline void process_delete_vm(struct process_s *process)
{
	UNUSED(process);
}
#endif

static inline void
process_set_cnode_rref(struct process_s *process, rref_t cnode_rref)
{
	process->resource.cnode_rref = cnode_rref;
}

static inline struct raw_mutex *
process_mutex(struct process_s *process)
{
	return &process->mutex;
}

static inline struct fids *
process_fids_of(struct process_s *process)
{
	return &process->resource.fids;
}

/* Don't use this interface directly, be caution */
static inline struct vspace_s *
process_vspace_of(const struct process_s *process)
{
	return process->resource.vspace;
}

static inline unsigned long long
process_pmem_size_of(struct process_s *process)
{
	return raw_atomic_ullong_read_relaxed(&process->pmem_size);
}

/* Don't use this interface directly, be caution */
static inline rref_t
process_cnode_rref_of(const struct process_s *process)
{
	return process->resource.cnode_rref;
}

static inline void process_lock(struct process_s *process)
{
	raw_mutex_lock(&process->mutex);
}

static inline void process_unlock(struct process_s *process)
{
	raw_mutex_unlock(&process->mutex);
}

static inline int
process_oom_score_adj_of(struct process_s *process)
{
	return raw_atomic_int_read_relaxed(&process->oom_score_adj);
}

static inline int
process_oom_score_adj_min_of(struct process_s *process)
{
	return raw_atomic_int_read_relaxed(&process->oom_score_adj_min);
}

static inline void
process_set_oom_score_adj(struct process_s *process, int adj)
{
	(void)raw_atomic_int_set_relaxed(&process->oom_score_adj, adj);
}

static inline void
process_set_oom_score_adj_min(struct process_s *process, int adj_min)
{
	(void)raw_atomic_int_set_relaxed(&process->oom_score_adj_min, adj_min);
}

static inline unsigned int
process_detect_hang_of_ready_task_of(const struct process_s *process)
{
	return process->hangtask.detect_hang_of_ready_task;
}

static inline void
process_set_detect_hang_of_ready_task(struct process_s *process, unsigned int value)
{
	process->hangtask.detect_hang_of_ready_task = value;
}

static inline unsigned int
process_detect_hang_of_lock_task_of(const struct process_s *process)
{
	return process->hangtask.detect_hang_of_lock_task;
}

static inline void
process_set_detect_hang_of_lock_task(struct process_s *process, unsigned int value)
{
	process->hangtask.detect_hang_of_lock_task = value;
}

static inline void
process_set_flag(struct process_s *process,
		      unsigned int flag)
{
	process_lock(process);
	process->flags |= flag;
	process_unlock(process);
}

static inline int
process_get(struct process_s *process)
{
	struct raw_refcnt *refcnt;
	refcnt = process_refcnt_of(process);
	return raw_refcnt_get(refcnt);
}

static inline enum process_state
process_state_of(struct process_s *process)
{
	enum process_state state;

	raw_mutex_lock(&process->mutex);
	state = process->state;
	raw_mutex_unlock(&process->mutex);

	return state;
}

static inline void
process_set_state(struct process_s *process, enum process_state state)
{
	raw_mutex_lock(&process->mutex);
	process->state = state;
	raw_mutex_unlock(&process->mutex);
}

static inline struct process_loader *
process_loader_of(struct process_s *process)
{
	return &process->loader;
}

static inline struct process_s *
loader_process_of(const struct process_loader *loader)
{
	return loader->process;
}

static inline uintptr_t
process_interp_load_offset(struct vspace_layout *layout)
{
	return layout->interp_base;
}

static inline uint64_t
process_dso_sym_info_of(struct process_s *process)
{
	return process->dso_sym_info;
}

static inline struct security_s *
process_security_of(struct process_s *process)
{
	return &process->security;
}

/*
 * process_put maybe release the process structure.
 *
 * so must make sure:
 * don't use any attribute in process(process->xxx) after process_put called.
 */
static inline void
process_put(struct process_s *process)
{
	struct raw_refcnt *refcnt;
	refcnt = process_refcnt_of(process);
	(void)raw_refcnt_put(refcnt);
}

static inline int
process_resource_get(struct process_resource *resource)
{
	struct raw_refcnt_nr *refcnt_nr;
	refcnt_nr = process_resource_refcnt_of(resource);
	return raw_refcnt_nr_get(refcnt_nr);
}

static inline void
__resource_cnode_release(struct process_cnode_ref *ref)
{
	if (ref->resource != NULL && !IS_REF_ERR(ref->cnode_rref)) {
		process_resource_put(ref->resource);
	}
}

void process_vspace_put(struct vspace_s *vspace);

static inline void
__resource_vspace_release(struct process_vspace_ref *ref)
{
	if (ref->vspace != NULL) {
		process_vspace_put(ref->vspace);
	}
}

#ifdef CONFIG_SYSMGR_PM_SUSPEND
static inline enum proc_pm_attr
process_pm_attr_of(struct process_s *process)
{
	return process->pm_attr;
}

static inline void
process_set_pm_attr(struct process_s *process, enum proc_pm_attr attr)
{
	process->pm_attr = attr;
}
#else
static inline enum proc_pm_attr
process_pm_attr_of(struct process_s *process)
{
	return PM_ATTR_NONE;
}

static inline void
process_set_pm_attr(struct process_s *process, enum proc_pm_attr attr)
{
}
#endif

#ifdef CONFIG_SUBREAPER
static inline bool
process_has_child_subreaper(struct process_s *process)
{
	bool has_child_subreaper = false;

	raw_mutex_lock(&process->mutex);
	if (process->child_subreaper & HAS_CHILD_SUBREAPER) {
		has_child_subreaper = true;
	}
	raw_mutex_unlock(&process->mutex);

	return has_child_subreaper;
}

static inline void
process_set_has_child_subreaper_nolock(struct process_s *process, bool has_child_subreaper)
{
	if (has_child_subreaper) {
		process->child_subreaper |= HAS_CHILD_SUBREAPER;
	} else {
		process->child_subreaper &= ~(HAS_CHILD_SUBREAPER);
	}
}

static inline void
process_set_has_child_subreaper(struct process_s *process, bool has_child_subreaper)
{
	raw_mutex_lock(&process->mutex);
	process_set_has_child_subreaper_nolock(process, has_child_subreaper);
	raw_mutex_unlock(&process->mutex);
}

static inline bool
process_is_child_subreaper(struct process_s *process)
{
	bool is_child_subreaper = false;

	raw_mutex_lock(&process->mutex);
	if (process->child_subreaper & IS_CHILD_SUBREAPER) {
		is_child_subreaper = true;
	}
	raw_mutex_unlock(&process->mutex);

	return is_child_subreaper;
}

static inline void
process_set_is_child_subreaper(struct process_s *process, bool is_child_subreaper)
{
	raw_mutex_lock(&process->mutex);
	if (is_child_subreaper) {
		process->child_subreaper |= IS_CHILD_SUBREAPER;
	} else {
		process->child_subreaper &= ~(IS_CHILD_SUBREAPER);
	}
	raw_mutex_unlock(&process->mutex);
}
#endif

/* must be called in threads.mutex lock */
static inline void
__process_set_thread_group_flag(struct process_threads *threads,
				unsigned int flag)
{
	threads->flags |= flag;
}

static inline void
__process_clear_thread_group_flag(struct process_threads *threads,
				unsigned int flag)
{
	threads->flags &= (~flag);
}

static inline bool
__process_thread_group_in_exec(const struct process_threads *threads)
{
	return (threads->flags & THREAD_GROUP_EXEC) != 0U;
}

static inline bool
__process_thread_group_in_exit(const struct process_threads *threads)
{
	return (threads->flags & THREAD_GROUP_EXIT) != 0U;
}

static inline void
__process_set_group_exit_code(struct process_threads *threads, int exit_status)
{
	threads->exit_status = exit_status;
}

static inline bool
process_thread_group_in_exec(struct process_s *process)
{
	struct process_threads *threads = &process->threads;

	RAW_MUTEX_GUARD(_, &threads->mutex);
	return __process_thread_group_in_exec(threads);
}

static inline bool
process_thread_group_in_exit(struct process_s *process)
{
	struct process_threads *threads = &process->threads;

	RAW_MUTEX_GUARD(_, &threads->mutex);
	return __process_thread_group_in_exit(threads);
}

static inline unsigned int
process_exit_signal_of(const struct process_s *process)
{
	return process->exit_signal;
}

static inline uint32_t process_cnode_idx_of(const struct process_s *process)
{
	uint32_t cnode_idx = 0U;

	if (process == NULL) {
		hm_warn("invalid argument\n");
	} else {
		cnode_idx = process->cnode_idx;
	}
	return cnode_idx;
}

static inline __cpuset_t process_default_affinity_of(const struct process_s *process)
{
	return process->default_affinity;
}

static inline int
process_path_cnt_of(struct process_s *process)
{
	return raw_atomic_int_read(&process->path_cnt);
}

static inline int
process_path_cnt_inc(struct process_s *process)
{
	return raw_atomic_int_inc(&process->path_cnt);
}

static inline int
process_path_cnt_dec(struct process_s *process)
{
	return raw_atomic_int_dec(&process->path_cnt);
}

#ifdef CONFIG_TSM_COMMON
static inline struct process_ulwtinfo_s *process_ulwtinfo_of(struct process_s *process)
{
	return process->ulwtinfo;
}

static inline void process_set_ulwtinfo(struct process_s *process, struct process_ulwtinfo_s *ulwtinfo)
{
	process->ulwtinfo = ulwtinfo;
}
#endif /* CONFIG_TSM_COMMON */

static inline bool process_is_sysctrl_granted(struct process_s *process)
{
	return (process->sysctrl_rref != 0ULL);
}

static inline void process_set_sysctrl_granted(struct process_s *process, rref_t rref)
{
	process->sysctrl_rref = rref;
}

static inline void process_clean_sysctrl_granted(struct process_s *process)
{
	process->sysctrl_rref = 0ULL;
}

static inline bool current_process_is_killed(void)
{
	return (hm_actv_local_get_raising_hmsignals() & (1 << HMSIGNAL_KILL)) != 0U;
}

/* We need call procmgr_exit_oom after this function */
static inline bool process_exit_check_and_set(struct process_s *process)
{
	return (process_exit_status_of(process) != SIGKILL) &&
	       (vatomic8_cmpxchg(&process->is_oom_killed, 0, 1) == 0);
}

static inline bool process_is_oom_killed(struct process_s *process)
{
	return vatomic8_read_rlx(&process->is_oom_killed) == 1;
}

int query_process_stat(struct process_s *process, struct process_stat_s *proc_stat);
#endif
