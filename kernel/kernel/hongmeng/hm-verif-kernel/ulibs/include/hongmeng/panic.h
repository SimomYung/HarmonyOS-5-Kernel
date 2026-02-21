/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Panic
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 15 13:12:19 2018
 */
#ifndef ULIBS_HONGMENG_PANIC_H
#define ULIBS_HONGMENG_PANIC_H

#include <stdbool.h>

#include <hmasm/panic_trap_lib.h>
#include <hmkernel/compiler.h>
#include <hongmeng/types.h>
#include <hongmeng/compiler.h>
#include <libhmlog/hmlog.h>

extern void __noreturn __cold
__bug_impl(const char *assertion, const char *bug_file,
	   const char *function, int line_num);

void  __noreturn __cold __attribute__((format(printf, 1, 2)))
__dlist_bug(const char *fmt, ...);

#define EMPTY
#define __bugon_log(m, a, b, c, d, e, f, g, h, i, j, k, n, ...) m##n
#define bugon_log(m, ...) __bugon_log(m, __VA_ARGS__, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, EMPTY)
#undef EMPTY

#define bug_on_log1(a)						\
	if (unlikely((a))) {					\
		arch_panic_trap_preproc();			\
		__bug_impl(#a, NULL, __func__, __LINE__);	\
	}

#define bug_on_log2(a, ...)					\
	if (unlikely((a))) { 					\
		hm_panic(__VA_ARGS__);				\
	}

#define __condition_monitor(...)			\
do {							\
	bugon_log(bug_on_log, __VA_ARGS__)(__VA_ARGS__);\
} while (__false)

/*
 * USAGE:
 * 1. BUG_ON(condition);
 * 2. BUG_ON(condition, "format string", arg0, arg1, ...);
 *
 * NOTE:
 * Include the format string, BUG_ON can deal with 10 arguments
 * at most.
 */
#define BUG_ON  __condition_monitor

#ifdef CONFIG_DEBUG_BUILD
# define BUG_ON_D(...)	BUG_ON(__VA_ARGS__)
#else
# define BUG_ON_D(...)
#endif

#define BUG(reason)	\
do {			\
	arch_panic_trap_preproc();		     \
	__bug_impl(reason, NULL, __func__, __LINE__);\
} while (__false)

#define panic(fmt, ...)	\
do {			\
	arch_panic_trap_preproc(); \
	__hmlog(HMLOG_PANIC, fmt "\n", ##__VA_ARGS__);	\
	__panic_impl(NULL, __func__, __LINE__);\
} while (__false)

#define panicv(fmt, va)	\
do {			\
	arch_panic_trap_preproc(); \
	__hmlogv(HMLOG_PANIC, fmt "\n", va);		\
	__panic_impl(NULL, __func__, __LINE__);	\
} while (__false)

#endif
