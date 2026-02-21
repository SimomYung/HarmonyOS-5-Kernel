/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of statfs
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jan 13 14:26:43 2019
 */
#include <vfs.h>

#include <sys/statfs.h>
#include <sys/stat.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/atomic.h>
#include <fd.h>

#include "internal.h"

int vfs_statfs(const char *pathname, struct statfs *stfs)
{
	return lsyscall_vfs_statfs(pathname, stfs);
}

int ksys_statfs(const char *pathname, struct statfs *stfs)
{
	int err = actvxcap_fscall(ksys_statfs, get_ksys_param_is_fwd(), true, pathname, stfs, get_ksys_param_is_fwd());
	if (err < 0) {
		hm_debug("FS ksys_statfs failed: %s\n", hmstrerror(err));
	}

	return err;
}
