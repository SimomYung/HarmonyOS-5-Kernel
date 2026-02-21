/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: The generic time operation
 * Author: Huawei OS Kernel Lab
 * Create: Thy Dec 23 06:23:51 2021
 */
#ifndef KLIBS_PERFRA_TIME_ARM_H
#define KLIBS_PERFRA_TIME_ARM_H

#ifdef __arm__

#include <hmasm/types.h>

static inline __u64 __time_cntpct_read(void)
{
	__u64 cval = 0;
	asm volatile("mrrc p15, 0, %Q0, %R0, c14" : "=r" (cval));
	return cval;
}

static inline __u64 __time_cntfrq_read(void)
{
	__u64 freq = 0;
	asm volatile("mrc p15, 0, %0, c14, c0, 0" : "=r" (freq));
	return freq;
}

static inline __u64 __time_timer_read(void)
{
	return __time_cntpct_read();
}

static inline __u64 generic_timer_cntfrq(void)
{
	return __time_cntfrq_read();
}

#endif

#endif
