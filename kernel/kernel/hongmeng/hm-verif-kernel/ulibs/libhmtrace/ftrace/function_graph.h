/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of data struct and declaration of inner interface of function graph
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 16 15:15:41 2020
 */
#ifndef FUNCTION_GRAPH_H
#define FUNCTION_GRAPH_H

#include <lib/dlist.h>

#include "function_trace.h"

enum function_graph_flag {
	GRAPH_ENT,
	GRAPH_RET,
};

enum graph_show_format {
	NOT_MATCH_FMT,
	LEAF_NODE_FMT,
	NESTED_ENT_FMT,
	NESTED_RET_FMT,
};

struct function_graph_record {
	struct timespec time;
	uintptr_t ip;
	int depth;
	enum function_graph_flag flag;
};

struct graph_show_record {
	struct dlist_node dlist;
	struct function_graph_record record;
	const char *func_name;
	bool is_printed;
};

struct graph_handler_record {
	struct dlist_node dlist;
	struct function_graph_record record;
	uintptr_t ret_addr;
};

#ifdef CONFIG_FUNCTION_GRAPH
void function_graph_handler_hook(void);
void return_handler_trampoline(void);
uintptr_t return_handler_hook(void);
#else
static inline void function_graph_handler_hook(void)
{
	return;
}
#endif

extern struct ftrace_tracer_s function_graph;
#endif
