/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Functions for vdso
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 28 22:38:39 2020
 */

#ifndef ARCH_ARM_VDSO_H
#define ARCH_ARM_VDSO_H
#include <time.h>
#include <sys/time.h>
#include <hmasm/types.h>
#include <hmkernel/mm/udata.h>

/* Declare the external interfaces */
int __vdso_clock_gettime(clockid_t clk, struct timespec *ts);

/* Declare the internal interfaces */
int __safe_copy(void *dest, void *src, size_t len);
struct kshare_udata_s *__vdso_get_data(void);

#endif
