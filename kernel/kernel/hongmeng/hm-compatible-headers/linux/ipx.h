/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/ipx.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 05 17:20:38 2021
 */

#ifndef LINUX_IPX_H
#define LINUX_IPX_H

#include <linux/types.h>
#include <linux/socket.h>

#define IPX_NODE_LEN		6

struct sockaddr_ipx {
	__kernel_sa_family_t	sipx_family;
	__be16			sipx_port;
	__be32			sipx_network;
	unsigned char		sipx_node[IPX_NODE_LEN];
	__u8			sipx_type;
	unsigned char		sipx_zero;
};

#endif
