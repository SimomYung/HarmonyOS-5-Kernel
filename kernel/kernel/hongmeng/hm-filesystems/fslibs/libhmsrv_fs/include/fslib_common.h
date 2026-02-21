/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of common
 * Author: Huawei OS Kernel Lab
 * Create: Tus Apr 30 14:32:08 2019
 */

#ifndef LIBVFS_COMMON_H
#define LIBVFS_COMMON_H

#include <lib/utils.h>
#include <hongmeng/panic.h>
#include <stdbool.h>

#define min(x, y) ({			\
	typeof(x) __x = (x);		\
	typeof(y) __y = (y);		\
	__x < __y ? __x : __y;		\
})

#define max(x, y) ({			\
	typeof(x) __x = (x);		\
	typeof(y) __y = (y);		\
	__x > __y ? __x : __y;		\
})

static inline bool is_power_of_2(unsigned long n)
{
	return (n != 0 && IS_ALIGNED(n, n));
}

#if ((defined CONFIG_FSLIB_DEBUG) || (defined CONFIG_FSRV_DEBUG))

#define FS_NOFAIL(s) do {			\
	int ___ret;				\
	___ret = (s);				\
	if (___ret != 0) {			\
		hm_error("err %d\n", ___ret);	\
	}					\
	NOFAIL(___ret);				\
} while (0)

#define fs_bug_on(fmt...) BUG_ON(fmt)
#define fs_bug(fmt...) hm_warn(fmt);BUG_ON(1)
#define fs_debug(fmt...)  hm_info(fmt)

#else
#define fs_bug_on(fmt...)
#define fs_bug(fmt...) hm_warn(fmt)
#define FS_NOFAIL(s) NOFAIL(s)
/* fs_debug will be deleted by the compiler in the release version */
#define fs_debug(fmt...) hm_debug(fmt)

#endif	/* !CONFIG_FSLIB_DEBUG */

#define fs_build_bug_on(condition) ((void)sizeof(char[1 - 2*((!!(condition)) ? 1 : 0)]))

#define __user

#endif	/* !LIBVFS_COMMON_H */
