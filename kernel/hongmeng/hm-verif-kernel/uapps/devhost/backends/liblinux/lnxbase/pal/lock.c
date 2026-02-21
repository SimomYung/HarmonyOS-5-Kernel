/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for lock operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <liblinux/pal.h>

#include <assert.h>
#include <malloc.h>
#include <semaphore.h>
#include <hmkernel/capability.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmsync/raw_sem.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_thread_cond.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libhmsrv_sys/hm_timer.h>

#include <devhost/log.h>

#ifndef BUILD_BUG_ON
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*((!!(condition)) ? 1 : 0)]))
#endif

#ifndef LIBLINUX_PAL_FUTEX_UNINTERRUPTIBLE
#define LIBLINUX_PAL_FUTEX_UNINTERRUPTIBLE	1UL
#endif

#ifndef LIBLINUX_PAL_FUTEX_TIMEDOUT_DETECT
#define LIBLINUX_PAL_FUTEX_TIMEDOUT_DETECT	2UL
#endif

#ifndef LIBLINUX_PAL_FUTEX_LOCK_IN_LDK
#define LIBLINUX_PAL_FUTEX_LOCK_IN_LDK		4UL
#endif

#ifndef LIBLINUX_PAL_FUTEX_IOWAIT
#define LIBLINUX_PAL_FUTEX_IOWAIT		8UL
#endif

#ifndef LIBLINUX_PAL_FUTEX_MAX_TIMEOUT
#define LIBLINUX_PAL_FUTEX_MAX_TIMEOUT		~0ULL
#endif

