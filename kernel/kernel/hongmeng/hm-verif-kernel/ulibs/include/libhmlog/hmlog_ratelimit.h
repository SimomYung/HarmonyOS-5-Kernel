/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Definitions of hmlog ratelimit
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 27 11:30:55 2022
 */
#ifndef ULIBS_INCLUDE_LIBHMLOG_HMLOG_RATELIMIT_H
#define ULIBS_INCLUDE_LIBHMLOG_HMLOG_RATELIMIT_H

#include <time.h>
#include <limits.h>
#include <vsync/atomic.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libhmlog/hmlog.h>

/* default to 1s */
#define DEFAULT_LOG_RATELIMIT_INTERVAL 1000U
#define DEFAULT_LOG_RATELIMIT_MAX_COUNT 10U

struct hmlog_ratelimit_state {
	/* some functions used by ratelimit may call ratelimit accidentally,
	 * the ratelimit used by this state can be disabled temporarily to avoid recursion */
	raw_atomic_int_t disabled;

	raw_atomic_int_t printed_count;

	/* in ms */
	unsigned int interval;
	/* max count of logs can be printed within the interval */
	unsigned int max_count;

	unsigned long long missed_count;

	/* the start timestamp of the interval, it is updated when
	 * the the interval is over at the next log */
	vatomic64_t begin_time_in_us;
};

#define __DEFINE_LOG_RATELIMIT(name, interval_init, max_count_init, diabled_count) \
	struct hmlog_ratelimit_state name \
	__attribute__((section("hmlog_ratelimit_state"))) = { \
		.disabled = RAW_ATOMIC_VAL_INIT(diabled_count), \
		.printed_count = RAW_ATOMIC_VAL_INIT(0), \
		.interval = (interval_init), \
		.max_count = (max_count_init), \
		.missed_count = 0ULL, \
		.begin_time_in_us = VATOMIC_INIT(0), \
	}

#define DEFINE_LOG_RATELIMIT(name, interval_init, max_count_init) \
	__DEFINE_LOG_RATELIMIT(name, interval_init, max_count_init, 0)

#define DEFINE_LOG_RATELIMIT_NO_RECURR_CHECK(name, interval_init, max_count_init) \
	__DEFINE_LOG_RATELIMIT(name, interval_init, max_count_init, INT_MIN)

bool hmlog_test_ratelimited(struct hmlog_ratelimit_state *rs,
		const char *file, const char *func, int line);

static inline void hmlog_init_ratelimit(struct hmlog_ratelimit_state *rs, unsigned int interval_in_ms,
					unsigned int max_count)
{
	raw_atomic_int_init(&rs->disabled, 0);
	raw_atomic_int_init(&rs->printed_count, 0);
	vatomic64_init(&(rs->begin_time_in_us), 0);
	rs->interval = interval_in_ms;
	rs->max_count = max_count;
	rs->missed_count = 0ULL;
}

#define __hmlog_printf_atrate(interval, max_count, level, file, func, line, fmt, ...) \
do { \
	static DEFINE_LOG_RATELIMIT(__local_rs, interval, max_count); \
	if (!hmlog_test_ratelimited(&__local_rs, file, func, line)) { \
		__hmlog_printf(level, file, func, line, fmt, ##__VA_ARGS__); \
	} \
} while(0)

#ifdef __hmfeature_debug__
#define __hmlog_atrate(interval, max_count, lv, fmt, args...) \
	__hmlog_printf_atrate(interval, max_count, lv, __FILE__, __func__, __LINE__, fmt, ##args)
#else
/* non-debug version should not include FILE PATH in binary */
#define __hmlog_atrate(interval, max_count, lv, fmt, args...) \
	__hmlog_printf_atrate(interval, max_count, lv, NULL, __func__, __LINE__, fmt, ##args)
#endif /* __hmfeature_debug__ */

#define hm_info_atrate(interval, max_count, fmt, ...) \
	__hmlog_atrate(interval, max_count, HMLOG_INFO, log_fmt(fmt), ##__VA_ARGS__)
#define hm_warn_atrate(interval, max_count, fmt, ...) \
	__hmlog_atrate(interval, max_count, HMLOG_WARN, log_fmt(fmt), ##__VA_ARGS__)
#define hm_error_atrate(interval, max_count, fmt, ...) \
	__hmlog_atrate(interval, max_count, HMLOG_ERROR, log_fmt(fmt), ##__VA_ARGS__)

#define __hmlog_ratelimited(lv, fmt, args...) \
	__hmlog_atrate(DEFAULT_LOG_RATELIMIT_INTERVAL, DEFAULT_LOG_RATELIMIT_MAX_COUNT, \
			lv, fmt, ##args)

#define hm_info_ratelimited(fmt, ...) \
	__hmlog_ratelimited(HMLOG_INFO, log_fmt(fmt), ##__VA_ARGS__)
#define hm_warn_ratelimited(fmt, ...) \
	__hmlog_ratelimited(HMLOG_WARN, log_fmt(fmt), ##__VA_ARGS__)
#define hm_error_ratelimited(fmt, ...) \
	__hmlog_ratelimited(HMLOG_ERROR, log_fmt(fmt), ##__VA_ARGS__)

#endif /* ULIBS_INCLUDE_LIBHMLOG_HMLOG_RATELIMIT_H */
