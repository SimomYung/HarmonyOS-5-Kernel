/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of resmgr interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 22 15:11:26 2019
 */

#ifndef SYSMGR_RES_H
#define SYSMGR_RES_H

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/compiler.h>
#include <hmkernel/sched_module.h>
#include <intapi/thread/thread.h>
#include <libstrict/strict.h>
#include <intapi/res/res_def.h>

#ifdef CONFIG_RESMGR

extern void *resmgr_get_root(void);
extern int resmgr_pre_attach(struct process_s *process, struct thread_s *thread);
extern void resmgr_post_attach(struct process_s *process, struct thread_s *thread);
extern int resmgr_add_process(struct process_s *parent, struct process_s *child);
extern void resmgr_remove_process(struct process_s *process);
extern void resmgr_add_thread(struct thread_s *parent, struct thread_s *thread);
extern void resmgr_remove_thread(struct thread_s *thread);
extern int resmgr_init(void);
extern int resmgr_post_init(void);

int resmgr_path_from_thread(struct thread_s *thread, char *buf, size_t buf_len);
int resmgr_path_from_thread_try(struct thread_s *thread, char *buf, size_t buf_len);
int resmgr_path_from_thread_cpuset(struct thread_s *thread, char *buf, size_t buf_len, bool sfx);

enum resmgr_interate_flag {
	RESMGR_ITERATE_CONTINUE, /* continue to iterate res group node */
	RESMGR_ITERATE_BREAK,	 /* stop to iterate res group node */
};

extern int resmgr_do_each_node_preorder(struct res_group_node *root,
	enum resmgr_interate_flag (*iterate_callback)(
		struct res_group_node *curr_node, void *arg),
	void *arg);

extern int resmgr_do_each_node_postorder(struct res_group_node *root,
	enum resmgr_interate_flag (*iterate_callback)(
		struct res_group_node *curr_node, void *arg),
	void *arg);

extern int resmgr_do_each_child(struct res_group_node *root,
	enum resmgr_interate_flag (*iterate_callback)(
		struct res_group_node *curr_node, void *arg),
	void *arg);

#else
static inline int resmgr_pre_attach(struct process_s *process, struct thread_s *thread)
{
	UNUSED(process);
	UNUSED(thread);
	return E_HM_OK;
}
static inline void resmgr_post_attach(struct process_s *process, struct thread_s *thread)
{
	UNUSED(process);
	UNUSED(thread);
}
static inline int resmgr_add_process(struct process_s *parent, struct process_s *child)
{
	UNUSED(parent);
	UNUSED(child);
	return E_HM_OK;
}
static inline void resmgr_remove_process(struct process_s *process)
{
	UNUSED(process);
}
static inline void resmgr_add_thread(struct thread_s *parent, struct thread_s *thread)
{
	UNUSED(parent, thread);
}
static inline void resmgr_remove_thread(struct thread_s *thread)
{
	UNUSED(thread);
}
static inline int resmgr_init(void)
{
	return E_HM_OK;
}

static inline void *resmgr_get_root(void)
{
	return NULL;
}
#endif /* CONFIG_RESMGR */

#ifdef CONFIG_RESMGR_LOAD
__u32 resmgr_thread_load_group_of(const struct thread_s *thread);
bool resmgr_thread_load_inherit_nice(const struct thread_s *thread);
#else
__maybe_unused
static __u32 resmgr_thread_load_group_of(const struct thread_s *thread)
{
	UNUSED(thread);
	return (__u32)DEADLINE_INVALID_GROUP_ID;
}

static inline bool
resmgr_thread_load_inherit_nice(const struct thread_s *thread)
{
	UNUSED(thread);
	return false;
}
#endif /* CONFIG_RESMGR_LOAD */

#ifdef CONFIG_RESMGR_CPUSET
extern void resmgr_thread_cpuset_of(const struct thread_s *thread, __cpuset_t *cpumask);
extern void resmgr_cpuset_lock(void);
extern void resmgr_cpuset_unlock(void);
#else
static inline void
resmgr_thread_cpuset_of(const struct thread_s *thread, __cpuset_t *cpumask)
{
	UNUSED(thread);
	cpumask->__bits = (__u32)((1UL << __U(CONFIG_NR_CPUS)) - 1UL);
}

static inline void resmgr_cpuset_lock(void) {}
static inline void resmgr_cpuset_unlock(void) {}

#endif /* CONFIG_RESMGR_CPUSET */

#ifdef CONFIG_RESMGR_CPU
extern cref_t resmgr_scheduler_of_process(struct process_s *process);
extern cref_t resmgr_scheduler_of_thread(struct thread_s *thread);
#endif

#endif // RES_H
