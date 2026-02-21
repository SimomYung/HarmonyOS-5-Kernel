/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Audit iptable target impl file
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 17 10:01:02 2021
 */
#include <stdbool.h>
#include <lib/dlist.h>
#include <sys/socket.h>
#include <hongmeng/errno.h>
#include "ux_netfilter.h"
#include "ux_pktbuf.h"
#include "ux_iptables.h"
#include "ux_ipt_reject.h"
#include "ux_netdev.h"
#include "ux_pbuf.h"
#include "ux_netlog.h"
#include <lwip/icmp.h>

static unsigned int reject_tg(const ux_pktbuf_t *skb,
			      const struct ux_netdev *in,
			      const struct ux_netdev *out,
			      const struct ux_ipt_target *match,
			      const void *targetinfo)
{
	UNUSED(skb);
	UNUSED(in);
	UNUSED(out);
	UNUSED(match);
	unsigned int ret = UX_NF_DROP;
	const struct ux_ipt_reject_info *rej = targetinfo;

	switch(rej->with) {
	case UX_IPT_ICMP_NET_UNREACHABLE:
		ret = UX_NF_ICMP_NET_REJ;
		break;
	case UX_IPT_ICMP_HOST_UNREACHABLE:
		ret = UX_NF_ICMP_HOST_REJ;
		break;
	case UX_IPT_ICMP_PROT_UNREACHABLE:
		ret = UX_NF_ICMP_PROT_REJ;
		break;
	case UX_IPT_ICMP_PORT_UNREACHABLE:
		ret = UX_NF_ICMP_PORT_REJ;
		break;
	case UX_IPT_ICMP_NET_PROHIBITED:
		ret = UX_NF_ICMP_NET_PRO_REJ;
		break;
	case UX_IPT_ICMP_HOST_PROHIBITED:
		ret = UX_NF_ICMP_HOST_PRO_REJ;
		break;
	case UX_IPT_ICMP_ADMIN_PROHIBITED:
		ret = UX_NF_ICMP_ADMIN_PRO_REJ;
		break;
	case UX_IPT_TCP_RESET:
	case UX_IPT_ICMP_ECHOREPLY:
	default:
		ret = UX_NF_DROP;
		break;
	}

	return ret;
}

static int reject_tg_check(const char *tablename,
			   const struct ux_ipt_ip *ip,
			   const void *targetinfo,
			   unsigned int target_size)
{
	UNUSED(tablename);
	UNUSED(ip);
	int ret;
	const struct ux_ipt_reject_info *rej = targetinfo;

	if (target_size != ux_ipt_align(sizeof(struct ux_ipt_reject_info))) {
		net_debug(HMNET_CORE, "reject_tg_check: target size is invalid\n");
		ret = E_HM_INVAL;
	} else if (rej->with == UX_IPT_ICMP_NET_UNREACHABLE ||
		   rej->with == UX_IPT_ICMP_HOST_UNREACHABLE ||
		   rej->with == UX_IPT_ICMP_PROT_UNREACHABLE ||
		   rej->with == UX_IPT_ICMP_PORT_UNREACHABLE ||
		   rej->with == UX_IPT_ICMP_NET_PROHIBITED ||
		   rej->with == UX_IPT_ICMP_HOST_PROHIBITED ||
		   rej->with == UX_IPT_ICMP_ADMIN_PROHIBITED) {
		ret = E_HM_OK;
	} else {
		net_debug(HMNET_CORE, "reject_tg_check: rej->with: %d is unspported\n", (int)rej->with);
		ret = E_HM_OPNOTSUPP;
	}

	return ret;
}

static struct ux_ipt_target reject_tg_reg = {
	.name        = "REJECT",
	.revision    = 0,
	.family      = AF_INET,
	.target      = reject_tg,
	.checkentry  = reject_tg_check,
	.targetsize  = sizeof(struct ux_ipt_reject_info),
	.list	     = {0},
};

int init_reject_tg(void)
{
	ux_ipt_register_target(&reject_tg_reg);

	return 0;
}
