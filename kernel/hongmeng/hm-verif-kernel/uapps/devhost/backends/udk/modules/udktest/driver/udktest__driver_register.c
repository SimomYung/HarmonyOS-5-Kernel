/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */

#include <udk/module.h>
#include <udk/device.h>
#include <udk/driver.h>
#include <libstrict/strict.h>

#include <helper.h>

#include "udktest__driver.h"

static void teardown_unregister_driver(const char *libname)
{
	UNUSED(libname);
}

/* Currently, null ops is allowed */
static int register_null_ops(void)
{
	int ret;

	ret = udk_register_driver("null_ops_driver", NULL, 0, NULL, NULL);
	teardown_unregister_driver("null_ops_driver");

	return (ret != 0) ? 0 : -1;
}

int udktest__driver_register(void)
{
	PROLOGUE;

	TCALL("udktest__driver_register_noops", register_null_ops);

	return ret;
}
