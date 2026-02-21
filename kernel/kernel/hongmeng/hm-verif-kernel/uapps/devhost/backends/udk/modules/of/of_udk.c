/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Framework of UDK OF
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 09 15:50:54 2019
 */

#include <inttypes.h>
#include <devhost/device.h>
#include <udk/mm.h>
#include <udk/of.h>
#include <udk/irq.h>
#include <udk/log.h>
#include <udk/errno.h>
#include <udk/iolib.h>
#include <udk/module.h>
#include <udk/device.h>
#include <udk/driver.h>
#include <udk/irq_ext.h>
#include <udk/securec.h>
#include <hmasm/page.h>
#include <udk/lib/string.h>
#include <libstrict/strict.h>
#include <internal/dtb.h>
#include <internal/compat.h>
#include <libstrict/strict.h>

#include "of_dev.h"
#include "of_fdt.h"

#define OF_UDK_DRIVER	"of_udk"

static bool probe_done = false;

static int of_udk_bind(void *ctx, struct device_info *info)
{
	struct udk_dt_node *soc_root = NULL, *devnode = NULL;
	struct udk_device *udkdev = NULL;
	void *dtb = NULL;
	int ret;

	UNUSED(ctx);

	if (probe_done) {
		return -EBUSY;
	}

	udkdev = udk_device_of(info);
	if (udkdev == NULL) {
		return -EINVAL;
	}

	dtb = udkdev->priv;
	if (dtb == NULL) {
		udk_error("Invalid DTB memory\n");
		return -EINVAL;
	}

	ret = udk_compat_unflatten_tree();
	if (ret != 0) {
		udk_info("compat unflatten tree failed\n");
	}

	udk_of_root = udk_of_unflatten_tree(dtb);
	if (udk_of_root == NULL) {
		udk_error("resolve dts failed\n");
		return -ENOENT;
	}

	soc_root = udk_of_find_node_by_fullname(udk_of_root, "/soc");
	if (soc_root == NULL) {
		udk_error("soc node not found\n");
		return -ENOENT;
	}

	ret = udk_of_init_soc_root(soc_root);
	if (ret < 0) {
		udk_warn("root node has been set up\n");
	}

	/* nod = root, NULL = lookup data, ofroot_udkdev = parent */
	for_each_child_dt_node(soc_root, devnode) {
		ret = udk_of_device_instantiate(devnode, udkdev);
		if (ret < 0) {
			udk_error("Failed to populate devices from DTS\n");
			break;
		}
	}

	probe_done = true;

	return ret;
}

static int of_udk_create(void *ctx,
			 const char *name,
			 const struct device_create_args *args,
			 struct device_info **pinfo)
{
	struct udk_device *udkdev = NULL;
	void *dtb = NULL;
	int ret;

	UNUSED(ctx, args);

	/* of_root will reference of property stored in dtb blob,
	 * so no need to free it. */
	ret = udk_acquire_bootfdt(&dtb, NULL);
	if (ret < 0) {
		udk_error("load kernel firmware data failed: %s\n", strerror(-ret));
		return -EINVAL;
	}
	udk_set_dtb(dtb);

	/* what's the parent devid of root:of? */
	udkdev = udk_device_alloc(name);
	if (udkdev == NULL) {
		udk_error("Failed to allocate root device\n");
		return -ENOMEM;
	}
	udkdev->priv = dtb;

	/* set probeable */
	udk_device_set_probeable(udkdev, OF_UDK_DRIVER, strlen(OF_UDK_DRIVER) + 1);

	/* register the device to devhost */
	ret = udk_device_add(udkdev);
	if (ret != 0) {
		udk_error("devhost create device failed\n");
		udk_device_free(udkdev);
		return ret;
	}

	if (pinfo != NULL) {
		*pinfo = udk_device_devinfo_of(udkdev);
	}

	return 0;
}

static struct hm_driver_ops of_ops __read_mostly = {
	.bind = of_udk_bind,
	.create = of_udk_create,
	.owner	= UDK_THIS_MODULE,
};

static int of_udk_init(void)
{
	int ret;

	udk_info("UDK of driver\n");

	ret = udk_request_module("of_compat.ko");
	if (ret < 0) {
		udk_info("Not liblinux-compatible, create udk null and zero device\n");

		ret = udk_special_device_create();
		if (ret < 0) {
			udk_error("Failed to create udk special device\n");
			return ret;
		}
	}

	return udk_register_driver(OF_UDK_DRIVER, OF_UDK_DRIVER, strlen(OF_UDK_DRIVER) + 1,
				   &of_ops, NULL);
}

static int of_udk_exit(void)
{
	return -EPERM;
}

udk_module_init(of_udk_init);
udk_module_compat(OF_UDK_DRIVER);
udk_module_exit(of_udk_exit);
