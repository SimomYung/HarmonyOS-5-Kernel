/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Definition for acquire process by pid scopedptr
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 23 10:50:29 2023
 */
#ifndef SYSMGR_INTAPI_PROCESS_PROC_ACQUIRE_H
#define SYSMGR_INTAPI_PROCESS_PROC_ACQUIRE_H

#define SYSMGR_SCOPEDPTR

#include <libhmsync/sysmgr_scopedptr.h>
#include <intapi/process/process.h>
#include <intapi/nsmgr/ns_pid.h>
#include <sysif/utils.h>

#ifdef CONFIG_ACTV_LOCAL_AREA_SERVICE_INFO
static inline struct thread_s *thread_of_base_credential(void)
{
	unsigned long credential;
	credential = (unsigned long)hm_actv_service_info();
	return (struct thread_s *)sysif_cred_to_thread(credential);
}

static inline struct pid_ns *current_thread_ns_of(void)
{
	struct thread_s *current_thread = thread_of_base_credential();
	BUG_ON(current_thread == NULL);

	return thread_curr_active_pidns_of(current_thread);
}
#else
static inline struct thread_s *thread_of_base_credential(void)
{
	return NULL;
}

static inline struct pid_ns *current_thread_ns_of(void)
{
	return root_pid_ns();
}
#endif

struct process_s *process_of_current(void);

declare_scopeptr_withlock(process_by_pid, offset_of(struct process_s, refcnt), raw_mutex_lock, raw_mutex_unlock,
			  struct raw_mutex *, tid.mutex, struct pid_s *, find_get_pid_s,
			  find_get_process_by_pid_s, pid_put, struct pid_ns *, ns,
			  pid_t, pid);

#define acquire_process_by_pid_in_ns(name, ns, pid) \
	acquire_scopedptr_withlock(process_by_pid, name, offset_of(struct process_s, refcnt), ns, pid)

#define acquire_process_by_pid(name, pid) \
	acquire_process_by_pid_in_ns(name, current_thread_ns_of(), pid) \

#define acquire_process_by_pid_start(name, pid) \
	{\
	acquire_process_by_pid_in_ns(name, current_thread_ns_of(), pid);

#define acquire_process_by_pid_end(name)  }
#endif
