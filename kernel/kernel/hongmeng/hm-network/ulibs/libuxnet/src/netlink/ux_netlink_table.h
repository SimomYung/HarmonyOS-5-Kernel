/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Netlink table operations library
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan  3 17:10:16 2019
 */

#ifndef __UX_NETLINK_TABLE_H_
#define __UX_NETLINK_TABLE_H_

#include <lib/dlist.h>
#include <libhmsync/raw_rwlock.h>

#include "ux_netlink_core.h"

typedef struct ux_netlink_table {
	struct dlist_node head;
	struct raw_rwlock lock;
} ux_netlink_table_t;

int ux_netlink_table_init(void);
void ux_netlink_table_exit(void);

int ux_netlink_table_insert(ux_netlink_sock_t *nsk, uint32_t port);
void ux_netlink_table_remove(ux_netlink_sock_t *nsk);

ux_netlink_sock_t *ux_netlink_table_lookup(int proto, uint32_t port);

#ifdef CONFIG_UXNET_NETLINK
int ux_netlink_unicast(int proto, unsigned int dst_port,
		       const char *msg, const size_t len, bool nonblock);
#else
static inline int ux_netlink_unicast(int proto, unsigned int dst_port,
				     const char *msg, const size_t len, bool nonblock)
{
	UNUSED(proto, dst_port, msg, len);
	return E_HM_OK;
}
#endif
int ux_netlink_broadcast(int proto, unsigned int dst_group,
			 const char *msg, const size_t len);

#endif
