/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK compat interfaces implementation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 26 15:14:11 2019
 */
#include <udk/compat.h>
#include <internal/compat.h>
#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>
#include <udk/errno.h>
#include <udk/device.h>
#include <udk/module.h>
#include <udk/driver.h>
#include <udk/log.h>
#include <devhost/device.h>
#include <libsysif/devmgr/types.h>
#include <libdevhost/compatible.h>
#include <libhmactv/actv.h>

static const struct udk_compat_ops *compat_ops = NULL;

void udk_compat_register_ops(const struct udk_compat_ops *ops)
{
	compat_ops = ops;
}

int udk_compat_unflatten_tree(void)
{
	if (compat_ops == NULL || compat_ops->unflatten_tree == NULL) {
		return -ENOSYS;
	}

	return compat_ops->unflatten_tree();
}

int udk_compat_get_device(struct device_info *info, struct device **pdev_out)
{
	if (compat_ops == NULL || compat_ops->get_device == NULL) {
		return -ENOSYS;
	}

	return compat_ops->get_device(info, pdev_out);
}

bool udk_is_compat_client(void)
{
	return hm_actv_local_is_compat();
}

static inline struct udk_module *udk_drv_owner_of(const struct udk_driver *udk_drv)
{
	if (udk_drv->drv_ops->owner == NULL) {
		return NULL;
	}
	return (struct udk_module *)(*udk_drv->drv_ops->owner);
}

static int native_create(void *ctx, const char *name,
			 const struct device_create_args *args,
			 struct device_info **devinfo)
{
	struct udk_driver *udk_driver = (struct udk_driver *)ctx;
	int ret;

	if (udk_driver == NULL || udk_driver->drv_ops == NULL) {
		return -EINVAL;
	}
	if (udk_driver->drv_ops->create == NULL) {
		return 0;
	}

	ret = udk_module_open(udk_drv_owner_of(udk_driver));
	if (ret != 0) {
		return ret;
	}

	ret = udk_driver->drv_ops->create(udk_driver->ctx, name, args, devinfo);
	udk_module_close(udk_drv_owner_of(udk_driver));

	return ret;
}

static int native_bind(void *ctx, struct device_info *devinfo)
{
	struct udk_driver *udk_driver = (struct udk_driver *)ctx;
	int ret;

	if (udk_driver == NULL || udk_driver->drv_ops == NULL) {
		return -EINVAL;
	}
	if (udk_driver->drv_ops->bind == NULL) {
		return 0;
	}
	ret = udk_module_open(udk_drv_owner_of(udk_driver));
	if (ret != 0) {
		return ret;
	}
	ret = udk_driver->drv_ops->bind(udk_driver->ctx, devinfo);
	udk_module_close(udk_drv_owner_of(udk_driver));

	return ret;
}

static const struct hm_driver_ops g_udk_ops = {
	.create = native_create,
	.bind = native_bind,
	.owner = NULL,
};

static void devhost_udkdrv_release(hm_driver_t *drv)
{
	struct udk_driver *udk_driver = NULL;

	udk_driver = container_of(drv, struct udk_driver, drv);

	if (drv->libname != NULL) {
		free(drv->libname);
		drv->libname = NULL;
	}
	free(udk_driver);
}

int udk_register_driver(const char *libname,
			const char *compat,
			size_t compat_len,
			struct hm_driver_ops *ops,
			void *ctx)
{
	struct udk_driver *udk_driver = NULL;
	hm_driver_t *drv = NULL;
	int ret;

	if (libname == NULL || ops == NULL) {
		return -EINVAL;
	}
	/* udk_driver will free in udk_unregister_driver */
	udk_driver = (struct udk_driver *)malloc(sizeof(struct udk_driver));
	if (udk_driver == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*udk_driver);

	drv = &udk_driver->drv;
	drv->libname = strndup(libname, DRIVER_NAME_LEN - 1);
	if (drv->libname == NULL) {
		free(udk_driver);
		return -ENOMEM;
	}
	ret = devhost_driver_init(drv, &g_udk_ops, udk_driver);
	if (ret < 0) {
		free(drv->libname);
		free(udk_driver);
		return ret;
	}
	ret = devhost_driver_add_compat(drv, compat, compat_len);
	if (ret < 0) {
		/* if devhost_driver_register failed, devhost_drv_put will rollback */
		devhost_drv_put(drv);
		return ret;
	}

	udk_driver->drv_ops = ops;
	udk_driver->ctx = ctx;
	drv->release = devhost_udkdrv_release;

	ret = devhost_driver_register(drv);
	if (ret < 0) {
		/* if devhost_driver_register failed, devhost_drv_put will rollback */
		devhost_drv_put(drv);
		return ret;
	}

	return ret;
}

int udk_unregister_driver(const char *libname)
{
	hm_driver_t *drv = NULL;
	struct compatible_string compat_str;
	int ret;

	udk_debug("start unregister driver: %s\n", libname);

	if (libname == NULL) {
		return -EINVAL;
	}
	ret = compatible_string_init_from_libname(&compat_str, libname);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	ret = devhost_acquire_driver(compatible_string_next(&compat_str, NULL), &drv);
	compatible_string_reset(&compat_str);
	if (ret < 0) {
		return ret;
	}
	devhost_driver_destroy(drv);

	return 0;
}
