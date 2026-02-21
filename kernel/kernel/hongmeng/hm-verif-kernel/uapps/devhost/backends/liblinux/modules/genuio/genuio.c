/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 13 16:05:44 2019
 */
#include <linux/kmod.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux_device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jiuyue Ma");
MODULE_DESCRIPTION("generic uio driver");
MODULE_VERSION("0.01");

static int genuio_bind(void* ctx, struct device_info *info)
{
	struct linux_device_object *obj = NULL;
	struct platform_device *pdev = NULL;
	int ret;

	if (info == NULL) {
		return -EINVAL;
	}
	obj = container_of(info, struct linux_device_object, info);

	pdev = to_platform_device(obj->dev);

	pdev->driver_override = "uio_pdrv_genirq";
	ret = device_attach(obj->dev);
	if (ret == 0) {
		return -ENOSYS;
	}
	return 0;
}

static struct hm_driver_ops genuio_ops __read_mostly = {
	.bind = genuio_bind,
	.create = NULL,	/* non-bus driver, no create support */
};

static int __check_uio_pdrv(struct device_driver *drv, void *drvname)
{
	return !strcmp(drv->name, (const char *)drvname);
}

static int __ensure_module(char *drvname, const char *modname)
{
	int ret = bus_for_each_drv(&platform_bus_type, NULL, drvname, __check_uio_pdrv);
	if (ret == 0) {
		return request_module(modname);
	}
	return 0;
}

static int __init genuio_init(void)
{
	if (__ensure_module("uio", "uio.ko") < 0) {
		pr_err("load module uio.ko failed\n");
		return -ENOSYS;
	}
	if (__ensure_module("uio_pdrv_genirq", "uio_pdrv_genirq.ko") < 0) {
		pr_err("load module uio_pdrv_genirq.ko failed\n");
		return -ENOSYS;
	}
	return devhost_register_driver("genuio.ko", &genuio_ops, NULL);
}
module_init(genuio_init);
