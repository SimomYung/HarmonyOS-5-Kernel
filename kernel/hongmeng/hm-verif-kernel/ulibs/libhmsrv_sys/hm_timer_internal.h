/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Internal interfaces of timer-related operation
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 18 21:44:10 2019
 */
#ifndef _ULIBS_LIBHMSRV_SYS_HM_TIMER_INTERNAL_H_
#define _ULIBS_LIBHMSRV_SYS_HM_TIMER_INTERNAL_H_

#include <time.h>
#include <sys/time.h>
#include <sys/timex.h>
#include <sys/times.h>
#include <time/vdso.h>
#include <hmasm/param.h>
#include <hmkernel/mm/udata.h>
#include <libhmsrv_sys/hm_timer.h>

#define ADD_OFFSET	0
#define SUB_OFFSET	1

/* Minimum update interval in seconds */
#define UPDATE_MINSEC	256
/* Maximum update interval in seconds */
#define UPDATE_MAXSEC	2048


#define TIME_MAX_TOCK ((~0ULL) - 1ULL)

#define SETITIMER_PROCESS_FLAG		0
#define SETITIMER_THREAD_FLAG		1

/* kernel space timer */
int do_nanosleep_kernel(const struct timespec *req, struct timespec *rem);
int do_clock_nanosleep_kernel(clockid_t clk_id, int abs_flag,
			      const struct timespec *req,
			      struct timespec *rem);
int do_clock_gettime_kernel(clockid_t clk_id, struct timespec *time);
int do_adjtimex_kernel(struct timex *tx);
int do_setitimer_kernel(int which, const struct itimerval *value, struct itimerval *old);
int do_getitimer_kernel(int which, struct itimerval *value);
int utimer_create_sigev_thread(clockid_t clk_id, const struct sigevent *sev, int *timerid);
int utimer_settime_kernel(int timerid, int flags, const struct itimerspec *value, struct itimerspec *old);
int utimer_gettime_kernel(int timerid, struct itimerspec *value);
int utimer_getoverrun_kernel(int timerid, unsigned long long *count);
int utimer_delete_kernel(int timerid);
int utimer_delete_kernel_sync(int timerid);
int utimer_next_alarm_utimer_pid_and_name(int *pid_rp, char *name_rp, int name_size);

/*
 * The tid/pid is stored in the top bits, but negated.
 * Bit[1:0]: clock type (0 = PROF, 1 = VIRT, 2 = SCHED, 3 = FD).
 * Bit 2: 1 = thread, 0 = process.
 */
#define CPUCLOCK_TYPE_MASK	0x3u
#define CLOCK_THREAD_MASK	0x4u
#define CLOCK_ID_MASK		(CPUCLOCK_TYPE_MASK | CLOCK_THREAD_MASK)
#define CLOCK_FD		0x3u

#define CPUCLOCK_PROF		0x0u
#define CPUCLOCK_VIRT		0x1u
#define CPUCLOCK_SCHED		0x2u
#define CPUCLOCK_MAX		0x3u

static inline bool is_clock_fd(clockid_t clk_id)
{
	return (clk_id >= 0) ? false : (((unsigned int)clk_id & CLOCK_ID_MASK) == CLOCK_FD);
}

/* clk_id is encoded as (-pid - 1) * 8 + 2 */
static inline int clockid_to_pid(clockid_t clk_id)
{
	return -(clk_id / 8); /* divide 8 to decode */
}

/* clk_id is encoded as (-tid - 1) * 8 + 6 */
static inline int clockid_to_tid(clockid_t clk_id)
{
	return -(clk_id / 8); /* divide 8 to decode */
}

static inline __u64 time_tock_add_safe(__u64 ltock, __u64 rtock)
{
	__u64 sum = ltock + rtock;

	if (sum < ltock || sum < rtock) {
		sum = TIME_MAX_TOCK;
	}

	return sum;
}

static inline int timespec_nsec_check(const struct timespec *timespec)
{
	int err = E_HM_OK;

	/* The value of tv_nsec must be in [0, NSEC_PER_SEC). */
	if (timespec->tv_nsec < 0 || timespec->tv_nsec >= NSEC_PER_SEC) {
		err = E_HM_INVAL;
	}

	return err;
}

#endif
