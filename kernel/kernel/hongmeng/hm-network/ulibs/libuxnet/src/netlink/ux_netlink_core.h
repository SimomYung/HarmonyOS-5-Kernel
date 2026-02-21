/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Netlink core methods library
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan  3 17:10:16 2019
 */

#ifndef __UX_NETLINK_CORE_H_
#define __UX_NETLINK_CORE_H_

#include "hm_netlink.h"

#include "ux_socket.h"
#include "ux_pktbuf.h"
#include "ux_netlink_proto.h"

#include <lib/dlist.h>

#define NETLINK_SERVICE		0x01

#define NETLINK_PAYLOAD_MAX	8192

typedef struct ux_netlink_sock {
	ux_sock_t sk;
	uint32_t port;
	uint32_t group;
	uint32_t dst_port;
	uint32_t dst_group;
	uint8_t bound;
	struct dlist_node head;
	ux_netlink_proto_t *proto;
	int flags;
	int protocol;
} ux_netlink_sock_t;

static inline ux_netlink_sock_t *netlink_sock(const ux_sock_t *sk)
{
	return (ux_netlink_sock_t *)(void *)sk;
}

static inline int ux_netlink_is_service(const ux_netlink_sock_t *nsk)
{
	return (int)((unsigned int)nsk->flags & NETLINK_SERVICE);
}
#ifdef CONFIG_UXNET_NETLINK
int ux_netlink_create_service(ux_netlink_proto_t *proto);
void ux_netlink_release_service(ux_netlink_proto_t *proto);
#else
static inline int ux_netlink_create_service(ux_netlink_proto_t *proto)
{
	UNUSED(proto);
	return E_HM_OK;
}

static inline void ux_netlink_release_service(ux_netlink_proto_t *proto)
{
	UNUSED(proto);
}
#endif

int ux_netlink_dump_done(ux_sock_t *sk, uint32_t seq);
int af_netlink_init(void);

void ux_netlink_error(ux_sock_t *sk, const struct nlmsghdr *nlh, int err);
int ux_netlink_recv_data(ux_sock_t *sk, ux_pktbuf_t *pkt, bool nonblock);

static inline size_t ux_nlmsg_space(size_t payload)
{
	return (size_t)NLMSG_SPACE(payload);
}

static inline int ux_nlmsg_data_len(const struct nlmsghdr *nlh)
{
	return (int)(nlh->nlmsg_len - NLMSG_HDRLEN);
}

static inline void *ux_nlmsg_data(struct nlmsghdr *nlh)
{
	return (void *)((unsigned char *)nlh + NLMSG_HDRLEN);
}

int ux_netlink_attr_get(uintptr_t data, size_t len, const struct rtattr *attr);

static inline int ux_netlink_attr_get_u32(const uint32_t *value,
					  const struct rtattr *attr)
{
	return ux_netlink_attr_get((uintptr_t)value, sizeof(uint32_t), attr);
}

/**
 * @return true: failed; false: success
 */
bool ux_netlink_attr_put(ux_pktbuf_t *pkt, uint16_t attr, const void *data, size_t len);

/**
 * @return true: failed; false: success
 */
static inline bool ux_netlink_attr_put_u32(ux_pktbuf_t *pkt, uint16_t attr,
					   uint32_t value)
{
	return ux_netlink_attr_put(pkt, attr, &value, sizeof(uint32_t));
}

/**
 * @return true: failed; false: success
 */
static inline bool ux_netlink_attr_put_u16(ux_pktbuf_t *pkt, uint16_t attr,
					   uint16_t value)
{
	return ux_netlink_attr_put(pkt, attr, &value, sizeof(uint16_t));
}

static inline struct nlattr *ux_nla_nest_start(ux_pktbuf_t *pkt, uint16_t type)
{
	struct nlattr *nla = (struct nlattr *)pkt->tail;

	if (ux_netlink_attr_put(pkt, type, NULL, 0)) {
		return NULL;
	}

	return nla;
}

static inline size_t ux_nla_nest_end(const ux_pktbuf_t *pkt, struct nlattr *attr)
{
	attr->nla_len = (uint16_t)((char *)pkt->tail - (char *)attr);

	return pkt->len;
}

#endif
