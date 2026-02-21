/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: filters for tcp protocols
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 16:32:17 2021
 */

#include <lib/dlist.h>
#include <libhmlog/hmlog.h>

#include "ux_socket.h"
#include "ux_pbuf.h"
#include "ux_conntrack.h"
#include "ux_netfilter.h"

static int tcp_get_tuple_from_pkt(const void *skb,
				  unsigned int dataoff,
				  struct ux_conntrack_tuple *tuple)
{
	struct tcphdr *hp;

	hp = (struct tcphdr *)((unsigned char *)skb + dataoff);
	tuple->src.port = hp->th_sport;
	tuple->dst.ud.pt.port = hp->th_dport;

	return UX_NF_ACCEPT;
}

static int tcp_swap_tuple(struct ux_conntrack_tuple *tuple,
			  const struct ux_conntrack_tuple *orig)
{
	tuple->src.port = orig->dst.ud.pt.port;
	tuple->dst.ud.pt.port = orig->src.port;

	return UX_NF_ACCEPT;
}

/* Returns verdict for packet, or -1 for invalid. */
static int tcp_handle_packet(struct ux_conntrack *conntrack,
			     const void *skb,
			     enum ux_conntrack_stage_states ctinfo)
{
	return UX_NF_ACCEPT;
}

static int tcp_create(struct ux_conntrack *conntrack,
		      const void *skb)
{
	return UX_NF_ACCEPT;
}

static ux_ct_protocol_desc_t ux_ct_tcp_proto = {
	.protocol = IPPROTO_TCP,
	.name = "tcp",
	.get_tuple_from_pkt = tcp_get_tuple_from_pkt,
	.swap_tuple = tcp_swap_tuple,
	.handle_packet = tcp_handle_packet,
	.create = tcp_create,
};

ux_ct_protocol_desc_t *ux_get_tcp_proto(void)
{
	return &ux_ct_tcp_proto;
}
