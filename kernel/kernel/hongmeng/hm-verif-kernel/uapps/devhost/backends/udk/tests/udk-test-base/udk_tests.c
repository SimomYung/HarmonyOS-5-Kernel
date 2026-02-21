/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 10 16:29:43 2019
 */

#include "udk_tests.h"
#include <stddef.h>
#include <libtest/test.h>

#include "udk_test_helper.h"

#define UDK_TEST_LAUNCHER(fn, ioc)                                             \
	static int fn(void)                                                    \
	{                                                                      \
		return udk_test_device_ioctl(ioc);                             \
	}

UDK_TEST_LAUNCHER(udk_test__device, UDKTEST_IOC_DEVICE);
UDK_TEST_LAUNCHER(udk_test__driver, UDKTEST_IOC_DRIVER);

struct test udk_tests[] = {
	DEF_TEST(udk_test__device, "udk_test__device", FUNC_TEST)
	DEF_TEST(udk_test__driver, "udk_test__driver", FUNC_TEST)
	DEF_TEST(udk_test__fops, "udk_test__fops", FUNC_TEST)
	DEF_TEST(test_udk__net, "udk_test__net", FUNC_TEST)
	DEF_TEST(udk_test__block, "udk_test__block", FUNC_TEST)
	DEF_TEST(udk_test__shmlock, "udk_test__shmlock", FUNC_TEST)
	DEF_TEST(udk_test__unload, "udk_test__unload", FUNC_TEST)
	{
		.desc = NULL,
		.func = NULL,
		.funcname = NULL,
	}
};

struct test_modules tests[] = {
	DEF_MODULE_SETUP_TEARDOWN(do_all_udk_tests, udk_tests, udk_test_setup, udk_test_teardown)
	{
		.desc = NULL,
		.funcname = NULL,
		.test_list = NULL,
		.setup = NULL,
		.teardown = NULL,
	}
};
