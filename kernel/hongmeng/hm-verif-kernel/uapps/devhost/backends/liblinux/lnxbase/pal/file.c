/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for file operations
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar  4 15:50:55 2020
 */
#include <sys/uio.h>
#include <liblinux/pal.h>

#include <devhost/file.h>
#include <devhost/poll.h>
#include <devhost/transfs.h>
#include <lnxbase/lnxbase.h>
#include <libsysif/utils.h>

void *liblinux_pal_fget_ex(enum liblinux_fd_type fd_type, int fd, void (*fget)(void*))
{
	lnxbase_fget_fn_t fget_fn = lnxbase_get_fget_fn(fd_type);
	if (fget_fn != NULL) {
		return fget_fn(fd, fget);
	}

	return NULL;
}

void *liblinux_pal_fget(int fd)
{
	return liblinux_pal_fget_ex(DEFAULT_FD, fd, NULL);
}

void *liblinux_pal_transfs_fget(int fd)
{
	return liblinux_pal_fget_ex(TRANSFS_FD, fd, NULL);
}

void liblinux_pal_fput_ex(enum liblinux_fd_type fd_type, void *file)
{
	lnxbase_fput_fn_t fput = lnxbase_get_fput_fn(fd_type);
	if (fput != NULL) {
		fput(file);
	}
}

void liblinux_pal_transfs_fput(void *file)
{
	liblinux_pal_fput_ex(TRANSFS_FD, file);
}

void *liblinux_pal_gdentry(const char *pathname)
{
	lnxbase_gdentry_fn_t gdentry = lnxbase_get_dentry_fn();
	if (gdentry != NULL) {
		return gdentry(pathname);
	}

	return NULL;
}

void liblinux_pal_pdentry(void *sb, void *dentry)
{
	lnxbase_pdentry_fn_t pdentry = lnxbase_put_dentry_fn();
	if (pdentry != NULL) {
		pdentry(sb, dentry);
	}
}

int liblinux_pal_get_unused_fd_flags(unsigned flags)
{
	int ret;

	ret = lnxbase_get_unused_fd_flags(flags);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return ret;
}

void liblinux_pal_fd_install(unsigned int fd, void *file)
{
	lnxbase_fd_install(fd, file, NULL);
}

void liblinux_pal_fd_install_ex(unsigned int fd, void *file, struct dh_fd_install_context *ctx)
{
	lnxbase_fd_install(fd, file, ctx);
}

int liblinux_pal_alloc_unused_fd(unsigned int flags, void *file, struct dh_fd_install_context *ctx)
{
	int ret = 0;
	ret = lnxbase_alloc_unused_fd(flags, file, ctx);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

void liblinux_pal_put_unused_fd(int fd)
{
	lnxbase_put_unused_fd(fd);
}

int liblinux_pal_ksys_access(const char *pathname, int mode)
{
	return lnxbase_ksys_access(pathname, mode);
}

int liblinux_pal_vfs_open(const char *filename, unsigned int flag, mode_t mode)
{
	return lnxbase_vfs_open(filename, flag, mode);
}

ssize_t liblinux_pal_ksys_pwrite(int fd, int64_t pos, const void *buf, size_t nbyte)
{
	return lnxbase_ksys_pwrite(fd, pos, buf, nbyte);
}

ssize_t liblinux_pal_ksys_pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
	return lnxbase_ksys_pwritev(fd, iov, iovcnt, offset);
}

ssize_t liblinux_pal_ksys_pread(int fd, int64_t pos, void *buf, size_t nbyte)
{
	return lnxbase_ksys_pread(fd, pos, buf, nbyte);
}

ssize_t liblinux_pal_ksys_preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
	return lnxbase_ksys_preadv(fd, iov, iovcnt, offset);
}

int liblinux_pal_vfs_close(int fd)
{
	return lnxbase_vfs_close(fd);
}

int liblinux_pal_vfs_fstat(int fd, struct stat *stat)
{
	return lnxbase_vfs_fstat(fd, stat);
}

int liblinux_pal_ksys_fstat(int fd, struct stat *stat)
{
	return lnxbase_ksys_fstat(fd, stat);
}

