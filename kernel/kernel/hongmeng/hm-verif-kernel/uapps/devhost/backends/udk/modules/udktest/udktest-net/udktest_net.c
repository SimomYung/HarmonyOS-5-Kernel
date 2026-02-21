/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 09 15:00:50 2019
 */

#include "udktest_net.h"
#include <udk/module.h>
#include <udk/log.h>
#include <string.h>

#include "../helper.h"

static int udk_net_test_so_init(void)
{
	int ret;

	udk_debug("udk test netdrv begin\n");
	ret = test_alloc_free_netdev();
	ASSERT_ZERO(ret);

	return 0;
}

udk_module_init(udk_net_test_so_init);
