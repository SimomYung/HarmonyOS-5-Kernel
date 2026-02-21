/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This is the header of udk sysfs interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 19 18:44:08 2019
 */

#ifndef __DEVHOST_UDK_SYSFS_H__
#define __DEVHOST_UDK_SYSFS_H__

#include <udk/device.h>

#define UDK_SYSFS_PATH_NAME_MAX 128

struct udk_sysfs_fops {
	/* ctx: private data, the same as what is given in udk_sysfs_create_file.
	 */
	int (*read)(void *ctx, void *dst, size_t pos, size_t size, size_t *rsize);
	int (*write)(void *ctx, void *src, size_t pos, size_t size, size_t *wsize);
};

int udk_sysfs_register_device(const struct udk_device *dev);
int udk_sysfs_unregister_device(const struct udk_device *dev);
int udk_sysfs_create_dir(const struct udk_device *dev, const char *name);
int udk_sysfs_create_dev_dir(const char *prefix_name, const struct udk_device *dev);
int udk_sysfs_create_simple_dir(const char *name);
int udk_sysfs_create_simple_symlink(char *path, char *link_path);
int udk_sysfs_create_dir_only(const char *dir);
int udk_sysfs_create_size_node(const char *dir, struct udk_sysfs_fops *fops, void *ctx);
int udk_sysfs_create_file(const struct udk_device *dev,
			   const char *name,
			   struct udk_sysfs_fops *fops,
			   void *ctx);
int udk_sysfs_create_uevent(const char *prefix_name, const struct udk_device *dev,
			    struct udk_sysfs_fops *fops, void *ctx);
int udk_sysfs_create_block_node(const char *node_name, struct udk_sysfs_fops *fops, void *ctx);
int udk_sysfs_create_symlink(const struct udk_device *dev,
			      const char *name,
			      const char *linkdata);
int udk_sysfs_create_block_symlink(const char *prefix_name,
				   const struct udk_device *dev);
int udk_sysfs_create_class_symlink(const char *prefix_name,
				   const struct udk_device *dev);
int udk_sysfs_delete(const struct udk_device *dev, const char *name);

int udk_sysfs_dir_create(const char *path);
int udk_sysfs_device_dir_create(const struct udk_device *dev, const char *type);
int udk_sysfs_device_dir_destroy(const struct udk_device *dev, const char *type);
int udk_sysfs_module_dir_destroy(const char *module);
int udk_sysfs_module_dir_create(const char *module);
int udk_sysfs_file_destroy(const char *prefix, const char *suffix);
int udk_sysfs_file_create(const char *prefix, const char *suffix,
			  const struct udk_sysfs_fops *fops, void *ctx);
int udk_sysfs_create_blk_queue_dir(const struct udk_device *dev, const char *prefix,
				   const char *path, size_t len);
int udk_sysfs_create_blk_queue_qos(const char *prefix_name, struct udk_sysfs_fops *fops, void *ctx);

#ifdef CONFIG_HISI_UDK_MAS_BLK
int udk_sysfs_create_mas_blk(const char *prefix_name, const char *mas_name,
	struct udk_sysfs_fops *fops, void *ctx);
#endif

#endif /* __DEVHOST_UDK_SYFS_H__ */
