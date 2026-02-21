/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Netlink protocol operations library
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan  3 17:10:16 2019
 */

#ifndef __UX_NETLINK_PROTO_H_
#define __UX_NETLINK_PROTO_H_

#include "ux_socket.h"
#include "ux_pktbuf.h"
#include "hm_netlink.h"

#define NETLINK_PROTO_MAX	0x20

typedef struct ux_netlink_proto {
	int protocol;
	int (*rcv)(ux_sock_t *sk, ux_pktbuf_t *pkt);
	/* for dynamic create protocol */
	struct raw_refcnt_nr refcnt;
	/* is dynamic */
	bool is_dynamic;
} ux_netlink_proto_t;

#ifdef CONFIG_UXNET_NETLINK
int ux_netlink_proto_register(ux_netlink_proto_t *proto);
void ux_netlink_proto_unregister(ux_netlink_proto_t *proto);
void ux_netlink_proto_get(ux_netlink_proto_t *proto);
void ux_netlink_proto_put(ux_netlink_proto_t *proto);
ux_netlink_proto_t *ux_netlink_proto_lookup(int protocol);
#else
static inline int ux_netlink_proto_register(ux_netlink_proto_t *proto)
{
	UNUSED(proto);
	return E_HM_OK;
}

static inline void ux_netlink_proto_unregister(ux_netlink_proto_t *proto)
{
	UNUSED(proto);
}

static inline void ux_netlink_proto_get(ux_netlink_proto_t *proto)
{
	UNUSED(proto);
}

static inline void ux_netlink_proto_put(ux_netlink_proto_t *proto)
{
	UNUSED(proto);
}

static inline ux_netlink_proto_t *ux_netlink_proto_lookup(int protocol)
{
	UNUSED(protocol);
	return NULL;
}
#endif

int ux_netlink_rcv(ux_sock_t *sk, ux_pktbuf_t *pkt,
		   void (*handler)(ux_sock_t *, const ux_pktbuf_t *, struct nlmsghdr *));

#endif
