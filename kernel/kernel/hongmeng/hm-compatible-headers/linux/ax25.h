/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/ax25.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 05 17:21:48 2021
 */
#ifndef LINUX_AX25_H
#define LINUX_AX25_H

#include <linux/socket.h>

typedef struct {
	char			ax25_call[7];
} ax25_address;

struct sockaddr_ax25 {
	__kernel_sa_family_t	sax25_family;
	ax25_address		sax25_call;
	int			sax25_ndigis;
};

#endif
