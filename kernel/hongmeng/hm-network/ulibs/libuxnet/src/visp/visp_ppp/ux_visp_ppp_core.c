/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Interface for visp network ppp core
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 25 16:05:16 2021
 */

#include "ux_visp_ppp_netdev.h"
#include "ux_visp_ppp_core.h"
#include "ux_socket.h"

int af_visp_ppp_init(void)
{
	int ret;
	ret = visp_ppp_netdev_init();
	if (ret) {
		net_error(HMNET_PPP, "init visp ppp netdev failed\n");
		return ret;
	}
	return 0;
}
