 /*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Ops for ubi operation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 29 10:42:14 2021
 */
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/irqdomain.h>
#include <linux/completion.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/mtd/ubi.h>
#include <linux/cdev.h>
#include <linux_device.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/fs_cmd.h>
#include <linux/kallsyms.h>
#include <linux/idr.h>
#include <linux/securec.h>
#include <liblinux/pal.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ubifs ctrl");
MODULE_VERSION("0.01");

static DEFINE_IDR(ubi_idr);
static DEFINE_MUTEX(ubi_idr_lock);

struct ubi_idr_desc {
	struct ubi_volume_desc *vol_desc;
	atomic_t refcnt;
};

static int alloc_id_for_desc(struct ubi_idr_desc *desc)
{
	int id;

	mutex_lock(&ubi_idr_lock);
	id = idr_alloc(&ubi_idr, desc, 0, 0, GFP_KERNEL);
	mutex_unlock(&ubi_idr_lock);

	return id;
}

static void get_refcnt(struct ubi_idr_desc *desc)
{
	atomic_inc(&desc->refcnt);
}

static void put_refcnt(struct ubi_idr_desc *desc)
{
	atomic_dec(&desc->refcnt);
}

static struct ubi_idr_desc *find_desc_from_id(int id)
{
	struct ubi_idr_desc *desc = NULL;

	mutex_lock(&ubi_idr_lock);
	desc = idr_find(&ubi_idr, id);
	get_refcnt(desc);
	mutex_unlock(&ubi_idr_lock);

	return desc;
}

typedef int (*ubi_major2num)(unsigned int major);
static int get_ubi_major2num(unsigned int major)
{
	ubi_major2num __ubi_major2num =
		(ubi_major2num)kallsyms_lookup_name("ubi_major2num");
	if (__ubi_major2num == NULL) {
		return -EOPNOTSUPP;
	}

	return __ubi_major2num(major);
}

static int ubifs_ctrl_get_ubi_num(int major)
{
	int err;

	err = get_ubi_major2num(major);
	if (err < 0) {
		pr_err("Get ubi num failed major %d\n", major);
	}

	return err;
}

static int ubifs_ctrl_open_volume(int ubi_num, int vol_id, int mode)
{
	int err;
	struct ubi_idr_desc *desc = liblinux_pal_malloc(sizeof(struct ubi_idr_desc));
	if (desc == NULL) {
		pr_err("Nomemory failed \n");
		return -ENOMEM;
	}

	atomic_set(&desc->refcnt, 1);
	desc->vol_desc = ubi_open_volume(ubi_num, vol_id, mode);
	if (IS_ERR(desc->vol_desc)) {
		pr_err("Open volume failed %d\n", IS_ERR(desc->vol_desc));
		err = PTR_ERR(desc->vol_desc);
		liblinux_pal_free(desc);
		return err;
	}

	return alloc_id_for_desc(desc);
}

static int ubifs_ctrl_get_devinfo(int ubi_num, unsigned long di)
{
	struct ubi_device_info devinfo = {0};
	int err;

	err = ubi_get_device_info(ubi_num, &devinfo);
	if (err < 0) {
		pr_err("Get device info failed ubi_num %d\n", ubi_num);
		return err;
	}

	err = copy_to_user((void __user *)(uintptr_t)(di),
			   &devinfo, sizeof(struct ubi_device_info));
	if (err) {
		pr_err("copy from user failed \n");
		return -EFAULT;
	}

	return 0;
}

static int ubifs_ctrl_get_volinfo(struct ubi_volume_desc *desc,
				  unsigned long vi, unsigned long name)
{
	struct ubi_volume_info vinfo = {0};
	int err;

	ubi_get_volume_info(desc, &vinfo);

	err = copy_to_user((void __user *)(uintptr_t)(vi),
			   &vinfo, sizeof(struct ubi_volume_info));
	if (err) {
		pr_err("copy from user failed \n");
		return -EFAULT;
	}

	err = copy_to_user((void __user *)(uintptr_t)(name),
			   vinfo.name, strlen(vinfo.name) + 1);
	if (err) {
		pr_err("copy from user failed \n");
		return -EFAULT;
	}

	return 0;
}

static void ubifs_ctrl_close_volume(struct ubi_idr_desc *desc, int id)
{
	mutex_lock(&ubi_idr_lock);
	idr_remove(&ubi_idr, id);
	put_refcnt(desc);
	mutex_unlock(&ubi_idr_lock);
}

