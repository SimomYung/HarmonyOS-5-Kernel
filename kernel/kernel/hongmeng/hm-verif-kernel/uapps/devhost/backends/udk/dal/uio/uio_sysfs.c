/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Udk uio sysfs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 3 20:19:18 2021
 */
#include "uio_sysfs.h"
#include <udk/mm.h>
#include <udk/log.h>
#include <udk/errno.h>
#include <udk/sysfs.h>
#include <udk/device.h>
#include <udk/lib/idr.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>

#define UIO_ROOT_NAME	"name"
#define UIO_ROOT_MAPS	"maps"
#define UIO_MAPS_MAP	"maps/map"
#define UIO_MAP_NAME	"name"
#define UIO_MAP_ADDR	"addr"
#define UIO_MAP_SIZE	"size"

#define MAX_SYSFS_STR_LEN 32
/*
 * Structure of uio sysfs directory
 *  /sys/devices/
 *   |-uio0
 *      |-name (char *)
 *      |-maps
 *         |-map0
 *            |-name (char *)
 *            |-addr (unsigned long, hex)
 *            |-size (unsigned long, hex)
 *         ...
 *         |-mapN
 *   ...
 *   |-uioN
 */

enum UIO_CTX_TYPE {
	UIO_CTX_STRING,
	UIO_CTX_ULONGLONG,
};

static int uio_sysfs_copy_ctx(char *buf, size_t buf_size, const void *ctx,
			      enum UIO_CTX_TYPE type)
{
	int err;

	if (buf_size == 0 || buf_size > MAX_SYSFS_STR_LEN) {
		return -EINVAL;
	}
	switch (type) {
	case UIO_CTX_STRING:
		err = snprintf_s(buf, buf_size, buf_size - 1,
				 "%s\n", (char *)ctx);
		break;
	case UIO_CTX_ULONGLONG:
		err = snprintf_s(buf, buf_size, buf_size - 1,
				 "0x%llx\n", *(unsigned long long *)ctx);
		break;
	default:
		return -EINVAL;
	}

	return err;
}

static int uio_sysfs_read(const void *ctx, void *dst, size_t pos,
			  size_t dst_size, enum UIO_CTX_TYPE type)
{
	char buf[MAX_SYSFS_STR_LEN];
	size_t count, buf_size;
	int err;

	if (ctx == NULL) {
		return 0;
	}
	if (dst == NULL || dst_size == 0 || pos >= MAX_SYSFS_STR_LEN - 1) {
		return -EINVAL;
	}
	err = uio_sysfs_copy_ctx(buf, MAX_SYSFS_STR_LEN, ctx, type);
	if (err < 0) {
		return err;
	}
	buf_size = (size_t)(long)err;
	if (pos >= buf_size) {
		return 0;
	}
	count = (dst_size < buf_size - pos) ? dst_size : buf_size - pos;
	err = udk_copy_to_user(dst, buf + pos, count);
	if (err < 0) {
		return err;
	}

	return (int)count;
}

static int uio_sysfs_read_string(void *ctx, void *dst, size_t pos, size_t dst_size,
				 size_t *rsize)
{
	int size;

	size = uio_sysfs_read(ctx, dst, pos, dst_size, UIO_CTX_STRING);
	if (size < 0) {
		return size;
	}
	*rsize = (size_t)(long)size;

	return 0;
}

static int uio_sysfs_read_ulonglong(void *ctx, void *dst, size_t pos, size_t dst_size,
				    size_t *rsize)
{
	int size;

	size = uio_sysfs_read(ctx, dst, pos, dst_size, UIO_CTX_ULONGLONG);
	if (size < 0) {
		return size;
	}
	*rsize = (size_t)(long)size;

	return 0;
}

static struct udk_sysfs_fops uio_sysfs_string_fops = {
	.read = uio_sysfs_read_string,
	.write = NULL,
};

static struct udk_sysfs_fops uio_sysfs_ulonglong_fops = {
	.read = uio_sysfs_read_ulonglong,
	.write = NULL,
};

static int uio_sysfs_create_map_attr(const struct udk_device *dev, unsigned int idx,
				     const char *name, struct udk_sysfs_fops *fops,
				     void *ctx)
{
	char path[UDK_SYSFS_PATH_NAME_MAX];
	int err;

	err = snprintf_s(path, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
			 UIO_MAPS_MAP"%u/%s", idx, name);
	if (err < 0) {
		return -EINVAL;
	}
	err = udk_sysfs_create_file(dev, (const char *)path, fops, ctx);
	if (err < 0) {
		udk_error("uio create sys-fs file %s failed, err: %s\n",
			  path, strerror(-err));
		return err;
	}

	return 0;
}

