/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of fs build bug
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr  1 15:40:42 2020
 */

#ifndef LIBHMSRV_UTILS_BUILD_BUG_H
#define LIBHMSRV_UTILS_BUILD_BUG_H

#if defined(__GNUC__) && !defined(__clang__)
#include <libstrict/strict.h>

#define DEFINE_FS_BUILD_ASSERT(func, line, msg) \
	extern void __fs_##func##_##line##_assert__(void) __attribute__((__error__(msg)))

#define CALL_FS_BUILD_ASSERT(func, line) __fs_##func##_##line##_assert__()

#define fs_build_assert(cond, msg) do {			\
	const char *func = __func__;			\
	int line = __LINE__;				\
	UNUSED(func);					\
	UNUSED(line);					\
	DEFINE_FS_BUILD_ASSERT(func, line, msg);	\
	if (!(cond)) {					\
		CALL_FS_BUILD_ASSERT(func, line);	\
	}						\
} while (0)

#define FS_BUILD_BUG_ON(cond) fs_build_assert(!(cond), "BUILD FAIL: " #cond)
#else
#define FS_BUILD_BUG_ON(cond) do { } while (0)
#endif

#endif
