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

#define DEFAULT_RES_ADDR 0xdeadcafeUL
#define DEFAULT_RES_SIZE 0x12345678UL

#define DEFAULT_PROP_NAME "test_prop"

static struct udk_device *dev = NULL;

static int resprop_get_nonexist_res(void)
{
	struct udk_resource *res = NULL;
	int found_1 = 0, found_2 = 0;

	if (dev == NULL) {
		return -1;
	}

	res = udk_device_get_resource(dev, IORESOURCE_REG, 3);
	if (res != NULL) {
		found_1 = 1;
	}

	udk_for_each_resource(dev, res) {
		if (res->addr == DEFAULT_RES_ADDR &&
		    res->size == DEFAULT_RES_SIZE &&
		    res->flags & IORESOURCE_REG) {
			found_2 = 1;
			break;
		}
	}

	/* Should found nothing */
	return (found_1 == 0 && found_2 == 0) ? 0 : -1;
}

static int resprop_get_nonexist_prop(void)
{
	struct hm_property_s *prop = NULL;

	if (dev == NULL) {
		return -1;
	}

	prop = udk_device_get_prop(dev, DEFAULT_PROP_NAME);

	/* Should found nothing */
	return (prop == NULL) ? 0 : -1;
}

static int resprop_add_resource(void)
{
	int ret, found = 0;
	struct udk_resource *res = NULL;

	if (dev == NULL) {
		return -1;
	}

	ret = udk_device_add_resource(dev, DEFAULT_RES_ADDR,
				      DEFAULT_RES_SIZE, IORESOURCE_REG);
	if (ret < 0) {
		udk_error("Failed to add resource %lx\n", DEFAULT_RES_ADDR);
		return -1;
	}

	udk_for_each_resource(dev, res) {
		if (res->addr == DEFAULT_RES_ADDR &&
		    res->size == DEFAULT_RES_SIZE &&
		    res->flags & IORESOURCE_REG) {
			found = 1;
			break;
		}
	}
	if (found == 0) {
		udk_error("resource not found\n");
	}

	return (found == 1) ? 0 : -1;
}

static int resprop_add_prop(void)
{
	int ret;
	bool found = 0;

	if (dev == NULL) {
		return -1;
	}

	ret = udk_device_add_property(dev, DEFAULT_PROP_NAME, 0, NULL);
	if (ret < 0) {
		udk_error("Failed to add property %s\n", DEFAULT_PROP_NAME);
		return -1;
	}

	found = udk_device_prop_read_bool(dev, DEFAULT_PROP_NAME);

	return (found == 1) ? 0 : -1;
}

static int resprop_setup(void)
{
	int ret;

	/* Allocate test device */
	dev = udk_device_alloc("udktest_resprop");
	if (dev == NULL) {
		return -ENOMEM;
	}

	/* Add test device */
	ret = udk_device_add(dev);
	if (ret < 0) {
		udk_device_destroy(dev);
		dev = NULL;
	}

	return (ret == 0) ? 0 : -1;
}

static void teardown_release_device(void)
{
	if (dev != NULL) {
		udk_device_destroy(dev);
		dev = NULL;
	}
}

static int __udktest__device_resprop(void)
{
	PROLOGUE;

	TCALL("udktest__device_resprop_get_empty_res", resprop_get_nonexist_res);
	TCALL("udktest__device_resprop_get_empty_prop", resprop_get_nonexist_prop);
	TCALL("udktest__device_resprop_add_resource", resprop_add_resource);
	/* Add multiple add operations */
	TCALL("udktest__device_resprop_add_prop", resprop_add_prop);

	return ret;
}

int udktest__device_resprop(void)
{
	int ret;

	ret = resprop_setup();
	if (ret) {
		return ret;
	}

	ret = __udktest__device_resprop();

	teardown_release_device(); /* Rollback on an any occasions */

	return ret;
}
