/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Udk netlink
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 29 09:43:06 2020
 */

#include <udk/net/netlink.h>
#include <hmnet/hm_netlink.h>
#include <hongmeng/errno.h>
#include <stdbool.h>

int udk_netlink_kernel_create(int unit)
{
	return -hmerrno2posix(hm_netlink_create_service(unit));
}

int udk_netlink_kernel_release(int unit)
{
	return -hmerrno2posix(hm_netlink_release_service(unit));
}

int udk_netlink_kernel_unicast(int unit, unsigned int dst_port,
			       const char *msg, const size_t len,
			       bool nonblock)
{
	int ret;

	if (!nonblock) {
		ret = hm_netlink_unicast(unit, 0, dst_port, msg, len);
	} else {
		ret = hm_netlink_unicast_nowait(unit, 0, dst_port, msg, len);
	}

	return -hmerrno2posix(ret);
}
