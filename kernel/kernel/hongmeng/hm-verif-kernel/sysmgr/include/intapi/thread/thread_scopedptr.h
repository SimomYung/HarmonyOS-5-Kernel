/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Scopedptr only for thread
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 04 21:56:59 2021
 */
#ifndef H_SYSMGR_INCLUDE_THREAD_THREAD_SCOPEDPTR_H
#define H_SYSMGR_INCLUDE_THREAD_THREAD_SCOPEDPTR_H

#define SYSMGR_SCOPEDPTR

#include <libhmsync/sysmgr_scopedptr.h>
#include <libhmsync/raw_mutex_guard.h>
#include <intapi/thread/thread.h>
#include <intapi/process/pid.h>
#include <intapi/nsmgr/ns_pid.h>
#include <intapi/process/proc_acquire.h>

declare_scopeptr_withlock(thread_by_tid, offset_of(struct thread_s, refcnt), raw_mutex_lock, raw_mutex_unlock,
			  struct raw_mutex *, mutex, struct tid_s *, find_get_tid_s,
			  find_get_thread_by_tid_s, tid_put, struct pid_ns *, ns,
			  pid_t, tid);

#define acquire_thread_by_tid_in_ns(name, ns, tid) \
	acquire_scopedptr_withlock(thread_by_tid, name, offset_of(struct thread_s, refcnt), ns, tid)

#define acquire_thread_by_tid(name, tid) \
	acquire_thread_by_tid_in_ns(name, current_thread_ns_of(), tid)

#endif
