/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Utils for timer
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 01 14:43:50 2019
 */
#include "hm_timer_utils.h"

#include <inttypes.h>
#include <time/vdso.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <hmkernel/mm/udata.h>
#include <libstrict/overflow.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhwsecurec/securec.h>
#include <libsysif/sysmgr/api.h>
#include <hmasm/param.h>

#define MIN_DELTA_TOCK        1ULL
#define MAX_FREQ_BIT          32UL /* max timer frequence we sopport is 4G(2^32) */
#define MAX_SHIFT_BIT         (64UL - MAX_FREQ_BIT)

#ifdef __HOST_LLT__

#define TIMER_KTOCK_FREQ      10000000ULL

static int timer_tock_freq(uint64_t *tock_freq)
{
	int err = E_HM_OK;
	int read_size;
	unsigned long ret_value = 0;

	read_size = hm_sysctrl_read_conf(__SYSCONF_CLOCK_FREQ,
					 (void *)&ret_value, sizeof(ret_value));
	if (read_size == (int)sizeof(ret_value) || read_size == 0) {
		*tock_freq = (ret_value == 0UL) ? TIMER_KTOCK_FREQ : (uint64_t)ret_value;
	} else {
		err = (read_size < 0) ? read_size : E_HM_INVAL;
	}

	return err;
}
#endif

/* equal to the calculate [tocks * NSEC_PER_SEC / tock_freq]
 * like this [tocks * (NSEC_PER_SEC << mov / tock_freq) >> mov)]
 * so the mul = NSEC_PER_SEC << mov / tock_freq, and this calculate
 * method is suitable when tock_freq < 4G(2^32).
 */
void mul_mov_cal(__u64 *mul, __u32 *mov, __u64 pa, __u64 pb)
{
	/* When we change the tock to timespec, at first, we should keep mul large,
	 * then the accuracy of nsec we get will be better. The largest mul_tmp is 2^64,
	 * and pb is NSEC_PER_SEC(~2^30), mov_tmp can be 34. But here we set mov_tmp 32 at first,
	 * this is also well and enough for the accuracy. The mov_st just refers to the mov_tmp.
	 */
	const __u32 mov_st = (__u32)MAX_SHIFT_BIT;
	__u64 mul_tmp;
	__u32 mov_tmp = (__u32)MAX_SHIFT_BIT;
	BUG_ON(pa == 0ULL);
	while (mov_tmp > 0U) {
		mul_tmp = pb << mov_tmp;
		/* just carry bit */
		mul_tmp += (pa >> 1);
		mul_tmp /= pa;
		if ((mul_tmp >> mov_st) == 0ULL) {
			break;
		}
		mov_tmp--;
	}
	*mul = mul_tmp;
	*mov = mov_tmp;
}

static int __tocks_to_timespec_udata(uint64_t mtocks, struct timespec *ts,
				     const struct kshare_udata_s *vdata)
{
	uint64_t sec;
	uint64_t nsec;
	uint64_t tocks = mtocks;
	uint64_t tock_freq = (uint64_t)vdata->freq;
	uint64_t vdata_freq = (uint64_t)vdata->sec_length;

#ifdef __HOST_LLT__
	int err = timer_tock_freq(&tock_freq);
	if (err != E_HM_OK) {
		hm_error("timer_tock_freq failed, err=%s\n", hmstrerror(err));
		return err;
	}
#endif

	if (tock_freq == vdata_freq || vdata_freq == 0 ||
	    (uint64_t)vdata->tock_mono_last > tocks) {
		BUG_ON(tock_freq == 0ULL);
		sec = tocks / tock_freq;
		tocks %= tock_freq;
		nsec = tocks * NSEC_PER_SEC / tock_freq;
	} else {
		sec = vdata->tock_mono_sec;
		nsec = vdata->tock_mono_nsec;
		tocks = tocks - (uint64_t)vdata->tock_mono_last;
		sec += tocks / vdata_freq;
		tocks %= vdata_freq;
		nsec += (tocks * (uint64_t)vdata->mul) >> vdata->mov;
		while (nsec >= NSEC_PER_SEC) {
			nsec -= NSEC_PER_SEC;
			sec++;
		}
	}

	ts->tv_sec = (time_t)sec;
	ts->tv_nsec = (long)nsec;
	return E_HM_OK;
}

