/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Tracepoint of udk
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 07 11:08:30 2023
 */
#include <libhmtrace_ng/hmtrace.h>
#include <internal/init.h>
#include <udk/block/udk_block_mq.h>
#include <udk/block/udk_disk.h>

static __thread bool udk_tp_func_running = false;

static inline void set_tp_func_running(bool running)
{
	udk_tp_func_running = running;
}

static inline bool is_tp_func_running(void)
{
	return udk_tp_func_running;
}

#include <lib/trace/tracepoint.h>
MAKE_TRACEPOINT_GROUP_STR(block);
MAKE_TRACEPOINT_GROUP_STR(ufs);

COPY_TRACE_ARGS_DEFN_x(1);
COPY_TRACE_ARGS_DEFN_x(2);
COPY_TRACE_ARGS_DEFN_x(3);
COPY_TRACE_ARGS_DEFN_x(4);
COPY_TRACE_ARGS_DEFN_x(5);
COPY_TRACE_ARGS_DEFN_x(6);
COPY_TRACE_ARGS_DEFN_x(7);
COPY_TRACE_ARGS_DEFN_x(8);
COPY_TRACE_ARGS_DEFN_x(9);
COPY_TRACE_ARGS_DEFN_x(10);
COPY_TRACE_ARGS_DEFN_x(11);
COPY_TRACE_ARGS_DEFN_x(12);

#define TRACE_INCLUDE_FILE <udk/trace/events/udk_trace_list.h>
#include <lib/trace/define_trace.h>

static int trace_init(void)
{
	int ret = hm_trace_register_events(TRACE_EVENT_MODULE_UDK);
	if (ret != E_HM_OK) {
		hm_error("trace register events failed, ret=%s\n", hmstrerror(ret));
	}
	return ret;
}
udk_init_call(trace_init);
