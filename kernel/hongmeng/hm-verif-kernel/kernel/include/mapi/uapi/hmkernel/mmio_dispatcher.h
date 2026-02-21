/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: mmio dispatcher_size
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 03 14:27:32 2022
 */

#ifndef HMAPI_HMKERNEL_MMIO_DISPATCHER_H
#define HMAPI_HMKERNEL_MMIO_DISPATCHER_H

#include <hmkernel/types.h>
struct __dispatcher_kick_arg_s {
	__u32 chn_id;
	__u32 intr_no;
};

struct __dispatcher_actv_arg_s {
	__u64 opaque;
	__u64 pool;
};
#endif
