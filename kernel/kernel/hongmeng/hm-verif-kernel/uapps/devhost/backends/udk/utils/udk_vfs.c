/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: This is the implementation of udk vfs interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 22 11:24:17 2022
 */

#include <fd.h>
#include <vfs.h>
#include <fs_transfer_fd.h>
#include <hongmeng/errno.h>
#include <libhmucap/ucap.h>
#include <libfdtable/fdtable.h>
#include <libsysif/utils_common.h>

#include <udk/log.h>
#include <udk/vfs.h>
#ifdef CONFIG_VFS_BLK_HMCRYPTO
#include <intapi/vfs/fs_block_alg.h>
#endif

int udk_vfs_open(const char *filename, unsigned int flag, mode_t mode)
{
	int fd = vfs_open(filename, flag, mode);

	return (fd < 0) ? -hmerrno2posix(fd) : fd;
}

int udk_vfs_close(int fd)
{
	return -hmerrno2posix(vfs_close(fd));
}

int udk_vfs_fstat(int fd, struct stat *stat)
{
	return -hmerrno2posix(vfs_fstat(fd, stat));
}

int udk_ksys_fstat_nocheck(int fd, struct stat *stat)
{
	return -hmerrno2posix(vfs_ksys_fstat_nocheck(fd, stat));
}

bool udk_vfs_is_dev_mounted(dev_t major, dev_t minor)
{
	return vfs_is_dev_mounted(major, minor);
}

ssize_t udk_vfs_pread(int fd, int64_t pos, void *buf, size_t nbyte)
{
	ssize_t nread = vfs_pread(fd, pos, buf, nbyte);

	return (nread < 0) ? (ssize_t)-hmerrno2posix((int)nread) : nread;
}

ssize_t udk_vfs_pwrite(int fd, int64_t pos, const void *buf, size_t nbyte)
{
	ssize_t nwrite = vfs_pwrite(fd, pos, buf, nbyte);

	return (nwrite < 0) ? (ssize_t)-hmerrno2posix((int)nwrite) : nwrite;
}

#ifdef UDK_FUSION
#include <intapi/vfs/file.h>
#include <intapi/vfs/read_write.h>
ssize_t udk_vfs_sysmgr_preadv(struct file *file, const struct iovec *vec, int vlen, int64_t pos)
{
	ssize_t nread = vfs_sysmgr_preadv(file, vec, vlen, pos);

	return (nread < 0) ? (ssize_t)-hmerrno2posix((int)nread) : nread;
}

ssize_t udk_vfs_sysmgr_pwritev(struct file *file, const struct iovec *vec, int vlen, int64_t pos)
{
	ssize_t nwrite = vfs_sysmgr_pwritev(file, vec, vlen, pos);

	return (nwrite < 0) ? (ssize_t)-hmerrno2posix((int)nwrite) : nwrite;
}

bool udk_vfs_need_fsync(int fd, uint32_t cnode_idx)
{
	return vfs_need_fsync(fd, cnode_idx);
}

void udk_vfs_set_file_mode(struct file *file, unsigned int mode)
{
	vfs_set_file_mode(file, mode);
}

void udk_vfs_unset_file_mode(struct file *file, unsigned int mode)
{
	vfs_unset_file_mode(file, mode);
}
#else
ssize_t udk_vfs_sysmgr_preadv(struct file *file, const struct iovec *vec, int vlen, int64_t pos)
{
	UNUSED(file, vec, vlen, pos);
	return -ENOSYS;
}

ssize_t udk_vfs_sysmgr_pwritev(struct file *file, const struct iovec *vec, int vlen, int64_t pos)
{
	UNUSED(file, vec, vlen, pos);
	return -ENOSYS;
}

bool udk_vfs_need_fsync(int fd, uint32_t cnode_idx)
{
	UNUSED(fd);
	UNUSED(cnode_idx);
	/* need fsync by default */
	return true;
}

void udk_vfs_set_file_mode(struct file *file, unsigned int mode)
{
	UNUSED(file, mode);
}

void udk_vfs_unset_file_mode(struct file *file, unsigned int mode)
{
	UNUSED(file, mode);
}
#endif

int udk_vfs_fsync(int fd)
{
	return -hmerrno2posix(vfs_fsync(fd));
}

int udk_vfs_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz)
{
	int ret = vfs_ioctl(fd, cmd, arg, arg_sz);
	return (ret < 0) ? -hmerrno2posix(ret) : ret;
}

static void fd_release(rref_t fdt_rref, int fd, int fd_type)
{
	(void)syscap_FDTableFreeFd(fdt_rref, (unsigned int)fd, fd_type, NULL);
}

static int fd_acquire(rref_t fdt_rref, int base_fd, int fd_type, unsigned long flag)
{
	struct sysarg_fdtable_fd_config config = {0};

	config.start = (unsigned int)base_fd;
	config.remote_fd = -1;
	config.fd_type = (unsigned int)fd_type;
	config.flags = (flag != 0) ? FDTABLE_FD_FLAG_CLOEXEC : 0;

	return syscap_FDTableAllocFd(fdt_rref, &config);
}

static int fd_alloc_child(rref_t fdt_rref, int sfiles_idx, uint32_t send_cidx, struct server_info *si)
{
	int ret;
	ret = fd_acquire(fdt_rref, 0, FD_FS, 0);
	if (ret < 0) {
		(void)actv_fscall(vfs_sfile_put, si, send_cidx, 0, sfiles_idx);
	}

	return ret;
}

