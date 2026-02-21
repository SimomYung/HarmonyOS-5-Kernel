/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network demux
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 10 19:41:36 2019
 */

#ifndef __UX_DEMUX_H
#define __UX_DEMUX_H

#include <lib/dlist.h>

#include "ux_netdev.h"
#include "ux_netlog.h"

typedef struct ux_packet_demux {
	struct dlist_node head;
	unsigned short type;
	ux_netdev_t *dev;

	int (*init)(ux_netdev_t *dev);
	void (*uninit)(const ux_netdev_t *dev);
	int (*func)(ux_netdev_t *dev, void *data, int len);

	int (*find_ip_reachable)(char **ifname, const u32_t gw);
	int (*set_route_entry)(int action, struct rtentry *rt);
	int (*set_default)(ux_netdev_t *dev, int action,
			   struct sockaddr *gw);
} ux_packet_demux_t;

int ux_register_packet_demux(ux_packet_demux_t *demux);
void ux_unregister_packet_demux(ux_packet_demux_t *demux);

ux_packet_demux_t *ux_packet_demux_lookup(unsigned short type);

#endif
