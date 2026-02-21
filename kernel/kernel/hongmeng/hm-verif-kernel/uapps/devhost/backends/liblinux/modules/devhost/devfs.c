/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Description: Layer to ldk about devfs
 * Create: Mon Dec 13 18:21:00 2021
 */
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/file.h>
#include <linux/rwsem.h>
#include <linux/types.h>
#include <linux/mount.h>
#include <linux/securec.h>
#include <linux/device.h>
#include <linux/uidgid.h>
#include <lnxbase/lnxbase.h>

#include "devfs.h"
#include "devhost.h"
#include "linux_device.h"

#ifndef CONFIG_DEVTMPFS
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,19,0)
extern void put_filp(struct file *);
#endif

static struct super_block g_fake_devfs;

static struct inode *__alloc_dev_inode(struct device *dev)
{
	struct inode *inode = NULL;

	inode = kzalloc(sizeof(struct inode), GFP_KERNEL);
	if (inode == NULL) {
		return NULL;
	}
	init_special_inode(inode,
			   is_blockdev(dev) ? S_IFBLK : S_IFCHR,
			   dev->devt);

	address_space_init_once(&inode->i_data);
	inode->i_mapping = &inode->i_data;
	inode->i_data.host = inode;
	inode->i_sb = &g_fake_devfs;
	init_rwsem(&inode->i_rwsem);

	return inode;
}

static struct file *__alloc_file(struct inode *inode, unsigned int flags)
{
	struct file *file = NULL;
	struct dentry dentry;
	struct path path = {
		.mnt = NULL,
		.dentry = &dentry,
	};
	dentry.d_inode = inode;

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0)
	file = alloc_file(&path, OPEN_FMODE(flags), inode->i_fop);
	if (!IS_ERR(file)) {
		file->f_flags = flags;
	}
#else
	file = alloc_file(&path, flags | FMODE_NONOTIFY, inode->i_fop);
#endif
	/* dentry need set NULL because dentry is local variable */
	if (!IS_ERR(file)) {
		file->f_path.dentry = NULL;
	}
	return file;
}

int __devfs_node_init(struct device *dev)
{
	struct inode *dev_inode = NULL;

	if (linux_device_object_of(dev) == NULL) {
		return -EINVAL;
	}
	dev_inode = __alloc_dev_inode(dev);
	if (dev_inode == NULL) {
		return -ENOMEM;
	}
	linux_device_object_of(dev)->inode = dev_inode;
	return 0;
}

struct file *__devfs_open_file(struct linux_device_object *obj, unsigned int flags, unsigned int mode)
{
	struct file *filp = NULL;
	struct device_info *info = NULL;
	int ret;

	if (obj == NULL) {
		pr_err("open failed obj = NULL\n");
		return ERR_PTR(-EINVAL);
	}
	if (obj->inode == NULL) {
		pr_err("open failed obj->inode = NULL\n");
		return ERR_PTR(-EINVAL);
	}

	info = &obj->info;
	filp = __alloc_file(obj->inode, flags);
	if (IS_ERR(filp)) {
		pr_err("alloc file failed\n");
		return filp;
	}
	BUG_ON(filp->f_op == NULL);

	filp->f_mapping = &obj->inode->i_data;

	if (filp->f_op->open != NULL) {
		ret = filp->f_op->open(obj->inode, filp);
		if (ret < 0) {
			pr_err("open device failed, devid=%d, device_name=%s, devt=0x%x\n",
			       info->devid, info->devname, obj->dev->devt);
			put_filp(filp);
			return ERR_PTR(ret);
		}
	}
	return filp;
}

void rm_dev_inode(struct inode *dev_inode)
{
	if (S_ISCHR(dev_inode->i_mode)) {
		if (dev_inode->i_cdev != NULL) {
			cd_forget(dev_inode);
		}
	}

	kfree(dev_inode);
}

#else /* CONFIG_DEVTMPFS */

static struct file_system_type *g_devfs_type;
static struct vfsmount *g_devfs_mnt;

