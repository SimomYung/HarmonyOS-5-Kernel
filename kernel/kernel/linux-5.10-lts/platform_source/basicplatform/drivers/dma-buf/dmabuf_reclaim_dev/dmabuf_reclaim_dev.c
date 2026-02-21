/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Implement of camera buffer.
 * Create: 2018-11-28
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/dmabuf/dmabuf_reclaim_dev.h>

#include <liblinux/file_utils.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dmabuf/dmabuf_reclaim.h>
#include <linux/dmabuf/dmabuf_reclaim_fd.h>
#include <linux/dmabuf/dmabuf_reclaim_manage.h>
#include <linux/dmabuf/mm_dma_heap.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <securec.h>

#include "mm_heap_helpers.h"

static long dmabuf_reclaim_cdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

struct dmabuf_reclaim_device {
	struct cdev c_dev;
	dev_t dev_no;
};

struct file_operations dmabuf_reclaim_fops = {
	.unlocked_ioctl = dmabuf_reclaim_cdev_ioctl,
};

struct dmabuf_reclaim_device *dmabuf_reclaim_dev = NULL;
struct class *dmabuf_reclaim_class = NULL;

static long dma_reclaim_ioctl_handle_wr_func(int (*func)(struct dma_reclaim_ioctl_sw_para *), unsigned long arg,
					     size_t para_size)
{
	long ret = 0;
	void __user *upara;
	unsigned long cp_ret;
	void *para = kmalloc(para_size, GFP_KERNEL);

	if (!para) {
		return -ENOMEM;
	}

	upara = u64_to_user_ptr(arg);
	if (!upara) {
		ret = -EINVAL;
		goto exit;
	}

	cp_ret = copy_from_user(para, upara, para_size);
	if (cp_ret) {
		ret = -EFAULT;
		goto exit;
	}

	ret = func(para);
	if (ret) {
		goto exit;
	}

	cp_ret = copy_to_user(upara, para, para_size);
	if (cp_ret) {
		ret = -EFAULT;
	}

exit:
	kfree(para);
	return ret;
}

static int mm_dmabuf_reclaim(struct dma_reclaim_ioctl_sw_para *para)
{
	if (!is_dmabuf_reclaim_enable()) {
		return 0;
	}
	return mm_dmabuf_reclaim_ino(para->pid, para->ino);
}

static int mm_dmabuf_resume(struct dma_reclaim_ioctl_sw_para *para)
{
	if (!is_dmabuf_reclaim_enable()) {
		return 0;
	}
	return mm_dmabuf_resume_pid(para->pid);
}

static int mm_dmabuf_clear(struct dma_reclaim_ioctl_sw_para *para)
{
	if (!is_dmabuf_reclaim_enable()) {
		return 0;
	}
	return mm_dmabuf_clear_pid(para->pid);
}

static int mm_dmabuf_reclaim_single(struct dma_reclaim_ioctl_sw_para *para)
{
	if (!is_dmabuf_reclaim_enable()) {
		return 0;
	}
	return mm_dmabuf_reclaim_fd(para->pid, para->fd);
}

static int mm_dmabuf_resume_single(struct dma_reclaim_ioctl_sw_para *para)
{
	if (!is_dmabuf_reclaim_enable()) {
		return 0;
	}
	return mm_dmabuf_resume_fd(para->pid, para->fd);
}

static long dmabuf_reclaim_cdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	switch (cmd) {
	case DMA_BUF_RECLAIM_INIT_ESWAP_SPACE: {
		ret = mm_dmabuf_init_swap_space(arg);
		break;
	}
	case DMA_BUF_RECLAIM_SW_TO_BACKGROUND: {
		ret = dma_reclaim_ioctl_handle_wr_func(mm_dmabuf_reclaim, arg,
						       sizeof(struct dma_reclaim_ioctl_sw_para));
		break;
	}
	case DMA_BUF_RECLAIM_SW_TO_FRONT: {
		ret = dma_reclaim_ioctl_handle_wr_func(mm_dmabuf_resume, arg, sizeof(struct dma_reclaim_ioctl_sw_para));
		break;
	}
	case DMA_BUF_RECLAIM_APP_DIE: {
		ret = dma_reclaim_ioctl_handle_wr_func(mm_dmabuf_clear, arg, sizeof(struct dma_reclaim_ioctl_sw_para));
		break;
	}
	case DMA_BUF_RECLAIM_FD: {
		ret = dma_reclaim_ioctl_handle_wr_func(mm_dmabuf_reclaim_single, arg,
						       sizeof(struct dma_reclaim_ioctl_sw_para));
		break;
	}
	case DMA_BUF_RESUME_FD: {
		ret = dma_reclaim_ioctl_handle_wr_func(mm_dmabuf_resume_single, arg,
						       sizeof(struct dma_reclaim_ioctl_sw_para));
		break;
	}
	default:
		pr_err("%s: dmabuf cdev ioctl error!\n", __func__);
		return -EFAULT;
	}
	return ret;
}

static int __init dmabuf_cdev_init(void)
{
	int ret;
	struct device *dev = NULL;
	pr_info("%s: HELLO KERNEL FOR DMA_REC CDEV!\n", __func__);

	dmabuf_reclaim_dev = kmalloc(sizeof(*dmabuf_reclaim_dev), GFP_KERNEL);
	if (!dmabuf_reclaim_dev) {
		ret = -ENOMEM;
		goto alloc_dev_fail;
	}
	ret = alloc_chrdev_region(&dmabuf_reclaim_dev->dev_no, 0, 1, "dmabuf");
	if (ret < 0) {
		goto alloc_chrdev_fail;
	}
	cdev_init(&dmabuf_reclaim_dev->c_dev, &dmabuf_reclaim_fops);
	ret = cdev_add(&dmabuf_reclaim_dev->c_dev, dmabuf_reclaim_dev->dev_no, 1);
	if (ret < 0) {
		goto cdev_add_fail;
	}
	dmabuf_reclaim_class = class_create(THIS_MODULE, "dma_reclaim"); // sys/devices/virtual/dmabuf/dma_reclaim
	if (IS_ERR(dmabuf_reclaim_class)) {
		ret = -EBUSY;
		goto class_create_fail;
	}
	dev = device_create(dmabuf_reclaim_class, NULL, dmabuf_reclaim_dev->dev_no, NULL,
				      "dma_reclaim"); // mknod /dev/dma_reclaim
	if (IS_ERR(dev)) {
		ret = -EBUSY;
		goto device_create_fail;
	}
	return 0;
device_create_fail:
	class_destroy(dmabuf_reclaim_class);
class_create_fail:
	cdev_del(&dmabuf_reclaim_dev->c_dev);
cdev_add_fail:
	unregister_chrdev_region(dmabuf_reclaim_dev->dev_no, 1);
alloc_chrdev_fail:
	kfree(dmabuf_reclaim_dev);
alloc_dev_fail:
	return ret;
}

static void __exit dmabuf_cdev_exit(void)
{
	device_destroy(dmabuf_reclaim_class, dmabuf_reclaim_dev->dev_no);
	class_destroy(dmabuf_reclaim_class);
	cdev_del(&dmabuf_reclaim_dev->c_dev);
	unregister_chrdev_region(dmabuf_reclaim_dev->dev_no, 1);
	kfree(dmabuf_reclaim_dev);
	dmabuf_reclaim_dev = NULL;
}

module_init(dmabuf_cdev_init);
module_exit(dmabuf_cdev_exit);
MODULE_LICENSE("GPL v2");