/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of threadpool managed by sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 4 10:24:35 2019
 */

#ifndef H_SYSMGR_THREADPOOL_H
#define H_SYSMGR_THREADPOOL_H

#include <unistd.h>

#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <hmkernel/capability.h>
#include <intapi/thread/thread.h>
#include <intapi/process/pid.h>

void threadpool_init(void);

int threadpool_insert(struct thread_s *thread);
void threadpool_remove(struct thread_s *thread);

struct thread_s *threadpool_search_by_cref(cref_t tcb_cref);
struct thread_s *threadpool_search_by_cref_nolock(cref_t tcb_cref);

struct raw_rwlock *threadpool_rwlock(void);

#define THREAD_LOCK_GUARD RAW_RWLOCK_RD_GUARD(threadptr, threadpool_rwlock())

declare_scopeptr1(thread_by_cref, offset_of(struct thread_s, refcnt), THREAD_LOCK_GUARD,
		threadpool_search_by_cref_nolock, cref_t, tcb_cref);

#define acquire_thread_by_cref(name, cref) \
	acquire_scopedptr(thread_by_cref, name, offset_of(struct thread_s, refcnt), cref)

#define acquire_thread_by_cref_start(name, cref) \
	{\
		acquire_scopedptr(thread_by_cref, name, offset_of(struct thread_s, refcnt), cref)

#define acquire_thread_by_cref_end(name) \
	}

#define thread_init_refcount(ptr) \
	raw_refcnt_init(ptr, 1, thread_delete_from_ref)

#endif
