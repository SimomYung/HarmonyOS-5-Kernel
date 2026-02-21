/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Uev list for lock tracing
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 19 21:02:33 2022
 */

// Include template and declarations:
#include <libhmsync/thread.h>
#include <libhmsync/uev/uev.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhwsecurec/securec.h>
#include <vsync/atomic.h>

#define TID_SHIFT 14U
#define COUNTER_MASK ((1U << TID_SHIFT) - 1U)
// Note that the overflow of the lock counter and the thread id is safe.
// The only downside are potential false positives in the tools analyzing
// extremely long traces.
#define COUNTER_ARRAY_SIZE 16
static vatomic32_t local_lock_id_ctr[COUNTER_ARRAY_SIZE] = {
	ATOMIC_INIT(1), ATOMIC_INIT(1), ATOMIC_INIT(1), ATOMIC_INIT(1),
	ATOMIC_INIT(1), ATOMIC_INIT(1), ATOMIC_INIT(1), ATOMIC_INIT(1),
	ATOMIC_INIT(1), ATOMIC_INIT(1), ATOMIC_INIT(1), ATOMIC_INIT(1),
	ATOMIC_INIT(1), ATOMIC_INIT(1), ATOMIC_INIT(1), ATOMIC_INIT(1)};
static vatomic32_t wraparound = ATOMIC_INIT(0);

static unsigned get_next_lock_id(void)
{
	uint32_t tid = ((uint32_t)thread_get_current_tid());
	uint32_t cnt = vatomic32_get_inc(&local_lock_id_ctr[tid % COUNTER_ARRAY_SIZE]);
	if (cnt >= (uint32_t)COUNTER_MASK && vatomic32_read_rlx(&wraparound) == 0) {
		hm_warn("Lock id wraparound: TID:%"PRIu32", CNT:%"PRIu32"\n", tid, cnt);
		vatomic32_write_rlx(&wraparound, 1);
	}
	return (tid << TID_SHIFT) | (cnt & COUNTER_MASK);
}

uint64_t uev_trace_lock_event_set_lockid(vatomic32_t *atomic_id)
{
	uint32_t id = vatomic32_read_rlx(atomic_id);
	if (id == 0UL) {
		id = get_next_lock_id();
		uint32_t old_id = vatomic32_cmpxchg_rlx(atomic_id, 0U, id);
		if (old_id != 0U) {
			return old_id;
		}
	}
	return (uint64_t)id;
}

uint64_t uev_trace_lock_event_get_time(void)
{
	struct timespec ts;
	int err = hm_clock_gettime(CLOCK_MONOTONIC, &ts);
	if (err < 0) {
		hm_error("get timestamp failed, err=%s\n", hmstrerror(err));
		return UINT64_MAX;
	}
	return (uint64_t)(unsigned long)ts.tv_sec * (uint64_t)NSEC_PER_SEC +
	       (uint64_t)(unsigned long)ts.tv_nsec;
}

// Generate the definitions:
#define UEV_POINT       DEFINE_UEV
#include <libhmsync/uev/uev_list.h>
#undef UEV_POINT
