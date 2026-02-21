/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Native interfaces of timer-related operation
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 23 15:55:48 2019
 */

#include "hm_timer_internal.h"
#include "hm_timer_utils.h"

#include <stdbool.h>
#include <syscall.h>
#include <sys/time.h>
#include <hmasm/lsyscall.h>
#include <libhmsrv_sys/hm_vdso.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libsysif/sysmgr/api.h>
#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>
#include <errno.h>
#include <vfs.h>
#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/pac.h>
#endif

#define __nsecs_to_jiffies(t)	\
	(((t) + NSEC_PER_SEC / __UL(HZ) - 1UL) / (NSEC_PER_SEC / __UL(HZ)))

int hm_nanosleep(const struct timespec *req, struct timespec *rem)
{
	int err;

	err = (int)lsyscall_syscall2(__NR_nanosleep, (long)(uintptr_t)req,
				     (long)(uintptr_t)rem);
	if (err < 0) {
		err = posix2hmerrno(-err);
	}

	return err;
}

#ifdef PAC_FWCFI_SYSSERVICE
static __ro_after_init
int (*hm_vdso_get_time_sym_fun_pac)(clockid_t, struct timespec *) = NULL;

static int hm_clock_gettime_udata(clockid_t clk_id, struct timespec *time_spec)
{
	int err = E_HM_NOSYS;
	if (hm_vdso_get_time_sym_fun_pac) {
		err = hm_vdso_get_time_sym_fun_pac(clk_id, time_spec);
	}
	return err;
}

__PAC_INIT_SECTION
int hm_clock_gettime_init(void)
{
	void *fun_addr = hm_lookup_vdso_sym(HM_VDSO_GET_TIME_SYM);
	int (*hm_vdso_get_time_sym_fun_pac_addr)(clockid_t, struct timespec *) =
		(int (*)(clockid_t, struct timespec *)) fun_addr;

	PAC_FECFI_SIGN(hm_vdso_get_time_sym_fun_pac_addr, typeof(hm_vdso_get_time_sym_fun_pac_addr));

	hm_vdso_get_time_sym_fun_pac = hm_vdso_get_time_sym_fun_pac_addr;
	return E_HM_OK;
}
#else

static int (*hm_vdso_get_time_sym_fun)(clockid_t, struct timespec *) = NULL;

static int hm_clock_gettime_udata_maybe_lookup(clockid_t clk_id, struct timespec *time_spec)
{
	int err = E_HM_OK;

	if (!hm_vdso_get_time_sym_fun) {
		void *fun_addr = hm_lookup_vdso_sym(HM_VDSO_GET_TIME_SYM);
		hm_vdso_get_time_sym_fun = (int (*)(clockid_t, struct timespec *)) fun_addr;
		if (hm_vdso_get_time_sym_fun == NULL) {
			err = E_HM_NOSYS;
		}
	}
	if (err == E_HM_OK) {
		err = hm_vdso_get_time_sym_fun(clk_id, time_spec);
	}

	return err;
}

static int hm_clock_gettime_udata(clockid_t clk_id, struct timespec *time_spec)
{
	return hm_clock_gettime_udata_maybe_lookup(clk_id, time_spec);
}
int hm_clock_gettime_init(void)
{
	return E_HM_OK;
}
#endif

int hm_clock_gettime(clockid_t clk_id, struct timespec *time_spec)
{
	int err;

	if (time_spec == NULL) {
		err = E_HM_POSIX_FAULT;
	} else {
		err = hm_clock_gettime_udata(clk_id, time_spec);
		if (err != E_HM_OK) {
			err = (int)lsyscall_syscall2(__NR_clock_gettime, clk_id,
					(long)(uintptr_t)time_spec);
			if (err < 0) {
				err = posix2hmerrno(-err);
			}
		}
	}

	return err;
}

time_t hm_time(time_t *t)
{
	struct timespec ts;
	long ret = (long)hm_clock_gettime(CLOCK_REALTIME, &ts);
	if (ret == (long)E_HM_OK) {
		ret = ts.tv_sec;
		if (t != NULL) {
			if (safe_copy(t, &(ts.tv_sec), sizeof(time_t)) != 0) {
				ret = E_HM_POSIX_FAULT;
			}
		}
	}
	return ret;
}

