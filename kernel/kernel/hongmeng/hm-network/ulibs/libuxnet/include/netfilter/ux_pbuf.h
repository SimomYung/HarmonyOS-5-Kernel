/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: base packet buffer header info
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 16:32:17 2021
 */

#ifndef __HM_NET_UX_PBUF_H
#define __HM_NET_UX_PBUF_H

#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

typedef struct ux_pbuf_header {
	struct iphdr iphdr;
	union {
		struct udphdr udphdr;
		struct tcphdr tcphdr;
		struct icmphdr icmphdr;
	} th; /* transport layer */
} ux_pbuf_header_t;

#endif
