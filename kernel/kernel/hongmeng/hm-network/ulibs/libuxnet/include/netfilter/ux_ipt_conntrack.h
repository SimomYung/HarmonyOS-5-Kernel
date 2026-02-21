/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: CONNTACK filter rule head file
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 15:12:25 2021
 */
#ifndef _UX_IPT_CONNTRACK_H_
#define _UX_IPT_CONNTRACK_H_
#include <netinet/in.h>
#include "ux_conntrack.h"

#define ux_ipt_conntrack_state_bit(ctinfo) (1 << ((ctinfo) % UX_CT_IS_REPLY + 1))
#define UX_IPT_CONNTRACK_STATE_INVALID (1 << 0)
#define ADDR_LEN 4

/* flags, invflags: */
enum {
	UX_IPT_CONNTRACK_STATE = 1 << 0,
};

union nf_inet_addr {
	unsigned int	all[ADDR_LEN];
	unsigned int	ip;
	unsigned int	ip6[ADDR_LEN];
	struct in_addr	in;
	struct in6_addr	in6;
};

struct ux_ipt_conntrack_mtinfo1 {
	union nf_inet_addr origsrc_addr, origsrc_mask;
	union nf_inet_addr origdst_addr, origdst_mask;
	union nf_inet_addr replsrc_addr, replsrc_mask;
	union nf_inet_addr repldst_addr, repldst_mask;
	unsigned int expires_min, expires_max;
	unsigned short l4proto;
	unsigned short origsrc_port, origdst_port;
	unsigned short replsrc_port, repldst_port;
	unsigned short match_flags, invert_flags;
	unsigned char state_mask, status_mask;
};

int init_conntrack_mt(void);
#endif