#define SEC_TIMES_USEC     1000000L
#define USEC_TIMES_NSEC    1000L
int hm_usleep(unsigned int useconds)
{
	struct timespec tv = {
		.tv_sec = (long)(unsigned long)useconds / SEC_TIMES_USEC,
		.tv_nsec = ((long)(unsigned long)useconds % SEC_TIMES_USEC) * USEC_TIMES_NSEC
	};
	return hm_nanosleep(&tv, &tv);
}

int hm_gettimeofday(struct timeval *restrict tv, const void *restrict tz)
{
	int err;

	err = (int)lsyscall_syscall2(__NR_gettimeofday, (long)(uintptr_t)tv,
				     (long)(uintptr_t)tz);
	if (err < 0) {
		err = posix2hmerrno(-err);
	}

	return err;
}

int hm_clock_settime(clockid_t clk_id, const struct timespec *time_spec)
{
	int err;

	err = (int)lsyscall_syscall2(__NR_clock_settime, (long)clk_id,
				     (long)(uintptr_t)time_spec);
	if (err < 0) {
		err = posix2hmerrno(-err);
	}

	return err;
}

int hm_settimeofday(const struct timeval *tv, const struct __ktimezone_s *tz)
{
	int err;

	err = (int)lsyscall_syscall2(__NR_settimeofday, (long)(uintptr_t)tv, (long)(uintptr_t)tz);
	if (err < 0) {
		err = posix2hmerrno(-err);
	}

	return err;
}

int hm_adjtimex(struct timex *buf)
{
	int err;

	err = (int)lsyscall_syscall1(__NR_adjtimex, (long)(uintptr_t)buf);
	if (err < 0) {
		err = posix2hmerrno(-err);
	}

	return err;
}

int hm_setitimer(int which, const struct itimerval *value, struct itimerval *old)
{
	int err;

	err = (int)lsyscall_syscall3(__NR_setitimer, (long)which,
				(long)(uintptr_t)value,
				(long)(uintptr_t)old);
	if (err < 0) {
		err = posix2hmerrno(-err);
	}

	return err;
}

int hm_getitimer(int which, struct itimerval *curr_value)
{
	int err;

	err = (int)lsyscall_syscall2(__NR_getitimer, (long)which,
				(long)(uintptr_t)curr_value);
	if (err < 0) {
		err = posix2hmerrno(-err);
	}

	return err;
}

int hm_kernel_timer_create(clockid_t clk_id, struct ksigevent *ksev, int *timerid)
{
	int err;

	err = (int)lsyscall_syscall3(__NR_timer_create, (long)clk_id,
				(long)(uintptr_t)ksev,
				(long)(uintptr_t)timerid);
	if (err < 0) {
		err = posix2hmerrno(-err);
	}

	return err;
}

int hm_timer_create(clockid_t clk_id, const struct sigevent *sev, int *timerid)
{
	int ret;
	struct ksigevent ksev;

	mem_zero_s(ksev);

	if (sev == NULL || sev->sigev_notify == SIGEV_NONE ||
	    sev->sigev_notify == SIGEV_SIGNAL) {
		if (sev != NULL) {
			ksev.sigev_value = sev->sigev_value;
			ksev.sigev_signo = sev->sigev_signo;
			ksev.sigev_notify = sev->sigev_notify;
			ksev.sigev_tid = 0;
		}
		ret = hm_kernel_timer_create(clk_id, &ksev, timerid);
	} else if (sev->sigev_notify == SIGEV_THREAD) {
		ret = utimer_create_sigev_thread(clk_id, sev, timerid);
	} else {
		ret = E_HM_INVAL;
	}

	return ret;
}

int hm_timer_settime(int timerid, int flags,
		     const struct itimerspec *value, struct itimerspec *old)
{
	int ret;

	if (timerid <= (int)POSIX_TIMER_MAX_NUM) {
		ret = (int)lsyscall_syscall4(__NR_timer_settime, (long)timerid,
					(long)flags, (long)(uintptr_t)value,
					(long)(uintptr_t)old);
		if (ret < 0) {
			ret = posix2hmerrno(-ret);
		}
	} else if (timerid <= (int)UTIMER_MAX_NUM) {
		ret = utimer_settime_kernel(timerid, flags, value, old);
	} else {
		ret = E_HM_INVAL;
	}

	return ret;
}

