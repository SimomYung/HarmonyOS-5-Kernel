/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: filters for default protocols
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 16:32:17 2021
 */
#include <lib/dlist.h>
#include <libhmlog/hmlog.h>

#include "ux_socket.h"
#include "ux_pbuf.h"
#include "ux_conntrack.h"
#include "ux_netfilter.h"

static int generic_get_tuple_from_pkt(const void *skb,
				unsigned int dataoff,
				struct ux_conntrack_tuple *tuple)
{
	tuple->src.port = 0;
	tuple->dst.ud.all = 0;

	return UX_NF_ACCEPT;
}

static int generic_swap_tuple(struct ux_conntrack_tuple *tuple,
				const struct ux_conntrack_tuple *orig)
{
	tuple->src.port = 0;
	tuple->dst.ud.all = 0;

	return UX_NF_ACCEPT;
}

/* Handle this specific packet. */
static int generic_handle_packet(struct ux_conntrack *conntrack,
		  const void *skb,
		  enum ux_conntrack_stage_states ctinfo)
{
	return UX_NF_ACCEPT;
}

static int generic_create(struct ux_conntrack *conntrack, const void *skb)
{
	return UX_NF_ACCEPT;
}

static ux_ct_protocol_desc_t ux_ct_default_proto = {
	.protocol			= 0,
	.name			= "unknown",
	.get_tuple_from_pkt		= generic_get_tuple_from_pkt,
	.swap_tuple		= generic_swap_tuple,
	.handle_packet			= generic_handle_packet,
	.create			= generic_create,
};

ux_ct_protocol_desc_t *ux_get_default_proto(void)
{
	return &ux_ct_default_proto;
}
