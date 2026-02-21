/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: SMT cookie mechanism prctl interface.
 */
#ifndef SYSMGR_INTAPI_PROCESS_SMT_COOKIE_H
#define SYSMGR_INTAPI_PROCESS_SMT_COOKIE_H

#include <hmkernel/types.h>
#include <intapi/process/pid.h>
#include <libstrict/strict.h>
#include <libalgo/hash.h>
#include <procmgr/procmgr_ops.h>

struct proc_prctl_args;

#ifdef CONFIG_CORE_SCHEDULING
/**
 * prctl's smt cookie option definition.
 */
#define PR_SCHED_CORE 62
#define PR_SCHED_CORE_PREFER 63
#define PR_SCHED_CORE_GET 0
#define PR_SCHED_CORE_CREATE 1
#define PR_SCHED_CORE_SHARE_TO 2
#define PR_SCHED_CORE_SHARE_FROM 3
#define PR_SCHED_CORE_DESTROY 4
#define PR_SCHED_CORE_MAX 5

#define SMT_COOKIE_HASH_BITS 32

/**
 * Get the hash value of a cookie.
 */
static inline uint64_t smt_cookie_hash(__u64 cookie)
{
	return hash_64(cookie, SMT_COOKIE_HASH_BITS);
}

/**
 * SMT prefer cookie share interface for memory reclaim.
 * If the `src` doesn't have a prefer cookie, create one.
 *  - src: the source thread.
 *  - dst: the destination thread.
 */
int smt_prefer_cookie_share(struct thread_s *src, struct thread_s *dst);

/**
 * On thread exiting, decreasing the refcnt of cookie and deallocating
 * the memory if the cookie value is not the default one.
 */
void procmgr_smt_cookie_ops_on_thread_exit(struct thread_s *thread);

struct procmgr_smt_cookie_ops_s {
	void (*on_thread_exit)(struct thread_s *thread);
};

/**
 * For the prctl interface.
 */
int prctl_sched_core(unsigned long credential, struct proc_prctl_args *args);

#else
static inline uint64_t smt_cookie_hash(__u64 cookie)
{
	UNUSED(cookie);
	return 0;
}

static inline int smt_prefer_cookie_share(struct thread_s *src, struct thread_s *dst)
{
	UNUSED(src);
	UNUSED(dst);
	return E_HM_INVAL;
}

static inline void procmgr_smt_cookie_ops_on_thread_exit(struct thread_s *thread)
{
	UNUSED(thread);
}

static inline int prctl_sched_core(unsigned long credential, struct proc_prctl_args *args)
{
	UNUSED(credential, args);
	return E_HM_INVAL;
}
#endif

#endif
