/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of ioctl
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 20 14:26:43 2020
 */

#include <vfs.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

#include <fd.h>

#include "internal.h"

int vfs_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz)
{
	UNUSED(arg_sz);
	return lsyscall_vfs_ioctl(fd, cmd, arg);
}

int vfs_ioctl_nocheck(int fd, unsigned int cmd, unsigned long arg, int arg_sz)
{
	UNUSED(arg_sz);
	return (int)actvxcap_fscall(vfs_ioctl_nocheck, false, true, fd, cmd, arg);
}

int ksys_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz)
{
	UNUSED(arg_sz);
	return actvxcap_fscall(ksys_ioctl, get_ksys_param_is_fwd(), true, fd, cmd, arg, get_ksys_param_is_fwd());
}
