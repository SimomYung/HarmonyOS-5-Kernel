/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Uapi for power track
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 5 22:08:32 2024
 */
#ifndef MAPI_UAPI_HMKERNEL_POWER_TRACK_H
#define MAPI_UAPI_HMKERNEL_POWER_TRACK_H

#include <hmasm/types.h>

struct __sys_power_cpu_state {
	__u32 cpu_id;
	__u32 len;
	__u64 *cpu_state_time; /* unit: usec */
};

#endif /* MAPI_UAPI_HMKERNEL_POWER_TRACK_H */
