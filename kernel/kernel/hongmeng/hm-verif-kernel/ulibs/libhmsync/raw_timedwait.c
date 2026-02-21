/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Internal futex_wait interface with user-specified time and hmsignal mask
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 06 13:51:53 2019
 */

#include <libhmsrv_sys/hm_futex.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhwsecurec/securec.h>
#include "raw_thread_impl.h"

static int calc_timeout_val(clockid_t clk, const struct timespec *to, __u64 *timeout)
{
	int err = E_HM_OK;
	struct timespec now, *top = NULL;

	mem_zero_s(now);

	if (to->tv_nsec >= NSEC_PER_SEC || to->tv_nsec < 0) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = hm_clock_gettime(clk, &now);
		if (err != E_HM_OK) {
			hm_error("failed to get time: clk=%d, errno=%s\n", clk, hmstrerror(err));
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		now.tv_sec = to->tv_sec - now.tv_sec;
		if ((now.tv_nsec = to->tv_nsec - now.tv_nsec) < 0) {
			now.tv_sec--;
			now.tv_nsec += NSEC_PER_SEC;
		}

		if (now.tv_sec < 0) {
			err = E_HM_TIMEDOUT;
		}
	}

	if (err == E_HM_OK) {
		top = &now;

		err = timespec_to_values(top, timeout);
		if (err != 0) {
			hm_warn("time transfre failed\n");
		}
	}

	return err;
}

int raw_futex_timedwait(volatile int *futex, int val, clockid_t clk,
			const struct timespec *to, struct __hmsigmask *mask)
{
	int err = 0;
	__u64 timeout = (__u64)-1;

	if (to != NULL) {
		err = calc_timeout_val(clk, to, &timeout);
	}

	if (err == 0) {
		err = raw_timedwait(futex, val, &timeout, mask, FUTEX_TYPE_PRIVATE);
	}

	return err;
}
