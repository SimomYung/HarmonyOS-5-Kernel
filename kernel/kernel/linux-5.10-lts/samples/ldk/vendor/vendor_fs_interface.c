// SPDX-License-Identifier: GPL-2.0
/*
 * Routines that mimic syscalls, but don't use the user address space or file
 * descriptors.  Only for init/ and related early init code.
 */
#include <linux/init.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/file.h>
#include <linux/security.h>
#include <linux/fsnotify.h>
#include <linux/dirent.h>
#include <linux/mount.h>
#include <linux/syscalls.h>

#include <platform_include/basicplatform/linux/fs/vendor_fs_interface.h>

/* platform_source/basicplatform/fs/vendor_fs_interface.c */
long vendor_access(const char *filename, int mode)
{
	return ksys_access(filename, mode);
}
EXPORT_SYMBOL(vendor_access);

long vendor_mkdir(const char *pathname, umode_t mode)
{
	return ksys_mkdir(pathname, mode);
}
EXPORT_SYMBOL(vendor_mkdir);

long vendor_rmdir(const char *pathname)
{
	return ksys_rmdir(pathname);
}
EXPORT_SYMBOL(vendor_rmdir);

long vendor_unlink(const char *pathname)
{
	return ksys_unlink(pathname);
}
EXPORT_SYMBOL(vendor_unlink);

long vendor_rename(const char *old_name, const char *new_name)
{
	return ksys_rename(old_name, new_name);
}
EXPORT_SYMBOL(vendor_rename);
#define FILE_NAME_LEN  64

struct plat_file {
	struct inode f_inode;
	struct dentry dentry;
	struct inode d_inode;
	int fd;
	struct mutex flock;
	char name[FILE_NAME_LEN];
};
long vendor_getdents64(struct file *filp, struct linux_dirent64 *buf, unsigned int count)
{
	struct plat_file *internal_fp = NULL;
	long ret = 0;

	if (IS_ERR_OR_NULL(filp))
		return -EINVAL;

	internal_fp = filp->private_data;
	if (internal_fp == NULL)
		return -EINVAL;

	mutex_lock(&internal_fp->flock);
	ret = ksys_getdents64(internal_fp->fd, buf, (size_t)count);
	mutex_unlock(&internal_fp->flock);

	return ret;
}
EXPORT_SYMBOL(vendor_getdents64);
