/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Lock guard of rwlock
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 18 18:44:56 2019
 */
#ifndef ULIBS_INCLUDE_LIBHMSYNC_RAW_RWLOCK_GUARD_H
#define ULIBS_INCLUDE_LIBHMSYNC_RAW_RWLOCK_GUARD_H

#include <stddef.h>
#include <libhmsync/raw_rwlock.h>

/*
 * For usage, see raw_mutex_guard.h
 */

#ifdef  __cplusplus
extern "C" {
#endif

struct raw_rwlock_guard {
	struct raw_rwlock *rwlock;
};

static inline void raw_rwlock_guard_unlock(struct raw_rwlock_guard *lg)
{
	if (lg->rwlock != NULL) {
		(void)raw_rwlock_unlock(lg->rwlock);
	}
}

#ifndef assert
#define assert(c)
#endif

#define __DEFINE_RWLOCK_GUARD(name) \
	struct raw_rwlock_guard __hm_rwlock_lg_##name __attribute__ ((cleanup(raw_rwlock_guard_unlock))) = {.rwlock = NULL}

/* read lock */
#define RAW_RWLOCK_RD_GUARD(name, ptr_rwlock)	\
	__DEFINE_RWLOCK_GUARD(name);	\
	__hm_rwlock_lg_##name.rwlock = (ptr_rwlock); \
	(void)raw_rwlock_rdlock(__hm_rwlock_lg_##name.rwlock);

#define RAW_RWLOCK_RD_GUARD_START(name, ptr_rwlock)	\
	{ \
	RAW_RWLOCK_RD_GUARD(name, ptr_rwlock)

#define RAW_RWLOCK_RD_GUARD_END(name, ptr_rwlock)	\
	assert(__hm_rwlock_lg_##name.rwlock == (ptr_rwlock));	\
	}

/* write lock */
#define RAW_RWLOCK_WR_GUARD(name, ptr_rwlock)	\
	__DEFINE_RWLOCK_GUARD(name);	\
	__hm_rwlock_lg_##name.rwlock = (ptr_rwlock); \
	(void)raw_rwlock_wrlock(__hm_rwlock_lg_##name.rwlock);

#define RAW_RWLOCK_WR_GUARD_START(name, ptr_rwlock)	\
	{ \
	RAW_RWLOCK_WR_GUARD(name, ptr_rwlock)

#define RAW_RWLOCK_WR_GUARD_END(name, ptr_rwlock)	\
	assert(__hm_rwlock_lg_##name.rwlock == (ptr_rwlock));	\
	}

#ifdef  __cplusplus
}
#endif

#endif