/* This func is for platform which can provide kernel tock frequency */
int tocks_to_timespec_udata(uint64_t tocks, struct timespec *ts)
{
	int err = E_HM_OK;
	struct kshare_udata_s *vdata = NULL;

	BUG_ON(ts == NULL);

	vdata = (struct kshare_udata_s*)vdso_get_udata();
	if (vdata == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = __tocks_to_timespec_udata(tocks, ts, vdata);
	}

	return err;
}

static int __timespec_to_tock_udata(uint64_t ts_sec, uint64_t ts_nsec, __u64 *tocks)
{
	uint64_t sec;
	uint64_t nsec;
	uint64_t tock_freq;
	uint64_t vdata_freq;
	struct kshare_udata_s *vdata = NULL;

	vdata = (struct kshare_udata_s*)vdso_get_udata();
	if (vdata == NULL) {
		return E_HM_INVAL;
	}

	sec = vdata->tock_mono_sec;
	nsec = vdata->tock_mono_nsec;
	vdata_freq = (uint64_t)vdata->sec_length;
	tock_freq = (uint64_t)vdata->freq;

#ifdef __HOST_LLT__
	int err = timer_tock_freq(&tock_freq);
	if (err != E_HM_OK) {
		hm_error("timer_tock_freq failed, err=%s\n", hmstrerror(err));
		return err;
	}
#endif

	if ((vdata_freq != tock_freq) && (vdata_freq != 0) &&
	    ((ts_sec > sec) || (ts_sec == sec && ts_nsec > nsec))) {
		sec = ts_sec - sec;
		nsec = ts_nsec - nsec;
		while ((int64_t)nsec < 0) {
			nsec += NSEC_PER_SEC;
			sec--;
		}
		*tocks = vdata->tock_mono_last;
		*tocks += sec * vdata_freq;
		*tocks += (nsec * vdata_freq) / NSEC_PER_SEC;
	} else {
		*tocks = ts_sec * tock_freq;
		*tocks += ts_nsec * tock_freq / NSEC_PER_SEC;
	}
	return E_HM_OK;
}

int timespec_to_tocks(const struct timespec *ts, __u64 *tocks)
{
	int err = E_HM_OK;
	uint64_t sec;
	uint64_t nsec;
	struct kshare_udata_s *vdata = NULL;

	sec = (uint64_t)(unsigned long)ts->tv_sec;
	nsec = (uint64_t)(unsigned long)ts->tv_nsec;
	vdata = (struct kshare_udata_s*)vdso_get_udata();
	if (vdata == NULL) {
		return E_HM_INVAL;
	}

	if (sec == 0ULL && nsec == 0ULL) {
		*tocks = 0;
	} else {
		BUG_ON(vdata->freq == 0U);
		if (sec >= (MAX_TOCK_OFFSET / (uint64_t)vdata->freq)) {
			*tocks = (__u64)MAX_TOCK_OFFSET;
		} else {
			err = __timespec_to_tock_udata(sec, nsec, tocks);
			*tocks = (*tocks == 0ULL) ? MIN_DELTA_TOCK : *tocks;
		}
	}
	return err;
}

int timespec_to_values_udata(const struct timespec *ts, __u64 *value)
{
	int err = E_HM_OK;

	BUG_ON(value == NULL);
	if (ts == NULL) {
		*value = (__u64)(-1);
	} else if (ts->tv_nsec < 0 || ts->tv_nsec >= NSEC_PER_SEC || ts->tv_sec < 0) {
		err = E_HM_INVAL;
	} else {
		err = timespec_to_tocks(ts, value);
	}
	return err;
}

int values_to_timespec(uint64_t value, struct timespec *ts)
{
	return tocks_to_timespec_udata(value, ts);
}

int tocks_to_timeval(uint64_t tocks, struct timeval *tv)
{
	struct timespec ts = {0, 0};
	int err = E_HM_OK;

	if (tv == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = tocks_to_timespec_udata(tocks, &ts);
	}
	if (err == E_HM_OK) {
		tv->tv_sec = ts.tv_sec;
		tv->tv_usec = ts.tv_nsec / NSEC_PER_USEC;
	}

	return err;
}
