/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: interface for network vcan device
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan  2 22:03:24 2019
 */

#include <net/if_arp.h>

#include "hm_can.h"

#include "ux_netdev.h"

#include "ux_can_vcan.h"

static int ux_can_vcan_xmit(ux_netdev_t *dev, const void *data, int len, int flags)
{
	return 0;
}

static int ux_can_vcan_set_mtu(ux_netdev_t *dev, int mtu)
{
	return 0;
}

static ux_netdev_ops_t vcan_ops = {
	.ndo_xmit = ux_can_vcan_xmit,
	.ndo_set_mtu = ux_can_vcan_set_mtu,
};

int ux_can_vcan_init(void)
{
	ux_netdev_t *dev = NULL;
	int ret;

	dev = ux_netdev_alloc("vcan0", 0);
	if (dev == NULL) {
		return E_HM_NOMEM;
	}

	dev->ops = &vcan_ops;
	dev->type = ARPHRD_CAN;

	ux_rtnl_lock();
	ret = ux_netdev_register(dev);
	ux_rtnl_unlock();
	if (ret) {
		ux_netdev_free(dev);
		return ret;
	}

	return 0;
}
