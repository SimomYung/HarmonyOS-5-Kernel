/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef _UKMD_LOG_H_
#define _UKMD_LOG_H_

#include <linux/types.h>
#include <linux/ktime.h>
#include "uksm_debug.h"

#define void_unused(x)    ((void)(x))

enum {
	UKMD_LOG_LVL_ERR = 0,
	UKMD_LOG_LVL_WARNING,
	UKMD_LOG_LVL_INFO,
	UKMD_LOG_LVL_DEBUG,
	UKMD_LOG_LVL_MAX,
};

#define xkmd_dbg_nop(...)  ((void)#__VA_ARGS__)

#define xkmd_pr_err(prefix, msg, ...)  do { \
		pr_err("E %s[%s:%d]"msg, prefix, __func__, __LINE__, ## __VA_ARGS__); \
	} while (0)

#define xkmd_pr_warn(prefix, level, msg, ...)  do { \
		if (level >= UKMD_LOG_LVL_WARNING) \
			pr_warn("W %s[%s:%d]"msg, prefix, __func__, __LINE__, ## __VA_ARGS__); \
	} while (0)

#define xkmd_pr_info(prefix, level, msg, ...)  do { \
		if (level >= UKMD_LOG_LVL_INFO) \
			pr_info("I %s[%s:%d]"msg, prefix, __func__, __LINE__, ## __VA_ARGS__); \
	} while (0)

#define ukmd_pr_err(msg, ...) xkmd_pr_err("UKMD", msg, ## __VA_ARGS__)
#define ukmd_pr_warn(msg, ...) xkmd_pr_warn("UKMD", g_ukmd_log_level, msg, ## __VA_ARGS__)
#define ukmd_pr_info(msg, ...) xkmd_pr_info("UKMD", g_ukmd_log_level, msg, ## __VA_ARGS__)

#ifdef CONFIG_UKMD_DEBUG_ENABLE

#define xkmd_pr_debug(prefix, level, msg, ...)  do { \
		if (level >= UKMD_LOG_LVL_DEBUG) \
			pr_info("D %s[%s:%d]"msg, prefix, __func__, __LINE__, ## __VA_ARGS__); \
	} while (0)

#define ukmd_pr_debug(msg, ...) xkmd_pr_debug("UKMD", g_ukmd_log_level, msg, ## __VA_ARGS__)

#else

#define xkmd_pr_debug(prefix, msg, level, ...) xkmd_dbg_nop(__VA_ARGS__)
#define ukmd_pr_debug(msg, ...) xkmd_pr_debug("UKMD", g_ukmd_log_level, msg, __VA_ARGS__)

#endif

#define ukmd_check_and_return(condition, ret, level, msg, ...) \
	do { \
		if (condition) { \
			ukmd_pr_##level(msg, ##__VA_ARGS__);\
			return ret; \
		} \
	} while (0)

#define ukmd_check_and_no_retval(condition, level, msg, ...) \
	do { \
		if (condition) { \
			ukmd_pr_##level(msg, ##__VA_ARGS__);\
			return; \
		} \
	} while (0)

#ifdef CONFIG_UKMD_ASSERT_ENABLE
#define ukmd_assert(cond)  do { \
		if (cond) { \
			ukmd_pr_err("assertion failed! %s,%s:%d,%s\n", #cond,  __FILE__, __LINE__, __func__); \
			BUG(); \
		} \
	} while (0)
#else
#define ukmd_assert(ptr)
#endif

#endif /* UKMD_LOG_H */