#define LIBLINUX_STRUCT_CAST(ldk, hm)	\
static inline struct raw_##hm * ldk##_cast(struct liblinux_pal_##ldk##_t *data) \
{	\
	return ptr_to_type(data, struct raw_##hm *);	\
}

LIBLINUX_STRUCT_CAST(mutex, mutex)
LIBLINUX_STRUCT_CAST(cond, thread_cond)
LIBLINUX_STRUCT_CAST(sem, sem)
LIBLINUX_STRUCT_CAST(rwlock, rwlock)

#ifdef CONFIG_USER_FUTEX
int liblinux_pal_thread_block(volatile int *futex, int val,
				unsigned long long timeout, /* ns */
				unsigned long flags,
				const struct liblinux_wchan_info *wchan_info)
{
	struct __hmsigmask maskall = {(__u64)-1};
	struct __hmsigmask *mask = NULL;
	unsigned long long tock;
	unsigned long long *ptimeout = NULL;
	struct timespec ts;
	int ret;
	unsigned int __flags = FUTEX_TYPE_PRIVATE;
	struct __block_param_s block_param = { .fname = "devhost.elf" };

	if (timeout != LIBLINUX_PAL_FUTEX_MAX_TIMEOUT) {
		ptimeout = &tock;
		ts.tv_sec = (long)(timeout / NSEC_PER_SEC);
		ts.tv_nsec = (long)(timeout % NSEC_PER_SEC);
		(void)timespec_to_values(&ts, &tock);
	}

	if (____IS(flags & LIBLINUX_PAL_FUTEX_UNINTERRUPTIBLE)) {
		mask = &maskall;
	}

	if (____IS(flags & LIBLINUX_PAL_FUTEX_TIMEDOUT_DETECT)) {
		__flags |= FUTEX_BLOCKED_TIMEDOUT_DETECT;
	}

	if (____IS(flags & LIBLINUX_PAL_FUTEX_LOCK_IN_LDK)) {
		__flags |= (FUTEX_FLAG_LOCK_IN_LDK | FUTEX_BLOCKED_HAS_LOCK_OWNER);
	}

	if (____IS(flags & LIBLINUX_PAL_FUTEX_IOWAIT)) {
		__flags |= __FUTEX_FLAG_IOWAIT;
	}

	if (wchan_info) {
		block_param.sname = wchan_info->name;
		block_param.fname_size = strnlen(block_param.fname, SYM_MOD_NAME_SIZE - 1U);
		block_param.sname_size = strnlen(block_param.sname, SYM_FUNC_NAME_SIZE - 1U);
		block_param.offset = wchan_info->offset;
		block_param.size = wchan_info->size;

		ret = hm_thread_block(futex, val, ptimeout, mask, __flags, &block_param);
	} else {
		ret = hm_thread_block(futex, val, ptimeout, mask, __flags, NULL);
	}
	if (ret == E_HM_HMSIGINTR) {
		return -EINTR;
	}

	return -hmerrno2posix(ret);
}

int liblinux_pal_thread_unblock(unsigned long long tcb_cref, volatile int *futex, int sync)
{
	int ret;
	unsigned int flags_ex;

	flags_ex = (sync != 0) ? FUTEX_FLAG_YIELD_SOON : 0;
	ret = hm_thread_unblock(tcb_cref, futex, FUTEX_TYPE_PRIVATE | flags_ex);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return ret;
}
#endif

int liblinux_pal_futex_timedwait_ex(volatile int *futex, int val,
				    unsigned long long timeout, /* ns */
				    unsigned long flags,
				    const struct liblinux_wchan_info *wchan_info)
{
	struct __hmsigmask maskall = {(__u64)-1};
	struct __hmsigmask *mask = NULL;
	unsigned long long tock;
	unsigned long long *ptimeout = NULL;
	struct timespec ts;
	int ret;
	unsigned int __flags = FUTEX_TYPE_PRIVATE;
	struct __block_param_s block_param = { .fname = "devhost.elf" };

	if (timeout != LIBLINUX_PAL_FUTEX_MAX_TIMEOUT) {
		ptimeout = &tock;
		ts.tv_sec = (long)(timeout / NSEC_PER_SEC);
		ts.tv_nsec = (long)(timeout % NSEC_PER_SEC);
		(void)timespec_to_values(&ts, &tock);
	}

	if (____IS(flags & LIBLINUX_PAL_FUTEX_UNINTERRUPTIBLE)) {
		mask = &maskall;
	}

	if (____IS(flags & LIBLINUX_PAL_FUTEX_TIMEDOUT_DETECT)) {
		__flags |= FUTEX_BLOCKED_TIMEDOUT_DETECT;
	}

	if (____IS(flags & LIBLINUX_PAL_FUTEX_LOCK_IN_LDK)) {
		__flags |= (FUTEX_FLAG_LOCK_IN_LDK | FUTEX_BLOCKED_HAS_LOCK_OWNER);
	}

	if (____IS(flags & LIBLINUX_PAL_FUTEX_IOWAIT)) {
		__flags |= __FUTEX_FLAG_IOWAIT;
	}

	if (wchan_info) {
		block_param.sname = wchan_info->name;
		block_param.fname_size = strnlen(block_param.fname, SYM_MOD_NAME_SIZE - 1U);
		block_param.sname_size = strnlen(block_param.sname, SYM_FUNC_NAME_SIZE - 1U);
		block_param.offset = wchan_info->offset;
		block_param.size = wchan_info->size;

		ret = hm_futex_timedwait_ex(futex, val, ptimeout, mask, __flags,
					    FUTEX_BITSET_MATCH_ANY, 0x00UL, 0x00UL,
					    &block_param);
	} else {
		ret = hm_futex_timedwait(futex, val, ptimeout, mask, __flags);
	}
	if (ret == E_HM_HMSIGINTR) {
		return -EINTR;
	}

	return -hmerrno2posix(ret);
}

int liblinux_pal_futex_timedwait(volatile int *futex, int val,
				 unsigned long long timeout, /* ns */
				 unsigned long flags)
{
	return liblinux_pal_futex_timedwait_ex(futex, val, timeout, flags, NULL);
}

int liblinux_pal_futex_wake_ex(volatile int *futex, int sync)
{
	int ret;
	unsigned int flags_ex;

	flags_ex = (sync != 0) ? FUTEX_FLAG_YIELD_SOON : 0;
	ret = hm_futex_wake(futex, (unsigned int)-1, FUTEX_TYPE_PRIVATE | flags_ex);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return ret;
}

int liblinux_pal_futex_wake(volatile int *futex)
{
	return liblinux_pal_futex_wake_ex(futex, 0);
}

void liblinux_pal_mutex_init(struct liblinux_pal_mutex_t *mutex)
{
	BUILD_BUG_ON(sizeof(struct liblinux_pal_mutex_t) < sizeof(struct raw_mutex));
	raw_mutex_init(mutex_cast(mutex));
}

void liblinux_pal_mutex_destroy(struct liblinux_pal_mutex_t *mutex)
{
	raw_mutex_destroy(mutex_cast(mutex));
}

void liblinux_pal_mutex_lock(struct liblinux_pal_mutex_t *mutex)
{
	raw_mutex_lock(mutex_cast(mutex));
}

int  liblinux_pal_mutex_trylock(struct liblinux_pal_mutex_t *mutex)
{
	int ret;

	ret = raw_mutex_trylock(mutex_cast(mutex));
	return -hmerrno2posix(ret);
}

void liblinux_pal_mutex_unlock(struct liblinux_pal_mutex_t *mutex)
{
	raw_mutex_unlock(mutex_cast(mutex));
}

int liblinux_pal_mutex_is_locked(struct liblinux_pal_mutex_t *mutex)
{
	return raw_mutex_is_locked(mutex_cast(mutex));
}

void liblinux_pal_rwlock_init(struct liblinux_pal_rwlock_t *lock)
{
	BUILD_BUG_ON(sizeof(struct liblinux_pal_rwlock_t) < sizeof(struct raw_rwlock));
	raw_rwlock_init(rwlock_cast(lock), NULL);
}

int  liblinux_pal_rwlock_tryrdlock(struct liblinux_pal_rwlock_t *lock)
{
	int ret;

	ret = raw_rwlock_tryrdlock(rwlock_cast(lock));
	return -hmerrno2posix(ret);
}

void liblinux_pal_rwlock_rdlock(struct liblinux_pal_rwlock_t *lock)
{
	(void)raw_rwlock_rdlock(rwlock_cast(lock));
}

int  liblinux_pal_rwlock_trywrlock(struct liblinux_pal_rwlock_t *lock)
{
	int ret;

	ret = raw_rwlock_trywrlock(rwlock_cast(lock));
	return -hmerrno2posix(ret);
}

void liblinux_pal_rwlock_wrlock(struct liblinux_pal_rwlock_t *lock)
{
	(void)raw_rwlock_wrlock(rwlock_cast(lock));
}

void liblinux_pal_rwlock_unlock(struct liblinux_pal_rwlock_t *lock)
{
	(void)raw_rwlock_unlock(rwlock_cast(lock));
}

void liblinux_pal_rwlock_destroy(struct liblinux_pal_rwlock_t *lock)
{
	raw_rwlock_destroy(rwlock_cast(lock));
}

int liblinux_pal_cond_init(struct liblinux_pal_cond_t *cond,
			   const struct liblinux_pal_cond_attr_t *attr)
{
	int ret;

	BUILD_BUG_ON(sizeof(struct liblinux_pal_cond_t) < sizeof(struct raw_thread_cond));
	BUILD_BUG_ON(sizeof(struct liblinux_pal_cond_attr_t) < sizeof(struct raw_thread_cond_attr));
	ret = raw_thread_cond_init(cond_cast(cond),
				   ptr_to_type(attr, const struct raw_thread_cond_attr *));
	return -hmerrno2posix(ret);
}

int liblinux_pal_cond_destroy(struct liblinux_pal_cond_t *cond)
{
	int ret;

	ret = raw_thread_cond_destroy(cond_cast(cond));
	return -hmerrno2posix(ret);
}

int liblinux_pal_cond_wait(struct liblinux_pal_cond_t *cond, struct liblinux_pal_mutex_t *mutex)
{
	int ret;

	ret = raw_thread_cond_wait(cond_cast(cond), mutex_cast(mutex));
	return -hmerrno2posix(ret);
}

int liblinux_pal_cond_signal(struct liblinux_pal_cond_t *cond)
{
	int ret;

	ret = raw_thread_cond_signal(cond_cast(cond));
	return -hmerrno2posix(ret);
}

int liblinux_pal_cond_broadcast(struct liblinux_pal_cond_t *cond)
{
	int ret;

	ret = raw_thread_cond_broadcast(cond_cast(cond));
	return -hmerrno2posix(ret);
}

int liblinux_pal_sem_init(struct liblinux_pal_sem_t *sem, unsigned int value)
{
	int ret;

	BUILD_BUG_ON(sizeof(struct liblinux_pal_sem_t) < sizeof(struct raw_sem));
	ret = raw_sem_init(sem_cast(sem), value);
	return -hmerrno2posix(ret);
}

int liblinux_pal_sem_trywait(struct liblinux_pal_sem_t *sem)
{
	int ret;

	ret = raw_sem_trywait(sem_cast(sem));
	return -hmerrno2posix(ret);
}

int liblinux_pal_sem_wait(struct liblinux_pal_sem_t *sem)
{
	int ret;

	ret = raw_sem_wait(sem_cast(sem));
	return -hmerrno2posix(ret);
}

int liblinux_pal_sem_timedwait(struct liblinux_pal_sem_t *sem,
			       const struct liblinux_pal_timespec_t *ts)
{
	int ret;
	struct timespec tv = {
		.tv_sec = ts->ts_sec,
		.tv_nsec = ts->ts_nsec,
	};
	ret = raw_sem_timedwait(sem_cast(sem), &tv);
	return -hmerrno2posix(ret);
}

int liblinux_pal_sem_post(struct liblinux_pal_sem_t *sem)
{
	int ret;

	ret = raw_sem_post(sem_cast(sem));
	return -hmerrno2posix(ret);
}

int liblinux_pal_sem_getvalue(struct liblinux_pal_sem_t *sem)
{
	int ret;

	ret = raw_sem_getvalue(sem_cast(sem));
	return -hmerrno2posix(ret);
}
