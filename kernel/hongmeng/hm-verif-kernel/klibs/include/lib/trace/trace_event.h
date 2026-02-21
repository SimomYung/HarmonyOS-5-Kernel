/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Macro of tracepoint
 * Author: Huawei OS Kernel Lab
 * Create: Tue SEP 11 11:11:53 2023
 */
#ifndef KLIBS_TRACE_EVENT_H
#define KLIBS_TRACE_EVENT_H

#include <lib/dlist.h>
#include <lib/trace/trace_header.h>

struct trace_event_call {
	struct dlist_node perf_events;
};

#endif /* KLIBS_TRACE_EVENT_H */
