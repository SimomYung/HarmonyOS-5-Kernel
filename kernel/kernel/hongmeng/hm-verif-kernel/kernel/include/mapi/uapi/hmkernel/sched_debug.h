/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Declaration of sched_debug for uapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 26 14:59:09 2020
 */

#ifndef UAPI_MODULE_SCHED_DEBUG_H
#define UAPI_MODULE_SCHED_DEBUG_H

#include <hmkernel/types.h>
#include <hmkernel/capability.h>

/* we store ices rq start&end cpu */
#define ICES_RQ_CPU_RANGE 		2
#define INVALID_CPU 			(__u32)(-1)
#define INVALID_ICES_RQ 		(__u32)(-1)

struct cpu_info_s {
	__u32 cpu;
	/* prio of curr_thread */
	__u32 prio;
	__u64 cpu_clk;
	__u32 nr_switches;
	/* running thread name of curr cpu */
	char curr_thread[HMCAP_THREAD_NAME_SIZE];
};

struct rt_rq_info_s {
	__u32 nr_ready;
};

struct ices_rq_info_s {
	__u32 rq_id;
	__u64 cpu_clk;
	__u32 nr_ready;
	__u32 cpu_range[ICES_RQ_CPU_RANGE];
};

/* per cpu info */
struct __sched_debug_args_s {
	struct cpu_info_s cpu_info;
	struct rt_rq_info_s rt_rq_info;
	struct ices_rq_info_s ices_rq_info;
} __aligned(8);

#endif