int liblinux_pal_ksys_fstat_nocheck(int fd, struct stat *stat)
{
	return lnxbase_ksys_fstat_nocheck(fd, stat);
}

int liblinux_pal_ksys_statfs(const char *pathname, struct statfs *statfs)
{
	return lnxbase_ksys_statfs(pathname, statfs);
}

int liblinux_pal_ksys_fstatat(int dfd, const char *filename, struct stat *stat, unsigned int flags)
{
	return lnxbase_ksys_fstatat(dfd, filename, stat, flags);
}

int liblinux_pal_vfs_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz)
{
	return lnxbase_vfs_ioctl(fd, cmd, arg, arg_sz);
}

int liblinux_pal_ksys_rename(const char *oldpath, const char *newpath)
{
	return lnxbase_ksys_rename(oldpath, newpath);
}

int liblinux_pal_ksys_readlinkat(int dfd, const char *path, char *buf, size_t bufsize)
{
	return lnxbase_ksys_readlinkat(dfd, path, buf, bufsize);
}

int liblinux_pal_ksys_mkdir(const char *pathname, mode_t mode)
{
	return lnxbase_ksys_mkdir(pathname, mode);
}

int liblinux_pal_ksys_rmdir(const char *pathname)
{
	return lnxbase_ksys_rmdir(pathname);
}

int liblinux_pal_ksys_unlink(const char *pathname)
{
	return lnxbase_ksys_unlink(pathname);
}

int liblinux_pal_ksys_fchown(int fd, uid_t uid, gid_t gid)
{
	return lnxbase_ksys_fchown(fd, uid, gid);
}

int liblinux_pal_ksys_open(const char *filename, unsigned int flag, mode_t mode)
{
	return lnxbase_ksys_open(filename, flag, mode);
}

int liblinux_pal_ksys_openat(int dfd, const char *filename, unsigned int flag, mode_t mode)
{
	return lnxbase_ksys_openat(dfd, filename, flag, mode);
}

int liblinux_pal_ksys_close(int fd)
{
	return lnxbase_ksys_close(fd);
}

int liblinux_pal_ksys_ioctl(int fd, unsigned int cmd, unsigned long arg, int arg_sz)
{
	return lnxbase_ksys_ioctl(fd, cmd, arg, arg_sz);
}

int liblinux_pal_ksys_lseek(int fd, int64_t offset, int whence, int64_t *pos)
{
	return lnxbase_ksys_lseek(fd, offset, whence, pos);
}

ssize_t liblinux_pal_ksys_write(int fd, const void *buf, size_t nbyte)
{
	return lnxbase_ksys_write(fd, buf, nbyte);
}

ssize_t liblinux_pal_vfs_read(int fd, void *buf, size_t nbyte)
{
	return lnxbase_vfs_read(fd, buf, nbyte);
}

ssize_t liblinux_pal_ksys_read(int fd, void *buf, size_t nbyte)
{
	return lnxbase_ksys_read(fd, buf, nbyte);
}

ssize_t liblinux_pal_ksys_readdir(int fd, void *buf, size_t nbyte)
{
	return lnxbase_ksys_readdir(fd, buf, nbyte);
}

int liblinux_pal_ksys_fsync(int fd)
{
	return lnxbase_ksys_fsync(fd);
}

int liblinux_pal_ksys_sync(void)
{
	return lnxbase_ksys_sync();
}

long liblinux_pal_ksys_symlink(const char *oldname, const char *newname)
{
	return lnxbase_ksys_symlink(oldname, newname);
}

void liblinux_pal_sync_wakeup_trad(void *filp_node, unsigned int revents)
{
	return devhost_sync_revents_to_hmkobj_trad(filp_node, revents);
}

void liblinux_pal_sync_wakeup(void *kobj_ctx, unsigned int revents)
{
	return devhost_sync_revents_to_hmkobj(kobj_ctx, revents);
}

void *liblinux_pal_sync_open(void *kobj_ctx)
{
	return devhost_sync_open(kobj_ctx);
}

void liblinux_pal_sync_close(void *kobj_ctx)
{
	return devhost_sync_close(kobj_ctx);
}
