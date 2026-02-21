/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Declaration of function graph API
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 04 16:15:38 2020
 */
#ifndef ULIBS_LIBHMTRACE_FTRACE_FUNC_GRAPH_H
#define ULIBS_LIBHMTRACE_FTRACE_FUNC_GRAPH_H

void hm_ftrace_register_function_graph(void);
void hm_ftrace_unregister_function_graph(void);
void hm_ftrace_function_graph_set_tracing_on(void);
void hm_ftrace_function_graph_set_tracing_off(void);
void hm_ftrace_function_graph_display(void);
#endif
