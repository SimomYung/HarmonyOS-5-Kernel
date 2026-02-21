/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header file of vgettime.c
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 23 15:44:05 2021
 */

#ifndef ARCH_AARCH64_VGETTIME_H
#define ARCH_AARCH64_VGETTIME_H
#include <time.h>
#include <sys/time.h>
#include <hmasm/types.h>
#include <hmkernel/mm/udata.h>

static inline __u64 read_tock_user(void)
{
	isb();
#ifdef CONFIG_SUPPORT_CDC_ACE_VTIMER
	return read_sysreg("cntvct_el0");
#else
	return read_sysreg("cntpct_el0");
#endif
}

static inline void timespec_add_nsec(struct timespec *ts, __u64 nsec)
{
	while (nsec >= __NSEC_PER_SEC) {
		nsec -= __NSEC_PER_SEC;
		ts->tv_sec++;
	}
	ts->tv_nsec = (long)nsec;
}

static inline __u64 get_nsec(const struct kshare_udata_s *vdata, __u64 rato_hwcnt)
{
#ifdef CONFIG_TIMER_HARDEN_S4
	__u64 tock_current = rato_hwcnt + vdata->pcs4offset;
#else
	__u64 tock_current = rato_hwcnt;
#endif
	__u64 delta = tock_current - vdata->tock_last;
	__u64 nsec;

	nsec = (delta * vdata->mul + vdata->tock_mono_nsec_calib) >> vdata->mov;
	nsec += vdata->tock_mono_nsec;

	return nsec;
}

#endif
