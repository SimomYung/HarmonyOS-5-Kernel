/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for tcb uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 13:36:28 2019
 */
#ifndef UAPI_TCB_STAT_H
#define UAPI_TCB_STAT_H

#include <hmkernel/types.h>
#include <hmkernel/sched/mips_common.h>

struct __tcb_stat_s {
	__u64 sched;
	__u64 fetch_time;
	__u64 run_time;
	__u64 run_utime;
	__u64 run_stime;
	__u64 ready_time;
	__u64 suspend_time;
	__u64 nvcsw;       // voluntary context switch
	__u64 nivcsw;      // involuntary context switch
};

#ifdef CONFIG_MODULE_MIPS_LOAD_TRACKING
#define MIPS_HIST_SIZE_MAX       3

struct __tcb_mips_demand {
        struct mips_pmu_data pmu_data[MIPS_HIST_SIZE_MAX];
        __u64 window_start;
        int curr_avg_ipc;
};
#endif

#endif
