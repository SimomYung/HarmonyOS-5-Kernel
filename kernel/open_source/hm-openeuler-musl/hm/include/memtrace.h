/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020 ~ 2020. All rights reserved.
 * Description: Interface declaration for memory trace
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 27 18:36:09 2020
 */
#ifndef _MEMTRACE_H
#define _MEMTRACE_H

#include <stddef.h>

void mtrace_record_malloc(void *p, size_t size);
void mtrace_record_free(const void *p);
void mtrace_fork_prehook(void);
void mtrace_fork_posthook(void);
size_t mtrace_get_obj_size(const void *p);

#endif
