/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide function graph API
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 24 19:54:44 2019
 */
#include <hmkernel/errno.h>
#include <libstrict/strict.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmtrace/hmtrace.h>
#include <libhmtrace/hmtracectrl.h>
#include <libhmtrace/ftrace/function_graph.h>

#ifdef CONFIG_FUNCTION_GRAPH
#include "function_graph.h"

void hm_ftrace_register_function_graph(void)
{
	int err;

	err = hm_ftrace_register_tracer(&function_trace_class, &function_graph);
	if (err) {
		hm_warn("register function_graph tracer failed: %s\n", hmstrerror(err));
	}

	err = hm_ftrace_bind_local_buffer(&function_graph,
					  FUNC_TRACE_DEFAULT_RB_SIZE);
	if (err) {
		hm_warn("function trace bind local buffer failed: %s\n", hmstrerror(err));
		err = hm_ftrace_unregister_tracer(&function_graph);
		if (err) {
			hm_warn("unregister function trace failed: %s\n", hmstrerror(err));
			return;
		}
	}

	hm_ftrace_function_graph_set_tracing_on();
	hm_info("register function graph success\n");
}

void hm_ftrace_unregister_function_graph(void)
{
	int err;

	err = hm_ftrace_unregister_tracer(&function_graph);
	if (err) {
		hm_warn("unregister function trace failed: %s\n", hmstrerror(err));
		return;
	}
}

void hm_ftrace_function_graph_set_tracing_on(void)
{
	int err;

	err = hm_ftrace_set_tracer_tracing_on_by_pid(hm_getpid(), "function_graph");
	if (err) {
		hm_warn("enable function trace failed: %s\n", hmstrerror(err));
	}
}

void hm_ftrace_function_graph_set_tracing_off(void)
{
	int err;

	err = hm_ftrace_set_tracer_tracing_off_by_pid(hm_getpid(), "function_graph");
	if (err) {
		hm_warn("disable function trace failed: %s\n", hmstrerror(err));
	}
}

void hm_ftrace_function_graph_display(void)
{
	int err;

	err = hm_ftrace_report_events(&function_graph, NULL, 0);
	if (err) {
		hm_warn("show function trace info failed: %s\n", hmstrerror(err));
	}
}
#else
void hm_ftrace_register_function_graph(void)
{
	return;
}

void hm_ftrace_unregister_function_graph(void)
{
	return;
}

void hm_ftrace_function_graph_set_tracing_on(void)
{
	return;
}

void hm_ftrace_function_graph_set_tracing_off(void)
{
	return;
}

void hm_ftrace_function_graph_display(void)
{
	return;
}
#endif
