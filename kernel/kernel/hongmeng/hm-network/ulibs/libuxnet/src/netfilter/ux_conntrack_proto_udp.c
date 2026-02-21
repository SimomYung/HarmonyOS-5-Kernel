/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: filters for udp protocols
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 16:32:17 2021
 */

#include <lib/dlist.h>
#include <libhmlog/hmlog.h>

#include "ux_socket.h"
#include "ux_pbuf.h"
#include "ux_conntrack.h"
#include "ux_netfilter.h"

static int udp_get_tuple_from_pkt(const void *skb,
				  unsigned int dataoff,
				  struct ux_conntrack_tuple *tuple)
{
	struct udphdr *hp;

	hp = (struct udphdr *)((unsigned char *)skb + dataoff);
	tuple->src.port = hp->uh_sport;
	tuple->dst.ud.pt.port = hp->uh_dport;

	return UX_NF_ACCEPT;
}

static int udp_swap_tuple(struct ux_conntrack_tuple *tuple,
			  const struct ux_conntrack_tuple *orig)
{
	tuple->src.port = orig->dst.ud.pt.port;
	tuple->dst.ud.pt.port = orig->src.port;
	return UX_NF_ACCEPT;
}

static int udp_handle_packet(struct ux_conntrack *conntrack,
			     const void *skb,
			     enum ux_conntrack_stage_states ctinfo)
{
	return UX_NF_ACCEPT;
}

static int udp_create(struct ux_conntrack *conntrack, const void *skb)
{
	return UX_NF_ACCEPT;
}

static ux_ct_protocol_desc_t ux_ct_udp_proto = {
	.protocol = IPPROTO_UDP,
	.name = "udp",
	.get_tuple_from_pkt = udp_get_tuple_from_pkt,
	.swap_tuple = udp_swap_tuple,
	.handle_packet = udp_handle_packet,
	.create = udp_create,
};

ux_ct_protocol_desc_t *ux_get_udp_proto(void)
{
	return &ux_ct_udp_proto;
}
