/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: REJECT iptable target head file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov  1 16:21:31 2021
 */
#ifndef _UX_IPT_REJECT_H_
#define _UX_IPT_REJECT_H_

enum ux_ipt_reject_with {
	UX_IPT_ICMP_NET_UNREACHABLE,
	UX_IPT_ICMP_HOST_UNREACHABLE,
	UX_IPT_ICMP_PROT_UNREACHABLE,
	UX_IPT_ICMP_PORT_UNREACHABLE,
	UX_IPT_ICMP_ECHOREPLY,
	UX_IPT_ICMP_NET_PROHIBITED,
	UX_IPT_ICMP_HOST_PROHIBITED,
	UX_IPT_TCP_RESET,
	UX_IPT_ICMP_ADMIN_PROHIBITED,
};

struct ux_ipt_reject_info {
	enum ux_ipt_reject_with with;
};

int init_reject_tg(void);

#endif
