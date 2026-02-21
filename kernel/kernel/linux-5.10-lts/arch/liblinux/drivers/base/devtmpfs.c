// SPDX-License-Identifier: GPL-2.0-only
#include <linux/namei.h>
#include <linux/init_syscalls.h>
#include <linux/fs_struct.h>
#include <uapi/linux/mount.h>
#include <trace/hooks/liblinux.h>

static int dev_mkdir(const char *name, umode_t mode, struct task_struct **task)
{
	struct dentry *d;
	struct path path;
	int ret;

	d = kern_path_create(AT_FDCWD, name, &path, LOOKUP_DIRECTORY);
	if (IS_ERR(d))
		return PTR_ERR(d);

	ret = vfs_mkdir(d_inode(path.dentry), d, mode);
	if (!ret)
		d_inode(d)->i_private = task;
	done_path_create(&path, d);
	return ret;
}

static void hook_devtmpfs_setup(void *args, struct task_struct **task, int *err)
{
	int ret;

	ret = unshare_fs_struct();
	if (ret)
		goto out;
	ret = dev_mkdir("/dev", 0755, task);
	if (ret)
		goto out;
	ret = init_mount("devtmpfs", "/dev", "devtmpfs", MS_SILENT, NULL);
	if (ret)
		goto out;
	init_chdir("/dev");
	init_chroot(".");
out:
	*err = ret;
}

INIT_VENDOR_HOOK(ldk_rvh_devtmpfs_setup, hook_devtmpfs_setup);
