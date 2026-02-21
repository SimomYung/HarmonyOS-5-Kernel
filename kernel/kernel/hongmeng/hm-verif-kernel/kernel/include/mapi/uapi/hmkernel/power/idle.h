/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Uapi for idle
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 08 18:59:08 2023
 */
#ifndef MAPI_UAPI_HMKERNEL_POWER_IDLE_H
#define MAPI_UAPI_HMKERNEL_POWER_IDLE_H

#include <hmasm/types.h>
#include <hmkernel/types.h>

#define IDLE_EXIT_STATE				(-1)

struct __idle_sys_stat {
	__u64 cost_ns;
	__s32 state;
	__bool is_intercept;
};

struct __idle_select_stat {
	__u64 first_timeout;
	__u64 next_timeout;
	__bool all_cpu_in_idle;
	__bool irq_is_continual;
};

#endif
