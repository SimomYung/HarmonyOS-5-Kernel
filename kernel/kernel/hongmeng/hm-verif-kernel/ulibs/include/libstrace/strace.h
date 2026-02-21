/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Strace interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 16 11:50:11 2020
 */

#ifndef ULIBS_INCLUDE_LIBSTRACE_STRACE_H
#define ULIBS_INCLUDE_LIBSTRACE_STRACE_H

#ifdef CONFIG_HMVSYSCALL_TRACE
#include <stdbool.h>
#include <time.h>

enum strace_enter_state {
	STRACE_ENTERING = 0,
	STRACE_EXITING
};

struct strace_entry_info {
	struct timespec ts;
};

extern bool __is_strace_enabled;

extern void hm_strace_entry(enum strace_enter_state state, long scno, long ret_val,
			    struct strace_entry_info *info, va_list va);

/* These functions would be called by strace process. */
extern int hm_strace_on(long pid);
extern int hm_strace_off(void);

#define hm_strace_enter(info, va)					\
do {									\
	if (__is_strace_enabled) {					\
		hm_strace_entry(STRACE_ENTERING, 0, 0, info, va);	\
	}								\
} while (0)

#define hm_strace_exit(scno, rc, info, va)				\
do {									\
	if (__is_strace_enabled) {					\
		hm_strace_entry(STRACE_EXITING, scno, rc, info, va);	\
	}								\
} while (0)

#define hm_vsyscall_trace(table, num, va)			\
({								\
	long rc;						\
	struct strace_entry_info strace_info;			\
	hm_strace_enter(&strace_info, (va));			\
	rc = (table)[(num)]((va));				\
	hm_strace_exit((num), rc, &strace_info, (va));		\
	rc;							\
})
#else /* !CONFIG_HMVSYSCALL_TRACE */
#define hm_vsyscall_trace(table, num, va) (table)[(num)]((va))
#endif /* CONFIG_HMVSYSCALL_TRACE */

#endif
