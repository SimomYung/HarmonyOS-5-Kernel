/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Declaration of sched_attr
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 25 15:36:52 2020
 */
#ifndef UAPI_HMKERNEL_SCHED_ATTR_H
#define UAPI_HMKERNEL_SCHED_ATTR_H

#define TCB_ATTR_LEN		8

enum {
	THREAD_ATTR_RT_PRIORITY = 0,
	THREAD_ATTR_PRIORITY,
	THREAD_ATTR_NICE,
	THREAD_ATTR_AFFINITY,
	THREAD_ATTR_SCHED_POLICY,
	THREAD_ATTR_MAX = THREAD_STATE_ATTR_SIZE,
};
#endif
