/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Netlink protocol operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan  3 17:10:16 2019
 */

#include <errno.h>

#include <libhmsync/raw_rwlock.h>

#include "ux_netlink_proto.h"

static struct raw_rwlock ux_netlink_proto_lock = RAW_RDLOCK_INITIALIZER;
static ux_netlink_proto_t *ux_netlink_protos[NETLINK_PROTO_MAX] = { 0 };

int ux_netlink_proto_register(ux_netlink_proto_t *proto)
{
	int ret = 0;

	if (proto->protocol < 0 || proto->protocol >= NETLINK_PROTO_MAX) {
		return E_HM_INVAL;
	}

	(void)raw_rwlock_wrlock(&ux_netlink_proto_lock);
	if (!ux_netlink_protos[proto->protocol]) {
		ux_netlink_protos[proto->protocol] = proto;
	} else {
		ret = E_HM_OBJEXIST;
	}
	(void)raw_rwlock_unlock(&ux_netlink_proto_lock);

	return ret;
}

void ux_netlink_proto_unregister(ux_netlink_proto_t *proto)
{
	if (proto->protocol < 0 || proto->protocol >= NETLINK_PROTO_MAX) {
		return;
	}

	(void)raw_rwlock_wrlock(&ux_netlink_proto_lock);
	ux_netlink_protos[proto->protocol] = NULL;
	(void)raw_rwlock_unlock(&ux_netlink_proto_lock);
}

void ux_netlink_proto_get(ux_netlink_proto_t *proto)
{
	int ret;

	if (!proto->is_dynamic) {
		return;
	}

	ret = raw_refcnt_nr_get(&proto->refcnt);
	BUG_ON(ret <= 0, "proto refcnt fail, refcnt %d, ret %d(%s)\n",
	       raw_refcnt_nr_counter(&proto->refcnt), ret, hmstrerror(ret));
}

void ux_netlink_proto_put(ux_netlink_proto_t *proto)
{
	int ret;

	if (!proto->is_dynamic) {
		return;
	}

	ret = raw_refcnt_nr_counter(&proto->refcnt);
	BUG_ON(ret <= 0, "proto refcnt invalid, refcnt: %d, ret: %s\n",
	       ret, hmstrerror(ret));

	if (raw_refcnt_nr_put(&proto->refcnt) == 0) {
		free(proto);
	}
}

ux_netlink_proto_t *ux_netlink_proto_lookup(int protocol)
{
	ux_netlink_proto_t *proto = NULL;

	if (protocol < 0 || protocol >= NETLINK_PROTO_MAX) {
		return NULL;
	}

	(void)raw_rwlock_rdlock(&ux_netlink_proto_lock);
	proto = ux_netlink_protos[protocol];
	if (proto != NULL) {
		ux_netlink_proto_get(proto);
	}
	(void)raw_rwlock_unlock(&ux_netlink_proto_lock);

	return proto;
}

int ux_netlink_rcv(ux_sock_t *sk, ux_pktbuf_t *pkt,
		   void (*handler)(ux_sock_t *, const ux_pktbuf_t *, struct nlmsghdr *))
{
	struct nlmsghdr *nlh = NULL;
	unsigned int msgsize;

	while (pkt->len >= NLMSG_HDRLEN) {
		nlh = (struct nlmsghdr *)pkt->data;

		/* payload is too short for nlmsg */
		if (nlh->nlmsg_len < NLMSG_HDRLEN ||
		    pkt->len < nlh->nlmsg_len) {
			net_debug(HMNET_NETLINK, "error too short\n");
			return 0;
		}

		/* service only handle request */
		if ((nlh->nlmsg_flags & NLM_F_REQUEST) == 0U) {
			net_debug(HMNET_NETLINK, "no request\n");
			goto next;
		}

		/* service do not handle control message */
		if (nlh->nlmsg_type < NLMSG_MIN_TYPE) {
			net_debug(HMNET_NETLINK, "control message\n");
			goto next;
		}

		handler(sk, pkt, nlh);
next:
		msgsize = NLMSG_ALIGN(nlh->nlmsg_len);
		/* this is the last one message */
		if (msgsize > pkt->len) {
			(void)ux_pktbuf_pull(pkt, (unsigned int)pkt->len);
			break;
		}

		(void)ux_pktbuf_pull(pkt, msgsize);
	}
	return 0;
}
