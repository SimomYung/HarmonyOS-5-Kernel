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

typedef enum {
	/* the order and value matters don't move them around */
	VGDUMP_INACTIVE_NO_REQ = 0, /* 00 */
	VGDUMP_ACTIVE_NO_REQ = 1, /* 01 */
	VGDUMP_INACTIVE_WITH_REQ = 2, /* 10 */
	VGDUMP_ACTIVE_WITH_REQ = 3 /* 11 */
} gdump_status_t;

#define V_GDUMP_IS_ACTIVE(_s_) ((_s_) & (vuint8_t)0x1)
#define V_GDUMP_IS_REQ_PRESENT(_s_) ((_s_) & (vuint8_t)0x2)
#define V_GDUMP_PLACE_REQ(_s_) ((_s_) | (vuint8_t)0x2)

#undef GD_V4_COLLECT_STATS
#if defined(GD_V4_COLLECT_STATS)
enum GD_PROFILE {
	GD_MAKE_R,
	GD_FIND_R,
	GD_FIND_SKIP,

	GD_FIND_A,

	GD_SYNCS,
	GD_REPEAT_CAS,
	GD_YIELDS,

	GD_PROFILE_CNT,
};

#include <stdatomic.h>
atomic_int _gd_profile[GD_PROFILE_CNT] = { 0 };
#define GD_PROFILE(_idx_) _gd_profile[_idx_]++
#define GD_PROFILE_PRINT()                                               \
	do {                                                             \
		for (vsize_t i = 0; i < GD_PROFILE_CNT; i++) {           \
			fprintf(stderr, "%zu = %d ", i, _gd_profile[i]); \
		}                                                        \
		fprintf(stderr, "\n\n");                                 \
	} while (false)
#else
#define GD_PROFILE(_idx_) \
	do {              \
	} while (false)
#define GD_PROFILE_PRINT()
#endif

static inline void _gdump_exit(gdump_t *, gdump_thread_t *);

static inline void _gdump_init_thread(gdump_t *gd, gdump_thread_t *thread)
{
	vatomic8_write_rlx(&thread->status, 0);
	vatomic64_write_rlx(&thread->timestamp, 0);
	V_UNUSED(gd);
}

static inline void gdump_enter(gdump_t *gd, gdump_thread_t *thread)
{
	vuint8_t status = 0;
	vuint64_t timestamp = 0;

	ASSERT(gd);

	if (thread->enter_count > 0) {
		/* the signal is not to be given on recursion */
		thread->enter_count++;
	} else {
		thread->enter_count = 1;

		status = vatomic8_xchg_rlx(&thread->status,
					   VGDUMP_ACTIVE_NO_REQ);

		if (V_GDUMP_IS_REQ_PRESENT(status)) {
			timestamp = vatomic64_read_rlx(&thread->timestamp);
			vatomic64_write_rlx(&thread->timestamp, timestamp + 1);
			vatomic_fence();
		}
	}
}

static inline void gdump_exit(gdump_t *gd, gdump_thread_t *thread)
{
	vuint8_t status = 0;
	vuint64_t timestamp = 0;

	_gdump_exit(gd, thread);

	if (thread->enter_count == 0) {
		status = vatomic8_xchg_rel(&thread->status,
					   VGDUMP_INACTIVE_NO_REQ);

		if (V_GDUMP_IS_REQ_PRESENT(status)) {
			timestamp = vatomic64_read_rlx(&thread->timestamp);
			vatomic64_write_rel(&thread->timestamp, timestamp + 1);
		}
	}
	V_UNUSED(gd, thread);
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
					     gdump_cache_entry_t *out)
{
	vuint64_t new_timestamp = 0;
	vuint64_t timestamp = vatomic64_read_acq(&thread->timestamp);
	vuint8_t status = vatomic8_read_acq(&thread->status);
	vuint8_t o_status = 0;

	out->thread = thread;
	out->timestamp = timestamp;

	while (!V_GDUMP_IS_REQ_PRESENT(status)) {
		/* if there is no req placed, place it */
		o_status = vatomic8_cmpxchg_acq(&thread->status, status,
						V_GDUMP_PLACE_REQ(status));

		if (o_status == status) {
			GD_PROFILE(GD_MAKE_R);
			break;
		} else {
			status = o_status;
			new_timestamp = vatomic64_read_acq(&thread->timestamp);
			if (new_timestamp > timestamp) {
				GD_PROFILE(GD_FIND_SKIP);
				return false;
			}

			GD_PROFILE(GD_REPEAT_CAS);
			verification_ignore();
		}
	}

	if (V_GDUMP_IS_REQ_PRESENT(status)) {
		GD_PROFILE(GD_FIND_R);
	}

	if (V_GDUMP_IS_ACTIVE(status)) {
		GD_PROFILE(GD_FIND_A);
		return true;
	}

	return false;
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
	vuint64_t cur_ts = vatomic64_read_acq(&entry->thread->timestamp);
	return (cur_ts != entry->timestamp);
}

#include "gdump_sync.h"
#include "gdump_common.h"
#undef V_GDUMP_IS_ACTIVE
#undef V_GDUMP_IS_REQ_PRESENT
#undef V_GDUMP_PLACE_REQ
#undef GD_PROFILE
#undef GD_PROFILE_PRINT
#undef GD_V4_COLLECT_STATS
#endif
