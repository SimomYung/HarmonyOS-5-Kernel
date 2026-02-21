/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of xattr
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 27 17:29:00 2019
 */

#include <vfs.h>
#include <fd.h>
#include <libstrict/strict.h>
#include <libmem/utils.h>
#include <sys/xattr.h>
#include "internal.h"

int vfs_getxattr(const char *path, const char *name,
		 char *value, size_t size, ssize_t *nbyte)
{
	if (nbyte == NULL) {
		return E_HM_INVAL;
	}
	ssize_t err = lsyscall_vfs_getxattr(path, name, value, size);
	if (err < 0) {
		return (int)err;
	}
	*nbyte = err;
	return E_HM_OK;
}

int vfs_setxattr(const char *path, const char *name,
		 const char *value, size_t size, int flags)
{
	return lsyscall_vfs_setxattr(path, name, value, size, flags);
}

int vfs_fsetxattr(int fd, const char *name, const char *value, size_t size,
		  int flags)
{
	return lsyscall_vfs_fsetxattr(fd, name, value, size, flags);
}

int vfs_removexattr(const char *path, const char *name)
{
	return lsyscall_vfs_removexattr(path, name);
}
