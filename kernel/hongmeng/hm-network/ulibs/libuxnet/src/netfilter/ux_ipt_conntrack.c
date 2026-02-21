/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: CONNTACK filter rule impl file
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 15:12:25 2021
 */
#include <stdbool.h>
#include <lib/dlist.h>
#include <sys/socket.h>
#include "ux_pktbuf.h"
#include "ux_ipt_conntrack.h"
#include "ux_iptables.h"
#include "ux_netdev.h"
#include "ux_conntrack.h"

static bool conntrack_mt(const ux_pktbuf_t *skb,
			 const struct ux_netdev *in,
			 const struct ux_netdev *out,
			 const struct ux_ipt_match *match,
			 const void *matchinfo)
{
	struct ux_ipt_conntrack_mtinfo1 *info;
	struct ux_conntrack *ct;
	enum ux_conntrack_stage_states ctinfo;
	unsigned int statebit;

	BUG_ON(matchinfo == NULL);
	info = (struct ux_ipt_conntrack_mtinfo1 *)matchinfo;
	ct = ux_conntrack_get(skb, &ctinfo);
	if (ct != NULL) {
		statebit = ux_ipt_conntrack_state_bit(ctinfo);
	} else {
		statebit = UX_IPT_CONNTRACK_STATE_INVALID;
	}

	if (info->match_flags & UX_IPT_CONNTRACK_STATE) {
		if (!!(info->state_mask & statebit) ^
		    !(info->invert_flags & UX_IPT_CONNTRACK_STATE)) {
			return false;
		}
	}

	if (ct == NULL) {
		return info->match_flags & UX_IPT_CONNTRACK_STATE;
	}

	return true;
}

static int ct_checkentry(const char *tablename,
			 const struct ux_ipt_ip *ip,
			 const void *matchinfo,
			 unsigned int matchsize)
{
	UNUSED(tablename);
	UNUSED(ip);
	UNUSED(matchinfo);
	if (matchsize != ux_ipt_align(sizeof(struct ux_ipt_conntrack_mtinfo1))) {
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static struct ux_ipt_match conntrack_mt_reg = {
	.name       = "conntrack",
	.revision   = 1,
	.family     = AF_INET,
	.match      = conntrack_mt,
	.matchsize  = sizeof(struct ux_ipt_conntrack_mtinfo1),
	.checkentry = ct_checkentry,
};

int init_conntrack_mt(void)
{
	ux_add_ipt_match(&conntrack_mt_reg);

	return 0;
}
