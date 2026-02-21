/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This is the implementation of udk sysfs interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 19 18:43:51 2019
 */
#include <udk/log.h>
#include <udk/sysfs.h>

#include <fs_sysfs.h>
#include <udk/device.h>
#include <udk/log.h>
#include <libdevhost/sysfs.h>
#include <libhwsecurec/securec.h>

#define SYSFS_DEVICES		"devices"
#define SYSFS_BLOCK		"block"
#define SYSFS_MISC		"misc"
#define SYSFS_VIRTUAL		"virtual"
#define SYSFS_CLASS		"class"
#define SYSFS_MODULE		"module"
#define SYSFS_UEVENT		"uevent"
#define SYSFS_SIZE		"size"
#define SYS_DEVICES_PATH	"/sys/devices"

int udk_sysfs_dir_create(const char *path)
{
	int ret;

	ret = libdh_sysfs_create_dir(path, 0);
	if (ret < 0 && ret != E_HM_POSIX_EXIST) {
		udk_warn("udk sysfs create %s failed, %s\n", path, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}

int udk_sysfs_register_device(const struct udk_device *dev)
{
	char path[SYSFS_PATH_NAME_MAX];
	int ret;

	if (dev == NULL || dev->name == NULL) {
		return -EINVAL;
	}

	ret = udk_sysfs_dir_create(SYSFS_DEVICES);
	if (ret < 0) {
		return ret;
	}
	ret = snprintf_s(path, SYSFS_PATH_NAME_MAX, SYSFS_PATH_NAME_MAX - 1,
			 SYSFS_DEVICES"/%s", dev->name);
	if (ret < 0) {
		return -EINVAL;
	}
	ret = libdh_sysfs_create_dir(path, 0);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

int udk_sysfs_unregister_device(const struct udk_device *dev)
{
	char path[SYSFS_PATH_NAME_MAX];
	int ret;

	if (dev == NULL || dev->name == NULL) {
		return -EINVAL;
	}

	ret = snprintf_s(path, SYSFS_PATH_NAME_MAX, SYSFS_PATH_NAME_MAX - 1,
			 SYSFS_DEVICES"/%s", dev->name);
	if (ret < 0) {
		return -EINVAL;
	}
	ret = libdh_sysfs_delete(path);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

static int sysfs_init_dev_subpath(const struct udk_device *dev, const char *name,
				  char *path, size_t path_len)
{
	int ret;

	if (dev == NULL || dev->name == NULL ||
	    name == NULL || strlen(name) + 1 > path_len) {
		return -EINVAL;
	}
	ret = snprintf_s(path, path_len, path_len - 1,
			 SYSFS_DEVICES"/%s/%s", dev->name, name);
	if (ret < 0) {
		return -EINVAL;
	}

	return 0;
}

int udk_sysfs_create_dir(const struct udk_device *dev, const char *name)
{
	char path[SYSFS_PATH_NAME_MAX];
	int ret;

	ret = sysfs_init_dev_subpath(dev, name, path, SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		return ret;
	}
	ret = libdh_sysfs_create_dir(path, 0);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

/* init dev path with prefix */
static int sysfs_init_dev_path(const char *prefix_name,
				const struct udk_device *dev,
				char *path, size_t path_len)
{
	int ret;

	if (dev == NULL || dev->name == NULL || prefix_name == NULL ||
	    (strlen(prefix_name) + strlen(dev->name) + 1) > path_len) {
		return -EINVAL;
	}

	ret = snprintf_s(path, path_len, path_len - 1,
			 SYSFS_DEVICES"/%s/%s", prefix_name, dev->name);
	if (ret < 0) {
		return -EINVAL;
	}

	return 0;
}

int udk_sysfs_create_dev_dir(const char *prefix_name, const struct udk_device *dev)
{
	char path[UDK_SYSFS_PATH_NAME_MAX];
	int ret;

	ret = sysfs_init_dev_path(prefix_name, dev, path, UDK_SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		return ret;
	}
	ret = libdh_sysfs_create_dir(path, 0);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

int udk_sysfs_create_simple_symlink(char *path, char *link_path)
{
	if (path == NULL || link_path == NULL) {
		return -EINVAL;
	}

	return libdh_sysfs_create_symlink(path, 0, link_path);
}

int udk_sysfs_create_simple_dir(const char *name)
{
	char path[UDK_SYSFS_PATH_NAME_MAX];
	int ret;

	ret = snprintf_s(path, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
			 SYSFS_DEVICES"/%s", name);
	if (ret < 0) {
		return -EINVAL;
	}

	ret = libdh_sysfs_create_dir(path, 0);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

int udk_sysfs_create_dir_only(const char *dir)
{
	char path[UDK_SYSFS_PATH_NAME_MAX];
	int ret;

	ret = snprintf_s(path, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
			 "%s", dir);
	if (ret < 0) {
		return -EINVAL;
	}

	ret = libdh_sysfs_create_dir(path, 0);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

static int udk_sysfs_read(const struct kernfs_entry *entry,
			  const struct libdh_kernfs_rw_args *r_args,
			  unsigned long *r_size_out)
{
	int ret;
	unsigned long size = 0UL;
	const struct libdh_sysfs_args *udk_sysfs_args = NULL;
	struct udk_sysfs_fops *fops = NULL;
	if (r_args == NULL) {
		return E_HM_INVAL;
	}

	udk_sysfs_args = libdh_sysfs_acquire_args(entry);
	if (udk_sysfs_args == NULL) {
		return E_HM_INVAL;
	}

	fops = (struct udk_sysfs_fops *)udk_sysfs_args->ops;
	if ((fops == NULL) || (fops->read == NULL)) {
		return E_HM_INVAL;
	}

	ret = fops->read(udk_sysfs_args->ctx, (void *)(uintptr_t)r_args->ubuf,
			 (size_t)r_args->pos, (size_t)r_args->size,
			 ptr_to_type(&size, size_t *));
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	if (r_size_out != NULL) {
		*r_size_out = size;
	}

	return E_HM_OK;
}

static int udk_sysfs_write(const struct kernfs_entry *entry,
			   const struct libdh_kernfs_rw_args *w_args,
			   unsigned long *w_size_out)
{
	int ret;
	unsigned long size = 0UL;
	const struct libdh_sysfs_args *udk_sysfs_args = NULL;
	struct udk_sysfs_fops *fops = NULL;
	if (w_args == NULL) {
		return E_HM_INVAL;
	}

	udk_sysfs_args = libdh_sysfs_acquire_args(entry);
	if (udk_sysfs_args == NULL) {
		return E_HM_INVAL;
	}

	fops = (struct udk_sysfs_fops *)udk_sysfs_args->ops;
	if ((fops == NULL) || (fops->write == NULL)) {
		return E_HM_INVAL;
	}

	ret = fops->write(udk_sysfs_args->ctx, (void *)(uintptr_t)w_args->ubuf,
			  (size_t)w_args->pos, (size_t)w_args->size,
			  ptr_to_type(&size, size_t *));
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	if (w_size_out != NULL) {
		*w_size_out = size;
	}

	return E_HM_OK;
}

static struct libdh_kernfs_ops udk_sysfs_fops __read_mostly = {
	.read = udk_sysfs_read,
	.write = udk_sysfs_write,
	.poll = NULL,
};

int udk_sysfs_create_file(const struct udk_device *dev,
			  const char *name,
			  struct udk_sysfs_fops *fops,
			  void *ctx)
{
	char path[SYSFS_PATH_NAME_MAX];
	struct libdh_sysfs_args udk_sysfs_args;
	int ret;

	ret = sysfs_init_dev_subpath(dev, name, path, SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		return ret;
	}

	udk_sysfs_args.ops = (void *)fops;
	udk_sysfs_args.ctx = ctx;
	udk_sysfs_args.mode = 0;

	ret = libdh_sysfs_create_file(path, &udk_sysfs_fops,
				      &udk_sysfs_args, NULL);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

int udk_sysfs_create_size_node(const char *dir, struct udk_sysfs_fops *fops, void *ctx)
{
	char path[SYSFS_PATH_NAME_MAX];
	struct libdh_sysfs_args udk_sysfs_args;
	int ret;

	ret = snprintf_s(path, SYSFS_PATH_NAME_MAX, SYSFS_PATH_NAME_MAX - 1,
			 "%s/"SYSFS_SIZE, dir);
	if (ret < 0) {
		return -EINVAL;
	}

	udk_sysfs_args.ops = (void *)fops;
	udk_sysfs_args.ctx = ctx;
	udk_sysfs_args.mode = 0;

	ret = libdh_sysfs_create_file(path, &udk_sysfs_fops,
				      &udk_sysfs_args, NULL);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

static int sysfs_init_uevent_path(const char *prefix_name,
				  const struct udk_device *dev,
				  char *path, size_t path_len)
{
	int ret;

	if (dev == NULL || dev->name == NULL || prefix_name == NULL ||
	    (strlen(prefix_name) + strlen(dev->name) + 1) > path_len) {
		return -EINVAL;
	}

	ret = snprintf_s(path, path_len, path_len - 1,
			 SYSFS_DEVICES"/%s/%s/"SYSFS_UEVENT, prefix_name, dev->name);
	if (ret < 0) {
		return -EINVAL;
	}

	return 0;
}

int udk_sysfs_create_uevent(const char *prefix_name, const struct udk_device *dev,
			    struct udk_sysfs_fops *fops, void *ctx)
{
	char path[SYSFS_PATH_NAME_MAX];
	struct libdh_sysfs_args udk_sysfs_args;
	int ret;

	ret = sysfs_init_uevent_path(prefix_name, dev, path, SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		return ret;
	}

	udk_sysfs_args.ops = (void *)fops;
	udk_sysfs_args.ctx = ctx;
	udk_sysfs_args.mode = 0;

	ret = libdh_sysfs_create_file(path, &udk_sysfs_fops,
				      &udk_sysfs_args, NULL);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

#ifdef CONFIG_HISI_UDK_MAS_BLK
static int sysfs_init_mas_blk_path(const char *prefix_name,
				  const char *mas_name,
				  char *path, size_t path_len)
{
	int ret;

	if ((mas_name == NULL) || (prefix_name == NULL) ||
	    ((strlen(prefix_name) + strlen(mas_name) + 1) > path_len)) {
		return -EINVAL;
	}

	ret = snprintf_s(path, path_len, path_len - 1, "/%s/%s",
		prefix_name, mas_name);
	if (ret < 0) {
		return -EINVAL;
	}

	return 0;
}

int udk_sysfs_create_mas_blk(const char *prefix_name, const char *mas_name,
	struct udk_sysfs_fops *fops, void *ctx)
{
	char path[SYSFS_PATH_NAME_MAX];
	struct libdh_sysfs_args udk_sysfs_args;
	int ret;

	ret = sysfs_init_mas_blk_path(prefix_name, mas_name, path, SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		udk_error("fail %s %d\n", mas_name, ret);
		return ret;
	}

	udk_sysfs_args.ops = (void *)fops;
	udk_sysfs_args.ctx = ctx;
	udk_sysfs_args.mode = 0;

	ret = libdh_sysfs_create_file(path, &udk_sysfs_fops,
				      &udk_sysfs_args, NULL);
	if (ret < 0) {
		udk_error("fail %s %d\n", mas_name, ret);
		return -hmerrno2posix(ret);
	}

	return 0;
}
#endif

int udk_sysfs_create_block_node(const char *node_name, struct udk_sysfs_fops *fops,
				void *ctx)
{
	char path[SYSFS_PATH_NAME_MAX] = {0};
	struct libdh_sysfs_args udk_sysfs_args = {0};
	int ret;

	ret = snprintf_s(path, SYSFS_PATH_NAME_MAX, SYSFS_PATH_NAME_MAX - 1, SYSFS_BLOCK"/%s", node_name);
	if (ret < 0) {
		return -EINVAL;
	}

	udk_sysfs_args.ops = (void *)fops;
	udk_sysfs_args.ctx = ctx;
	udk_sysfs_args.mode = 0;

	ret = libdh_sysfs_create_file(path, &udk_sysfs_fops,
				      &udk_sysfs_args, NULL);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

int udk_sysfs_create_symlink(const struct udk_device *dev,
			     const char *name,
			     const char *linkdata)
{
	char path[SYSFS_PATH_NAME_MAX];
	char link_path[SYSFS_PATH_NAME_MAX];
	int ret;

	ret = sysfs_init_dev_subpath(dev, name, path, SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		return ret;
	}
	ret = sysfs_init_dev_subpath(dev, linkdata, link_path, SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		return ret;
	}
	ret = libdh_sysfs_create_symlink(path, 0, link_path);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

static int sysfs_init_full_path(const char *prefix_name, const struct udk_device *dev,
				char *path, size_t path_len)
{
	int ret;

	if (dev == NULL || dev->name == NULL || prefix_name == NULL ||
	    (strlen(prefix_name) + strlen(dev->name) + 1) > path_len) {
		return -EINVAL;
	}

	ret = snprintf_s(path, path_len, path_len - 1,
			 SYS_DEVICES_PATH"/%s/%s", prefix_name, dev->name);
	if (ret < 0) {
		return -EINVAL;
	}

	return 0;
}

static int sysfs_init_class_path(const struct udk_device *dev, char *path, size_t path_len)
{
	int ret;

	if (dev == NULL || dev->name == NULL || strlen(dev->name) + 1 > path_len) {
		return -EINVAL;
	}

	ret = snprintf_s(path, path_len, path_len - 1, SYSFS_CLASS"/"SYSFS_BLOCK"/%s", dev->name);
	if (ret < 0) {
		return -EINVAL;
	}

	return 0;
}

int udk_sysfs_create_class_symlink(const char *prefix_name, const struct udk_device *dev)
{
	char path[SYSFS_PATH_NAME_MAX];
	char link_path[SYSFS_PATH_NAME_MAX];
	int ret;

	/* symbol link target need be full path */
	mem_zero_a(link_path);
	ret = sysfs_init_full_path(prefix_name, dev, link_path, SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		return ret;
	}

	mem_zero_a(path);
	ret = sysfs_init_class_path(dev, path, SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		return ret;
	}

	ret = libdh_sysfs_create_symlink(path, 0, link_path);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

static int sysfs_init_block_path(const struct udk_device *dev, char *path, size_t path_len)
{
	int ret;

	if (dev == NULL || dev->name == NULL || strlen(dev->name) + 1 > path_len) {
		return -EINVAL;
	}

	ret = snprintf_s(path, path_len, path_len - 1, SYSFS_BLOCK"/%s", dev->name);
	if (ret < 0) {
		return -EINVAL;
	}

	return 0;
}

int udk_sysfs_create_block_symlink(const char *prefix_name, const struct udk_device *dev)
{
	char path[SYSFS_PATH_NAME_MAX];
	char link_path[SYSFS_PATH_NAME_MAX];
	int ret;

	/* symbol link target need be full path */
	ret = sysfs_init_full_path(prefix_name, dev, link_path, SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		return ret;
	}

	ret = sysfs_init_block_path(dev, path, SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		return ret;
	}

	ret = libdh_sysfs_create_symlink(path, 0, link_path);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

int udk_sysfs_delete(const struct udk_device *dev, const char *name)
{
	char path[SYSFS_PATH_NAME_MAX];
	int ret;

	ret = sysfs_init_dev_subpath(dev, name, path, SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		return ret;
	}
	ret = libdh_sysfs_delete(path);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

/*
 * This is only done to avoid unnecessary operations.
 * Concurrency protection is not required.
 * libdh_sysfs_create_dir returns E_HM_EXIST if the same dir is created concurrently.
 * udk_sysfs_dir_create will return 0 if the same dir exists.
 */
static bool udk_sysfs_default_done = false;
static int sysfs_device_dir_default(void)
{
	int ret;

	if (udk_sysfs_default_done) {
		return 0;
	}
	/* /sys/devices */
	ret = udk_sysfs_dir_create(SYSFS_DEVICES);
	if (ret < 0) {
		return ret;
	}
	/* /sys/devices/virtual */
	ret = udk_sysfs_dir_create("devices/virtual");
	if (ret < 0) {
		return ret;
	}
	/* /sys/devices/virtual/misc */
	ret = udk_sysfs_dir_create("devices/virtual/misc");
	if (ret < 0) {
		return ret;
	}
	/* /sys/devices/virtual/block */
	ret = udk_sysfs_dir_create("devices/virtual/block");
	if (ret < 0) {
		return ret;
	}
	/* /sys/class */
	ret = udk_sysfs_dir_create(SYSFS_CLASS);
	if (ret < 0) {
		return ret;
	}
	/* /sys/class/misc */
	ret = udk_sysfs_dir_create("class/misc");
	if (ret < 0) {
		return ret;
	}
	/* /sys/class/block */
	ret = udk_sysfs_dir_create("class/block");
	if (ret < 0) {
		return ret;
	}
	/* /sys/block */
	ret = udk_sysfs_dir_create(SYSFS_BLOCK);
	if (ret < 0) {
		return ret;
	}
	/* /sys/module */
	ret = udk_sysfs_dir_create(SYSFS_MODULE);
	if (ret < 0) {
		return ret;
	}
	udk_sysfs_default_done = true;
	return 0;
}

/*
 * 1. /sys/devices/virtual/misc and block/ is the actual location of the sysfs node.
 * 2. /sys/class/misc/.. and block/.. will link to 1's node
 * 3. /sys/block/.. will link to /sys/devices/virtual/block/..
 * 4. /sys/module/.. will create module's parameters
 */
static int sysfs_device_dir_symlink(const struct udk_device *dev,
				    const char *full_path, const char *type)
{
	int ret;
	char path[SYSFS_PATH_NAME_MAX];
	size_t size = SYSFS_PATH_NAME_MAX;

	/* /sys/class/`type`/udkdev->name link to /sys/devices/virtual/`type`/udkdev->name */
	mem_zero_a(path);
	ret = snprintf_s(path, size, size - 1, "%s/%s/%s", SYSFS_CLASS, type, dev->name);
	if (ret < 0) {
		udk_warn("device %s create linkpath failed, %s\n", dev->name, strerror(-ret));
		return ret;
	}
	ret = libdh_sysfs_create_symlink(path, 0, full_path);
	if (ret < 0) {
		udk_warn("link %s to %s failed, %s\n", path, full_path, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	/* /sys/block/udkdev->name link to /sys/devices/virtual/`type`/udkdev->name */
	if (strcmp(type, "block") == 0) {
		mem_zero_a(path);
		ret = snprintf_s(path, size, size - 1, "%s/%s", SYSFS_BLOCK, dev->name);
		if (ret < 0) {
			udk_warn("device %s create linkpath failed, %s\n", dev->name, strerror(-ret));
			return ret;
		}
		ret = libdh_sysfs_create_symlink(path, 0, full_path);
		if (ret < 0) {
			udk_warn("link %s to %s failed, %s\n", path, full_path, hmstrerror(ret));
			return -hmerrno2posix(ret);
		}
	}

	return 0;
}

static int sysfs_device_dir_unlink(const struct udk_device *dev, const char *type)
{
	int ret;
	char path[SYSFS_PATH_NAME_MAX];
	size_t size = SYSFS_PATH_NAME_MAX;

	/* /sys/class/`type`/udkdev->name link to /sys/devices/virtual/`type`/udkdev->name */
	mem_zero_a(path);
	ret = snprintf_s(path, size, size - 1, "%s/%s/%s", SYSFS_CLASS, type, dev->name);
	if (ret < 0) {
		udk_warn("device %s create linkpath failed, %s\n", dev->name, strerror(-ret));
		return ret;
	}
	ret = libdh_sysfs_delete(path);
	if (ret < 0) {
		udk_warn("delete %s failed, %s\n", path, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	/* /sys/block/udkdev->name link to /sys/devices/virtual/`type`/udkdev->name */
	if (strcmp(type, "block") == 0) {
		mem_zero_a(path);
		ret = snprintf_s(path, size, size - 1, "%s/%s", SYSFS_BLOCK, dev->name);
		if (ret < 0) {
			udk_warn("device %s create linkpath failed, %s\n", dev->name, strerror(-ret));
			return ret;
		}
		ret = libdh_sysfs_delete(path);
		if (ret < 0) {
			udk_warn("delete %s failed, %s\n", path, hmstrerror(ret));
			return -hmerrno2posix(ret);
		}
	}

	return 0;
}

int udk_sysfs_module_dir_destroy(const char *module)
{
	int ret;
	char path[SYSFS_PATH_NAME_MAX];
	size_t size = SYSFS_PATH_NAME_MAX;

	if (module == NULL) {
		return -EINVAL;
	}
	mem_zero_a(path);
	ret = snprintf_s(path, size, size - 1, "%s/%s/%s", SYSFS_MODULE, module, "parameters");
	if (ret < 0) {
		udk_warn("obtain module %s prefix path failed, %s\n", module, strerror(-ret));
		return ret;
	}
	ret = libdh_sysfs_delete(path);
	if (ret < 0) {
		udk_warn("delete %s failed, %s\n", path, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	/* /sys/module/`module`/parameters */
	mem_zero_a(path);
	ret = snprintf_s(path, size, size - 1, "%s/%s", SYSFS_MODULE, module);
	if (ret < 0) {
		udk_warn("obtain module %s prefix path failed, %s\n", module, strerror(-ret));
		return ret;
	}
	ret = libdh_sysfs_delete(path);
	if (ret < 0) {
		udk_warn("delete %s failed, %s\n", path, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return 0;
}

int udk_sysfs_module_dir_create(const char *module)
{
	int ret;
	char path[SYSFS_PATH_NAME_MAX];
	size_t size = SYSFS_PATH_NAME_MAX;

	if (module == NULL) {
		return -EINVAL;
	}
	/* /sys/module/`module`/parameters */
	mem_zero_a(path);
	ret = snprintf_s(path, size, size - 1, "%s/%s", SYSFS_MODULE, module);
	if (ret < 0) {
		udk_warn("obtain module %s prefix path failed, %s\n", module, strerror(-ret));
		return ret;
	}
	ret = udk_sysfs_dir_create(path);
	if (ret < 0) {
		return ret;
	}
	mem_zero_a(path);
	ret = snprintf_s(path, size, size - 1, "%s/%s/%s", SYSFS_MODULE, module, "parameters");
	if (ret < 0) {
		udk_warn("obtain module %s prefix path failed, %s\n", module, strerror(-ret));
		return ret;
	}
	ret = udk_sysfs_dir_create(path);
	if (ret < 0) {
		return ret;
	}
	return 0;
}

int udk_sysfs_device_dir_destroy(const struct udk_device *dev, const char *type)
{
	int ret;
	char path[SYSFS_PATH_NAME_MAX];
	size_t size = SYSFS_PATH_NAME_MAX;

	if ((dev == NULL) || (type == NULL)) {
		return -EINVAL;
	}
	ret = sysfs_device_dir_unlink(dev, type);
	if (ret < 0) {
		return ret;
	}

	/* /sys/devices/virtual/`type`/udkdev->name */
	mem_zero_a(path);
	ret = snprintf_s(path, size, size - 1, "%s/%s/%s/%s", SYSFS_DEVICES, SYSFS_VIRTUAL, type, dev->name);
	if (ret < 0) {
		udk_warn("create prefix path failed, %s\n", strerror(-ret));
		return ret;
	}
	ret = libdh_sysfs_delete(path);
	if (ret < 0) {
		udk_warn("delete %s failed, %s\n", path, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}

int udk_sysfs_device_dir_create(const struct udk_device *dev, const char *type)
{
	int ret;
	char path[SYSFS_PATH_NAME_MAX];
	char full_path[SYSFS_PATH_NAME_MAX];
	size_t size = SYSFS_PATH_NAME_MAX;

	if ((dev == NULL) || (type == NULL)) {
		return -EINVAL;
	}
	ret = sysfs_device_dir_default();
	if (ret < 0) {
		return ret;
	}
	/* /sys/devices/virtual/`type`/udkdev->name */
	mem_zero_a(path);
	ret = snprintf_s(path, size, size - 1, "%s/%s/%s/%s", SYSFS_DEVICES, SYSFS_VIRTUAL, type, dev->name);
	if (ret < 0) {
		udk_warn("create prefix path failed, %s\n", strerror(-ret));
		return ret;
	}
	ret = udk_sysfs_dir_create(path);
	if (ret < 0) {
		return ret;
	}

	/* obtain full path for symlink */
	mem_zero_a(full_path);
	ret = snprintf_s(full_path, size, size - 1, "%s/%s", "/sys", path);
	if (ret < 0) {
		udk_warn("obtain full path failed, %s\n", strerror(-ret));
		return ret;
	}
	ret = sysfs_device_dir_symlink(dev, full_path, type);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

int udk_sysfs_file_destroy(const char *prefix, const char *suffix)
{
	int ret;
	char path[SYSFS_PATH_NAME_MAX];
	size_t size = SYSFS_PATH_NAME_MAX;

	if ((prefix == NULL) || (suffix == NULL)) {
		return -EINVAL;
	}
	mem_zero_a(path);
	ret = snprintf_s(path, size, size - 1, "%s/%s", prefix, suffix);
	if (ret < 0) {
		udk_warn("obtain file path failed, prefix %s, suffix %s, %s\n",
			 prefix, suffix, strerror(-ret));
		return ret;
	}

	ret = libdh_sysfs_delete(path);
	if (ret < 0) {
		udk_warn("delete file %s failed, %s\n", path, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}

int udk_sysfs_file_create(const char *prefix, const char *suffix,
			  const struct udk_sysfs_fops *fops, void *ctx)
{
	int ret;
	char path[SYSFS_PATH_NAME_MAX];
	size_t size = SYSFS_PATH_NAME_MAX;
	struct libdh_sysfs_args udk_sysfs_args;

	if ((prefix == NULL) || (suffix == NULL)) {
		return -EINVAL;
	}
	mem_zero_a(path);
	ret = snprintf_s(path, size, size - 1, "%s/%s", prefix, suffix);
	if (ret < 0) {
		udk_warn("obtain file path failed, prefix %s, suffix %s, %s\n",
			 prefix, suffix, strerror(-ret));
		return ret;
	}

	udk_sysfs_args.ops = (void *)fops;
	udk_sysfs_args.ctx = ctx;
	udk_sysfs_args.mode = 0;
	ret = libdh_sysfs_create_file(path, &udk_sysfs_fops, &udk_sysfs_args, NULL);
	if (ret < 0) {
		udk_warn("create file %s failed, %s\n", path, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}

static int sysfs_init_block_queue_path(const char *prefix,
				  const struct udk_device *dev,
				  char *path, size_t path_len)
{
	int ret;

	if (dev == NULL || dev->name == NULL || prefix == NULL ||
	    (strlen(prefix) + strlen(dev->name) + 1) > path_len) {
		return -EINVAL;
	}

	ret = snprintf_s(path, path_len, path_len - 1,
			 SYSFS_DEVICES"/%s/%s/%s", prefix, dev->name, "queue");
	if (ret < 0) {
		return -EINVAL;
	}

	return 0;
}

int udk_sysfs_create_blk_queue_dir(const struct udk_device *dev, const char *prefix,
				   const char *path, size_t len)
{
	int ret;

	ret = sysfs_init_block_queue_path(prefix, dev, path, len);
	if (ret < 0) {
		return ret;
	}

	ret = udk_sysfs_dir_create(path);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

int udk_sysfs_create_blk_queue_qos(const char *prefix, struct udk_sysfs_fops *fops, void *ctx)
{
	char path[SYSFS_PATH_NAME_MAX];
	struct libdh_sysfs_args udk_sysfs_args;
	int ret;

	ret = snprintf_s(path, SYSFS_PATH_NAME_MAX, SYSFS_PATH_NAME_MAX - 1, "%s/%s", prefix, "qos_on");
	if (ret < 0) {
		return ret;
	}

	udk_sysfs_args.ops = (void *)fops;
	udk_sysfs_args.ctx = ctx;
	udk_sysfs_args.mode = 0;

	ret = libdh_sysfs_create_file(path, &udk_sysfs_fops,
				      &udk_sysfs_args, NULL);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return 0;
}

