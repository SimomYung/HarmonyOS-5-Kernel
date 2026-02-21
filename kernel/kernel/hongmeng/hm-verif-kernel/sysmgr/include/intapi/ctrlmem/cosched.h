/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: ctrlmem pcache related functions in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 10 15:55:28 2023
*/

#ifndef INTAPI_CTRLMEM_PROCESS_PREEMPT_H
#define INTAPI_CTRLMEM_PROCESS_PREEMPT_H

#include <hmasm/memory.h>

int ctrlmem_cosched_alloc(uintptr_t *cosched_addr);
void ctrlmem_cosched_free(uintptr_t va);

#endif
