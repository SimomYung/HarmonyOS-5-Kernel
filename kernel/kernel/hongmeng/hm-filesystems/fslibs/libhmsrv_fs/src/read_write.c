/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of read write
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 23 8:47:23 2019
 */

#include <vfs.h>

/* hongmeng */
#include <spif_fs.h>
#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libmem/utils.h>
#include <hmasm/lsyscall.h>

/* local */
#include <fd.h>
#include <dirent.h>
#include <utils/fs_malloc.h>

#include "internal.h"

int vfs_lseek(int fd, int64_t offset, int whence, int64_t *pos)
{
	int64_t ret;

	if (pos == NULL) {
		return E_HM_INVAL;
	}

#ifdef __arm__
	/* 32: the high word of offset */
	ret = (int64_t)lsyscall_vfs__llseek(fd, (unsigned long)(((uint64_t)offset) >> 32),
					    (unsigned long)(long)offset, pos, whence);
#else
	ret = (int64_t)lsyscall_vfs_lseek(fd, (long)offset, whence);
	if (ret >= 0) {
		*pos = ret;
	}
#endif
	if (ret < 0) {
		if (fd >= VFS_STDFILES_NUM && ret != E_HM_SPIPE) {
			hm_debug("FS lseek failed: %s\n", hmstrerror((int)ret));
		}
	}

	return (ret < 0) ? (int)ret : E_HM_OK;
}

/* For compress write only */
int vfs_trans(int fd, const char *data, int pos, size_t nbyte)
{
	int err;

	err = actvcap_fscall(vfs_trans, fd, data, pos, nbyte);
	if (err < 0) {
		hm_debug("FS data trans failed: %s\n", hmstrerror(err));
	}

	return err;
}

ssize_t vfs_read(int fd, void *buf, size_t nbyte)
{
	__spif_trace_fslibs_scoped(vfs_read);
	return lsyscall_vfs_read(fd, buf, nbyte);
}

ssize_t vfs_pread(int fd, int64_t pos, void *buf, size_t nbyte)
{
#ifdef __arm__
#define ARG_SIZE_VFS_PREAD 2
	union {
		long long ll;
		long l[ARG_SIZE_VFS_PREAD];
	} p;
	p.ll = (long long)pos;
	return lsyscall_vfs_pread64_arm(fd, buf, nbyte, 0, p.l[0], p.l[1]);
#undef ARG_SIZE_VFS_PREAD
#else
	return lsyscall_vfs_pread64(fd, buf, nbyte, pos);
#endif
}

ssize_t vfs_preadv(int fd, const struct iovec *vec, int vlen, int64_t pos)
{
#ifdef __arm__
	return lsyscall_syscall5(__NR_preadv, (long)fd, (long)vec, (long)vlen,
				 (long)pos, (long)((uint64_t)pos >> 32)); /* 32: off_high is the high word */
#else
	return lsyscall_syscall4(__NR_preadv, (long)fd, (long)vec, (long)vlen,
				 (long)pos);
#endif
}

ssize_t vfs_write(int fd, const void *buf, size_t nbyte)
{
	return lsyscall_vfs_write(fd, buf, nbyte);
}

ssize_t vfs_pwrite(int fd, int64_t pos, const void *buf, size_t nbyte)
{
#ifdef __arm__
#define ARG_SIZE_VFS_PWRITE 2
	union {
		long long ll;
		long l[ARG_SIZE_VFS_PWRITE];
	} p;
	p.ll = (long long)pos;
	return lsyscall_vfs_pwrite64_arm(fd, buf, nbyte, 0, p.l[0], p.l[1]);
#undef ARG_SIZE_VFS_PWRITE
#else
	return lsyscall_vfs_pwrite64(fd, buf, nbyte, pos);
#endif
}

ssize_t vfs_writev(int fd, const struct iovec *vec, int vlen)
{
	return lsyscall_vfs_writev(fd, vec, vlen);
}

ssize_t vfs_pwritev(int fd, const struct iovec *vec, int vlen, int64_t pos)
{
#ifdef __arm__
	return lsyscall_syscall5(__NR_pwritev, (long)fd, (long)vec, (long)vlen,
				 (long)pos, (long)((uint64_t)pos >> 32)); /* 32: off_high is the high word */
#else
	return lsyscall_syscall4(__NR_pwritev, (long)fd, (long)vec, (long)vlen,
				 (long)pos);
#endif
}

ssize_t vfs_readdir(int fd, void *buf, size_t size)
{
	if (size < sizeof(struct dirent)) {
		return E_HM_INVAL;
	}

	if (safe_copy(buf, buf, size) < 0) {
		hm_debug("buf addr is invalid\n");
		return E_HM_POSIX_FAULT;
	}

	return lsyscall_vfs_readdir(fd, buf, size);
}

