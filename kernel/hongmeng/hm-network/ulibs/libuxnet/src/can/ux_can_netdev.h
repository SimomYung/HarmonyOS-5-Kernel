/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description:  Header file for network can netdev
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 09:49:09 2019
 */

#ifndef __UX_CAN_NETDEV_H_
#define __UX_CAN_NETDEV_H_

#include "ux_netdev.h"
#include "ux_pktbuf.h"

typedef struct ux_can_netdev {
	struct dlist_node raw_sock_list;
	struct raw_mutex raw_sock_list_lock;
} ux_can_netdev_t;

int ux_can_netdev_init(void);
void ux_can_netdev_exit(void);

int ux_can_netdev_xmit(ux_netdev_t *dev, const ux_pktbuf_t *pkt, int flags);

#endif
