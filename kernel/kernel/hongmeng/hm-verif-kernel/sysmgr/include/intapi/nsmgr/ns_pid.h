/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Structures and interfaces for pid namespace
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 06 14:59:25 2021
 */

#ifndef SYSMGR_INCLUDE_NSMGR_NS_PID_H
#define SYSMGR_INCLUDE_NSMGR_NS_PID_H

#include <lib/utils.h>
#include <libalgo/idr.h>
#include <intapi/nsmgr/ns_common.h>
#include <intapi/process/pid.h>

#ifndef CLONE_NEWPID
#define CLONE_NEWPID	(0x20000000)
#endif
#define PID_NS_ROOT_LEVEL 0U
#define PID_NS_MAX_LEVEL 32U
/* Used to distinguish whether to obtain the caller's ns or a specific ns */
#define CALLER_PIDNSID UINT_MAX

struct process_s;

struct pid_ns {
	struct ns_common ns_common;
	unsigned int level;
	struct pid_ns *parent;
	struct pidpool pidpool;
	struct raw_mutex reaper_lock;
	struct process_s *child_reaper;
};

static inline struct pidpool *pid_ns_pidpool_of(struct pid_ns *ns)
{
	return &ns->pidpool;
}

static inline struct idr *pid_ns_idr_of(struct pid_ns *ns)
{
	return &ns->pidpool.idr;
}

static inline int pid_ns_get(struct pid_ns *ns)
{
	return ns_common_get(&ns->ns_common);
}

static inline void pid_ns_put(struct pid_ns *ns)
{
	return ns_common_put(&ns->ns_common);
}

static inline unsigned int pid_ns_level_of(struct pid_ns *ns)
{
	return ns->level;
}

static inline int pidns_last_pid_of(struct pid_ns *ns)
{
	return idr_last_id_of(pid_ns_idr_of(ns));
}

static inline int pidns_set_last_pid(struct pid_ns *ns, int id)
{
	return pidpool_set_last_pid(pid_ns_pidpool_of(ns), id);
}

struct pid_ns *root_pid_ns(void);
struct idr *root_pidpool_idr(void);
int nstid_of_root(pid_t root_tid, int *tid_array, int len);

static inline void disable_pid_allocation(struct pid_ns *ns)
{
	raw_mutex_lock(&ns->pidpool.lock);
	ns->pidpool.alloc_flag &= ~PIDNS_ADDING;
	raw_mutex_unlock(&ns->pidpool.lock);
}
#ifdef CONFIG_PID_NS
struct ns_operation *fetch_pid_ns_op(void);
void pid_ns_kill_all_process(struct pid_ns *pidns, struct process_s *process);
struct pid_ns *get_target_pid_ns(unsigned long credential, unsigned int nsid);
int create_pid_ns(struct pid_ns *parent, unsigned int level, struct pid_ns **ns_new);
int copy_pid_ns(struct pid_ns *ns_old, struct thread_s *thread,
		unsigned int flags, struct pid_ns **ns_new);
/* returns true if the pid is the init process of the current namespace */
static inline bool is_init_of_current_pid_ns(struct pid_s *pid)
{
	return pid_s_nr_of_level(pid, pid->tid.level) == 1;
}

static inline void pidns_set_child_reaper(struct pid_ns *ns, struct process_s *child_reaper)
{
	RAW_MUTEX_GUARD(_, &ns->reaper_lock);
	ns->child_reaper = child_reaper;
}

static inline void pidns_reset_child_reaper(struct pid_ns *ns)
{
	RAW_MUTEX_GUARD(_, &ns->reaper_lock);
	ns->child_reaper = NULL;
}

struct process_s *pidns_get_child_reaper(struct pid_ns *ns);
#else

static inline struct ns_operation *fetch_pid_ns_op(void)
{
	return NULL;
}

static inline bool is_init_of_current_pid_ns(struct pid_s *pid)
{
	UNUSED(pid);
	return false;
}

static inline void pid_ns_kill_all_process(struct pid_ns *pidns, struct process_s *process)
{
	UNUSED(pidns, process);
	return;
}

static inline int copy_pid_ns(struct pid_ns *ns_old, struct thread_s *thread,
			      unsigned int flags, struct pid_ns **ns_new)
{
	int err = E_HM_OK;

	if ((flags & (unsigned int)CLONE_NEWPID) != 0U) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		*ns_new = ns_old;
		int rc = pid_ns_get(ns_old);
		BUG_ON(rc <= 0);
	}

	return err;
}

static inline void pidns_set_child_reaper(struct pid_ns *ns, struct process_s *child_reaper)
{
	UNUSED(ns, child_reaper);
	return;
}

static inline void pidns_reset_child_reaper(struct pid_ns *ns)
{
	UNUSED(ns);
	return;
}

static inline struct process_s *pidns_get_child_reaper(struct pid_ns *ns)
{
	UNUSED(ns);
	return NULL;
}

#endif
static inline struct pid_ns *common_to_pid_ns(struct ns_common *ns)
{
	struct pid_ns *pid_ns = NULL;

	if (ns->op == fetch_pid_ns_op()) {
		pid_ns =  container_of(ns, struct pid_ns, ns_common);
	}
	return pid_ns;
}
#endif