static int uio_sysfs_delete_map_attr(const struct udk_device *dev, unsigned int idx,
				     const char *name)
{
	char path[UDK_SYSFS_PATH_NAME_MAX];
	int err;

	err = snprintf_s(path, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
			 UIO_MAPS_MAP"%u/%s", idx, name);
	if (err < 0) {
		return -EINVAL;
	}
	err = udk_sysfs_delete(dev, (const char *)path);
	if (err < 0) {
		udk_error("uio delete sysfs %s failed, err: %s\n",
			  path, strerror(-err));
		return err;
	}

	return 0;
}

static int uio_sysfs_create_map_attrs(const struct udk_device *dev, unsigned int idx,
				      struct udk_uio_mem *mem)
{
	int err;

	/* /sys/devices/uio%u/maps/maps%u/addr */
	err = uio_sysfs_create_map_attr(dev, idx, UIO_MAP_ADDR,
					&uio_sysfs_ulonglong_fops, &mem->addr);
	if (err < 0) {
		return err;
	}
	/* /sys/devices/uio%u/maps/maps%u/size */
	err = uio_sysfs_create_map_attr(dev, idx, UIO_MAP_SIZE,
					&uio_sysfs_ulonglong_fops, &mem->size);
	if (err < 0) {
		(void)uio_sysfs_delete_map_attr(dev, idx, UIO_MAP_ADDR);
		return err;
	}
	/* /sys/devices/uio%u/maps/maps%u/name */
	err = uio_sysfs_create_map_attr(dev, idx, UIO_MAP_NAME,
					&uio_sysfs_string_fops, (void *)mem->name);
	if (err < 0) {
		(void)uio_sysfs_delete_map_attr(dev, idx, UIO_MAP_SIZE);
		(void)uio_sysfs_delete_map_attr(dev, idx, UIO_MAP_ADDR);
		return err;
	}

	return 0;
}

static int uio_sysfs_delete_map_attrs(const struct udk_device *dev,
				      unsigned int idx)
{
	int err;

	/* /sys/devices/uio%u/maps/maps%u/addr */
	err = uio_sysfs_delete_map_attr(dev, idx, UIO_MAP_ADDR);
	if (err < 0) {
		return err;
	}
	/* /sys/devices/uio%u/maps/maps%u/size */
	err = uio_sysfs_delete_map_attr(dev, idx, UIO_MAP_SIZE);
	if (err < 0) {
		return err;
	}
	/* /sys/devices/uio%u/maps/maps%u/name */
	err = uio_sysfs_delete_map_attr(dev, idx, UIO_MAP_NAME);
	if (err < 0) {
		return err;
	}

	return 0;
}

static int uio_sysfs_delete_map(unsigned int idx, void *item, void *data)
{
	struct udk_device *dev = (struct udk_device *)data;
	char path[UDK_SYSFS_PATH_NAME_MAX];
	int err;

	UNUSED(item);
	err = uio_sysfs_delete_map_attrs(dev, idx);
	if (err < 0) {
		return err;
	}
	/* /sys/devices/uio%u/maps/map%u/ */
	err = snprintf_s(path, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
			 UIO_MAPS_MAP"%u", idx);
	if (err < 0) {
		return err;
	}
	err = udk_sysfs_delete(dev, (const char *)path);
	if (err < 0) {
		return err;
	}

	return 0;
}

static inline void uio_sysfs_delete_map_range(const struct udk_device *dev,
					      unsigned int idx)
{
	unsigned int i;

	for (i = 0; i < idx; i++) {
		(void)uio_sysfs_delete_map(i, NULL, (void *)dev);
	}
}

static int uio_sysfs_create_map(unsigned int idx, void *item, void *data)
{
	struct udk_uio_mem *mem = (struct udk_uio_mem *)item;
	struct udk_device *dev = (struct udk_device *)data;
	char path[UDK_SYSFS_PATH_NAME_MAX];
	int err;

	/* /sys/devices/uio%u/maps/map%u/ */
	err = snprintf_s(path, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
			 UIO_MAPS_MAP"%u", idx);
	if (err < 0) {
		uio_sysfs_delete_map_range(dev, idx);
		return err;
	}
	err = udk_sysfs_create_dir(dev, (const char *)path);
	if (err < 0) {
		udk_error("uio create sysfs dir %s failed, err: %s\n",
			  path, strerror(-err));
		uio_sysfs_delete_map_range(dev, idx);
		return err;
	}
	err = uio_sysfs_create_map_attrs(dev, idx, mem);
	if (err < 0) {
		(void)udk_sysfs_delete(dev, (const char *)path);
		uio_sysfs_delete_map_range(dev, idx);
		return err;
	}

	return 0;
}

