/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: User interface of timer related function
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 20 05:35:28 2019
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_TIMER_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_TIMER_H

#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <inttypes.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/timex.h>
#include <lib/dlist.h>
#include <libalgo/rbtree.h>
#include <hongmeng/syscall.h>
#include <hmkernel/vtimekeep.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhwsecurec/securec.h>

#define NSEC_PER_SEC		1000000000
#define USEC_PER_SEC		1000000L
#define USEC_PER_MSEC		1000
#define NSEC_PER_USEC		1000
#define MSEC_PER_SEC		1000
#define NSEC_PER_MSEC		1000000

#define MAX_TOCK_OFFSET		(__VTIMEKEEP_U64_HALF - 1ULL)

/* NOTE: Remain problem: The number of posix timer is temperorily set to 512 */
#define POSIX_TIMER_MAX_NUM	(__U(1) << 9)
/* NOTE: The number of utimer is temperorily set to 1536 (2048 - 512) */
#define UTIMER_MAX_NUM		(__U(1) << 12)

#define UTIMER_OWNER_NAME_LEN	0x10

/* This is to conform to sigevent struct passed from musl libc */
struct ksigevent {
	union sigval sigev_value;
	int sigev_signo;
	int sigev_notify;
	int sigev_tid;
};

struct compat_ksigevent_s {
	int32_t sigev_value;
	int32_t sigev_signo;
	int32_t sigev_notify;
	int32_t sigev_tid;
};

struct compat_timeval_s {
	int32_t tv_sec;
	int32_t tv_usec;
};

struct compat_itimerval_s {
	struct compat_timeval_s it_interval;
	struct compat_timeval_s it_value;
};

struct compat_timespec_s {
	int32_t tv_sec;
	int32_t tv_nsec;
};

struct compat_itimerspec_s {
	struct compat_timespec_s it_interval;
	struct compat_timespec_s it_value;
};

struct compat_tms_s {
	int32_t tms_utime;
	int32_t tms_stime;
	int32_t tms_cutime;
	int32_t tms_cstime;
};

#define COMPAT_TIMEX_PADDING_SIZE    11
struct compat_timex_s {
	unsigned modes;
	int32_t offset, freq, maxerror, esterror;
	int status;
	int32_t constant, precision, tolerance;
	struct compat_timeval_s time;
	int32_t tick, ppsfreq, jitter;
	int shift;
	int32_t stabil, jitcnt, calcnt, errcnt, stbcnt;
	int tai;
	int __padding[COMPAT_TIMEX_PADDING_SIZE];
};

enum clock_type {
	CLOCK_KERNEL  = 0,
	CLOCK_PMU,
};

enum utimer_state {
	UTIMER_DELETING,
	UTIMER_ON_LIST,
	UTIMER_ON_TREE,
};

enum utimer_pool_state {
	UTIMER_POOL_FREE,
	UTIMER_POOL_WAIT,
	UTIMER_POOL_TIMEDWAIT,
};

enum utimer_clock_type {
	UTIMER_REALTIME,
	UTIMER_MONOTONIC,
	UTIMER_BOOTTIME,
	UTIMER_CLOCK_MAX,
};

int hm_nanosleep(const struct timespec *req, struct timespec *rem);
int hm_clock_nanosleep(clockid_t clk_id, int abs_flag,
		       const struct timespec *req, struct timespec *rem);
int hm_clock_gettime_init(void);
int hm_clock_gettime(clockid_t clk_id, struct timespec *time_spec);
int hm_clock_settime(clockid_t clk_id, const struct timespec *time_spec);
int hm_settimeofday(const struct timeval *tv, const struct __ktimezone_s *tz);
int hm_adjtimex(struct timex *buf);

int hm_getitimer(int which, struct itimerval *curr_value);
int hm_setitimer(int which, const struct itimerval *value, struct itimerval *old);

int hm_kernel_timer_create(clockid_t clk_id, struct ksigevent *evp, int *timerid);
int hm_timer_create(clockid_t clk_id, const struct sigevent *sev, int *timerid);
int hm_timer_settime(int timerid, int flags,
		     const struct itimerspec *value, struct itimerspec *old);
