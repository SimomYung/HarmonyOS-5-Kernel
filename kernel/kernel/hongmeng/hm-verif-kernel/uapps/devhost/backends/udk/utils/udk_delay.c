/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This is the implementation of udk delay related interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 26 15:14:11 2019
 */
#include <udk/delay.h>

#include <internal/init.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhwsecurec/securec.h>

#define UDK_USEC_PER_MSEC	1000
/* Use 100ms to adjust loops_per_us */
#define UDK_INIT_TIME_USEC	(100 * UDK_USEC_PER_MSEC)

static unsigned long loops_per_us = 1000;

static inline void udk_loop_delay(unsigned long count)
{
	unsigned long cnt = count;

	while (cnt != 0) {
		asm volatile ("");
		cnt--;
	}
}

static inline unsigned long udk_sleep_on_delay(unsigned long time, unsigned long sec_unit)
{
	unsigned long t = time;

	if (t > sec_unit) {
		struct timespec req;
		req.tv_sec = (time_t)(t / sec_unit);
		req.tv_nsec = 0;
		(void)hm_nanosleep(&req, NULL);
		t %= sec_unit;
	}

	return t;
}

void udk_udelay(unsigned long us)
{
	us = udk_sleep_on_delay(us, USEC_PER_SEC);
	udk_loop_delay(us * loops_per_us);
}

void udk_mdelay(unsigned long ms)
{
	ms = udk_sleep_on_delay(ms, MSEC_PER_SEC);
	udk_loop_delay(ms * UDK_USEC_PER_MSEC * loops_per_us);
}

void udk_ndelay(unsigned long ns)
{
	ns = udk_sleep_on_delay(ns, NSEC_PER_SEC);
	udk_loop_delay(ns * loops_per_us / NSEC_PER_USEC);
}

static int udk_delay_init(void)
{
	struct timespec prev, now;
	unsigned long total_us;

	mem_zero_s(prev);
	mem_zero_s(now);
	(void)hm_clock_gettime(CLOCK_MONOTONIC, &prev);
	udk_udelay(UDK_INIT_TIME_USEC);
	(void)hm_clock_gettime(CLOCK_MONOTONIC, &now);

	total_us = (unsigned long)((now.tv_sec - prev.tv_sec) * NSEC_PER_SEC +
				   now.tv_nsec - prev.tv_nsec) / NSEC_PER_USEC;
	loops_per_us = loops_per_us * UDK_INIT_TIME_USEC / total_us;
	/* fallback to 1 if cpu is too slow */
	if (loops_per_us == 0) {
		loops_per_us = 1;
	}
	return 0;
}
udk_init_call(udk_delay_init);
