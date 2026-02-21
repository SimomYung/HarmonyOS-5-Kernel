/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Event definitions exported to system managers.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 06 02:40:44 2024
 */
#ifndef SYSMGR_INCLUDE_AAPI_TRACE_DEF_H
#define SYSMGR_INCLUDE_AAPI_TRACE_DEF_H

#include <lib/trace/tracepoint_defs.h>
enum trace_event_module {
	TRACE_EVENT_MODULE_KERNEL,
	TRACE_EVENT_MODULE_SYSMGR,
	TRACE_EVENT_MODULE_TPPMGR,
	TRACE_EVENT_MODULE_UDK,
	TRACE_EVENT_MODULE_LDK,
	TRACE_EVENT_MODULE_HMVIRT,
	TRACE_EVENT_MODULE_MAX,
};

struct tracepoint_config {
	char group_name[TRACE_EVENT_GROUP_NAME_SIZE];
	char tp_name[TRACE_EVENT_NAME_SIZE];
};

struct trace_register_events_param {
	struct tracepoint_config *tps;
	unsigned int *tp_type;
	unsigned int tp_num;
};

#endif
