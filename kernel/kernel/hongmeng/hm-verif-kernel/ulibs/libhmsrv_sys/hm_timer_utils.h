/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interconversion of tock/timespec/timeval
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 18 21:44:10 2019
 */
#ifndef _ULIBS_LIBHMSRV_SYS_HM_TIMER_UTILS_H_
#define _ULIBS_LIBHMSRV_SYS_HM_TIMER_UTILS_H_

#include <sys/time.h>
#include <inttypes.h>
#include <hongmeng/syscall.h>

/*
 * SIGTIMER is defined in src/internal/pthread_impl.h as a internal
 * header file for musl
 */
#define SIGTIMER		32
#define SIGEV_THREAD_ID		4

/*
 * Enable CLOCK_POWEROFF_ALARM adapt with HarmonyOS LDK
 */
#define CLOCK_POWEROFF_ALARM 12

int timespec_to_tocks(const struct timespec *ts, __u64 *tocks);
int tocks_to_timeval(uint64_t tocks, struct timeval *tv);
int values_to_timespec(uint64_t value, struct timespec *ts);

#endif
