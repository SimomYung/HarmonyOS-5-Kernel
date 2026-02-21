/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Implementation of open
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 18 15:23:43 2019
 */

#include <vfs.h>
#include <sys/types.h>

#include <libhmsrv_sys/hm_timer.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <hmnet/hm_socket.h>
#include <libstrict/strict.h>
#include <fd.h>

#include "internal.h"

int vfs_chown(const char *filename, uid_t uid, gid_t gid)
{
	return lsyscall_vfs_fchownat(AT_FDCWD, filename, uid, gid, 0);
}

int vfs_lchown(const char *filename, uid_t uid, gid_t gid)
{
	return lsyscall_vfs_fchownat(AT_FDCWD, filename, uid, gid, AT_SYMLINK_NOFOLLOW);
}

int vfs_fchown(int fd, uid_t uid, gid_t gid)
{
	return lsyscall_vfs_fchown(fd, uid, gid);
}

int vfs_dup(int oldfd)
{
	return lsyscall_vfs_dup(oldfd, oldfd);
}

int vfs_fwd_dup2(int oldfd, int newfd, uint32_t cnode_idx)
{
	return actvxcap_fscall(vfs_dup2_internal, true, true, oldfd, newfd, cnode_idx);
}

/* sendfile used, will be deleted later */
int vfs_fcntl(int fd, int cmd, unsigned long arg)
{
	return lsyscall_vfs_fcntl(fd, cmd, arg, fd);
}

int vfs_mkdir(const char *pathname, mode_t mode)
{
	return lsyscall_vfs_mkdirat(AT_FDCWD, pathname, mode);
}

int vfs_access(const char *pathname, int mode)
{
	return lsyscall_vfs_faccessat2(AT_FDCWD, pathname, mode, 0);
}

int vfs_fwd_access(const char *pathname, int mode)
{
	return actvxcap_fscall(vfs_access_internal, true, true, pathname, mode);
}

int vfs_unix_access(const char *name, int mode, vfs_unix_node_t *unode)
{
	struct __actvret_fscall_vfs_unix_access ret = {0};
	int err;

	if (unode == NULL) {
		return E_HM_POSIX_FAULT;
	}

	err = actvxcap_fscall(vfs_unix_access, true, true, name, mode, &ret);
	if (err == E_HM_OK) {
		unode->index = ret.index;
		unode->dev_id = ret.dev_id;
	}

	return err;
}

struct open_args {
	int dfd;
	const char *path;
	uint32_t flags;
	mode_t mode;
	uint32_t cnode_idx;
};

static int __vfs_openat(const struct open_args *oa, bool is_fwd, bool allow_fwd)
{
	return actvxcap_fscall(vfs_openat_internal, is_fwd, allow_fwd,
			       oa->cnode_idx, oa->dfd, oa->path, oa->flags, oa->mode);
}

int vfs_cnode_open(const char *filename, unsigned int flag, mode_t mode, uint32_t cidx)
{
	const struct open_args oa = {
		.dfd = AT_FDCWD,
		.path = filename,
		.flags = flag,
		.mode = mode,
		.cnode_idx = cidx,
	};
	return __vfs_openat(&oa, false, true);
}

int vfs_sys_open(const char *filename, unsigned int flag, mode_t mode)
{
	return vfs_cnode_open(filename, flag, mode, sysif_actv_src_cnode_idx());
}

/**
 * vfs_fwd_openat - open a file handle by dfd && filename
 *
 * Difference with vfs_openat:
 * 	'is_forward' is ture && 'allow_forward' is true,
 * 	that allow fs to get origin transaction instead of creat a new transaction.
 */
int vfs_fwd_openat(int dfd, const char *filename, unsigned int flag, mode_t mode)
{
	const struct open_args oa = {
		.dfd = dfd,
		.path = filename,
		.flags = flag,
		.mode = mode,
		.cnode_idx = 0,
	};
	return __vfs_openat(&oa, true, true);
}

int vfs_fwd_open(const char *filename, unsigned int flag, mode_t mode)
{
	return vfs_fwd_openat(AT_FDCWD, filename, flag, mode);
}

int vfs_openat(int dfd, const char *filename, unsigned int flag, mode_t mode)
{
	return lsyscall_vfs_openat(dfd, filename, flag, mode, -1);
}

int vfs_open(const char *filename, unsigned int flag, mode_t mode)
{
	return vfs_openat(AT_FDCWD, filename, flag, mode);
}

static void _vfs_open_exec_ret_set(struct vfs_open_exec_info *exec_info,
				   const struct __actvret_fscall_vfs_open_exec *ret)
{
	if (exec_info->file_cred != NULL) {
		*(exec_info->file_cred) = ret->file_cred;
	}
	*(exec_info->fd) = ret->uapp_rfd;
	if (exec_info->mnt_nosuid != NULL) {
		*(exec_info->mnt_nosuid) = ret->mnt_nosuid;
	}
}

/*
 * If the cnode_idx_for_root is non-zero, it will be used instead of the
 * cnode_idx to lookup.
 */
