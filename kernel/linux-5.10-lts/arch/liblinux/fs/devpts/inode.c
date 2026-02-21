// SPDX-License-Identifier: GPL-2.0-only
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/namei.h>
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/mutex.h>
#include <linux/idr.h>
#include <linux/uidgid.h>
#include <linux/devpts_fs.h>

#include <trace/hooks/liblinux.h>

#define PTS_INFO_NO_INIT	((void *) 1)

struct pts_mount_opts {
	int max;
};

struct pts_fs_info {
	struct idr allocated_ptys;
	struct pts_mount_opts mount_opts;
};

struct pts_info {
	int index;
	dev_t devt;
	struct device *dev;
	struct cdev *cdev;
	struct dentry dentry;
};

static struct pts_fs_info pts_fsi = {
	.allocated_ptys = IDR_INIT(pts_fsi.allocated_ptys),
	.mount_opts = {
		.max = NR_UNIX98_PTY_MAX,
	}
};

static int pty_limit = NR_UNIX98_PTY_DEFAULT;
static atomic_t pty_count = ATOMIC_INIT(0);
static struct vfsmount devpts_mnt;
static DEFINE_MUTEX(pty_idr_mutex);
static struct file_operations pts_fops;

struct pts_fs_info *devpts_acquire(struct file *filp)
{
	return &pts_fsi;
}

int devpts_new_index(struct pts_fs_info *fsi)
{
	int index = -ENOSPC;

	if (atomic_read(&pty_count) >= pty_limit)
		return index;

	mutex_lock(&pty_idr_mutex);
	if (atomic_inc_return(&pty_count) >= pty_limit)
		goto out;

	index = idr_alloc(&fsi->allocated_ptys, PTS_INFO_NO_INIT, 0,
			  fsi->mount_opts.max - 1, GFP_KERNEL);
out:
	if (index < 0)
		atomic_dec(&pty_count);
	mutex_unlock(&pty_idr_mutex);
	return index;
}

static void __device_release(struct device *dev)
{
	kfree(dev);
}

static char *pts_devnode(struct device *dev, umode_t *mode,
			 kuid_t *uid, kgid_t *gid);
static char *pts_devnode(struct device *dev, umode_t *mode,
			 kuid_t *uid, kgid_t *gid)
{
	*uid = current_fsuid();
	*gid = current_fsgid();

	return kasprintf(GFP_KERNEL, "pts/%s", dev_name(dev));
}

const struct device_type pts_type = {
	.name = "pst_dev",
	.devnode = pts_devnode,
};

struct dentry *devpts_pty_new(struct pts_fs_info *fsi, int index, void *priv)
{
	int ret;
	struct device *dev;
	struct cdev *cdev;
	dev_t devt;
	struct pts_info *info;

	info = kzalloc(sizeof(struct pts_info), GFP_KERNEL);
	if (!info)
		return ERR_PTR(-ENOMEM);

	devt = MKDEV(UNIX98_PTY_SLAVE_MAJOR, index);
	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev) {
		kfree(info);
		return ERR_PTR(-ENOMEM);
	}

	cdev = cdev_alloc();
	if (!cdev) {
		kfree(dev);
		kfree(info);
		return ERR_PTR(-ENOMEM);
	}

	cdev->ops = &pts_fops;
	ret = cdev_add(cdev, devt, 1);
	if (ret) {
		cdev_del(cdev);
		kfree(dev);
		kfree(info);
		return ret;
	}

	dev->devt = devt;
	dev->class = tty_class;
	dev->release = __device_release;
	dev->type = &pts_type;
	dev_set_name(dev, "%d", index);
	dev_set_uevent_suppress(dev, 1);

	ret = device_register(dev);
	if (ret)
		goto err;

	kobject_uevent(&dev->kobj, KOBJ_ADD);

	info->index = index;
	info->devt = devt;
	info->dev = dev;
	info->cdev = cdev;
	info->dentry.d_fsdata = priv;
	idr_replace(&fsi->allocated_ptys, info, index);

	return &info->dentry;
err:
	put_device(dev);
	cdev_del(cdev);
	kfree(info);
	return ERR_PTR(ret);
}

void devpts_pty_kill(struct dentry *dentry) {}

struct vfsmount *devpts_mntget(struct file *filp, struct pts_fs_info *fsi)
{
	return &devpts_mnt;
}

void *devpts_get_priv(struct dentry *dentry)
{
	return NULL;
}

void devpts_kill_index(struct pts_fs_info *fsi, int idx)
{
	struct pts_info *info;

	mutex_lock(&pty_idr_mutex);
	info = idr_find(&fsi->allocated_ptys, idx);
	if (!info) {
		mutex_unlock(&pty_idr_mutex);
		pr_err("ldk: pts idx=%d is invalid.\n", idx);
		return;
	} else if (info == PTS_INFO_NO_INIT) {
		idr_remove(&fsi->allocated_ptys, idx);
		atomic_dec(&pty_count);
		mutex_unlock(&pty_idr_mutex);
		return;
	}

	cdev_del(info->cdev);
	device_destroy(tty_class, info->devt);

	idr_remove(&fsi->allocated_ptys, idx);
	atomic_dec(&pty_count);
	mutex_unlock(&pty_idr_mutex);
	kfree(info);
}

void devpts_release(struct pts_fs_info *fsi) {}

static void hook_devpts_get_priv(void *__data, int index, void **ret)
{
	struct pts_info *info;

	mutex_lock(&pty_idr_mutex);
	info = idr_find(&pts_fsi.allocated_ptys, index);
	mutex_unlock(&pty_idr_mutex);

	if (!info || info == PTS_INFO_NO_INIT)
		*ret = NULL;
	else
		*ret = info->dentry.d_fsdata;
}
INIT_VENDOR_HOOK(ldk_vh_devpts_get_priv, hook_devpts_get_priv);

struct device *devpts_find_device(int index)
{
	struct pts_info *info;

	mutex_lock(&pty_idr_mutex);
	info = idr_find(&pts_fsi.allocated_ptys, index);
	mutex_unlock(&pty_idr_mutex);

	if (info && info != PTS_INFO_NO_INIT)
		return info->dev;

	return NULL;
}

static int __init init_devpts_fs(void)
{
	tty_default_fops(&pts_fops);
	return 0;
}
module_init(init_devpts_fs)
