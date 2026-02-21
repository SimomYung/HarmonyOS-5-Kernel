/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Add linux/if_pppol2tp.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 16 21:33:23 2023
 */
#ifndef _LINUX_IF_PPPOL2TP_H
#define _LINUX_IF_PPPOL2TP_H

#include <linux/types.h>
#include <linux/in.h>

struct pppol2tp_addr {
	__kernel_pid_t	pid;
	int	fd;
	struct sockaddr_in addr;
	__u16 s_tunnel, s_session;
	__u16 d_tunnel, d_session;
};

#endif
