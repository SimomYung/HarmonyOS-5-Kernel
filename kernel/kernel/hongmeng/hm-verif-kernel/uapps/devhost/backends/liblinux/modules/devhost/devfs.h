/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Description: Layer to ldk about devfs
 * Create: Mon Dec 13 18:21:00 2021
 */
#ifndef __DEVHOST_LINUX_DEVFS_H__
#define __DEVHOST_LINUX_DEVFS_H__
#include <linux/genhd.h>

typedef enum {
	MODE_REG = 1,
	MODE_CHR = 2,
	MODE_BLK = 4,
	MODE_FIFO = 8,
} vfs_mode_t;

#ifdef CONFIG_BLOCK
static inline int is_blockdev(struct device *dev)
{
	return dev->class == &block_class;
}
#else
static inline int is_blockdev(struct device *dev)
{
	return 0;
}
#endif

struct device;
struct devnode_req;
struct linux_device_object;
int __devfs_node_init(struct device *dev);
struct file *__devfs_open_file(struct linux_device_object *obj, unsigned int flags, unsigned int mode);

struct device_info;
int linux_get_devnode(struct device_info *devinfo,
		      char *devnode_name, unsigned int size,
		      struct devnode_req *req);
struct file *devhost_devfs_open(struct device *dev, unsigned int flags);

#ifdef CONFIG_DEVTMPFS
int devhost_devfs_init(void);
#else
static inline int devhost_devfs_init(void)
{
	return 0;
}
void rm_dev_inode(struct inode *dev_inode);
#endif

#endif