static int __devfs_state_check(void)
{
	if (g_devfs_type == NULL || g_devfs_mnt == NULL) {
		return -EINVAL;
	}
	return 0;
}

int __devfs_node_init(struct device *dev)
{
	if (__devfs_state_check()) {
		return -EINVAL;
	}

	if (linux_device_object_of(dev) == NULL) {
		return -EINVAL;
	}
	linux_device_object_of(dev)->inode = NULL;
	return 0;
}

static struct file *devtmpfs_open_filp(struct linux_device_object *obj, int flags, umode_t mode)
{
	const char *tmp_devnode = NULL;
	const char *devnode_path = NULL;
	struct file *filp = NULL;
	struct device *dev = linux_device_get_locked(obj);

	if (IS_ERR(dev)) {
		return ERR_CAST(dev);
	}
	devnode_path = device_get_devnode(dev, NULL, NULL, NULL, &tmp_devnode);
	device_unlock(dev);
	if (devnode_path == NULL) {
		return ERR_PTR(-EFAULT);
	}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
	filp = file_open_root_mnt(g_devfs_mnt, devnode_path, flags, mode);
#else
	filp = file_open_root(g_devfs_mnt->mnt_root, g_devfs_mnt, devnode_path, flags, mode);
#endif
	if (tmp_devnode != NULL) {
		kfree(tmp_devnode);
	}
	return filp;
}

struct file *__devfs_open_file(struct linux_device_object *obj, unsigned int flags, unsigned int mode)
{
	if (__devfs_state_check()) {
		pr_err("open failed, devfs state check fail\n");
		return ERR_PTR(-EINVAL);
	}

	flags &= ~O_CREAT; /* user can't create regular file at ldk */
	return devtmpfs_open_filp(obj, flags, mode);
}

/*
 * init devfs before 'platform_notify' is executed.
 * because 'platform_notify' depends on `__devfs_node_init`.
 */
int __init devhost_devfs_init(void)
{
	g_devfs_type = get_fs_type("devtmpfs");
	if (!g_devfs_type) {
		pr_err("backend ldk don't support devtmpfs\n");
		return -ENOSYS;
	}
	g_devfs_mnt = kern_mount(g_devfs_type);
	if (IS_ERR(g_devfs_mnt)) {
		module_put(g_devfs_type->owner); /* put_filesystem */
		g_devfs_type = NULL;
		pr_err("mount ldk devtmpfs failed\n");
		return PTR_ERR(g_devfs_mnt);
	}
	return 0;
}
#endif /* !CONFIG_DEVTMPFS */

struct file *devhost_devfs_open(struct device *dev, unsigned int flags)
{
	struct linux_device_object *obj = NULL;

	if (!dev) {
		pr_err("open failed, dev = NULL\n");
		return ERR_PTR(-EINVAL);
	}

	obj = linux_device_object_of(dev);
	if (!obj) {
		return ERR_PTR(-ENODEV);
	}

	return __devfs_open_file(obj, flags, 0);
}

int linux_get_devnode(struct device_info *devinfo,
		      char *devnode_name, unsigned int size,
		      struct devnode_req *req)
{
	int ret;
	struct device *dev = NULL;
	const char *tmp = NULL;
	const char *name = NULL;
	umode_t mode = 0;
	kuid_t uid = GLOBAL_ROOT_UID;
	kgid_t gid = GLOBAL_ROOT_GID;

	dev = linux_device_get_device(devinfo);
	if (dev->devt != 0) {
		name = device_get_devnode(dev, &mode, &uid, &gid, &tmp);
		if ((name == NULL) || (size <= strlen(name))) {
			return -EINVAL;
		}
		ret = strncpy_s(devnode_name, size, name, strlen(name));
		BUG_ON(ret != 0);
		devnode_name[strlen(name)] = '\0';

		if (tmp != NULL) {
			kfree(tmp);
		}
		req->mode = mode;
		req->uid = __kuid_val(uid);
		req->gid = __kgid_val(gid);
	}

	return 0;
}
