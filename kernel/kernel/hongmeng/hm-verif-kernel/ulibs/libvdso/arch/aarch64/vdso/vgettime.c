/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 28 09:49:33 2019
 */
#include <time.h>
#include <errno.h>
#include <syscall.h>
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
			if (rato_hwcnt_rp != NULL) {
				*rato_hwcnt_rp = read_tock_user();
			}
			dst->tock_last = src->tock_last;
			dst->tock_mono_nsec = src->tock_mono_nsec;
			dst->tock_mono_sec = src->tock_mono_sec;
			dst->tock_base_nsec = src->tock_base_nsec;
			dst->tock_base_sec = src->tock_base_sec;
			dst->tock_mono_nsec_calib = src->tock_mono_nsec_calib;
			dst->offs_boot_sec = src->offs_boot_sec;
			dst->offs_boot_nsec = src->offs_boot_nsec;
			dst->mul = src->mul;
			dst->mov = src->mov;
#ifdef CONFIG_TIMER_HARDEN_S4
			dst->pcs4offset = src->pcs4offset;
#endif
		} while (__seqcnt_retry_read(&src->tock_seq, seq) != 0);
	}
	return ret;
}

static void get_monotonic(const struct kshare_udata_s *vdata, struct timespec *ts, __u64 rato_hwcnt)
{
	__u64 nsec;

	ts->tv_sec  = (long)(long long)vdata->tock_mono_sec;
	nsec = get_nsec(vdata, rato_hwcnt);

	timespec_add_nsec(ts, nsec);
}

static void get_realtime(const struct kshare_udata_s *vdata, struct timespec *ts, __u64 rato_hwcnt)
{
	__u64 nsec;

	ts->tv_sec  = (time_t)((__s64)vdata->tock_mono_sec + vdata->tock_base_sec);
	nsec = get_nsec(vdata, rato_hwcnt) + (__u64)vdata->tock_base_nsec;

	timespec_add_nsec(ts, nsec);
}

static void get_monotonic_coarse(const struct kshare_udata_s *vdata,
				 struct timespec *ts)
{
	ts->tv_sec = (time_t)vdata->tock_mono_sec;
	ts->tv_nsec = (long)vdata->tock_mono_nsec;
}

static void get_realtime_coarse(const struct kshare_udata_s *vdata,
				struct timespec *ts)
{
	__u64 nsec;

	ts->tv_sec  = (time_t)((__s64)vdata->tock_mono_sec + vdata->tock_base_sec);

	nsec = vdata->tock_mono_nsec + (__u64)vdata->tock_base_nsec;

	timespec_add_nsec(ts, nsec);
}

static void get_boottime(const struct kshare_udata_s *vdata,
			 struct timespec *ts, __u64 rato_hwcnt)
{
	__u64 nsec;

	ts->tv_sec  = (time_t)(vdata->tock_mono_sec + vdata->offs_boot_sec);
	nsec = get_nsec(vdata, rato_hwcnt) + (__u64)vdata->offs_boot_nsec;

	timespec_add_nsec(ts, nsec);
}

static int gettime_syscall(long clk, long _ts)
{
	register long clkid asm("x0") = clk;
	register long ts asm("x1") = _ts;
	register long nr asm("x8") = __NR_clock_gettime;
	register int ret asm ("x0");

	asm volatile(
	"svc #0\n" : "=r" (ret)
	: "r" (clkid), "r" (ts), "r" (nr)
	: "memory");

	return ret;
}

int __kernel_clock_gettime(clockid_t clk, struct timespec *ts)
{
	struct kshare_udata_s *vdata = NULL;
	struct kshare_udata_s clk_data = {0};
	int ret = 0;
	__u64 rato_hwcnt = 0;

	vdata = (struct kshare_udata_s*)__vdso_get_data();
	if (clk == CLOCK_REALTIME_COARSE || clk == CLOCK_MONOTONIC_COARSE) {
		ret = vdata_read(vdata, &clk_data, NULL);
	} else {
		ret = vdata_read(vdata, &clk_data, &rato_hwcnt);
	}
	if (ret == 0) {
		if (clk == CLOCK_REALTIME) {
			get_realtime(&clk_data, ts, rato_hwcnt);
		} else if (clk == CLOCK_MONOTONIC) {
			get_monotonic(&clk_data, ts, rato_hwcnt);
		} else if (clk == CLOCK_REALTIME_COARSE) {
			get_realtime_coarse(&clk_data, ts);
		} else if (clk == CLOCK_MONOTONIC_COARSE) {
			get_monotonic_coarse(&clk_data, ts);
		} else if (clk == CLOCK_BOOTTIME) {
			get_boottime(&clk_data, ts, rato_hwcnt);
		} else {
			ret = gettime_syscall(clk, (long)(uintptr_t)ts);
		}
	} else {
		/* if return E_HM_INVAL, will not go to the syscall */
		ret = gettime_syscall(clk, (long)(uintptr_t)ts);
	}

	return ret;
}
