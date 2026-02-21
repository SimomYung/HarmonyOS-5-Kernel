/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 10:37:35 2019
 */

#include <udk/device.h>
#include <udk/iolib.h>
#include <udk/panic.h>
#include <udk/errno.h>
#include <udk/log.h>
#include <udk/securec.h>

#include <helper.h>

#include "udktest__device.h"

static int add_null(void)
{
	int ret;

	ret = udk_device_add(NULL);

	return (ret < 0) ? 0 : -1;
}

static int add_not_init(void)
{
	int ret;
	struct udk_device *dev = NULL;

	dev = (struct udk_device *)udk_malloc(sizeof(*dev));
	if (dev == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*dev);

	dev->name = "udktest_add_1";
	ret = udk_device_add(dev); /* Should fail */

	udk_free(dev);

	return (ret == -EINVAL) ? 0 : -1;
}

static int add_manual(void)
{
	int ret;
	struct udk_device *dev = NULL;

	dev = (struct udk_device *)udk_malloc(sizeof(*dev));
	if (dev == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*dev);

	dev->name = "udktest_add_3";
	ret = udk_device_init(dev);
	if (ret < 0) {
		udk_free(dev);
		return -EINVAL;
	}
	ret = udk_device_add(dev);

	udk_device_reset(dev);
	udk_free(dev);

	return (ret == 0) ? 0 : -1;
}

static int add_auto(void)
{
	int ret;
	struct udk_device *dev = NULL;

	dev = (struct udk_device *)udk_device_alloc("udktest_add_4");
	if (dev == NULL) {
		return -ENOMEM;
	}

	ret = udk_device_add(dev);

	udk_device_destroy(dev);

	return (ret == 0) ? 0 : -1;
}

static int add_double_init(void)
{
	int ret;
	struct udk_device *dev = NULL;

	dev = (struct udk_device *)udk_device_alloc("udktest_add_5");
	if (dev == NULL) {
		return -ENOMEM;
	}

	ret = udk_device_init(dev); /* Should return error on double init */

	udk_device_destroy(dev);

	return (ret == -EINVAL) ? 0 : -1;
}

int udktest__device_add(void)
{
	PROLOGUE;

	TCALL("udktest__device_basic_add_null", add_null);
	TCALL("udktest__device_basic_add_not_init", add_not_init);
	TCALL("udktest__device_basic_add_manual", add_manual);
	TCALL("udktest__device_basic_add_auto", add_auto);
	TCALL("udktest__device_basic_add_double_init", add_double_init);

	return ret;
}
