/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for network libvisp
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 29 09:56:30 2021
 */

#ifndef __HM_NET_UX_LIBVISP_H
#define __HM_NET_UX_LIBVISP_H

#include <visp_netdev.h>
#include <visp_stack.h>

#include "ux_visp.h"
#include "ux_netdev.h"
#include "ux_netlog.h"
#include "ux_socket.h"

typedef struct ux_visp_sock {
	ux_sock_t sk;

	int fd;
} ux_visp_sock_t;

int af_inet_init(void);

int visp_netdev_init(void);
int visp_socket_init(void);

/**
 * The netif name used by visp and HM is different ("Ethernet0" vs "eth0").
 */
int visp_get_ifname_by_index(unsigned int ifindex, char *name, size_t max_len);

void visp_event_notify_hook(int fd, struct visp_event_notify_args *args);
#endif
