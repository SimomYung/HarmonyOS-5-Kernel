/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 1998-2022 Erez Zadok
 * Copyright (c) 2009	   Shrikar Archak
 * Copyright (c) 2003-2022 Stony Brook University
 * Copyright (c) 2003-2022 The Research Foundation of SUNY
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 */

#ifndef _SHAREFS_H_
#define _SHAREFS_H_

#include <linux/dcache.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/seq_file.h>
#include <linux/statfs.h>
#include <linux/fs_stack.h>
#include <linux/magic.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/xattr.h>
#include <linux/exportfs.h>

static inline bool str_n_case_eq(const char *s1, const char *s2, size_t len)
{
	return !strncasecmp(s1, s2, len);
}

static inline bool qstr_case_eq(const struct qstr *q1, const struct qstr *q2)
{
	return q1->len == q2->len && str_n_case_eq(q1->name, q2->name, q2->len);
}
/*****************************************************************************
 * log print helpers
 *****************************************************************************/
void __sharefs_log(const char *level, const bool ratelimited,
		 const char *function, const char *fmt, ...);

__printf(4, 5) void __sharefs_log(const char *level, const bool ratelimited,
				const char *function, const char *fmt, ...);
#define sharefs_err(fmt, ...)	\
	__sharefs_log(KERN_ERR, false, __func__, fmt, ##__VA_ARGS__)
#define sharefs_warning(fmt, ...) \
	__sharefs_log(KERN_WARNING, false, __func__, fmt, ##__VA_ARGS__)
#define sharefs_info(fmt, ...) \
	__sharefs_log(KERN_INFO, false, __func__, fmt, ##__VA_ARGS__)
#define sharefs_err_ratelimited(fmt, ...)	\
	__sharefs_log(KERN_ERR, true, __func__, fmt, ##__VA_ARGS__)
#define sharefs_warning_ratelimited(fmt, ...) \
	__sharefs_log(KERN_WARNING, true, __func__, fmt, ##__VA_ARGS__)
#define sharefs_info_ratelimited(fmt, ...) \
	__sharefs_log(KERN_INFO, true, __func__, fmt, ##__VA_ARGS__)

#endif /* not _SHAREFS_H_ */