static int ubifs_ctrl_alloc_buf(char **buf, int len)
{
	char *src = NULL;
	int err;

	if (len == 0) {
		pr_err("The len is 0\n");
		return -EINVAL;
	}

	src = liblinux_pal_calloc(1, len);
	if (src == NULL) {
		pr_err("calloc failed\n");
		return -ENOMEM;
	}

	*buf = src;

	return 0;
}

static int ubifs_ctrl_leb_read(struct ubi_volume_desc *desc, int lnum,
			       unsigned long buf, int offs, int len)
{
	char *src = NULL;
	int err;

	err = ubifs_ctrl_alloc_buf(&src, len);
	if (err != 0) {
		pr_err("alloc buf failed %d\n", err);
		return err;
	}

	err = ubi_leb_read(desc, lnum, src, offs, len, 0);
	if (err < 0) {
		pr_err("Leb read failed %d\n", err);
		liblinux_pal_free(src);
		return err;
	}

	err = copy_to_user((void __user *)(uintptr_t)(buf), src, len);
	if (err) {
		pr_err("Copy to user failed %d\n", err);
		liblinux_pal_free(src);
		return -EFAULT;
	}

	liblinux_pal_free(src);

	return 0;
}

static int ubifs_ctrl_is_mapped(struct ubi_volume_desc *desc, int lnum)
{
	int err;

	err = ubi_is_mapped(desc, lnum);
	if (err < 0) {
		pr_err("ubi is mmaped failed %d", err);
	}

	return err;
}

static int ubifs_ctrl_leb_write(struct ubi_volume_desc *desc, int lnum,
				unsigned long buf, int offs, int len)
{
	char *src = NULL;
	int err;

	err = ubifs_ctrl_alloc_buf(&src, len);
	if (err != 0) {
		pr_err("alloc buf failed %d\n", err);
		return err;
	}

	err = copy_from_user(src, (void __user *)(uintptr_t)(buf), len);
	if (err < 0) {
		pr_err("Copy from user failed err %d\n", err);
		liblinux_pal_free(src);
		return -EFAULT;
	}

	err = ubi_leb_write(desc, lnum, src, offs, len);
	if (err < 0) {
		pr_err("Leb write failed %d\n", err);
	}

	liblinux_pal_free(src);

	return err;
}

static int ubifs_ctrl_leb_change(struct ubi_volume_desc *desc, int lnum,
				 unsigned long buf, int len)
{
	char *src = NULL;
	int err;

	err = ubifs_ctrl_alloc_buf(&src, len);
	if (err != 0) {
		pr_err("alloc buf failed %d\n", err);
		return err;
	}

	err = copy_from_user(src, (void *)(uintptr_t)(buf), len);
	if (err < 0) {
		pr_err("Copy failed\n");
		liblinux_pal_free(src);
		return -EFAULT;
	}

	err = ubi_leb_change(desc, lnum, src, len);
	if (err < 0) {
		pr_err("Leb read failed\n");
	}

	liblinux_pal_free(src);

	return err;
}

static int ubifs_ctrl_leb_map(struct ubi_volume_desc *desc, int lnum)
{
	int err;

	err = ubi_leb_map(desc, lnum);
	if (err < 0) {
		pr_err("Leb map failed %d\n", err);
	}

	return err;
}

static int ubifs_ctrl_leb_unmap(struct ubi_volume_desc *desc, int lnum)
{
	int err;

	err = ubi_leb_unmap(desc, lnum);
	if (err < 0) {
		pr_err("Leb map failed %d\n", err);
	}

	return err;
}

static int ubifs_ctrl_sync(int ubi_num)
{
	int err;

	err = ubi_sync(ubi_num);
	if (err < 0) {
		pr_err("ubi sync failed %d", err);
	}

	return err;
}