int hm_timer_delete(int timerid);
int hm_timer_delete_sync(int timerid);
int hm_clock_getres(clockid_t clk_id, struct timespec *time_spec);
time_t hm_time(time_t *t);
int hm_usleep(unsigned int useconds);
int hm_gettimeofday(struct timeval *restrict tv, const void *restrict tz);
int hm_timer_gettime(int timerid, struct itimerspec *value);
int hm_timer_getoverrun(int timerid, unsigned long long *count);
long hm_times(struct tms *tm);
int hm_timer_next_utimer_pid_and_name(int *pid_rp, char *name_rp, int name_size);

#ifdef CONFIG_DOPRA_TICK
long hm_dopra_tick(unsigned long long *ticks);
#endif

static inline int hmk_timespec_to_nsec(const struct timespec *ts, __u64 *nsec)
{
	int err = E_HM_OK;
	if (ts == NULL) {
		err = E_HM_FAULT;
	} else if (ts->tv_nsec < 0 || ts->tv_nsec >= NSEC_PER_SEC || ts->tv_sec < 0) {
		err = E_HM_INVAL;
	} else {
		*nsec = (__u64)ts->tv_sec * NSEC_PER_SEC + ts->tv_nsec;
	}
	return err;
}

static inline int hmk_timespec_diff_nsec(const struct timespec *lhs,
					 const struct timespec *rhs,
					 __u64 *diff)
{
	int err = E_HM_OK;
	__u64 lhs_nsec, rhs_nsec;

	err = hmk_timespec_to_nsec(lhs, &lhs_nsec);
	if (err == E_HM_OK) {
		err = hmk_timespec_to_nsec(rhs, &rhs_nsec);
	}
	if (err == E_HM_OK) {
		*diff = rhs_nsec - lhs_nsec;
	}

	return err;
}

int timespec_to_values(const struct timespec *ts, __u64 *value);
int timespec_to_values_udata(const struct timespec *ts, __u64 *value);
int tocks_to_timespec(uint64_t tocks, struct timespec *ts);
int tocks_to_timespec_udata(uint64_t tocks, struct timespec *ts);
void mul_mov_cal(__u64 *mul, __u32 *mov, __u64 pa, __u64 pb);

static inline bool timespec_valid(const struct timespec *tsp)
{
	bool ret = true;

	if (tsp != NULL) {
		if (tsp->tv_sec < 0 || tsp->tv_nsec < 0 ||
		    (unsigned long)tsp->tv_nsec >= NSEC_PER_SEC) {
			ret = false;
		}
	}
	return ret;
}

static inline bool timeval_valid(const struct timeval *tvp)
{
	bool ret = true;

	if (tvp != NULL) {
		if (tvp->tv_sec < 0 || tvp->tv_usec < 0 ||
		    tvp->tv_usec >= USEC_PER_SEC) {
			ret = false;
		}
	}
	return ret;
}

/* milliseconds to struct timespec */
static inline void msec_to_timespec(uint64_t msec, struct timespec *tsp)
{
	if (tsp != NULL) {
		tsp->tv_sec = (time_t)(msec / MSEC_PER_SEC);
		tsp->tv_nsec = (long)((msec % MSEC_PER_SEC) * NSEC_PER_MSEC);
	}
}

#define ULLONG_NSEC_MAX  (((~0ULL) / NSEC_PER_SEC) - 1ULL)
static inline __u64 tocks_to_ns(__u64 tocks)
{
	int err;
	__u64 ns = 0;
	struct timespec time = {0};

	err = tocks_to_timespec(tocks, &time);
	if (err == E_HM_OK) {
		if (time.tv_sec >= ULLONG_NSEC_MAX) {
			ns = ~0ULL;
		} else {
			ns = (__u64)(time.tv_sec * NSEC_PER_SEC + time.tv_nsec);
		}
	} else {
		hm_debug("tocks_to_ns failed, err=%s\n", hmstrerror(err));
	}
	return ns;
}

#define ULLONG_MSEC_MAX  (((~0ULL) / MSEC_PER_SEC) - 1ULL)
static inline __u64 tocks_to_ms(__u64 tocks)
{
	int err;
	__u64 ms = 0;
	struct timespec time = {0};

	err = tocks_to_timespec(tocks, &time);
	if (err == E_HM_OK) {
		if (time.tv_sec >= ULLONG_MSEC_MAX) {
			ms = ~0ULL;
		} else {
			ms = (__u64)(time.tv_sec * MSEC_PER_SEC + time.tv_nsec / NSEC_PER_MSEC);
		}
	} else {
		hm_debug("tocks_to_ms failed, err=%s\n", hmstrerror(err));
	}
	return ms;
}

__u64 hm_nsecs_to_jiffies(__u64 n);

#endif
