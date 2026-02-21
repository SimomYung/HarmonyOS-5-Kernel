/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Iptables ICMP filter rule impl code
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov  1 16:36:53 2021
 */
#include <stdbool.h>
#include <lib/dlist.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "ux_netdev.h"
#include "ux_pktbuf.h"
#include "ux_iptables.h"
#include "ux_netfilter.h"
#include "ux_ipt_icmp.h"

#define UX_IPT_ICMP_INV		0x01U
#define IHL_BYTES		4

static bool icmp_mt(const ux_pktbuf_t *skb,
		    const struct ux_netdev *in,
		    const struct ux_netdev *out,
		    const struct ux_ipt_match *match,
		    const void *matchinfo)
{
	UNUSED(in);
	UNUSED(out);
	UNUSED(match);
	struct ux_ipt_icmp *icmpinfo = (struct ux_ipt_icmp *)matchinfo;
	struct iphdr *ip;
	struct icmphdr *icmp;
	unsigned int iphdr_len;

	if (skb->len < sizeof(struct iphdr)) {
		return false;
	}

	ip = (struct iphdr *)skb->payload;
	iphdr_len = (unsigned int)(ip->ihl * IHL_BYTES);
	if (iphdr_len < sizeof(struct iphdr)) {
		return false;
	}

	if (ip->protocol == IPPROTO_ICMP) {
		if (skb->len >= iphdr_len + sizeof(struct icmphdr)) {
			icmp = (struct icmphdr *)((char *)ip + ip->ihl * IHL_BYTES);

			return ((unsigned int)((icmpinfo->type == 0xFF) ||
				 (icmp->type == icmpinfo->type && icmp->code >= icmpinfo->code[0] && icmp->code <= icmpinfo->code[1])) !=
				(unsigned int)((icmpinfo->invflags & UX_IPT_ICMP_INV) != 0));
		}
	}

	return false;
}

static int icmp_checkentry(const char *tablename,
			   const struct ux_ipt_ip *ip,
			   const void *matchinfo,
			   unsigned int matchinfosize)
{
	UNUSED(tablename);
	const struct ux_ipt_icmp *info = matchinfo;

	if ((ip->proto != IPPROTO_ICMP) || ((ip->invflags & UX_IPT_INV_PROTO) != 0)) {
		return E_HM_INVAL;
	}

	if (matchinfosize != ux_ipt_align(sizeof(struct ux_ipt_icmp))) {
		return E_HM_INVAL;
	}

	if ((info->invflags & ~UX_IPT_ICMP_INV) != 0) {
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static struct ux_ipt_match icmp_mt_reg[] = {
	{
		.name       = "icmp",
		.family     = AF_INET,
		.match      = icmp_mt,
		.matchsize  = sizeof(struct ux_ipt_icmp),
		.checkentry = icmp_checkentry,
		.list	    = {0},
		.revision   = 0,
	},
};

void init_icmp_mt(void)
{
	ux_add_ipt_match(&icmp_mt_reg[0]);
}
