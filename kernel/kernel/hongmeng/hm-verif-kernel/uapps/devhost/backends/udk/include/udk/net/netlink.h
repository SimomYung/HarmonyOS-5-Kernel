/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Interface of netlink
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug  8 11:07:49 2023
 */

#ifndef __UDK_NETLINK_H__
#define __UDK_NETLINK_H__
#include <stdbool.h>
#include <stddef.h>

int udk_netlink_kernel_create(int unit);
int udk_netlink_kernel_release(int unit);
int udk_netlink_kernel_unicast(int unit, unsigned int dst_port,
			       const char *msg, const size_t len,
			       bool nonblock);
#endif
