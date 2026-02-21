/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 29 21:25:08 CST 2023
 */

#ifndef __DEVHOST_NET_NAMESPACE_H__
#define __DEVHOST_NET_NAMESPACE_H__

#include <linux/net.h>

#define MAX_NAMESPACE_NR 8192
#define ROOT_NS_ID 8189

#ifdef CONFIG_NET_NS
void devhost_net_namespace_init(void);
int devhost_get_netns(struct net **netns);
#else
static inline void devhost_net_namespace_init(void) { return; }
static inline int devhost_get_netns(struct net **netns)
{
	*netns = &init_net;
	return 0;
}
#endif

#endif /* __DEVHOST_NET_SOCKET_H__ */
