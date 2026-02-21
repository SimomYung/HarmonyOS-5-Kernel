/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2022. All rights reserved.
 * Description: Export kernel panic.h to mapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 08 10:42:32 2019
 */
#ifndef MAPI_HMKERNEL_PANIC_H
#define MAPI_HMKERNEL_PANIC_H

#include <hmkernel/compiler.h>
#include <hmkernel/strict.h>
#include <hmasm/panic.h>
#include <lib/klog.h>

#ifndef __EARLY__

extern void __noreturn __cold
__panic_impl(const char *panic_file, const char *function, int line_num);

extern void __noreturn __cold
__bug_impl(const char *assertion, const char *bug_file,
	   const char *function, int line_num);

#ifdef CONFIG_DLIST_DEBUG
extern void __noreturn __printf(1, 2)
__dlist_bug(const char *fmt, ...);
#endif

#else

#define __panic_impl(fi, fu, l) do {	\
	arch_panic();			\
} while (false)

#define __bug_impl(ass, fi, fu, l) do {	\
	arch_panic();			\
} while (false)

#endif /* !__EARLY__ */

#define panic(fmt, ...)					\
do {							\
	__klog_panic(fmt, ##__VA_ARGS__);		\
	__panic_impl(NULL, __func__, __LINE__);	\
} while (false)

#define panic_at_pa(pa, fmt, ...)			\
do {							\
	pgstrtbl_stat_dump_neighborhood(pa);		\
	panic(fmt, ##__VA_ARGS__);			\
} while (false)

#define EMPTY
#define __bugon_log(m, a, b, c, d, e, f, g, h, i, j, k, n, ...) m##n
#define bugon_log(m, ...) __bugon_log(m, __VA_ARGS__, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, EMPTY)
#undef EMPTY

#define bug_on_log1(a)						\
	if (unlikely((a))) {					\
		__bug_impl(#a, NULL, __func__, __LINE__);	\
	}

#define bug_on_log2(a, ...)					\
	if (unlikely((a))) { 					\
		__klog_panic(__VA_ARGS__);			\
		__bug_impl(#a, NULL, __func__, __LINE__);	\
	}

#define __condition_monitor(...)			\
do {							\
	bugon_log(bug_on_log, __VA_ARGS__)(__VA_ARGS__);\
} while (false)

/* NOTE: just for fix MISRA C-2012 Rule 15.6 */
#define BUG_ON	__condition_monitor

#ifdef CONFIG_DEBUG_BUILD
# define BUG_ON_D(...)	BUG_ON(__VA_ARGS__)
#else
# define BUG_ON_D(...)
#endif

#endif
