/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header file for ux_vlan
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 20:35:17 2021
 */

#ifndef __UX_VLAN_H__
#define __UX_VLAN_H__

#include <sys/types.h>
#include <lib/dlist.h>
#include "ux_netdev.h"

typedef struct ux_netdev_vlan {
	struct dlist_node list;
	ux_netdev_t *dev;
	ux_netdev_t *realdev;
	unsigned short vid;
} ux_netdev_vlan_t;

#define VLAN_HLEN 4
struct vlanhdr {
	uint16_t tp_vid;
	uint16_t tp_proto;
} __attribute__((packed));

int vlan_add(const char *realdev_name, unsigned short vid);
int vlan_delete(const char *vlan_name);
ux_netdev_t *vlan_netdev_get(const ux_netdev_t *realdev, unsigned short vlan_id);

int vlan_tag(ux_netdev_t *dev, void **data, int *tot_len, unsigned short vlan_id);
void vlan_untag(void **data, int *len);
#endif
