/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Header file of shared futex
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 28 11:43:34 2020
 */
#ifndef H_SYSMGR_INCLUDE_THREAD_FUTEX_H
#define H_SYSMGR_INCLUDE_THREAD_FUTEX_H

#include <libhmsrv_sys/hm_futex.h>
#include <intapi/thread/thread.h>

#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
static inline bool is_compat_client(void)
{
	struct arch_actv_local *local = hm_actv_current_arch_actv_local();
	BUG_ON(local == NULL);
	return __RPC_INFO_DECODE_COMPAT_BIT((unsigned long long)local->rpcinfo) ||
	       __RPC_INFO_DECODE_ILP32_BIT((unsigned long long)local->rpcinfo);
}
#else
static inline bool is_compat_client(void)
{
	return false;
}
#endif

int thread_futex_wait(struct vspace_s *vspace,
		      struct thread_futex_wait_args *args,
		      bool mask, __u32 caller_tid);
int thread_futex_wake(struct vspace_s *vspace,
		      const struct thread_futex_wake_args *args);
int thread_futex_requeue(struct vspace_s *vspace,
			 const struct thread_futex_requeue_args *args);
int thread_update_vnotify_futex(struct vspace_s *vspace, unsigned int chn,
				const int *futex);
int thread_futex(struct vspace_s *vspace, struct thread_futex_args args, int caller_tid);
#endif
