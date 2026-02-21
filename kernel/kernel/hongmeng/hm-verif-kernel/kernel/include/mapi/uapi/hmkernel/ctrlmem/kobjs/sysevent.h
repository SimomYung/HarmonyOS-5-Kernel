/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: sysevent metadata as kobject - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 25 14:23:32 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_SYSEVENT_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_SYSEVENT_H

#include <hmkernel/types.h>
#include <vsync/atomic.h>

enum __sysevent_action {
	__SYSEVENT_ACTION_INIT = 0,
	__SYSEVENT_ACTION_READ,
	__SYSEVENT_ACTION_WRITE,
};

struct __ctrlmem_kobj_sysevent_s {
	vatomic32_t write_seq;
	vatomic32_t read_seq;
};

#endif
