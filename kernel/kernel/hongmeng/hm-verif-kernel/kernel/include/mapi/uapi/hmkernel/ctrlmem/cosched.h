/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header file of pcache - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 25 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_COSCHED_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_COSCHED_H

#include <hmkernel/errno.h>
#include <hmkernel/types.h>

#define __CTRLMEM_SCHED_PREEMPT_MAX_CPU_CNT	64
#define __CTRLMEM_SCHED_PREEMPT_MAX_SLV_CNT	4

struct __ctrlmem_sched_preempt_s {
	__u64  padding[__CTRLMEM_SCHED_PREEMPT_MAX_CPU_CNT];

	/* process's current sched level at different cpus */
	__u32 slv[__CTRLMEM_SCHED_PREEMPT_MAX_CPU_CNT];
	__u32 disabled_affinity[__CTRLMEM_SCHED_PREEMPT_MAX_SLV_CNT];

	__bool registered;
	__bool exit;
};

struct __ctrlmem_cosched_s {
	struct __ctrlmem_sched_preempt_s sched_preempt;
};

#endif /* MAPI_UAPI_HMKERNEL_CTRLMEM_COSCHED_H */