int vfs_transfs_put_dentry(unsigned int sb_index, unsigned int d_index)
{
	int err;

	err = actvxcap_fscall(vfs_transfs_put_dentry, true, true, sb_index, d_index);
	if (err != E_HM_OK) {
		hm_warn("fscall transfs put dentry failed, err=%s\n", hmstrerror(err));
	}

	return err;
}

int vfs_transfs_inode_permission(const char *pathname, int mask)
{
	int err;

	err = actvxcap_fscall(vfs_transfs_inode_permission, true, true, AT_FDCWD, mask, pathname);
	if (err != E_HM_OK) {
		hm_warn("fscall transfs check inode permission failed, err=%s\n", hmstrerror(err));
	}

	return err;
}

int vfs_transfs_query_dinfo(const char *pathname, unsigned int *s_index,
			    unsigned int *d_index)
{
	struct __actvret_fscall_vfs_transfs_query_dinfo ret = {0};
	int err;

	if (s_index == NULL || d_index == NULL || pathname == NULL) {
		return E_HM_INVAL;
	}

	err = actvxcap_fscall(vfs_transfs_query_dinfo, true, true, AT_FDCWD, 0, pathname, &ret);
	if (err == E_HM_OK) {
		*s_index = ret.s_index;
		*d_index = ret.d_index;
	}
	return err;
}

int vfs_transfs_fget(int fd, unsigned int *sb_index, unsigned int *f_index,
		     unsigned int *me_index, unsigned long long *filp)
{
	int err;
	struct __actvret_fscall_vfs_transfs_fget ret = {0};

	err = actvxcap_fscall(vfs_transfs_fget, true, true, fd, &ret);
	if (err != E_HM_OK) {
		hm_warn("fscall transfs_fget failed, err=%s\n", hmstrerror(err));
		return err;
	}

	*sb_index = ret.sb_index;
	*f_index = ret.f_index;
	*me_index = ret.me_index;
	*filp = ret.hm_filp;

	return E_HM_OK;
}

int vfs_transfs_fput(unsigned int me_idx, unsigned long long hm_filp)
{
	int err;

	err = actvxcap_fscall(vfs_transfs_fput, true, true, me_idx, hm_filp);
	if (err != E_HM_OK) {
		hm_debug("transfs fput failed, err=%s\n", hmstrerror(err));
	}

	return err;
}

ssize_t vfs_ksys_read(int fd, void *buf, size_t nbyte)
{
	ssize_t err;

	err = (ssize_t)actvxcap_fscall(ksys_read, get_ksys_param_is_fwd(), true, fd, buf, nbyte,
		get_ksys_param_is_fwd());
	if (err < 0) {
		hm_debug("FS ksys_read failed: %s\n", hmstrerror((int)err));
	}

	return err;
}

ssize_t vfs_ksys_write(int fd, const void *buf, size_t nbyte)
{
	ssize_t err;

	err = (ssize_t)actvxcap_fscall(ksys_write, get_ksys_param_is_fwd(), true, fd, buf, nbyte,
		get_ksys_param_is_fwd());
	if (err < 0) {
		hm_debug("FS ksys_write failed: %s\n", hmstrerror((int)err));
	}

	return err;
}

ssize_t vfs_ksys_readv(int fd, const struct iovec *vec, int vlen)
{
	int err;

	err = actvxcap_fscall(ksys_readv, get_ksys_param_is_fwd(), true, fd, vec, vlen, get_ksys_param_is_fwd());
	if (err < 0) {
		hm_debug("FS ksys_readv failed: %s\n", hmstrerror(err));
	}

	return (ssize_t)err;
}

ssize_t vfs_ksys_writev(int fd, const struct iovec *vec, int vlen)
{
	int err;

	err = actvxcap_fscall(ksys_writev, get_ksys_param_is_fwd(), true, fd, vec, vlen, get_ksys_param_is_fwd());
	if (err < 0) {
		hm_debug("FS ksys_writev failed: %s\n", hmstrerror(err));
	}

	return (ssize_t)err;
}

ssize_t vfs_ksys_pread(int fd, int64_t pos, void *buf, size_t nbyte)
{
	int err;

#ifdef __arm__
#define ARG_SIZE_KSYS_PREAD 2
	union {
		long long ll;
		long l[ARG_SIZE_KSYS_PREAD];
	} p;
	p.ll = (long long)pos;
	err = actvxcap_fscall(ksys_pread64_arm, get_ksys_param_is_fwd(), true,
		fd, buf, nbyte, 0, p.l[0], p.l[1]);
#undef ARG_SIZE_KSYS_PREAD
#else
	err = actvxcap_fscall(ksys_pread64, get_ksys_param_is_fwd(), true, fd, buf, nbyte, pos, get_ksys_param_is_fwd());
#endif
	if (err < 0) {
		hm_debug("FS ksys_pread failed: %s\n", hmstrerror(err));
	}

	return (ssize_t)err;
}

