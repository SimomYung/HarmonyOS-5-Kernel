/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/x25.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 05 17:20:58 2021
 */

#ifndef LINUX_X25_H
#define LINUX_X25_H

#include <linux/socket.h>

struct x25_address {
	char x25_addr[16];
};

struct sockaddr_x25 {
	__kernel_sa_family_t sx25_family;
	struct x25_address sx25_addr;
};

#endif
