/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes of net device
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 12 19:11:36 2019
 */
#ifndef __UDK_NET_H__
#define __UDK_NET_H__

#include <udk/net/netdev.h>

/* compat struct for ifmap */
struct ifmap32 {
	unsigned int mem_start;
	unsigned int mem_end;
	unsigned short int base_addr;
	unsigned char irq;
	unsigned char dma;
	unsigned char port;
};

/* compat struct for ifreq */
struct ifreq32 {
	union {
		char ifrn_name[IFNAMSIZ];
	} ifr_ifrn;
	union {
		struct sockaddr ifru_addr;
		struct sockaddr ifru_dstaddr;
		struct sockaddr ifru_broadaddr;
		struct sockaddr ifru_netmask;
		struct sockaddr ifru_hwaddr;
		short int ifru_flags;
		int ifru_ivalue;
		int ifru_mtu;
		struct ifmap32 ifru_map;
		char ifru_slave[IFNAMSIZ];
		char ifru_newname[IFNAMSIZ];
		unsigned int ifru_data;
	} ifr_ifru;
};

int udk_netif_tx(struct pkt_buf *pbuf);

/* open and close net device */
int udk_netdev_open(struct udk_net_device *dev);
int udk_netdev_close(struct udk_net_device *dev);

extern struct devhost_net_ops udk_net_ops;

#endif /* __UDK_NET_H__ */
