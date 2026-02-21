/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: UEV interface header for tiny/normal libhmsync variants.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 21 14:39:48 2022
 */

#ifndef ULIBS_LIBHMSYNC_UEV_HELPER_H
#define ULIBS_LIBHMSYNC_UEV_HELPER_H

#include <libhmsync/uev/uev.h>

#ifdef CONFIG_LOCK_TRACE_UEV_LOGGING

#define lock_trace_init_lock_id(lock) \
do { \
	vatomic32_write_rlx(&lock->lock_id, 0); \
} while(0)

static inline void uev_lock_trace_log(uint64_t return_address, vatomic32_t *lock_id, enum locktrace_event_type event)
{
	if (lock_id == NULL || (lock_id != NULL && atomic_read_rlx(lock_id) != ~0U)) {
		uev_trace_lock_event(event, return_address, lock_id);
	}
}

#define lock_trace_log(lock, event) \
do { \
	uev_lock_trace_log((uint64_t)(uintptr_t)__builtin_return_address(0), \
			lock ? &lock->lock_id : NULL, event); \
} while (0)

#define lock_trace_log_pair(lock_a, lock_b, event)	\
do {	\
	uev_trace_lock_pair_event(					\
		event,							\
		(uint64_t)(uintptr_t)__builtin_return_address(0),	\
		lock_a ? &lock_a->lock_id : NULL,			\
		lock_b ? &lock_b->lock_id : NULL);			\
} while (0)
#else
#define lock_trace_init_lock_id(lock) do {} while (0)
#define lock_trace_log(lock, event) do {} while (0)
#define lock_trace_log_pair(lock_a, lock_b, event) do {} while (0)
#endif

#endif
