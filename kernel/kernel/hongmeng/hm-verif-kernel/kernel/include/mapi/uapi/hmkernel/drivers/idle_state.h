/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: idle state kernel uapi interface
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 19 16:26:15 2021
 */
#ifndef UAPI_HMKERNEL_DRIVERS_IDLE_STATE_H
#define UAPI_HMKERNEL_DRIVERS_IDLE_STATE_H

#include <hmkernel/types.h>

#define EXT_FASTCALL_GLOBAL_IDLE_LATENCY_INFO		3

struct idle_latency_info {
	__u32 latency_us;
	__u32 min_idletime_us;
	__u32 cpumask;
#ifdef CONFIG_CPUIDLE_LP_MODE
	__u32 lp_min_idletime_us;
#endif
};

#define __PM_IDLE_EXIT		UINT_MAX
#define __PM_IDLE_C0		0	/* wfi */
/*
 * The range of idle index is 0-10. Need define an index value
 * for suspend which is different from idle.
 */
#define __PM_SUSPEND_CPU	11

#endif