static int udk_vfs_send_fd(int send_fd, uint32_t send_cidx, uint32_t recv_cidx, const char *pathname)
{
	int ret;
	struct server_info *si = NULL;
	vfs_transfer_fd_args_t transfer;

	mem_zero_s(transfer);
	ret = get_server_info(pathname, &si);
	if ((ret < 0) || (si == NULL)) {
		udk_warn("get vfs server failed, ret: %s\n", hmstrerror(ret));
		return ret;
	}

	transfer.send_cidx = send_cidx;
	transfer.recv_cidx = recv_cidx;
	transfer.send_fd = send_fd;
	ret = vfs_send_fd(si, &transfer);
	put_server_info(si);

	return ret;
}

static int udk_vfs_recv_fd(int sfiles_idx, uint32_t send_cidx, uint32_t recv_cidx, const char *pathname)
{
	int ret;
	int file_type = 0;
	int fd = -1;
	int remote_fd = -1;
	struct server_info *si = NULL;
	vfs_transfer_fd_args_t transfer;
	rref_t fdt_rref;

	mem_zero_s(transfer);

	ret = get_server_info(pathname, &si);
	if ((ret < 0) || (si == NULL)) {
		return ret;
	}

	ret = hm_fdtable_require_grant(hm_ucap_self_cnode_idx(), &fdt_rref);
	if (ret != E_HM_OK) {
		udk_warn("devhost acquire_fdtable_rref failed, rc=%s\n", hmstrerror(ret));
		put_server_info(si);
		return ret;
	}

	ret = fd_alloc_child(fdt_rref, sfiles_idx, send_cidx, si);
	if ((ret < 0)) {
		udk_warn("recv fd failed to alloc fd, %s\n", hmstrerror(ret));
		(void)hm_ucap_reject(0, fdt_rref);
		put_server_info(si);
		return ret;
	}
	fd = ret;
	transfer.sfiles_idx = sfiles_idx;
	transfer.send_cidx = send_cidx;
	transfer.recv_fd = fd;
	transfer.recv_cidx = recv_cidx;
	ret = vfs_recv_fd(si, &transfer, &file_type, &remote_fd);
	if (ret < 0) {
		udk_warn("recv fd from fsserver failed, %s\n", hmstrerror(ret));
		fd_release(fdt_rref, fd, FD_FS);
		(void)hm_ucap_reject(0, fdt_rref);
		put_server_info(si);
		return ret;
	}

	put_server_info(si);
	(void)hm_ucap_reject(0, fdt_rref);

	return transfer.recv_fd;
}

static int udk_vfs_put_fd(int sfiles_idx, uint32_t send_cidx, char *pathname)
{
	int ret;
	struct server_info *si = NULL;
	vfs_transfer_fd_args_t transfer;

	mem_zero_s(transfer);

	ret = get_server_info(pathname, &si);
	if ((ret < 0) || (si == NULL)) {
		udk_warn("vfs put send fd, %s\n", hmstrerror(ret));
		return ret;
	}

	transfer.send_cidx = send_cidx;
	transfer.recv_cidx = 0;
	transfer.sfiles_idx = sfiles_idx;
	ret = vfs_put_send_fd(si, &transfer);
	if (ret != E_HM_OK) {
		udk_warn("vfs put send fd, %s\n", hmstrerror(ret));
	}

	put_server_info(si);

	return ret;
}

int udk_vfs_transfer_fd(int send_fd)
{
	int recv_fd;
	int sfiles_idx;
	uint32_t send_cidx, recv_cidx;
	/* we use default fsmgr server to transfer fd */
	const char *pathname = "fsmgr";

	if (send_fd < 0) {
		udk_warn("invalid send fd\n");
		return -EINVAL;
	}
	/* send_cidx is the cnode of the process that owns send_fd */
	send_cidx = sysif_actv_fwd_cnode_idx();
	/* udk driver is the receiver of the transfer fd. */
	recv_cidx = hm_ucap_self_cnode_idx();
	sfiles_idx = udk_vfs_send_fd(send_fd, send_cidx, recv_cidx, pathname);
	if (sfiles_idx < 0) {
		udk_warn("send fd failed, %s\n", hmstrerror(sfiles_idx));
		return -hmerrno2posix(sfiles_idx);
	}

	/* recv_cidx should be 0, then vfs will do sec chk the receiver is legal */
	recv_fd = udk_vfs_recv_fd(sfiles_idx, send_cidx, 0, pathname);
	if (recv_fd < 0) {
		udk_vfs_put_fd(sfiles_idx, send_cidx, pathname);
		udk_warn("recv fd failed, %s\n", hmstrerror(recv_fd));
		return -hmerrno2posix(recv_fd);
	}

	return recv_fd;
}

int udk_vfs_fd_get_path(int fd, char *path_buf, unsigned int path_len)
{
	return -hmerrno2posix(vfs_fd_get_path(fd, hm_ucap_self_cnode_idx(), path_buf, path_len));
}

#ifdef CONFIG_VFS_BLK_HMCRYPTO
int udk_vfs_encrypt_decrypt_alg_register(encrypt_decrypt_alg_func endecrypt, const char *alg_name)
{
	return vfs_blk_encrypt_decrypt_alg_register(endecrypt, alg_name);
}
#else
int udk_vfs_encrypt_decrypt_alg_register(encrypt_decrypt_alg_func endecrypt, const char *alg_name)
{
	(void)endecrypt;
    (void)alg_name;
    return -1;
}
#endif
