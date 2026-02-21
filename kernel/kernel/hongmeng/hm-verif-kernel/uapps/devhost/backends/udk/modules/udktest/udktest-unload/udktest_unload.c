/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Define udk unload test module
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 05 05:13:04 2020
 */

#include <libstrict/strict.h>
#include <udk/device.h>
#include <udk/driver.h>
#include <udk/log.h>
#include <udk/module.h>
#include <string.h>
#include <errno.h>

#include "../helper.h"
#include "udktest_unload.h"

static struct hm_driver_ops g_driver_ops = {
	.bind = NULL,
	.create = NULL,
	.owner = UDK_THIS_MODULE,
};

static int test_unload_open(struct device_info *devinfo, void **ctx, unsigned int flags)
{
	UNUSED(devinfo, ctx, flags);
	return 0;
}

static int test_unload_close(void *ctx)
{
	UNUSED(ctx);
	return 0;
}

static struct devhost_fops g_devhost_fops = {
	.open = test_unload_open,
	.close = test_unload_close,
	.write = NULL,
	.read = NULL,
	.poll = NULL,
	.ioctl = NULL,
	.mmap = NULL,
	.llseek = NULL,
	.owner = UDK_THIS_MODULE,
};

static struct udk_device *udkdev = NULL;
static unsigned int major = 0;
static unsigned int minor = 0;

/* This string will be overridden to OK by udk-test-unload at module loading */
char *udktest_charp_param = "NOT OK";
udk_module_param(udktest_charp_param, charp);

static int udktest_unload_init(void)
{
	devnum_t devno = 0;
	int ret;

	udk_info("udk test unload init begin\n");

	udk_info("udk test unload param begin\n");
	ret = strcmp(udktest_charp_param, "OK");
	ASSERT_ZERO(ret);
	udk_info("udk test unload param end\n");

	udk_info("udk test unload driver begin\n");
	ret = udktest_unload_driver();
	ASSERT_ZERO(ret);
	udk_info("udk test unload driver end\n");

	udk_info("udk test unload device begin\n");
	ret = udktest_unload_device();
	ASSERT_ZERO(ret);
	udk_info("udk test unload device end\n");

	ret = udk_register_driver(DRVNAME, DRVCOMPAT, strlen(DRVCOMPAT) + 1, &g_driver_ops, NULL);
	if (ret < 0) {
		udk_error("fail to register driver, ret: %s\n", strerror(-ret));
		return ret;
	}

	ret = udk_alloc_chrdev_region(&devno, TEST_MINOR_START,
				      TEST_MAX_DEV_NUM, CHRDEV_NAME);
	if (ret) {
		return ret;
	}
	major = MAJOR(devno);
	minor = MINOR(devno);

	udkdev = udk_create_chrdev_duplicated(CHRDEV_NAME, devno, &g_devhost_fops);
	if (udkdev == NULL) {
		return -EINVAL;
	}

	udk_info("udk test unload init end\n");

	return 0;
}

static int udktest_unload_exit(void)
{
	devnum_t devno;
	int ret;

	udk_info("udk test unload exit begin\n");

	udk_device_destroy(udkdev);
	devno = MKDEV(major, minor);
	udk_unregister_chrdev_region(devno, TEST_MAX_DEV_NUM);

	ret = udk_unregister_driver(DRVNAME);
	if (ret < 0) {
		udk_error("fail to unregister driver, ret: %s\n", strerror(-ret));
		return ret;
	}

	udk_info("udk test unload exit end\n");

	return 0;
}

udk_module_init(udktest_unload_init);
udk_module_exit(udktest_unload_exit);
udk_module_compat(DRVCOMPAT);
