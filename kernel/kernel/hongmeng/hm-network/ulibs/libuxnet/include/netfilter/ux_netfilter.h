/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: filters for various protocols
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 22 16:32:17 2021
 */

#ifndef __HM_NET_UX_NETFILTER_H
#define __HM_NET_UX_NETFILTER_H

#include <lib/dlist.h>
#include "ux_netdev.h"

/* Max packet bytes needed, mainly header */
#define NF_MAX_PKTBUF 128

/* IP Hooks */
/* Pre routing---->Routing----->Arrive */
#define UX_NF_PRE_ROUTING	0
/* The packet incoming this */
#define UX_NF_LOCAL_IN		1
/* This is not target, need forward */
#define UX_NF_FORWARD		2
/* The packet outgoing from this */
#define UX_NF_LOCAL_OUT		3
/* After routing, leave out netfiler */
#define UX_NF_POST_ROUTING	4
#define UX_NF_NUMHOOKS		5

/*
 * For now, only support PF_INET, so max is 3
 * since PF_INET is defined 2.
 */
#define NPROTO			3

/* Return results from hook */
#define UX_NF_DROP			0
#define UX_NF_ACCEPT			1
#define UX_NF_REPEAT			2
#define UX_NF_ICMP_NET_REJ		3
#define UX_NF_ICMP_HOST_REJ		4
#define UX_NF_ICMP_PROT_REJ		5
#define UX_NF_ICMP_PORT_REJ		6
#define UX_NF_ICMP_NET_PRO_REJ		7
#define UX_NF_ICMP_HOST_PRO_REJ		8
#define UX_NF_ICMP_ADMIN_PRO_REJ	9

/* Max hook nums points */
#define UX_NF_MAX_HOOKS		6

/* forward declare */
struct netif;
struct ux_sock;
struct ux_pktbuf;
struct ux_netdev;

typedef int ux_nf_hookfn(unsigned int hook, struct ux_pktbuf *pkt,
			 struct ux_netdev *indev,
			 struct ux_netdev *outdev);

typedef struct ux_nf_hook_ops {
	struct dlist_node list;

	/* Implementation for hook. */
	ux_nf_hookfn *hook;
	/* Protocol family */
	int pf;
	/* The poit in processing packets */
	int hooknum;
	/* The priority of hook impl */
	int priority;
} ux_nf_hook_ops_t;

int ux_nf_register_hook(struct ux_nf_hook_ops *reg);
void ux_nf_unregister_hook(struct ux_nf_hook_ops *reg);

void ux_set_nf_pre_ct_hook(int (*hook)(const struct ux_pktbuf *pkt));
void ux_set_nf_post_ct_hook(void (*hook)(struct ux_conntrack *res));

#ifdef CONFIG_UXNET_NETFILTER
int ux_nf_hook(int protocol, unsigned int hook, ux_pktbuf_t *pktbuf,
	       ux_netdev_t *indev, ux_netdev_t *outdev);
#else
static inline int ux_nf_hook(int protocol, unsigned int hook, ux_pktbuf_t *pktbuf,
			     ux_netdev_t *indev, ux_netdev_t *outdev)
{
	UNUSED(protocol);
	UNUSED(hook);
	UNUSED(pktbuf);
	UNUSED(indev);
	UNUSED(outdev);

	return 0;
}
#endif

int ux_netfilter_init(void);
#endif
