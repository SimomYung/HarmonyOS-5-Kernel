/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Functions for vdso
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 28 22:49:27 2020
 */

#ifndef ARCH_AARCH64_VDSO_H
#define ARCH_AARCH64_VDSO_H
#include <time.h>
#include <sys/time.h>
#include <hmasm/types.h>
#include <hmkernel/mm/udata.h>

/* Declare the external interfaces */
int __kernel_clock_gettime(clockid_t clk, struct timespec *ts);
int __kernel_gettimeofday(struct timeval *tv, struct timezone *tz);
int __kernel_gettimeudata(struct kshare_udata_s *udata);

/* Declare the internal interfaces */
int __safe_copy(void *dest, void *src, size_t len);
struct kshare_udata_s *__vdso_get_data(void);

#endif