int hm_timer_gettime(int timerid, struct itimerspec *value)
{
	int ret;

	if (timerid <= (int)POSIX_TIMER_MAX_NUM) {
		ret = (int)lsyscall_syscall2(__NR_timer_gettime, (long)timerid,
					(long)(uintptr_t)value);
		if (ret < 0) {
			ret = posix2hmerrno(-ret);
		}
	} else if (timerid <= (int)UTIMER_MAX_NUM) {
		if (value != NULL) {
			ret = utimer_gettime_kernel(timerid, value);
		} else {
			ret = E_HM_POSIX_FAULT;
		}
	} else {
		ret = E_HM_INVAL;
	}

	return ret;
}

int hm_timer_getoverrun(int timerid, unsigned long long *count)
{
	int err;
	long res;
	unsigned long long count_copy;

	if (timerid <= (int)POSIX_TIMER_MAX_NUM) {
		res = lsyscall_syscall1(__NR_timer_getoverrun, (long)timerid);
		if (res >= 0) {
			count_copy = (unsigned long long)(long long)res;
			err = E_HM_OK;
		} else {
			err = posix2hmerrno((int)(-res));
		}
	} else if (timerid <= (int)UTIMER_MAX_NUM) {
		err = utimer_getoverrun_kernel(timerid, &count_copy);
	} else {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (safe_copy((void *)count, (void *)&count_copy,
			      sizeof(unsigned long long)) != E_HM_OK) {
			err = E_HM_POSIX_FAULT;
		}
	}

	return err;
}

int hm_timer_delete(int timerid)
{
	int ret;

	if (timerid <= (int)POSIX_TIMER_MAX_NUM) {
		ret = (int)lsyscall_syscall1(__NR_timer_delete, (long)timerid);
		if (ret < 0) {
			ret = posix2hmerrno(-ret);
		}
	} else if (timerid <= (int)UTIMER_MAX_NUM) {
		ret = utimer_delete_kernel(timerid);
	} else {
		ret = E_HM_INVAL;
	}

	return ret;
}

/*
 * Sync version of timer delete will wait for the handler function completing
 * when handler function and delete function running in the same time.
 * And returns E_HM_BUSY if handler function is still running after waiting a predfined amount of usecs.
 * Only utimer support sync version of delete.
 */
int hm_timer_delete_sync(int timerid)
{
	int ret;

	if (timerid <= (int)POSIX_TIMER_MAX_NUM) {
		ret = E_HM_INVAL;
	} else if (timerid <= (int)UTIMER_MAX_NUM) {
		ret = utimer_delete_kernel_sync(timerid);
	} else {
		ret = E_HM_INVAL;
	}

	return ret;
}

int hm_timer_next_utimer_pid_and_name(int *pid_rp, char *name_rp, int name_size)
{
	int err = E_HM_OK;
	int name_real_size = name_size;

	if (pid_rp == NULL || name_rp == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (name_real_size > (int)UTIMER_OWNER_NAME_LEN) {
			name_real_size = (int)UTIMER_OWNER_NAME_LEN;
		}
		err = utimer_next_alarm_utimer_pid_and_name(pid_rp, name_rp, name_real_size);
	}

	return err;
}

int hm_clock_nanosleep(clockid_t clk_id, int abs_flag,
		       const struct timespec *req, struct timespec *rem)
{
	int err;

	err = (int)lsyscall_syscall4(__NR_clock_nanosleep, (long)clk_id,
				     (long)abs_flag, (long)(uintptr_t)req,
				     (long)(uintptr_t)rem);
	if (err < 0) {
		err = posix2hmerrno(-err);
	}

	return err;
}

int hm_clock_getres(clockid_t clk_id, struct timespec *time_spec)
{
	int err;

	err = (int)lsyscall_syscall2(__NR_clock_getres, (long)clk_id,
				(long)(uintptr_t)time_spec);
	if (err < 0) {
		err = posix2hmerrno(-err);
	}

	return err;
}

long hm_times(struct tms *tm)
{
	int err = E_HM_OK;

	if (tm == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = (int)lsyscall_syscall1(__NR_times, (long)(uintptr_t)tm);
		if (err < 0) {
			err = posix2hmerrno(-err);
		}
	}

	return err;
}

#ifdef CONFIG_DOPRA_TICK
long hm_dopra_tick(unsigned long long *ticks)
{
	int err;

	err = (int)lsyscall_syscall1(__NR_dopra_tick, (long)(uintptr_t)ticks);
	if (err < 0) {
		err = posix2hmerrno(-err);
	}

	return err;
}

#endif

__u64 hm_nsecs_to_jiffies(__u64 n)
{
	return (__u64)__nsecs_to_jiffies(n);
}
