/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Framework of UDK OF PROXY
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 13 15:50:54 2023
 */

#include <libstrict/strict.h>
#include <devhost/device.h>

#include <udk/device.h>
#include <udk/driver.h>
#include <udk/log.h>
#include <udk/module.h>

#define OF_PROXY_UDK_DRIVER "of.proxy"

static int of_proxy_udk_create(void *ctx,
			 const char *name,
			 const struct device_create_args *args,
			 struct device_info **pinfo)
{
	UNUSED(ctx, args);

	/*
	 * NOTE: create fake chrdev
	 * support real device by proxy dtb in the future
	 */
	struct udk_device *dev = udk_create_chrdev_duplicated(name, 0, NULL);
	if (dev == NULL) {
		return -EINVAL;
	}
	*pinfo = udk_device_devinfo_of(dev);

	return 0;
}

static struct hm_driver_ops of_proxy_ops __read_mostly = {
	.bind	= NULL,
	.create	= of_proxy_udk_create,
	.owner	= UDK_THIS_MODULE,
};

static int of_proxy_udk_init(void)
{
	udk_info("UDK of proxy driver\n");

	return udk_register_driver(OF_PROXY_UDK_DRIVER, OF_PROXY_UDK_DRIVER,
				   strlen(OF_PROXY_UDK_DRIVER) + 1,
				   &of_proxy_ops, NULL);
}

static int of_proxy_udk_exit(void)
{
	return -EPERM;
}

udk_module_init(of_proxy_udk_init);
udk_module_compat(OF_PROXY_UDK_DRIVER);
udk_module_exit(of_proxy_udk_exit);
