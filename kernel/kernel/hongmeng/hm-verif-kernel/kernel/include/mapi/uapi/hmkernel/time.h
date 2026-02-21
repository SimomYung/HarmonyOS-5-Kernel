/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Time related definition
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 18 14:57:31 2022
 */
#ifndef UAPI_HMKERNEL_TIME_H
#define UAPI_HMKERNEL_TIME_H

#include <hmkernel/types.h>

#define __MSEC_PER_SEC		1000
#define __USEC_PER_MSEC		1000
#define __USEC_PER_SEC		(__USEC_PER_MSEC * __MSEC_PER_SEC)
#define __NSEC_PER_USEC		1000
#define __NSEC_PER_MSEC		(__NSEC_PER_USEC * __USEC_PER_MSEC)
#define __NSEC_PER_SEC		(__NSEC_PER_USEC * __USEC_PER_SEC)

#define ALARMTIMER_OWNER_NAME_LEN		0x10

/* System clock is not synchronized to a reliable server by adjtimex */
#define ADJ_TIME_ERROR		5

struct __time_real_t {
	__s64 sec;
	__s64 nsec;
};

struct __time_mono_t {
	__s64 sec;
	__s64 nsec;
};

struct __rtc_time_s {
	__u64 sec;
	unsigned int enable;
	unsigned int pending;
	int pid;
	char name[ALARMTIMER_OWNER_NAME_LEN];
};

struct __ktimezone_s {
	__s32 tz_minuteswest;
	__s32 tz_dsttime;
};

enum __rtc_ops {
	__RTC_READ_TIME,
	__RTC_SET_TIME,
	__RTC_READ_ALARM,
	__RTC_SET_ALARM,
	__RTC_SET_IRQ,
	__RTC_READ_OFFSET,
	__RTC_READ_PID_AND_NAME,
};
static inline void __time_real_set_zero(struct __time_real_t *rt)
{
	rt->sec = 0LL;
	rt->nsec = 0LL;
}

static inline void __time_mono_set_zero(struct __time_mono_t *mt)
{
	mt->sec = 0LL;
	mt->nsec = 0LL;
}

#endif
