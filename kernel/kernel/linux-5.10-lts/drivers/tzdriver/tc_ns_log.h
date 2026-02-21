/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef TC_NS_LOG_H
#define TC_NS_LOG_H

#include <linux/version.h>
#include <linux/sched.h>
#include <linux/printk.h>

/* defined for compile, no real means */
#ifndef CURRENT_PID
#define CURRENT_PID 0
#endif
/* defined for compile, no real means */
#ifndef CURRENT_COMM
#define CURRENT_COMM "none"
#endif

#ifndef GET_CURRENT_COMM
#define GET_CURRENT_COMM  /* USED IN LINUX MODE */
#endif

enum {
	TZ_DEBUG_VERBOSE = 0,
	TZ_DEBUG_DEBUG,
	TZ_DEBUG_INFO,
	TZ_DEBUG_WARN,
	TZ_DEBUG_ERROR,
};
#define MOD_TEE "tzdriver"

#define TEE_LOG_MASK TZ_DEBUG_INFO

#ifndef PRIx64
#define PRIx64     "llx"
#endif
#ifndef PRIu64
#define PRIu64     "llu"
#endif

/* kernel CA DO NOT USE THERE FUNCTIONS, use kernel pirnt functions */
#define tlogv(fmt, args...) \
do { \
	if (TZ_DEBUG_VERBOSE >= TEE_LOG_MASK) { \
		GET_CURRENT_COMM \
		(void)pr_info("[%s] (%i, %s)%s: " fmt, MOD_TEE, CURRENT_PID, CURRENT_COMM, __func__, ## args); \
	} \
} while (0)


#define tlogd(fmt, args...) \
do { \
	if (TZ_DEBUG_DEBUG >= TEE_LOG_MASK) { \
		GET_CURRENT_COMM \
		(void)pr_info("[%s] (%i, %s)%s: " fmt, MOD_TEE, CURRENT_PID, CURRENT_COMM, __func__, ## args); \
	} \
} while (0)


#define tlogi(fmt, args...) \
do { \
	if (TZ_DEBUG_INFO >= TEE_LOG_MASK) { \
		GET_CURRENT_COMM \
		(void)pr_info("[%s] (%i, %s)%s: " fmt, MOD_TEE, CURRENT_PID, CURRENT_COMM, __func__, ## args); \
	} \
} while (0)


#define tlogw(fmt, args...) \
do { \
	if (TZ_DEBUG_WARN >= TEE_LOG_MASK) { \
		GET_CURRENT_COMM \
		(void)pr_warn("[%s] (%i, %s)%s: " fmt, MOD_TEE, CURRENT_PID, CURRENT_COMM, __func__, ## args); \
	} \
} while (0)


#define tloge(fmt, args...) \
do { \
	GET_CURRENT_COMM \
	(void)pr_err("[%s] (%i, %s)%s: " fmt, MOD_TEE, CURRENT_PID, CURRENT_COMM, __func__, ## args); \
} while (0)

#endif
