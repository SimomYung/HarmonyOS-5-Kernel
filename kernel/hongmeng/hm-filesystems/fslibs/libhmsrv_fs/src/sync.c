/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of sync
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 20 14:26:43 2020
 */

#include <vfs.h>
#include <fd.h>
#include <unistd.h>
#include <fcntl.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include "internal.h"

int vfs_fsync(int fd)
{
	return lsyscall_vfs_fsync(fd);
}

void vfs_sync(void)
{
	(void)lsyscall_vfs_sync();
}

int ksys_fsync(int fd)
{
	int err;

	err = actvxcap_fscall(ksys_fsync, get_ksys_param_is_fwd(), true, fd);
	if (err < 0) {
		hm_debug("FS ksys_fsync failed: %s\n", hmstrerror(err));
	}

	return err;
}

int ksys_sync(void)
{
	int err = actvxcap_fscall(ksys_sync, get_ksys_param_is_fwd(), true);
	if (err < 0) {
		hm_warn("FS ksys_sync failed, err=%s\n", hmstrerror(err));
	}

	return err;
}
