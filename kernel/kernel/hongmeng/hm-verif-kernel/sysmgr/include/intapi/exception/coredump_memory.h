/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Definition for coredump memory operations
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 15 11:11:33 2024
 */

#ifndef SYSMGR_INTAPI_EXCEPTION_COREDUMP_MEMORY_H
#define SYSMGR_INTAPI_EXCEPTION_COREDUMP_MEMORY_H
#include <stdbool.h>

#ifdef CONFIG_COREDUMP_MEMORY_KDUMP
bool coredump_memory_kdump_resize_read(void);
int coredump_memory_kdump_resize_write(bool kdump_resize);
#endif

#endif
