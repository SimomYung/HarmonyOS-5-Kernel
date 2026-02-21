/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Header file of vtimekeep
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 08 09:58:11 2020
 */

#ifndef UAPI_HMKERNEL_VTIMEKEEP_H
#define UAPI_HMKERNEL_VTIMEKEEP_H

#include <hmasm/types.h>

#define __VTIMEKEEP_U64_MAX	0xffffffffffffffffull
#define __VTIMEKEEP_U64_HALF	(__VTIMEKEEP_U64_MAX >> 1u)

/* clock id compatible with posix */
#define VTIMEKEEP_CLOCK_REALTIME		0u
#define VTIMEKEEP_CLOCK_MONOTONIC		1u
#define VTIMEKEEP_CLOCK_PROCESS_CPUTIME_ID	2u
#define VTIMEKEEP_CLOCK_THREAD_CPUTIME_ID	3u
#define VTIMEKEEP_CLOCK_MONOTONIC_RAW		4u
#define VTIMEKEEP_CLOCK_REALTIME_COARSE		5u
#define VTIMEKEEP_CLOCK_MONOTONIC_COARSE	6u
#define VTIMEKEEP_CLOCK_BOOTTIME		7u
#define VTIMEKEEP_CLOCK_REALTIME_ALARM		8u
#define VTIMEKEEP_CLOCK_BOOTTIME_ALARM		9u
#define VTIMEKEEP_CLOCK_POWEROFF_ALARM		12u

#define VTIMEKEEP_MAX_ADJ_MULT_SHIFT	11
#define VTIMEKEEP_ADJ_MULT_PERCENT	100

#define MAX_TZ_MINUTESWEST		(15*60)
#define MIN_TZ_MINUTESWEST		(-15*60)

struct vtimekeep_tock_policy {
	__u64 sec_length;
	__u32 mul;
	__u32 mov;
	/* generally, adjtime_num is -1 */
	__s64 adjtime_num;
};

#endif
