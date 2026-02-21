/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Declare interface of char device
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 30 17:18:15 UTC 2023
 */

#ifndef __UDK_CHAR_H__
#define __UDK_CHAR_H__

#include <libhmsync/raw_scopedptr.h>

#include <udk/poll.h>
#include <udk/device.h>

struct udk_vma;
struct udk_char_device;

struct udk_char_context {
	void *ctx;
	unsigned int io_flags;
	struct udk_char_device *chrdev;
	struct raw_refcnt refcnt;
};

struct udk_char_operations {
	int (*open)(struct udk_char_context *ctx);
	int (*close)(struct udk_char_context *ctx);
	int (*read)(struct udk_char_context *ctx,
		    void *buf, unsigned long count, unsigned long long *off);
	int (*write)(struct udk_char_context *ctx,
		     const void *buf, unsigned long count, unsigned long long *off);
	long long (*llseek)(struct udk_char_context *ctx, long long offset, int whence);
	int (*poll)(struct udk_char_context *ctx,
		    struct udk_poll_wevent *pevent, unsigned int *revent);
	int (*ioctl)(struct udk_char_context *ctx, unsigned int cmd, unsigned long arg);
	int (*compat_ioctl)(struct udk_char_context *ctx, unsigned int cmd, unsigned long arg);
	int (*mmap)(struct udk_char_context *ctx, struct udk_vma *vma);

	/* used to configure devfs node attribute for special devices */
	int (*config_device)(struct udk_char_device *chrdev, struct udk_device_attr *attr);
	unsigned long __rsv[4];
	void **owner;
};

struct udk_char_device {
	struct udk_device udkdev;
	struct udk_char_operations *ops;
};

struct udk_char_device *udk_chrdev_create(const char *name, const devnum_t devno,
					  struct udk_char_operations *fops);
void udk_chrdev_destroy(struct udk_char_device *chrdev);
void udk_chrdev_free(struct udk_char_device *chrdev);

int udk_chrdev_init(void);

#endif /* __UDK_CHAR_H__ */
