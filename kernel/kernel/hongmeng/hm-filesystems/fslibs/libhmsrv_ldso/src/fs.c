/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of fs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 10:24:58 2019
 */

#include <vfs.h>
#include <errno.h>
#include <limits.h>
#include <fs_ldso.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <sys/stat.h>

int ldso_open(const char *name, unsigned int flags, unsigned int mode)
{
	return lsyscall_vfs_openat(AT_FDCWD, name, flags, mode, -1);
}

int ldso_close(int fd)
{
	return lsyscall_vfs_close(fd, fd);
}

ssize_t ldso_read(int fd, void *data, size_t len)
{
	ssize_t ret;

	if (data == NULL) {
		return E_HM_INVAL;
	}

	ret = lsyscall_vfs_read(fd, data, len);
	if (ret < 0) {
		hm_error("ldso: failed to read fd=%d: %s\n", fd, hmstrerror((int)ret));
	}

	return ret;
}
