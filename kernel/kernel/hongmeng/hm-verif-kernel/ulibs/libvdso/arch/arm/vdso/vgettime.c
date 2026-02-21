/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Vgettime
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 28 09:49:33 2019
 */
#include <time.h>
#include <errno.h>
#include <syscall.h>
#include <sys/time.h>
#include <hmasm/types.h>
#include <hmasm/processor.h>
#include <hmkernel/barrier.h>
#include <hmkernel/compiler.h>

#include "vdso.h"

static void clock_add_base_time(struct timespec *ts,
				__s64 base_sec, __u32 base_nsec)
{
	ts->tv_sec += (time_t)base_sec;

	base_nsec += (__u32)ts->tv_nsec;
	while (base_nsec >= __NSEC_PER_SEC) {
		base_nsec -= __NSEC_PER_SEC;
		ts->tv_sec++;
	}

	ts->tv_nsec = (long)base_nsec;
}

static int clock_read_realtime_coarse(const struct kshare_udata_s *sdata,
				      struct timespec *ts)
{
	__u64 m_nsec;
	__u32 freq;
	__s64 base = 0;
	__s64 base_sec = 0;
	__u32 base_nsec = 0;
	__u32 seq;
	int ret = E_HM_OK;

	do {
		seq = __seqcnt_start_read(&sdata->tock_seq);

		if (sdata->tock_offset != 0) {
			base = sdata->tock_offset;
			base_sec = sdata->tock_base_sec;
			base_nsec = sdata->tock_base_nsec;
		}

		ts->tv_sec = (time_t)sdata->tock_mono_sec;

		freq = sdata->freq;
		m_nsec = sdata->tock_mono_nsec;
	} while (__seqcnt_retry_read(&sdata->tock_seq, seq) != 0);

	/*
	 * If
	 * - freq is zero, or
	 * - base_{sec, nsec} == 0 but base != 0
	 * it means the clock is not inited.
	 */
	if (freq == 0U ||
	    (base != 0 && base_sec == 0 && base_nsec == 0U)) {
		ret = -ENOSYS;
	} else {
		ts->tv_nsec = (long)m_nsec;
		if (base != 0) {
			clock_add_base_time(ts, base_sec, base_nsec);
		}
	}

	return ret;
}

static int clock_read_monotime_coarse(const struct kshare_udata_s *sdata,
				      struct timespec *ts)
{
	__u32 freq;
	__u64 sec;
	__u64 nsec;
	__u32 seq;
	int ret;

	do {
		seq = __seqcnt_start_read(&sdata->tock_seq);

		freq = sdata->freq;
		sec = sdata->tock_mono_sec;
		nsec = sdata->tock_mono_nsec;
	} while (__seqcnt_retry_read(&sdata->tock_seq, seq) != 0);

	/* if freq is zero, it means the clock is not inited. */
	if (freq == 0U) {
		ret = -ENOSYS;
	} else {
		ts->tv_sec = (long)sec;
		ts->tv_nsec = (long)nsec;
		ret = 0;
	}

	return ret;
}

static int gettime_syscall(long clk, long _ts)
{
	register long clkid asm("r0") = clk;
	register long ts asm("r1") = _ts;
	register long ret asm ("r0");
	register long nr asm("r7") = __NR_clock_gettime;

	asm volatile(
	"svc #0\n"
	: "=r" (ret)
	: "r" (clkid), "r" (ts), "r" (nr)
	: "memory");

	return ret;
}

int __vdso_clock_gettime(clockid_t clk, struct timespec *ts)
{
	struct kshare_udata_s *vdata = NULL;
	int ret = E_HM_OK;

	vdata = (struct kshare_udata_s*)__vdso_get_data();
	if (vdata == NULL) {
		ret = gettime_syscall(clk, (long)ts);
	} else {
		/* 1210 only support coarse clk_id */
		switch (clk) {
		case CLOCK_REALTIME_COARSE:
			ret = clock_read_realtime_coarse(vdata, ts);
			break;
		case CLOCK_MONOTONIC_COARSE:
			ret = clock_read_monotime_coarse(vdata, ts);
			break;
		default:
			ret = gettime_syscall(clk, (long)ts);
			break;
		}
	}

	return ret;
}