ssize_t vfs_ksys_pwrite(int fd, int64_t pos, const void *buf, size_t nbyte)
{
	int err;

#ifdef __arm__
#define ARG_SIZE_KSYS_PWRITE 2
	union {
		long long ll;
		long l[ARG_SIZE_KSYS_PWRITE];
	} p;
	p.ll = (long long)pos;
	err = actvxcap_fscall(ksys_pwrite64_arm, get_ksys_param_is_fwd(), true,
		fd, buf, nbyte, 0, p.l[0], p.l[1]);
#undef ARG_SIZE_KSYS_PWRITE
#else
	err = actvxcap_fscall(ksys_pwrite64, get_ksys_param_is_fwd(), true, fd, buf, nbyte, pos,
		get_ksys_param_is_fwd());
#endif
	if (err < 0) {
		hm_debug("FS ksys_pwrite failed: %s\n", hmstrerror(err));
	}

	return (ssize_t)err;
}

ssize_t vfs_ksys_preadv(int fd, const struct iovec *vec, int vlen, int64_t pos)
{
	int err;

#ifdef __arm__
#define ARG_SIZE_KSYS_PREADV 2
	union {
		long long ll;
		long l[ARG_SIZE_KSYS_PREADV];
	} p;
	p.ll = (long long)pos;
	err = actvxcap_fscall(ksys_preadv_arm, get_ksys_param_is_fwd(), true, fd, vec, vlen,
		(unsigned long)p.l[0], (unsigned long)p.l[1]);
#undef ARG_SIZE_KSYS_PREADV
#else
	err = actvxcap_fscall(ksys_preadv, get_ksys_param_is_fwd(), true, fd, vec, vlen, pos,
		get_ksys_param_is_fwd());
#endif
	if (err < 0) {
		hm_debug("FS ksys_preadv failed: %s\n", hmstrerror(err));
	}

	return (ssize_t)err;
}

ssize_t vfs_ksys_pwritev(int fd, const struct iovec *vec, int vlen, int64_t pos)
{
	int err;

#ifdef __arm__
#define ARG_SIZE_KSYS_PWRITEV 2
	union {
		long long ll;
		long l[ARG_SIZE_KSYS_PWRITEV];
	} p;
	p.ll = (long long)pos;
	err = actvxcap_fscall(ksys_pwritev_arm, get_ksys_param_is_fwd(), true, fd, vec, vlen,
		(unsigned long)p.l[0], (unsigned long)p.l[1]);
#undef ARG_SIZE_KSYS_PWRITEV
#else
	err = actvxcap_fscall(ksys_pwritev, get_ksys_param_is_fwd(), true, fd, vec, vlen, pos,
		get_ksys_param_is_fwd());
#endif
	if (err < 0) {
		hm_debug("FS ksys_pwritev failed: %s\n", hmstrerror(err));
	}

	return (ssize_t)err;
}

int vfs_ksys_lseek(int fd, int64_t offset, int whence, int64_t *pos)
{
	int64_t err;

	if (pos == NULL) {
		return E_HM_INVAL;
	}
#ifdef __arm__
#define HIGH_WORD_OFFSET 32
	/* 32: the high word of offset */
	err = (int64_t)actvxcap_fscall(ksys__llseek, get_ksys_param_is_fwd(), true,
						fd, (unsigned long)(((uint64_t)offset) >> HIGH_WORD_OFFSET),
						(unsigned long)(long)offset, pos, whence);
#undef HIGH_WORD_OFFSET
#else
	err = (int64_t)actvxcap_fscall(ksys_lseek, get_ksys_param_is_fwd(), true, fd, (long)offset, whence);
	if (err >= 0) {
		*pos = err;
	}
#endif
	if (err < 0) {
		if (fd >= VFS_STDFILES_NUM && err != E_HM_SPIPE) {
			hm_debug("FS ksys_lseek failed: %s\n", hmstrerror((int)err));
		}
	}

	return (err < 0) ? (int)err : E_HM_OK;
}

ssize_t vfs_ksys_readdir(int fd, void *buf, size_t size)
{
	ssize_t err;

	if (size < sizeof(struct dirent)) {
		return E_HM_INVAL;
	}
	if (safe_copy(buf, buf, size) < 0) {
		hm_debug("ksys_readdir: buf addr is invalid\n");
		return E_HM_POSIX_FAULT;
	}

	err = (ssize_t)actvxcap_fscall(ksys_readdir, get_ksys_param_is_fwd(), true, fd, buf, size,
		get_ksys_param_is_fwd());
	if (err < 0) {
		hm_debug("FS ksys_readdir failed: %s\n", hmstrerror((int)err));
	}

	return err;
}
