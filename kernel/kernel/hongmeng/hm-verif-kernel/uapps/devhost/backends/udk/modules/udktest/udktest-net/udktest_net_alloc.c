/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 09 15:00:50 2019
 */
#include <udk/module.h>
#include <udk/device.h>
#include <udk/sync.h>
#include <udk/log.h>
#include <udk/net/netdev.h>
#include <string.h>
#include <unistd.h>

#include "../helper.h"
#include "udktest_net.h"

static int test_alloc_netdev(void)
{
	struct udk_net_device *pNetDev = NULL;

	pNetDev = udk_netdev_alloc("eth%d", NULL);
	ASSERT_ZERO((pNetDev == NULL));

	udk_netdev_free(pNetDev);

	return 0;
}

static int test_alloc_netdev_twice(void)
{
	unsigned int id_tmp;
	struct udk_net_device *pNetDev = NULL;

	pNetDev = udk_netdev_alloc("eth%d", NULL);
	ASSERT_ZERO((pNetDev == NULL));
	id_tmp = pNetDev->name_id;
	udk_netdev_free(pNetDev);

	pNetDev = udk_netdev_alloc("eth%d", NULL);
	ASSERT_ZERO((pNetDev == NULL));
	ASSERT_ZERO((pNetDev->name_id != id_tmp));
	udk_netdev_free(pNetDev);

	return 0;
}

int test_alloc_free_netdev(void)
{
	PROLOGUE;

	TCALL("test_alloc_netdev", test_alloc_netdev);
	TCALL("test_add_and_delete_netdev_twice", test_alloc_netdev_twice);

	return ret;
}
