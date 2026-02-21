/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header of idle state
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 22 07:19:57 2021
 */

#ifndef MAPI_HMKERNEL_DRIVERS_IDLE_STATE_H
#define MAPI_HMKERNEL_DRIVERS_IDLE_STATE_H

#include <uapi/hmkernel/drivers/idle_state.h>
#include <uapi/hmkernel/cpuset.h>

struct idle_state_info {
	unsigned int suspend_state;
	bool timer_stop;
};

struct idle_state {
	__cpuset_t pnode_idle_cpu;
	struct idle_state_info state;

	/* tppmgr idle framework need the following info */
	struct idle_latency_info latency;
};

#endif
