/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Define device test of udk unload test module
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 06 22:22:30 2020
 */

#include <udk/module.h>
#include <udk/device.h>
#include <libstrict/strict.h>

#include "../helper.h"
#include "udktest_unload.h"

static int destroy_null_chrdev(void)
{
	struct udk_chrdev *chrdev = NULL;

	udk_chrdev_destroy_duplicated(chrdev);

	return 0;
}

static int create_null_udkdev(void)
{
	struct udk_device *udkdev = NULL;
	const devnum_t devno = 1U;
	struct devhost_fops *fops = NULL;

	udkdev = udk_create_chrdev_duplicated(NULL, devno, fops);
	ASSERT_ZERO((udkdev != NULL));

	return 0;
}

static int create_test_chrdev(void)
{
	struct udk_chrdev *chrdev = NULL;
	struct udk_device *udkdev = NULL;
	const devnum_t devno = 1U;
	struct devhost_fops *fops = NULL;

	udkdev = udk_create_chrdev_duplicated("123456", devno, fops);
	ASSERT_ZERO((udkdev == NULL));

	chrdev = container_of(udkdev, struct udk_chrdev, udkdev);
	ASSERT_ZERO((chrdev == NULL));

	udk_chrdev_destroy_duplicated(chrdev);

	return 0;
}

static int udktest_unload_open(struct device_info *devinfo, void **ctx, unsigned int flags)
{
	UNUSED(devinfo, ctx, flags);
	return 0;
}

static int udktest_unload_close(void *ctx)
{
	UNUSED(ctx);
	return 0;
}

static struct devhost_fops dev_fops = {
	.open = udktest_unload_open,
	.close = udktest_unload_close,
	.read = NULL,
	.write = NULL,
	.ioctl = NULL,
	.mmap = NULL,
	.poll = NULL,
	.llseek = NULL,
	.owner = UDK_THIS_MODULE,
};

static int create_udkdev(void)
{
	struct udk_chrdev *chrdev = NULL;
	struct udk_device *udkdev = NULL;
	devnum_t devno = 0;
	int ret;

	ret = udk_alloc_chrdev_region(&devno, TEST_MINOR_START,
				      TEST_MAX_DEV_NUM, TEST_CHRDEV_NAME);
	ASSERT_ZERO(ret);

	udkdev = udk_create_chrdev_duplicated(TEST_CHRDEV_NAME, devno, &dev_fops);
	ASSERT_ZERO((udkdev == NULL));

	chrdev = container_of(udkdev, struct udk_chrdev, udkdev);
	ASSERT_ZERO((chrdev == NULL));

	udk_chrdev_destroy_duplicated(chrdev);

	return 0;
}

int udktest_unload_device(void)
{
	PROLOGUE;

	TCALL("destroy_null_chrdev", destroy_null_chrdev);
	TCALL("create_null_chrdev", create_null_udkdev);
	TCALL("create_test_chrdev", create_test_chrdev);
	TCALL("create_udkdev", create_udkdev);

	return 0;
}
