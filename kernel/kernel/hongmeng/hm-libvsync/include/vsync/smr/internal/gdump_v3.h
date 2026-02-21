/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef VSYNC_GARBAGE_DUMP_H
#define VSYNC_GARBAGE_DUMP_H

#include <vsync/vtypes.h>
#include <vsync/common/compiler.h>
#include <vsync/common/verify.h>
#include <vsync/common/dbg.h>
#include <vsync/common/assert.h>
#include <vsync/smr/internal/smr_nodes_list.h>
#include <vsync/smr/internal/smr_lock.h>
#include <vsync/smr/internal/gdump_list.h>
#include <vsync/smr/internal/gdump_types.h>
#include <vsync/smr/internal/gdump_cache.h>

#define GD_STATUS_IS_ACTIVE(_s_) ((_s_)&1UL)
#define GD_STATUS_GET_TS(_s_) ((_s_) >> 1)

static inline void _gdump_exit(gdump_t *, gdump_thread_t *);

static inline void _gdump_init_thread(gdump_t *gd, gdump_thread_t *thread)
{
	vatomic64_write_rlx(&thread->ts_status, 0);
	V_UNUSED(gd);
}

static inline void gdump_enter(gdump_t *gd, gdump_thread_t *thread)
{
	vuint64_t status_ts = 0;

	ASSERT(gd);
	if (thread->enter_count > 0) {
		/* the signal is not to be given on recursion */
		thread->enter_count++;
	} else {
		thread->enter_count = 1;

		status_ts = vatomic64_read_rlx(&thread->ts_status) >> 1;
		status_ts = ((status_ts + 1) << 1) | 1;

		vatomic64_write_rlx(&thread->ts_status, status_ts);
		vatomic_fence();
	}
}

static inline void gdump_exit(gdump_t *gd, gdump_thread_t *thread)
{
	_gdump_exit(gd, thread);
	if (thread->enter_count == 0) {
		vuint64_t status_ts = vatomic64_read_rlx(&thread->ts_status) &
				      ~(1UL);
		vatomic64_write_rel(&thread->ts_status, status_ts);
	}
}
/**
 * Determines if the thread is active inside a critical section.
 *
 * In addition, it caches the thread data in `out_entry`.
 *
 * @param thread address of gdump_thread_t object.
 * @param out_entry output parameter address of gdump_cache_entry_t, where the
 * thread data will be cached.
 * @return true the given thread is in a critical section.
 * @return false the given thread is not a critical section.
 */
static inline vbool_t _gdump_cache_and_check(gdump_thread_t *thread,
					     gdump_cache_entry_t *out_entry)
{
	vuint64_t cur_status = vatomic64_read_acq(&thread->ts_status);
	out_entry->timestamp = GD_STATUS_GET_TS(cur_status);
	out_entry->thread = thread;
	return GD_STATUS_IS_ACTIVE(cur_status);
}
/**
 * Determines if the thread has been in a quiescent state.
 *
 * @param thread address of gdump_thread_t object.
 * @param old_ts observed timestamp from the beginning of the grace period.
 * @return true yes the thread has been quiescent.
 * @return false the thread has not been in a quiescent state yet.
 */
static inline vbool_t _gdump_has_been_quiescent(gdump_cache_entry_t *entry)
{
	vuint64_t cur_ts = 0;
	vuint64_t status = vatomic64_read_acq(&entry->thread->ts_status);

	if (GD_STATUS_IS_ACTIVE(status)) {
		/* extract current timestamp */
		cur_ts = GD_STATUS_GET_TS(status);
		if (cur_ts == entry->timestamp) {
			/* the thread is still in the same cs */
			return false;
		}
	}
	/* the thread has either exited the CS status not active, or entered a new
     * CS we don't care about. */
	return true;
}

#include "gdump_sync.h"
#include "gdump_common.h"
#undef GD_STATUS_IS_ACTIVE
#undef GD_STATUS_GET_TS
#endif
