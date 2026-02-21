/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for netlink route
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan  3 17:10:16 2019
 */

#ifndef __UX_NETLINK_ROUTE_H_
#define __UX_NETLINK_ROUTE_H_
#include "ux_netdev.h"
#include "hm_netlink.h"

typedef struct ux_netlink_ops {
	int (*get_route_info)(void **buf, size_t *len);
	void (*release_route_info)(void);
	int (*get_neigh_info)(void **buf, size_t *len);
	void (*release_neigh_info)(void);
	void (*del_neigh_info)(uint32_t dst, char *ifname, uint16_t state);
} ux_netlink_ops_t;

typedef struct ux_netlink_route_link_ops {
	struct dlist_node node;
	const char *type;
	int (*addlink)(const char *ifname);
} ux_rtnl_link_ops_t;

#if defined(CONFIG_UXNET_NETLINK)
int ux_netlink_route_init(void);
void ux_netlink_route_exit(void);
void ux_rtnl_link_ops_register(ux_rtnl_link_ops_t *ops);
void ux_rtnl_link_ops_unregister(ux_rtnl_link_ops_t *ops);
void ux_netlink_register_ops(const ux_netlink_ops_t *ops);
#else
static inline int ux_netlink_route_init(void)
{
	return E_HM_OK;
}
static inline void ux_netlink_route_exit(void) {}
static inline void ux_rtnl_link_ops_register(ux_rtnl_link_ops_t *ops)
{
	UNUSED(ops);
}
static inline void ux_rtnl_link_ops_unregister(ux_rtnl_link_ops_t *ops)
{
	UNUSED(ops);
}
static inline void ux_netlink_register_ops(const ux_netlink_ops_t *ops)
{
	UNUSED(ops);
}
#endif

#endif
