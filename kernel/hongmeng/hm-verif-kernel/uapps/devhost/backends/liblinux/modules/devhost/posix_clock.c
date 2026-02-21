/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Posix clock operation of devhost
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 13 11:45:43 2020
 */
#include <linux/version.h>
#include <linux/syscalls.h>
#include <linux/posix-clock.h>
#include <devhost/device.h>

#if (LINUX_VERSION_CODE > KERNEL_VERSION(4,19,0))
#define FD_TO_CLOCKID(fd)	fd_to_clockid(fd)
#endif

static int
posix_clock_adjtime(unsigned long long file_id,
		    struct timex __user *tx)
{
	return (int)sys_clock_adjtime(FD_TO_CLOCKID(file_id), tx);
}

static int
posix_clock_gettime(unsigned long long file_id,
		    struct timespec __user *tp)
{
	return (int)sys_clock_gettime(FD_TO_CLOCKID(file_id), tp);
}

static int
posix_clock_getres(unsigned long long file_id,
		   struct timespec __user *tp)
{
	return (int)sys_clock_getres(FD_TO_CLOCKID(file_id), tp);
}

static int
posix_clock_settime(unsigned long long file_id,
		    const struct timespec __user *tp)
{
	return (int)sys_clock_settime(FD_TO_CLOCKID(file_id), tp);
}

struct devhost_posix_clock_ops posix_clkops __read_mostly = {
	.clock_adjtime = posix_clock_adjtime,
	.clock_gettime = posix_clock_gettime,
	.clock_getres  = posix_clock_getres,
	.clock_settime = posix_clock_settime,
};
