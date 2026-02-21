/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Arm pmuv3
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 17 10:15:56 2018
 */
#ifndef UAPI_HMKERNEL_DRIVERS_ARM_PMUV3_H
#define UAPI_HMKERNEL_DRIVERS_ARM_PMUV3_H

#include <hmkernel/types.h>

enum __pmuv3_config_cmd {
	PMUV3_CONFIG_START,
	PMUV3_CONFIG_STOP,
	PMUV3_CONFIG_RESET_EVENTS,
	PMUV3_CONFIG_ADD_EVENT,
	PMUV3_CONFIG_DEL_EVENT,
	PMUV3_CONFIG_READ_SPEC,
};

enum __pmuv3_event {
	PMUV3_EVENT_CYCLES	= (__u32)(-1),
	PMUV3_EVENT_SW_INCR	= 0,
	PMUV3_EVENT_INSNS	= 0x8,
	PMUV3_EVENT_LOADS	= 0x6,
	PMUV3_EVENT_STORES	= 0x7,
};

struct __pmuv3_config_add_event {
	__u32 counter;
	__u32 event;
	__u32 config;
};

struct __pmuv3_config_add_event_ret {
	__u32 counter;
};

struct __pmuv3_config_del_event {
	__u32 counter;
};

struct __pmuv3_config_read_spec_ret {
	__u32 ncounters;
};

struct __pmuv3_report {
	__u64 cycles;
	__u32 counters[0];
};

#endif
