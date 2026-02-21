/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Define driver test of udk unload test module
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 07 18:01:03 2020
 */

#include <udk/driver.h>
#include <udk/log.h>
#include <libstrict/strict.h>
#include <string.h>

#include "../helper.h"
#include "udktest_unload.h"

static int udktest_unload_bind(void* ctx, struct device_info *info)
{
	UNUSED(ctx, info);
	return 0;
}

static int udktest_unload_create(void* ctx, const char* name,
				 const struct device_create_args *args,
				 struct device_info **pinfo)
{
	UNUSED(ctx, name, args, pinfo);
	return 0;
}

static struct hm_driver_ops bind_ops = {
	.bind = udktest_unload_bind,
	.create = NULL,
};

static struct hm_driver_ops create_ops = {
	.bind = NULL,
	.create = udktest_unload_create,
};

static struct hm_driver_ops drv_ops = {
	.bind = udktest_unload_bind,
	.create = udktest_unload_create,
};

static int register_bind_ops(void)
{
	int ret;

	ret = udk_register_driver(DRVNAME, DRVCOMPAT, strlen(DRVCOMPAT) + 1, &bind_ops, NULL);
	ASSERT_ZERO(ret);
	ret = udk_unregister_driver(DRVNAME);
	ASSERT_ZERO(ret);

	return 0;
}

static int register_create_ops(void)
{
	int ret;

	ret = udk_register_driver(DRVNAME, DRVCOMPAT, strlen(DRVCOMPAT) + 1, &create_ops, '\0');
	ASSERT_ZERO(ret);
	ret = udk_unregister_driver(DRVNAME);
	ASSERT_ZERO(ret);

	return 0;
}

static int register_ops(void)
{
	int ret;

	ret = udk_register_driver(DRVNAME, DRVCOMPAT, strlen(DRVCOMPAT) + 1, &drv_ops, '\0');
	ASSERT_ZERO(ret);
	ret = udk_unregister_driver(DRVNAME);
	ASSERT_ZERO(ret);

	return 0;
}

static int register_null(void)
{
	int ret;

	ret = udk_register_driver(NULL, DRVCOMPAT, strlen(DRVCOMPAT) + 1, &drv_ops, '\0');
	ASSERT_ZERO((ret != -EINVAL));

	return 0;
}

static int register_not_exist(void)
{
	int ret;

	ret = udk_register_driver("123456", "123456", strlen("123456") + 1, &drv_ops, '\0');
	ASSERT_ZERO(ret);
	ret = udk_unregister_driver("123456");
	ASSERT_ZERO(ret);

	return 0;
}

static int unregister_null(void)
{
	int ret;

	ret = udk_unregister_driver(NULL);
	ASSERT_ZERO((ret != -EINVAL));

	return 0;
}

static int unregister_not_exist(void)
{
	int ret;

	ret = udk_unregister_driver("123456");
	ASSERT_ZERO((int)(ret == 0));

	return 0;
}

int udktest_unload_driver(void)
{
	PROLOGUE;

	TCALL("udktest_unload_driver_register_bind", register_bind_ops);
	TCALL("udktest_unload_driver_register_create", register_create_ops);
	TCALL("udktest_unload_driver_register", register_ops);
	TCALL("udktest_unload_driver_register_null", register_null);
	TCALL("udktest_unload_driver_register_not_exist", register_not_exist);
	TCALL("udktest_unload_driver_unregister_null", unregister_null);
	TCALL("udktest_unload_driver_unregister_not_exist", unregister_not_exist);

	return 0;
}