static int ubifs_ubi_dev_ctrl(unsigned long arg)
{
	struct ubi_dev_ctrl_ctx ctx = {0};
	struct ubi_idr_desc *desc = NULL;
	int err;

	err = copy_from_user(&ctx, (void __user *)(uintptr_t)arg,
			     sizeof(struct ubi_dev_ctrl_ctx));
	if (err) {
		pr_err("copy from user failed \n");
		return -EFAULT;
	}

	if (ctx.cmd == UBI_GET_VOL_INFO || ctx.cmd == UBI_CLOSE_VOL) {
		desc = find_desc_from_id(ctx.id);
		if (desc == NULL) {
			pr_err("acc find desc from id failed\n");
			return -EFAULT;
		}
	}

	switch (ctx.cmd) {
	case UBI_GET_UBI_NUM:
		err = ubifs_ctrl_get_ubi_num(ctx.major);
		break;
	case UBI_OPEN_VOL:
		err = ubifs_ctrl_open_volume(ctx.ubi_num, ctx.vol_id, ctx.mode);
		break;
	case UBI_GET_DEV_INFO:
		err = ubifs_ctrl_get_devinfo(ctx.ubi_num, ctx.di);
		break;
	case UBI_GET_VOL_INFO:
		err = ubifs_ctrl_get_volinfo(desc->vol_desc, ctx.vi, ctx.name);
		break;
	case UBI_CLOSE_VOL:
		ubifs_ctrl_close_volume(desc, ctx.id);
		break;
	default:
		pr_err("The ctrl cmd %d is no support\n", ctx.cmd);
		break;
	}

	if (desc != NULL) {
		put_refcnt(desc);
	}
	if (ctx.cmd == UBI_CLOSE_VOL) {
		if (atomic_read(&desc->refcnt) == 0) {
			ubi_close_volume(desc->vol_desc);
			liblinux_pal_free(desc);
		}
	}

	return err;
}

static int ubifs_ubi_dev_acc(unsigned long arg)
{
	struct ubi_dev_acc_ctx ctx = {0};
	struct ubi_idr_desc *desc = NULL;
	int err;

	err = copy_from_user(&ctx, (void __user *)(uintptr_t)arg,
			     sizeof(struct ubi_dev_acc_ctx));
	if (err) {
		pr_err("acc copy from user failed \n");
		return -EFAULT;
	}

	if (ctx.cmd != UBI_SYNC) {
		desc = find_desc_from_id(ctx.id);
		if (desc == NULL) {
			pr_err("acc find desc from id failed\n");
			return -EFAULT;
		}
	}

	switch (ctx.cmd) {
	case UBI_LEB_READ:
		err = ubifs_ctrl_leb_read(desc->vol_desc, ctx.lnum, ctx.buf, ctx.offs, ctx.len);
		break;
	case UBI_LEB_WRITE:
		err = ubifs_ctrl_leb_write(desc->vol_desc, ctx.lnum, ctx.buf, ctx.offs, ctx.len);
		break;
	case UBI_LEB_CHANGE:
		err = ubifs_ctrl_leb_change(desc->vol_desc, ctx.lnum, ctx.buf, ctx.len);
		break;
	case UBI_LEB_MAP:
		err = ubifs_ctrl_leb_map(desc->vol_desc, ctx.lnum);
		break;
	case UBI_LEB_UNMAP:
		err = ubifs_ctrl_leb_unmap(desc->vol_desc, ctx.lnum);
		break;
	case UBI_LEB_MAPPED:
		err = ubifs_ctrl_is_mapped(desc->vol_desc, ctx.lnum);
		break;
	case UBI_SYNC:
		err = ubifs_ctrl_sync(ctx.ubi_num);
		break;
	default:
		pr_err("The access cmd %d is no support\n", ctx.cmd);
		break;
	}

	if (desc != NULL) {
		put_refcnt(desc);
	}

	return err;
}

static long ubifs_cdev_ioctl(struct file *file, unsigned int cmd,
			     unsigned long arg)
{
	int err = -EFAULT;

	if (cmd == UBI_DEV_CTRL) {
		err = ubifs_ubi_dev_ctrl(arg);
	} else if (cmd == UBI_DEV_ACC) {
		err = ubifs_ubi_dev_acc(arg);
	}

	if (err < 0) {
		pr_err("handle ubi failed cmd %d err %d\n", cmd, err);
	}

	return err;
}

const struct file_operations ubifs_ctrl_operations = {
	.owner          = THIS_MODULE,
	.compat_ioctl   = ubifs_cdev_ioctl,
};

static struct miscdevice ubifs_ctrl_cdev = {
	.minor = 0,
	.name = "ubifs_ctrl",
	.fops = &ubifs_ctrl_operations,
};

static int __init ubifs_ctrl_init(void)
{
	int err;

	err = misc_register(&ubifs_ctrl_cdev);
	if (err) {
		pr_err("UBI error: cannot register device\n");
	}
	return err;
}
module_init(ubifs_ctrl_init);
