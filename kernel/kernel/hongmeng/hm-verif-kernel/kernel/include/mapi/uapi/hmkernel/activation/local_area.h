/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Local area
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 08 01:04:39 2020
 */
#ifndef MAPI_UAPI_HMKERNEL_ACTIVATION_LOCAL_AREA_H
#define MAPI_UAPI_HMKERNEL_ACTIVATION_LOCAL_AREA_H

#include <hmkernel/capability.h>
#include <hmkernel/ipc/hmsignal.h>
#include <hmasm/ucontext.h>

#define SYM_FUNC_NAME_SIZE 20
#define SYM_MOD_NAME_SIZE 12

struct __iowait_info_s {
	__u32 iowait;
};

struct __block_info_s {
	char fname[SYM_MOD_NAME_SIZE];  /* symbol in which binary file */
	char sname[SYM_FUNC_NAME_SIZE]; /* symbol name */
	unsigned long offset;
	unsigned long size;
};

struct __sched_info_s {
	/* sched info is inited or not */
	__u32 inited;
	/* sched state of current cpu */
	__u32 state;
	/* need to do reschedule immediately */
	__u32 resched;
};

struct __seccomp_info_s {
	/* keep same layout with linux seccomp_data to reduce data movement */
	__u32 scno;
	__u32 arch;
	__u64 instruction_pointer;
	__u64 seccomp_args[6];
	__u32 hscno;

	/* seccomp_info is inited or not */
	__u32 inited;

	/* ptr of seccomp filter */
	__u64 seccomp_filter;
	__u64 seccomp_filter_pacdb;
	__u64 seccomp_filter_coidx;
	__u32 need_check_thiscall;
};

struct __actv_kcov_info_s {
	__u32 kcov_idx_state;
};

struct __actv_sched_info_s {
	__u64 ready_time;
	__u64 block_time;
	__u32 ready_cnt;
	__u32 block_cnt;
	__u32 need_reset;
};

struct __lt_info_s {
	__u32 inited;
	__u32 group;
};

struct __actv_service_info_s {
	__u64 base_credential;
};

/*
 * if fs need flush info before trigger schedule, kernel need save uctx and
 * switch to flush_cb to do flush. And after that, switch back to saved uctx.
 */
struct __fs_info_s {
	/* mem used to save uctx when schedule is triggered */
	__u64 saved_uctx;
	/* callback pc which will do flush */
	__u64 flush_cb;
	/* prepare for flush_cb as stack while switch to fs callback */
	__u64 flush_stack;
};

#define __ACTV_LOCAL_AREA_FLAG_HMSIGNAL			(1U << 0)
#define __ACTV_LOCAL_AREA_FLAG_SCHED_INFO		(1U << 1)
#define __ACTV_LOCAL_AREA_FLAG_THREAD_INFO		(1U << 2)
#define __ACTV_LOCAL_AREA_FLAG_ACTV_INFO		(1U << 3)
#define __ACTV_LOCAL_AREA_FLAG_IOWAIT_INFO		(1U << 4)
#define __ACTV_LOCAL_AREA_FLAG_LT_INFO			(1U << 5)
#define __ACTV_LOCAL_AREA_FLAG_CRED			(1U << 6)
#define __ACTV_LOCAL_AREA_FLAG_SECCOMP_INFO		(1U << 7)
#define __ACTV_LOCAL_AREA_FLAG_SERVICE_INFO		(1U << 8)
#define __ACTV_LOCAL_AREA_FLAG_FS_INFO			(1U << 9)
#define __ACTV_LOCAL_AREA_FLAG_BLOCKED_INFO		(1U << 10)
/* Now only support 16 kinds of local area info */
#define __ACTV_LOCAL_AREA_FLAGS_MASK			(0xFFFFU)

struct __cred_s {
	__u64 cred;
	__u64 xact_cred;
	__u32 pac_cred;
	__u32 pac_xact;
};

/*
 * __actv_local_area_k contains some kernel information local to one activation
 * actv, which are delivered by kernel to actv stack in user space.
 */
struct __actv_local_area_k {
	/*
	 * Synchronize the raising_hmsignals of the base actv.
	 * The actv handler can read it and decide how to react.
	 * For example, handler may choose to pop itself immediately
	 * when finding the HMSIGNAL_KILL is base raising.
	 */
	struct __hmsigmask raising_hmsignals;
	/*
	 * Synchronize the sched info of actv.
	 * The scheduler may change strategy based on the sched info or
	 * sync sched info to user so that user can read it faster.
	 * For example, user can tell kernel to disable process preempt in
	 * current cpu and acquire cpu id without entering kernel.
	 */
	struct __sched_info_s sched_info;
	/*
	 * Synchronize the cred info of actv
	 * Actv handler can fetch tcb cred and xact cred from local area whitch
	 * will be used in sec_chk_hooks
	 */
	struct __cred_s creds;
#ifdef CONFIG_SECCOMP
	struct __seccomp_info_s seccomp_info;
#endif
#ifdef CONFIG_ACTV_SCHED_STAT
	/*
	 * This will be update while tcb state change.
	 */
	struct __actv_sched_info_s actv_sched_info;
#endif
	/*
	 * Synchronize the lock info of futex.
	 * For example, user can tell kernel the futex owner cnode_idx,
	 * LR, addr.
	 */
	struct __iowait_info_s iowait_info;

	/* Just used for iowait */
	struct __block_info_s block_info;
#ifdef CONFIG_ACTV_SCHED_LT
	/*
	 * Synchronize the load tracking info of tcb.
	 */
	struct __lt_info_s lt_info;
#endif
#ifdef CONFIG_ACTV_LOCAL_AREA_SERVICE_INFO
	/*
	 * Synchronize the service info, to deliver credential of base
	 * activation to activation server.
	 */
	struct __actv_service_info_s service_info;
#endif
#ifdef CONFIG_ACTV_SCHED_FS
	struct __fs_info_s fs_info;
#endif
};

#endif
