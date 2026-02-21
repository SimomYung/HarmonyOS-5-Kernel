/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: API of Raw mutex guard
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 13 17:32:29 2018
 */
#ifndef ULIBS_INCLUDE_LIBHMSYNC_RAW_MUTEX_GUARD_H
#define ULIBS_INCLUDE_LIBHMSYNC_RAW_MUTEX_GUARD_H

#include <stddef.h>
#include <libhmsync/raw_mutex.h>

/*
	-------------
	Lock guard
	-------------

	A lock guard is an object that manages a mutex object by keeping it always
	locked.

	On construction, the mutex object is locked by the calling thread, and on
	destruction, the mutex is unlocked. It is the simplest lock, and is specially
	useful as an object with automatic duration that lasts until the end of its
	context.

	In this way, it guarantees the mutex object is properly unlocked in error case,
	which make us create more readable error handling.

	Note though that the lock_guard object does not manage the lifetime of the
	mutex object in any way: the duration of the mutex object shall extend at least
	until the destruction of the lock_guard that locks it.

	NOTE: most of above texts are copied from:

	For usage, see test__sysmgr_mutex_guard()
*/

#ifdef  __cplusplus
extern "C" {
#endif

struct raw_mutex_guard {
	struct raw_mutex *mutex;
};

static inline void raw_mutex_guard_unlock(struct raw_mutex_guard *lg)
{
	if (lg->mutex != NULL) {
		raw_mutex_unlock(lg->mutex);
	}
}

#ifndef assert
#define assert(c)
#endif

/*
 * name: the name of this lock-guard instance.
 * ptr_mutex: a pointer to protected raw mutex
 */

#define __define_mutex_guard(name) \
	struct raw_mutex_guard __hm_mutex_lg_##name __attribute__ ((cleanup(raw_mutex_guard_unlock))) = {.mutex = NULL}
#define RAW_MUTEX_GUARD(name, ptr_mutex)	\
	__define_mutex_guard(name);	\
	__hm_mutex_lg_##name.mutex = (ptr_mutex); \
	raw_mutex_lock(__hm_mutex_lg_##name.mutex);

#define RAW_MUTEX_GUARD_S(name, mutex, ...) \
	({ \
	 RAW_MUTEX_GUARD(name, mutex); \
	 __VA_ARGS__; \
	 })

#define RAW_MUTEX_GUARD_START(name, ptr_mutex)	\
	{ \
	RAW_MUTEX_GUARD(name, ptr_mutex)

#define RAW_MUTEX_GUARD_END(name, ptr_mutex)	\
	assert(__hm_mutex_lg_##name.mutex == ptr_mutex); \
	}

#ifdef  __cplusplus
}
#endif

#endif
