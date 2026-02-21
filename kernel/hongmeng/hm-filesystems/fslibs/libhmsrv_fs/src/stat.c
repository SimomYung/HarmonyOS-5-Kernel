/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Implementation of stat
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 20 14:26:43 2020
 */

#include <vfs.h>

#include <sys/stat.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libmem/utils.h>
#include <hmasm/lsyscall.h>

#include <fd.h>

#include "internal.h"

int vfs_fstat(int fd, struct stat *st)
{
	return lsyscall_vfs_fstat(fd, st);
}

int vfs_cnode_fstat(int fd, __u32 cnode_idx, struct stat *st)
{
	int ret = actvcap_fscall(vfs_cnode_fstat, fd, cnode_idx, st);
	if (ret < 0) {
		hm_error("FS vfs fstat failed: %s\n", hmstrerror(ret));
	}

	return ret;
}

int vfs_stat(const char *pathname, struct stat *st)
{
	return lsyscall_vfs_fstatat(AT_FDCWD, pathname, st, 0);
}

int vfs_stat_nocheck(const char *pathname, struct stat *st)
{
	return (int)actvxcap_fscall(vfs_fstatat_nocheck, false, true, AT_FDCWD, pathname, st, 0);
}

ssize_t vfs_readlink(const char *path, char *buf, size_t bufsize)
{
	return lsyscall_vfs_readlinkat(AT_FDCWD, path, buf, bufsize);
}

int vfs_fstatat(int dfd, const char *filename, struct stat *st, unsigned int flags)
{
#ifdef __arm__
	return lsyscall_syscall4(__NR_fstatat64, (long)dfd, (long)filename,
				 (long)st, (long)flags);
#else
	return lsyscall_syscall4(__NR_newfstatat, (long)dfd, (long)filename,
				 (long)st, (long)flags);
#endif
}

ssize_t ksys_readlinkat(int dfd, const char *path, char *buf, size_t bufsize)
{
	int err = actvxcap_fscall(ksys_readlinkat, get_ksys_param_is_fwd(), true, dfd, path, buf, bufsize,
		get_ksys_param_is_fwd());
	if (err < 0) {
		hm_debug("FS ksys_readlink failed: %s\n", hmstrerror(err));
	}

	return err;
}

int ksys_fstat(int fd, struct stat *st)
{
	int err = actvxcap_fscall(ksys_fstat, get_ksys_param_is_fwd(), true, fd, st, get_ksys_param_is_fwd());
	if (err < 0) {
		hm_debug("FS ksys_fstat failed: %s\n", hmstrerror(err));
	}

	return err;
}

int ksys_fstat_nocheck(int fd, struct stat *st)
{
	int err = actvxcap_fscall(ksys_fstat_nocheck, get_ksys_param_is_fwd(), true, fd, st, get_ksys_param_is_fwd());
	if (err < 0) {
		hm_debug("FS ksys_fstat_nocheck failed: %s\n", hmstrerror(err));
	}

	return err;
}

int ksys_fstatat(int dfd, const char *filename, struct stat *st, unsigned int flags)
{
	int err = actvxcap_fscall(ksys_fstatat, get_ksys_param_is_fwd(), true, dfd, filename, st, flags,
		get_ksys_param_is_fwd());
	if (err < 0) {
		hm_debug("FS ksys_fstatat failed: %s\n", hmstrerror(err));
	}

	return err;
}

int ksys_stat(const char *pathname, struct stat *st)
{
	int err = actvxcap_fscall(ksys_fstatat, get_ksys_param_is_fwd(), true, AT_FDCWD, pathname, st, 0,
		get_ksys_param_is_fwd());
	if (err < 0) {
		hm_debug("FS ksys_stat failed: %s\n", hmstrerror(err));
	}

	return err;
}
