/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Interfaces of udk uio framework
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 3 15:53:58 2021
 */
#ifndef __UDK_UIO_H__
#define __UDK_UIO_H__
#include <udk/mm.h>
#include <udk/device.h>
#include <udk/lib/idr.h>

struct udk_uio_info;
struct udk_uio_mem {
	const char *name;
	unsigned long long addr;
	unsigned long size;
};

struct udk_uio_ops {
	int (*open)(struct udk_uio_info *info);
	int (*close)(struct udk_uio_info *info);
	int (*mmap)(struct udk_uio_info *info, struct udk_vma *vma);
	int (*irq_handler)(struct udk_uio_info *info, int irq);
	int (*irq_control)(struct udk_uio_info *info, int flags);
};

struct udk_uio_info {
	const char *name;
	int irq;
	unsigned int mem_flags;
	struct idr mem;
	struct udk_uio_ops *ops;
	void *priv;
};

struct udk_uio_info *udk_uio_info_create(struct udk_device *udkdev);
int udk_uio_info_destroy(struct udk_uio_info *info);
int udk_uio_register(struct udk_uio_info *info);
int udk_uio_unregister(const struct udk_uio_info *info);

#endif /* __UDK_UIO_H__ */
