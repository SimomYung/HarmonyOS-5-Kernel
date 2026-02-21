/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Per-cpu info pages as ctrlmem - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 07 11:02:47 2023
 */

#ifndef HMKERNEL_MAPI_UAPI_CTRLMEM_PCPINFO_H
#define HMKERNEL_MAPI_UAPI_CTRLMEM_PCPINFO_H

#include <hmkernel/types.h>
#include <hmkernel/const.h>
#include <hmasm/memory.h>
#include <hmasm/page.h>
#include <hmasm/current.h>

#define __PCPINFO_TCB_THREAD_NAME_SIZE	16
#define PER_CPU_PCPINFO_SIZE (PAGE_SIZE)

struct __pcpinfo_tcb_s {
	/* cref of thread */
	__u64 tcb_cref;
	/* cref of sched thread */
	__u64 sched_tcb_cref;
	/* thread running priority */
	__u32 running_prio;

	/* the virtual TID of thread in the ns where the thread is locate */
	__u32 vtid;
	/* the virtual PID of process in the ns where the process is located */
	__u32 vpid;
	/* the global TID of thread in root pid ns */
	__u32 tid;
	/* the global PID of process in root pid ns */
	__u32 pid;
	/* thread name */
	char name[__PCPINFO_TCB_THREAD_NAME_SIZE];

	__u32 kcov_idx_state;
	__u64 vs_key;
	__u8 xdep_critical;
};

struct __ctrlmem_pcpinfo_s {
	__u64 meta;
	__paddr_t self_pa;
	__u64 generation;
	struct __pcpinfo_tcb_s ptcb;
	__u8 padding[__PAGE_SIZE - sizeof(__u64) -
				   sizeof(__paddr_t) -
				   sizeof(__u64) -
				   sizeof(struct __pcpinfo_tcb_s)];
};

#endif