int vfs_open_exec(const char *path, struct vfs_open_exec_info *exec_info)
{
	struct __actvret_fscall_vfs_open_exec ret = {0};
	int err;
	int fd_proxy = -1;
	bool is_fwd = sysif_actv_fwd_cnode_idx() != 0;
	char *srv_name = FSMGR_SVRNAME;

	if (exec_info == NULL ||
	    exec_info->fd == NULL) {
		return E_HM_INVAL;
	}

	vfs_audit_watch_point_start();
	err = actvxcap_fscall(vfs_open_exec, is_fwd, true, AT_FDCWD, exec_info->cnode_idx,
			      exec_info->cnode_idx_for_root, path, &ret);
	if (err == 0) {
		fd_proxy = ret.sys_fd;
		_vfs_open_exec_ret_set(exec_info, &ret);
	}

	/* special case: uapp's root/cwd is not in fsmgr,
	 * change start server, and do retry.
	 */
	vfs_audit_watch_point_end(true, path, srv_name);
	return (err < 0) ? err : fd_proxy;
}

int vfs_close_exec(int sys_fd, uint32_t cnode_idx, int uapp_rfd)
{
	int err;

	err = actvxcap_fscall(vfs_close_exec, false, true, sys_fd, cnode_idx, uapp_rfd);
	if (err != E_HM_OK) {
		hm_debug("close exec file failed, sys_fd=%d, "
			 "cnode_idx=%"PRIu32", uapp_rfd=%d, err=%s\n",
			 sys_fd, cnode_idx, uapp_rfd,
			 hmstrerror(err));
	}

	return err;
}

int vfs_close(int fd)
{
	int err = lsyscall_vfs_close(fd, fd);
	err = (err == E_HM_ACTIVATION_CRASH) ? E_HM_OK : err;
	if (err != E_HM_OK) {
		hm_debug("close failed, fd=%d, err=%s\n", fd, hmstrerror(err));
	}

	return err;
}

int vfs_cnode_close(int fd, uint32_t cidx)
{
	return actvxcap_fscall(vfs_close_internal, false, true, fd, cidx);
}

int vfs_fallocate(int fd, int mode, uint64_t offset, uint64_t length)
{
#ifdef __arm__
	long off_low_addr;
	long off_high_addr;
	long len_low_addr;
	long len_high_addr;
	union {
		long long ll;
		long l[2]; /* 2 * 32bit splice to 64bit */
	} p;

	p.ll = (long long)(unsigned long long)offset;
	off_low_addr = p.l[0];
	off_high_addr = p.l[1];

	p.ll = (long long)(unsigned long long)length;
	len_low_addr = p.l[0];
	len_high_addr = p.l[1];

	return lsyscall_vfs_fallocate_arm(fd, mode, off_low_addr, off_high_addr, len_low_addr, len_high_addr);
#else
	return lsyscall_vfs_fallocate(fd, mode, (long)(unsigned long)offset, (long)(unsigned long)length);
#endif
}

int ksys_fchown(int fd, uid_t uid, gid_t gid)
{
	int err;

	err = actvxcap_fscall(ksys_fchown, get_ksys_param_is_fwd(), true, fd, uid, gid);
	if (err < 0) {
		hm_debug("FS ksys_fchown failed: %s\n", hmstrerror(err));
	}

	return err;
}

int ksys_mkdir(const char *pathname, mode_t mode)
{
	int err;

	err = actvxcap_fscall(ksys_mkdirat, get_ksys_param_is_fwd(), true, AT_FDCWD, pathname, mode);
	if (err < 0) {
		hm_debug("FS ksys_mkdir failed: %s\n", hmstrerror(err));
	}

	return err;
}

int ksys_access(const char *pathname, int mode)
{
	int err;

	err = actvxcap_fscall(ksys_faccessat2, get_ksys_param_is_fwd(), true, AT_FDCWD, pathname, mode, 0);
	if (err < 0) {
		hm_debug("FS ksys_access failed: %s\n", hmstrerror(err));
	}

	return err;
}

static int __ksys_openat(const struct open_args *oa, bool is_fwd, bool allow_fwd)
{
	int err;

	err = actvxcap_fscall(ksys_openat, is_fwd, allow_fwd, oa->dfd, oa->path, oa->flags, oa->mode);
	if (err < 0) {
		hm_debug("ksys_open failed, err=%s\n", hmstrerror(err));
	}

	return err;
}

int ksys_openat(int dfd, const char *filename, unsigned int flag, mode_t mode)
{
	const struct open_args oa = {
		.dfd = dfd,
		.path = filename,
		.flags = flag,
		.mode = mode,
		.cnode_idx = 0,
	};
	return __ksys_openat(&oa, get_ksys_param_is_fwd(), true);
}

int ksys_open(const char *filename, unsigned int flag, mode_t mode)
{
	return ksys_openat(AT_FDCWD, filename, flag, mode);
}

int ksys_close(int fd)
{
	int err;

	err = actvxcap_fscall(ksys_close, get_ksys_param_is_fwd(), true, fd);
	err = (err == E_HM_ACTIVATION_CRASH) ? E_HM_OK : err;
	if (err != E_HM_OK) {
		hm_debug("ksys_close failed, fd=%d, err=%s\n", fd, hmstrerror(err));
	}

	return err;
}
