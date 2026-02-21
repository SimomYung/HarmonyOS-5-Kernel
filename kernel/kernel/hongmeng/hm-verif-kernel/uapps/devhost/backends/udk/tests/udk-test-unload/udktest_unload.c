/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: UDK unload module test
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 05 18:59:09 2020
 */
#include "udktest_unload.h"
#include <libhmlog/hmlog.h>
#include <libhmsrv_io/mgmt.h>

static int test_load_module(void)
{
	int ret;

	ret = hmsrv_io_mgmt_load_module(DM_MGMT_ROOT_TARGET,
					UDK_TEST_UNLOAD_MODNAME, UDK_TEST_UNLOAD_MODARGS);
	if (ret != E_HM_OK) {
		hm_error("load module %s to devid %d failed: %s\n",
			 UDK_TEST_UNLOAD_MODNAME, DEFAULT_DEVID, hmstrerror(ret));
		return ret;
	}

	return 0;
}

static int test_unload_module(void)
{
	int ret;

	ret = hmsrv_io_mgmt_unload_module(DM_MGMT_ROOT_TARGET, UDK_TEST_UNLOAD_MODNAME);
	if (ret != E_HM_OK) {
		hm_error("unload module %s from root devhost failed: %s\n",
			 UDK_TEST_UNLOAD_MODNAME, hmstrerror(ret));
		return ret;
	}

	return 0;
}

struct test udk_unload_tests[] = {
	TESTHM("test_load_module", test_load_module)
	TESTHM("test_unload_module", test_unload_module)
	{
		.desc = NULL,
		.func = NULL,
		.funcname = NULL,
	}
};
