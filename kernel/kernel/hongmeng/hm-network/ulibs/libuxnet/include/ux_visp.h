/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for visp network
 * Author: Huawei OS Kernel Lab
 * Create: Fri June 18 17:35:37 2021
 */

#ifndef __UX_VISP_H_
#define __UX_VISP_H_

#include "ux_netdev.h"

#define NETIF_FLAG_UP           0x01U
#define NETIF_FLAG_BROADCAST    0x02U
#define NETIF_FLAG_DEBUG        0x04U
#define NETIF_FLAG_LOOPBACK     0x08U
#define NETIF_FLAG_POINTOPOINT  0x10U
#define NETIF_FLAG_NOTRAILERS   0x20U
#define NETIF_FLAG_LINK_UP      0x40U
#define NETIF_FLAG_NOARP        0x80U
#define NETIF_FLAG_PROMISC      0x100U
#define NETIF_FLAG_ALLMULTI     0x200U
#define NETIF_FLAG_MASTER       0x400U
#define NETIF_FLAG_SLAVE        0x800U
#define NETIF_FLAG_IGMP         0x1000U
#define NETIF_FLAG_PORTSEL      0x2000U
#define NETIF_FLAG_AUTOMEDIA    0x4000U
#define NETIF_FLAG_DYNAMIC      0x8000U
#define NETIF_FLAG_LOWER_UP     0x10000U
#define NETIF_FLAG_DORMANT      0x20000U
#define NETIF_FLAG_ECHO         0x40000U
#define NETIF_FLAG_ETHARP       0X80000U
#define NETIF_FLAG_ETHERNET     0x100000U
#define NETIF_FLAG_MLD6         0x200000U

struct netif {
	unsigned int ifindex;
	unsigned int flags;

	struct visp_netdev *netdev;
};

ux_netdev_ops_priv_t* visp_get_netdev_priv_ops(void);

#endif
