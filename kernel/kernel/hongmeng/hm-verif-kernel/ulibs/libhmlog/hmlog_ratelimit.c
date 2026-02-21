/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2025. All rights reserved.
 * Description: Implement ratelimit for hmlog
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 27 14:41:33 2022
 */

#include <hmasm/types.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libhmsrv_sys/hm_timer.h>

static bool test_ratelimited_fast(struct hmlog_ratelimit_state *rs);

static bool time_expired(__u64 now_in_us, __u64 begin_in_us,
		unsigned int interval_in_ms)
{
	bool expired = false;
	__u64 interval_in_us = (unsigned long long)interval_in_ms * USEC_PER_MSEC;

	if (now_in_us >= begin_in_us) {
		expired = ((now_in_us - begin_in_us) >= interval_in_us);
	}

	return expired;
}

static bool test_expire_limited(struct hmlog_ratelimit_state *rs,
		__u64 now_in_us, unsigned long long *missed_count)
{
	bool limited = false;
	bool expired = false;
	__u64 begin_in_us = vatomic64_read(&(rs->begin_time_in_us));
	if (begin_in_us == 0ULL) {
		expired = false;
		vatomic64_write(&(rs->begin_time_in_us), now_in_us);
	} else {
		expired = time_expired(now_in_us, begin_in_us, rs->interval);
	}

	if (expired) {
		/* reset after interval expired */
		if (vatomic64_cmpxchg(&(rs->begin_time_in_us), begin_in_us, now_in_us) == begin_in_us) {
			(void)raw_atomic_int_set(&rs->printed_count, 1);
			limited = false;
			*missed_count = rs->missed_count;
			rs->missed_count = 0U;
		} else {
			/* if begin_time_in_us is updated by other threads, just test whether reached limit  */
			limited = test_ratelimited_fast(rs);
		}
	} else {
		/* not expired and reached limit */
		limited = true;
	}

	return limited;
}

static int get_current_time(struct hmlog_ratelimit_state *rs, __u64 *now)
{
	int err;
	struct timespec cur_time;
	__u64 now_in_us = 0ULL;
	/* disable this rs temporarily since hm_clock_gettime
	 * may call ratelimit functions accidentally */
	(void)raw_atomic_int_inc_relaxed(&rs->disabled);
	err = hm_clock_gettime(CLOCK_MONOTONIC, &cur_time);
	(void)raw_atomic_int_dec_relaxed(&rs->disabled);
	if (err == E_HM_OK) {
		/* convert to us, won't overflow within unsigned long long */
		now_in_us = (__u64)(long long)(cur_time.tv_sec * USEC_PER_SEC)
			+ (__u64)(long long)(cur_time.tv_nsec / NSEC_PER_USEC);
	}
	*now = now_in_us;

	return err;
}

#define MAX_RATELIMIT_DISABLE_COUNT 5
static bool ratelimit_disabled(int disabled_count)
{
	return disabled_count > MAX_RATELIMIT_DISABLE_COUNT;
}

static bool test_ratelimited_slow(struct hmlog_ratelimit_state *rs,
		const char *file, const char *func, int line)
{
	int err;
	bool limited = false;
	__u64 now_in_us;
	unsigned long long missed_count = 0ULL;

	err = get_current_time(rs, &now_in_us);
	if (err == E_HM_OK) {
		limited = test_expire_limited(rs, now_in_us, &missed_count);
	}
	if (missed_count > 0U) {
		__hmlog_printf(HMLOG_WARN, file, func, line,
				"log suppressed. missed=%llu\n", missed_count);
	}

	return limited;
}

static void init_begin_time(struct hmlog_ratelimit_state *rs)
{
	int err = E_HM_OK;
	__u64 now_in_us;
	__u64 begin_in_us = vatomic64_read(&(rs->begin_time_in_us));
	if (begin_in_us == 0) {
		err = get_current_time(rs, &now_in_us);
		if (err == E_HM_OK) {
			vatomic64_write(&(rs->begin_time_in_us), now_in_us);
		}
	}
}

#define MAX_TRY_COUNT_FOR_FAST 5
static bool test_ratelimited_fast(struct hmlog_ratelimit_state *rs)
{
	bool limited = true;
	unsigned int cur_count = (unsigned int)raw_atomic_int_read(&rs->printed_count);
	unsigned int old_count;

	for (unsigned int i = 0; i < MAX_TRY_COUNT_FOR_FAST; i++) {
		if (cur_count >= rs->max_count) {
			break;
		}
		old_count = (unsigned int)raw_atomic_int_cas(&rs->printed_count, cur_count, cur_count + 1);
		if (old_count == cur_count) {
			if (old_count == 0) {
				/* init begin time for the first record */
				init_begin_time(rs);
			}
			limited = false;
			break;
		}
		cur_count = old_count;
	}
	return limited;
}

bool hmlog_test_ratelimited(struct hmlog_ratelimit_state *rs,
		const char *file, const char *func, int line)
{
	bool limited = false;
	int disabled_count;
	/* if interval or max_count is specified as 0, there is no limit. */
	if (rs->max_count > 0U && rs->interval > 0U) {
		disabled_count = raw_atomic_int_read(&rs->disabled);
		if (ratelimit_disabled(disabled_count)) { /* already disabled */
			limited = false;
			/* just allow to print and increase printed count */
			(void)raw_atomic_int_inc(&rs->printed_count);
		} else {
			limited = test_ratelimited_fast(rs);
			if (limited) {
				limited = test_ratelimited_slow(rs, file, func, line);
			}
		}
		if (limited) {
			rs->missed_count++;
		}
	}
	return limited;
}
