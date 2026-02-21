/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of clock
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 29 16:11:40 2020
 */

#include <vfs.h>

/* hongmeng */
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libmem/utils.h>

/* local */
#include <dirent.h>

#include "internal.h"

int vfs_clock_settime(clockid_t clk_id, const struct timespec *tp)
{
	int fd = clockid_to_fd(clk_id);

	return actvxcap_fscall(clock_settime, true, true, /* is_fwd, allow refwd */
				fd, ptr_to_ulong(tp));
}

int vfs_clock_gettime(clockid_t clk_id, struct timespec *tp)
{
	int fd = clockid_to_fd(clk_id);

	return actvxcap_fscall(clock_gettime, true, true, /* is_fwd, allow refwd */
			       fd, ptr_to_ulong(tp));
}

int vfs_clock_getres(clockid_t clk_id, struct timespec *tp)
{
	int fd = clockid_to_fd(clk_id);

	return actvxcap_fscall(clock_getres, true, true, /* is_fwd, allow refwd */
			       fd, ptr_to_ulong(tp));
}

int vfs_clock_adjtime(clockid_t clk_id, struct timex *tx)
{
	int fd = clockid_to_fd(clk_id);

	return  actvxcap_fscall(clock_adjtime, false, true, /* !is_fwd, allow refwd */
				fd, ptr_to_ulong(tx));
}
