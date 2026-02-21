/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Implement ratelimit for hmlog
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 27 14:41:33 2022
 */

#include <libhmlog/hmlog_ratelimit.h>
#include <libhmsrv_sys/hm_timer.h>

static bool time_expired(unsigned long long now_in_us, unsigned long long begin_in_us,
		unsigned int interval_in_ms)
{
	bool expired = false;
	unsigned long long interval_in_us = (unsigned long long)interval_in_ms * USEC_PER_MSEC;

	if (now_in_us >= begin_in_us) {
		expired = ((now_in_us - begin_in_us) >= interval_in_us);
	}

	return expired;
}

static bool test_limited_locked(struct hmlog_ratelimit_state *rs,
		unsigned long long now_in_us, unsigned long long *missed_count)
{
	bool limited = false;

	if (rs->begin_time_in_us == 0ULL) {
		rs->begin_time_in_us = now_in_us;
	}

	if (time_expired(now_in_us, rs->begin_time_in_us, rs->interval)) {
		*missed_count = rs->missed_count;
		/* reset after interval expired */
		rs->begin_time_in_us = now_in_us;
		rs->missed_count = 0U;
		(void)raw_atomic_int_set(&rs->printed_count, 0);
	}

	if ((unsigned int)raw_atomic_int_read(&rs->printed_count) >= rs->max_count) {
		rs->missed_count++;
		limited = true;
	} else {
		/* allow to print */
		(void)raw_atomic_int_inc(&rs->printed_count);
	}

	return limited;
}

static int get_current_time(unsigned long long *now)
{
	int err;
	struct timespec cur_time;
	unsigned long long now_in_us = 0ULL;
	err = hm_clock_gettime(CLOCK_MONOTONIC, &cur_time);
	if (err == E_HM_OK) {
		/* convert to us, won't overflow within unsigned long long */
		now_in_us = (unsigned long long)(long long)(cur_time.tv_sec * USEC_PER_SEC)
			+ (unsigned long long)(long long)(cur_time.tv_nsec / NSEC_PER_USEC);
	}
	*now = now_in_us;

	return err;
}

static bool do_test_ratelimited(struct hmlog_ratelimit_state *rs, unsigned long long *missed_count)
{
	int err;
	unsigned long long now_in_us;
	bool limited = false;

	/* if interval or max_count is specified as 0, there is no limit. */
	if (rs->max_count > 0U && rs->interval > 0U) {
		err = get_current_time(&now_in_us);
		if (err == E_HM_OK) {
			raw_mutex_lock(&(rs->state_lock));
			limited = test_limited_locked(rs, now_in_us, missed_count);
			raw_mutex_unlock(&(rs->state_lock));
		}
	}

	return limited;
}

#define MAX_RATELIMIT_DISABLE_COUNT 5
static bool ratelimit_disabled(int disabled_count)
{
	return disabled_count > MAX_RATELIMIT_DISABLE_COUNT;
}

bool hmlog_test_ratelimited(struct hmlog_ratelimit_state *rs,
		const char *file, const char *func, int line)
{
	bool limited = false;
	unsigned long long missed_count = 0ULL;
	int disabled_count;

	/* disable this rs temporarily since hm_clock_gettime or
	 * raw_mutex_lock/raw_mutex_unlock may call ratelimit functions accidentally */
	disabled_count = raw_atomic_int_inc_relaxed(&rs->disabled);
	if (ratelimit_disabled(disabled_count)) { /* already disabled */
		limited = false;
		/* just allow to print and increase printed count */
		(void)raw_atomic_int_inc(&rs->printed_count);
	} else {
		limited = do_test_ratelimited(rs, &missed_count);
	}
	(void)raw_atomic_int_dec_relaxed(&rs->disabled);

	if (missed_count > 0U) {
		__hmlog_printf(HMLOG_WARN, file, func, line,
				"log suppressed. missed=%llu\n", missed_count);
	}

	return limited;
}
