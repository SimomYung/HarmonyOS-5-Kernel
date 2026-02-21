/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Netlink modules operations
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb  6 15:47:16 UTC 2020
 */

#include <libhmlog/hmlog.h>

#include "ux_netlink_modules.h"
#include "ux_netlink_taskstats.h"
#include "ux_netlink_uevent.h"
#include "ux_netlog.h"

int ux_netlink_modules_init(void)
{
	int ret;

	ret = ux_netlink_taskstats_init();
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "netlink taskstats init failed\n");
		return ret;
	}

	ret = ux_netlink_uevent_init();
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "netlink uevent init failed\n");
		goto err_uevent;
	}

	return 0;

err_uevent:
	ux_netlink_taskstats_exit();

	return ret;
}

void ux_netlink_modules_exit(void)
{
	ux_netlink_taskstats_exit();
	ux_netlink_uevent_exit();
}
