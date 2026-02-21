/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Kev
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 28 19:56:43 2021
 */

#ifndef UAPI_HMKERNEL_EVENTS_KEV_PART_H
#define UAPI_HMKERNEL_EVENTS_KEV_PART_H

#include <hmasm/types.h>

#define KEV_EVENT_NAME_SIZE	32
#define KEV_EVENT_FILTER_SIZE	32
#define KEV_OUTPUT_ENABLE	1U
#define KEV_OUTPUT_DISABLE	0U

struct __kev_ctrl_read_data_args {
	__u64 vaddr;
	__u64 head;
	__u64 size;
	__u32 index;
};

struct __kev_ctrl_read_data_ret {
	__u64 head;
	__u64 size;
};

#endif
