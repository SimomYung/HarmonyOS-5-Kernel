/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */

#include <sys/ioctl.h>

#include <udk/module.h>
#include <udk/device.h>
#include <udk/iolib.h>
#include <udk/panic.h>
#include <udk/errno.h>
#include <udk/log.h>
#include <udk/securec.h>

#include <helper.h>

#include "udktest__device.h"

#define LONG_TEXT \
	"abcdefghijklmnopqrstuvwxyz"	\
	"abcdefghijklmnopqrstuvwxyz"	\
	"abcdefghijklmnopqrstuvwxyz"

static int alloc_testdev(void)
{
	struct udk_device *dev = NULL;
	int ret;

	dev = udk_device_alloc("helloworld");
	ret = (dev != NULL) ? 0 : -EINVAL;

	udk_device_destroy(dev);

	return ret;
}

static int alloc_long_name_dev(void)
{
	struct udk_device *dev = NULL;
	int ret;

	dev = udk_device_alloc(LONG_TEXT);
	ret = (dev != NULL) ? 0 : -EINVAL;

	udk_device_destroy(dev);

	return ret;
}

static int alloc_null_name(void)
{
	struct udk_device *dev = NULL;

	dev = udk_device_alloc(NULL);

	return (dev == NULL) ? 0 : -EINVAL;
}

int udktest__device_alloc(void)
{
	PROLOGUE;

	TCALL("udk_device_alloc_testdev", alloc_testdev);
	TCALL("udk_device_alloc_long_name_dev", alloc_long_name_dev);
	TCALL("udk_device_alloc_null_name", alloc_null_name);

	return ret;
}