static int uio_sysfs_create_maps(const struct udk_device *dev,
				 struct udk_uio_info *info)
{
	int err;

	/* /sys/devices/uio%u/maps/ */
	err = udk_sysfs_create_dir(dev, UIO_ROOT_MAPS);
	if (err < 0) {
		udk_error("uio sysfs create %s failed, err: %s\n",
			  UIO_ROOT_MAPS, strerror(-err));
		return err;
	}
	err = idr_for_each(&info->mem, uio_sysfs_create_map, (void *)dev);
	if (err < 0) {
		udk_warn("uio create sysfs file failed, err: %s\n",
			 strerror(-err));
		(void)udk_sysfs_delete(dev, UIO_ROOT_MAPS);
		return err;
	}

	return 0;
}

static int uio_sysfs_delete_maps(const struct udk_device *dev,
				 struct udk_uio_info *info)
{
	int err;

	err = idr_for_each(&info->mem, uio_sysfs_delete_map, (void *)dev);
	if (err < 0) {
		udk_warn("uio delete sysfs file failed, err: %s\n",
			 strerror(-err));
		return err;
	}
	/* /sys/devices/uio%u/maps/ */
	err = udk_sysfs_delete(dev, UIO_ROOT_MAPS);
	if (err < 0) {
		udk_error("uio sysfs delete %s failed, err: %s\n",
			  UIO_ROOT_MAPS, strerror(-err));
		return err;
	}

	return 0;
}

static int uio_sysfs_create_name(const struct udk_device *dev, struct udk_uio_info *info)
{
	int err;

	/* /sys/devices/uio%u/name */
	err = udk_sysfs_create_file(dev, UIO_ROOT_NAME, &uio_sysfs_string_fops,
				    (void *)info->name);
	if (err < 0) {
		udk_error("uio sysfs create file %s failed, err: %s\n",
			  UIO_ROOT_NAME, strerror(-err));
		return err;
	}

	return 0;
}

static int uio_sysfs_delete_name(const struct udk_device *dev)
{
	int err;

	/* /sys/devices/uio%u/name */
	err = udk_sysfs_delete(dev, UIO_ROOT_NAME);
	if (err < 0) {
		udk_error("uio sysfs delete file %s failed, err: %s\n",
			  UIO_ROOT_NAME, strerror(-err));
		return err;
	}

	return 0;
}

int uio_sysfs_create(const struct udk_device *dev, struct udk_uio_info *info)
{
	int err;

	if (dev == NULL || info == NULL) {
		return -EINVAL;
	}
	err = udk_sysfs_register_device(dev);
	if (err < 0) {
		udk_error("uio register sysfs failed, err: %s\n", strerror(-err));
		return err;
	}
	/* /sys/devices/uio%u/name */
	err = uio_sysfs_create_name(dev, info);
	if (err < 0) {
		(void)udk_sysfs_unregister_device(dev);
		return err;
	}
	/* /sys/devices/uio%u/maps/ */
	err = uio_sysfs_create_maps(dev, info);
	if (err < 0) {
		(void)udk_sysfs_delete(dev, UIO_ROOT_NAME);
		(void)udk_sysfs_unregister_device(dev);
		return err;
	}

	return 0;
}

int uio_sysfs_delete(const struct udk_device *dev, struct udk_uio_info *info)
{
	int err;

	if (dev == NULL || info == NULL) {
		return -EINVAL;
	}
	/* /sys/devices/uio%u/maps/ */
	err = uio_sysfs_delete_maps(dev, info);
	if (err < 0) {
		return err;
	}
	/* /sys/devices/uio%u/name */
	err = uio_sysfs_delete_name(dev);
	if (err < 0) {
		return err;
	}
	err = udk_sysfs_unregister_device(dev);
	if (err < 0) {
		udk_warn("uio unregister sysfs failed, err: %s\n", strerror(-err));
		return err;
	}

	return 0;
}
