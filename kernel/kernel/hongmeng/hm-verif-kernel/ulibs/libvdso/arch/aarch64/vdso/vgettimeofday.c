/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 18 19:08:04 2020
 */
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <hmasm/types.h>
#include <hmasm/pmu.h>
#include <hmkernel/mm/udata.h>
#include <hmkernel/barrier.h>
#include <hmkernel/compiler.h>
#include <string.h>
#include "vdso.h"
#include "vgettime.h"

static int vdata_read(const struct kshare_udata_s *src, struct kshare_udata_s *dst, __u64 *rato_hwcnt_rp)
{
	__u32 seq;
	int ret = 0;

	if (src == NULL) {
		ret = -EINVAL;
	} else {
		do {
			seq = __seqcnt_start_read(&src->tock_seq);

			*rato_hwcnt_rp = read_tock_user();
			dst->tock_last = src->tock_last;
			dst->mul  = src->mul;
			dst->mov  = src->mov;
			dst->tock_mono_sec    = src->tock_mono_sec;
			dst->tock_mono_nsec = src->tock_mono_nsec;
			dst->tock_mono_nsec_calib = src->tock_mono_nsec_calib;
			dst->tock_base_sec    = src->tock_base_sec;
			dst->tock_base_nsec   = src->tock_base_nsec;
			dst->tz_minuteswest = src->tz_minuteswest;
			dst->tz_dsttime = src->tz_dsttime;
#ifdef CONFIG_TIMER_HARDEN_S4
			dst->pcs4offset = src->pcs4offset;
#endif
		} while (__seqcnt_retry_read(&src->tock_seq, seq) != 0);
	}
	return ret;
}

static void get_realtime(const struct kshare_udata_s *vdata, struct timespec *ts, __u64 rato_hwcnt)
{
	__u64 nsec;

	ts->tv_sec  = (time_t)((__s64)vdata->tock_mono_sec + vdata->tock_base_sec);
	nsec = get_nsec(vdata, rato_hwcnt) + (__u64)vdata->tock_base_nsec;

	timespec_add_nsec(ts, nsec);
}

int __kernel_gettimeofday(struct timeval *tv, struct timezone *tz)
{
	int ret = 0;

	struct kshare_udata_s *vdata = NULL;
	struct kshare_udata_s clk_data = {0};
	struct timespec ts = {0};
	__u64 rato_hwcnt = 0;

	if (__safe_copy((void *)tv, (void *)tv, sizeof(struct timeval)) != 0) {
		ret = -EFAULT;
	}

	if (ret == 0 && tz != NULL) {
		if (__safe_copy((void *)tz, (void *)tz, sizeof(struct timezone)) != 0) {
			ret = -EFAULT;
		}
	}

	if (ret == 0) {
		vdata = (struct kshare_udata_s*)__vdso_get_data();
		ret = vdata_read(vdata, &clk_data, &rato_hwcnt);
		if (ret == 0) {
			get_realtime(&clk_data, &ts, rato_hwcnt);
			tv->tv_sec = ts.tv_sec;
			tv->tv_usec = ts.tv_nsec / (long)(int)__NSEC_PER_USEC;
			if (tz != NULL) {
				tz->tz_minuteswest = clk_data.tz_minuteswest;
				tz->tz_dsttime = clk_data.tz_dsttime;
			}
		} else {
			ret = -ENOSYS;
		}
	}

	return ret;
}
