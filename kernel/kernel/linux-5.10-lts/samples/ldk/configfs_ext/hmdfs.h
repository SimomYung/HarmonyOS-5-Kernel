/* SPDX-License-Identifier: GPL-2.0 */
/*
 * fs/hmdfs/hmdfs.h
 *
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd.
 */

#ifndef HMDFS_H
#define HMDFS_H

#include <linux/fs.h>
#include <linux/kfifo.h>
#include <linux/kobject.h>
#include <linux/kref.h>
#include <linux/sched.h>
#include <linux/version.h>

#define HMDFS_PAGE_SIZE	  4096
#define HMDFS_PAGE_OFFSET 12

/* max xattr value size, not include '\0' */
#define HMDFS_XATTR_SIZE_MAX	4096
/* max listxattr response size, include '\0' */
#define HMDFS_LISTXATTR_SIZE_MAX 4096

// 20 digits +'\0', Converted from a u64 integer
#define HMDFS_ACCOUNT_HASH_MAX_LEN 21
#define CTRL_PATH_MAX_LEN	   21

#define HMDFS_SUPER_MAGIC 0x20200302

#define DEFAULT_WRITE_CACHE_TIMEOUT 30
#define DEFAULT_SRV_REQ_MAX_ACTIVE 16

#define HMDFS_INODE_INVALID_FILE_ID	(1U << 31)
#define HMDFS_FID_VER_BOOT_COOKIE_SHIFT	15

/* According to task_struct instead of workqueue_struct */
#define HMDFS_WQ_NAME_LEN 16

#define HMDFS_DEF_WB_TIMEOUT_MS 60000
#define HMDFS_MAX_WB_TIMEOUT_MS 900000

#define HMDFS_READPAGES_NR_MAX	32

#define HMDFS_CID_SIZE 64

extern uint64_t path_hash(const char *path, int len, bool case_sense);

static inline bool str_n_case_eq(const char *s1, const char *s2, size_t len)
{
	return !strncasecmp(s1, s2, len);
}

static inline bool qstr_case_eq(const struct qstr *q1, const struct qstr *q2)
{
	return q1->len == q2->len && str_n_case_eq(q1->name, q2->name, q2->len);
}

static inline bool qstr_eq(const struct qstr *q1, const struct qstr *q2)
{
	return q1->len == q2->len && !strncmp(q1->name, q2->name, q2->len);
}

/*****************************************************************************
 * log print helpers
 *****************************************************************************/
void __hmdfs_log(const char *level, const bool ratelimited,
		 const char *function, const char *fmt, ...);

__printf(4, 5) void __hmdfs_log(const char *level, const bool ratelimited,
				const char *function, const char *fmt, ...);
#define hmdfs_err(fmt, ...)	\
	__hmdfs_log(KERN_ERR, false, __func__, fmt, ##__VA_ARGS__)
#define hmdfs_warning(fmt, ...) \
	__hmdfs_log(KERN_WARNING, false, __func__, fmt, ##__VA_ARGS__)
#define hmdfs_info(fmt, ...) \
	__hmdfs_log(KERN_INFO, false, __func__, fmt, ##__VA_ARGS__)
#define hmdfs_err_ratelimited(fmt, ...)	\
	__hmdfs_log(KERN_ERR, true, __func__, fmt, ##__VA_ARGS__)
#define hmdfs_warning_ratelimited(fmt, ...) \
	__hmdfs_log(KERN_WARNING, true, __func__, fmt, ##__VA_ARGS__)
#define hmdfs_info_ratelimited(fmt, ...) \
	__hmdfs_log(KERN_INFO, true, __func__, fmt, ##__VA_ARGS__)
#ifdef CONFIG_HMDFS_FS_DEBUG
#define hmdfs_debug(fmt, ...) \
	__hmdfs_log(KERN_DEBUG, false, __func__, fmt, ##__VA_ARGS__)
#define hmdfs_debug_ratelimited(fmt, ...) \
	__hmdfs_log(KERN_DEBUG, true, __func__, fmt, ##__VA_ARGS__)
#else
#define hmdfs_debug(fmt, ...)       ((void)0)
#define hmdfs_debug_ratelimited(fmt, ...)       ((void)0)
#endif

#endif // HMDFS_H
