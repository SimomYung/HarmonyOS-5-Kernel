/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: ustrcpy
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 30 18:02:23 CST 2023
 */
#ifndef AARCH64_KERNEL_USTRCPY_H
#define AARCH64_KERNEL_USTRCPY_H

#include <hmkernel/types.h>

#define __USTRCPY_FAULT_SRC	(-1)
#define __USTRCPY_FAULT_DST	(-2)

extern long __ustrcpy(void *dst, const void *src, size_t length);

#endif
