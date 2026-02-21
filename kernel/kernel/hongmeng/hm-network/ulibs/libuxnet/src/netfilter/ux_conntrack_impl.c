/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: filters implement for various protocols
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 29 15:39:58 2021
 */

#include <sys/socket.h>
#include <lib/dlist.h>
#include <libhmlog/hmlog.h>

#include "ux_socket.h"
#include "ux_pbuf.h"
#include "ux_conntrack.h"
#include "ux_netfilter.h"

#define PKT_IHDR_SIZE 4

static int ux_ct_prect(unsigned int hooknum,
		       struct ux_pktbuf *pkt,
		       struct ux_netdev *in,
		       struct ux_netdev *out)
{
	unsigned int len;
	ux_pbuf_header_t *pbuf_hdr;

	pbuf_hdr = (ux_pbuf_header_t *)(pkt->payload);
	len = (unsigned int)(pbuf_hdr->iphdr.ihl * PKT_IHDR_SIZE);
	/* Raw sockets. */
	if (len < sizeof(struct iphdr)) {
		net_warn(HMNET_NETFILTER, "Pre conntrack: raw sockets\n");
		return UX_NF_ACCEPT;
	}
	/* Need to reassemble packet in this case */
	if (pbuf_hdr->iphdr.frag_off & htons(IP_OFFMASK | IP_MF)) {
		net_warn(HMNET_NETFILTER, "Packet is frag, refuse to hook connection track\n");
		return UX_NF_DROP;
	}
	return ux_conntrack_in(hooknum, pkt, in, out);
}

static int ux_ct_complete(unsigned int hooknum,
			  ux_pktbuf_t *pctrkb,
			  struct ux_netdev *in,
			  struct ux_netdev *out)
{
	/* Check finally */
	return ux_ct_confirm(pctrkb);
}

/* Connection tracking just only track and record packets */
static struct ux_nf_hook_ops ux_ct_prect_pre_ops = {
	.hook = ux_ct_prect,
	.pf	= PF_INET,
	.hooknum = UX_NF_PRE_ROUTING,
	.priority = UX_CT_HI_PRIO,
};

static struct ux_nf_hook_ops ux_ct_complete_in_ops = {
	.hook = ux_ct_complete,
	.pf = PF_INET,
	.hooknum = UX_NF_LOCAL_IN,
	.priority = UX_CT_LW_PRIO,
};

static struct ux_nf_hook_ops ux_ct_prect_out_ops = {
	.hook = ux_ct_prect,
	.pf = PF_INET,
	.hooknum = UX_NF_LOCAL_OUT,
	.priority = UX_CT_HI_PRIO,
};

static struct ux_nf_hook_ops ux_ct_complete_post_ops = {
	.hook = ux_ct_complete,
	.pf	= PF_INET,
	.hooknum = UX_NF_POST_ROUTING,
	.priority = UX_CT_LW_PRIO,
};

int ux_nf_conntrack_init(void)
{
	int ret;

	ret = ux_conntrack_init();
	if (ret < 0) {
		net_error(HMNET_NETFILTER, "Conntrack base init fail\n");
		return -1;
	}

	(void)ux_nf_register_hook(&ux_ct_prect_pre_ops);
	(void)ux_nf_register_hook(&ux_ct_complete_in_ops);
	(void)ux_nf_register_hook(&ux_ct_prect_out_ops);
	(void)ux_nf_register_hook(&ux_ct_complete_post_ops);

	return ret;
}

void ux_nf_conntrack_exit(void)
{
	ux_nf_unregister_hook(&ux_ct_complete_post_ops);
	ux_nf_unregister_hook(&ux_ct_prect_out_ops);
	ux_nf_unregister_hook(&ux_ct_complete_in_ops);
	ux_nf_unregister_hook(&ux_ct_prect_pre_ops);
	ux_conntrack_exit();
}
