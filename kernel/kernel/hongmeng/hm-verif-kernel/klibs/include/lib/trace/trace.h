/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interface of trace
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 25 08:47:55 2023
 */
#ifndef KLIBS_TRACE_H
#define KLIBS_TRACE_H

#include <hmkernel/types.h>

#ifdef __HOST_LLT__
extern struct trace_savedstring_buffer_s savedstring_buffer;
#endif

int trace_add_tp_string(char *str, long key, unsigned int *str_id);
int trace_init_tp_string(bool reset_buffer);

#endif /* KLIBS_TRACE_H */
