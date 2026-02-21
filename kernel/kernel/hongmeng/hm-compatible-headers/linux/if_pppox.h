/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Add linux/if_pppox.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 16 21:33:23 2023
 */
#ifndef _LINUX_IF_PPPOX_H
#define _LINUX_IF_PPPOX_H

#include <linux/socket.h>
#include <linux/if_pppol2tp.h>

#define PX_PROTO_OL2TP 1

struct sockaddr_pppol2tp {
	__kernel_sa_family_t sa_family;
	unsigned int    sa_protocol;
	struct pppol2tp_addr pppol2tp;
} __attribute__((packed));

#endif

