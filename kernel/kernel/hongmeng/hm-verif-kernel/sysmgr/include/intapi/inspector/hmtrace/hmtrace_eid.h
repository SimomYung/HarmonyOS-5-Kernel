/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Introduce hmtrace event id management
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 19 17:40:19 2020
 */

#ifndef SYSMGR_INSPECTOR_HMTRACE_EID_H
#define SYSMGR_INSPECTOR_HMTRACE_EID_H

#include <limits.h>

#define EID_MIN		0
#define EID_MAX		INT_MAX

void hmtrace_init_eid_allocator(void);
int hmtrace_alloc_eid(void);
void hmtrace_free_eid(int eid);
#endif
