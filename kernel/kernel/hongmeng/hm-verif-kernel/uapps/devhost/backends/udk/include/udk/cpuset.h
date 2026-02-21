/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: cpuset interfaces of udk
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 14 10:19:03 2022
 */
#ifndef __UDK_CPUSET_H__
#define __UDK_CPUSET_H__

#include <stdbool.h>
#include <udk/types.h>

/* should be consistent with __cpuset_t in hmkernel/cpuset.h */
typedef struct { __u32 __bits; } udk_cpuset_t;

udk_cpuset_t udk_cpuset_empty(void);
udk_cpuset_t udk_cpuset_add(udk_cpuset_t uset, unsigned int cpu);
bool udk_cpuset_equal(udk_cpuset_t useta, udk_cpuset_t usetb);

#endif /* __UDK_CPUSET_H__ */
