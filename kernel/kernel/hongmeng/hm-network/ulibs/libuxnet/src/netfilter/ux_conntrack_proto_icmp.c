/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: filters for icmp
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 16:32:17 2021
 */

#include <lib/dlist.h>
#include <libhmlog/hmlog.h>

#include "ux_socket.h"
#include "ux_pbuf.h"
#include "ux_conntrack.h"
#include "ux_netfilter.h"

#define ICMP_ORI_REPLY 2

static int icmp_get_tuple_from_pkt(const void *skb,
				   unsigned int dataoff,
				   struct ux_conntrack_tuple *tuple)
{
	struct icmphdr *hp;

	/* ux_ip_hdr size is equal dataoff, generally */
	hp = (struct icmphdr *)((unsigned char *)skb + dataoff);

	tuple->dst.ud.icmp.type = hp->type;
	tuple->src.port = hp->un.echo.id;
	tuple->dst.ud.icmp.code = hp->code;

	return UX_NF_ACCEPT;
}

static int icmp_swap_tuple(struct ux_conntrack_tuple *tuple,
			   const struct ux_conntrack_tuple *orig)
{
	uint8_t dstype = 0;
	uint8_t type = orig->dst.ud.icmp.type;

	if (ICMP_INFOTYPE(type) == 0) {
		return UX_NF_DROP;
	}
	/* type swap */
	if (type == ICMP_ECHOREPLY) {
		dstype = ICMP_ECHO;
	}
	if ((type >= ICMP_TIMESTAMP) && (type <= ICMP_ADDRESSREPLY)) {
		dstype = ((type % ICMP_ORI_REPLY) == 0) ? (type - 1) : (type + 1);
	}

	tuple->src.port = orig->src.port;
	tuple->dst.ud.icmp.type = dstype;
	tuple->dst.ud.icmp.code = orig->dst.ud.icmp.code;

	return UX_NF_ACCEPT;
}

static int icmp_handle_packet(struct ux_conntrack *ct,
			      const void *skb,
			      enum ux_conntrack_stage_states ctinfo)
{
	return UX_NF_ACCEPT;
}

static int icmp_create(struct ux_conntrack *conntrack,
		       const void *skb)
{
	return UX_NF_ACCEPT;
}

static ux_ct_protocol_desc_t ux_ct_icmp_proto = {
	.protocol = IPPROTO_ICMP,
	.name = "icmp",
	.get_tuple_from_pkt = icmp_get_tuple_from_pkt,
	.swap_tuple = icmp_swap_tuple,
	.handle_packet = icmp_handle_packet,
	.create = icmp_create,
};

ux_ct_protocol_desc_t *ux_get_icmp_proto(void)
{
	return &ux_ct_icmp_proto;
}
