/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: net namespace header file
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 25 15:55:04 CST 2023
 */

#ifndef SYSMGR_INCLUDE_NSMGR_NS_NET_H
#define SYSMGR_INCLUDE_NSMGR_NS_NET_H

#include <lib/utils.h>
#include <intapi/thread/thread.h>
#include <intapi/nsmgr/ns_common.h>

#ifndef CLONE_NEWNET
#define CLONE_NEWNET	(0x40000000)
#endif

struct ns_group;
struct net_ns {
	struct ns_common ns_common;
};

static inline int
net_ns_get(struct net_ns *ns)
{
	return ns_common_get(&ns->ns_common);
}

static inline void
net_ns_put(struct net_ns *ns)
{
	return ns_common_put(&ns->ns_common);
}

static inline struct net_ns *
common_to_net_ns(struct ns_common *ns)
{
	return container_of(ns, struct net_ns, ns_common);
}

struct ns_operation *fetch_net_ns_op(void);

#ifdef CONFIG_NET_NS
int copy_net_ns(struct thread_s *new, struct net_ns *ns_old,
				unsigned int flags, struct net_ns **ns_new);
#else
static inline int copy_net_ns(struct thread_s *new, struct net_ns *ns_old,
							  unsigned int flags, struct net_ns **ns_new)
{
	UNUSED(new);
	int err = E_HM_OK;

	if ((flags & (unsigned int)CLONE_NEWNET) != 0U) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		*ns_new = ns_old;
		int ref = net_ns_get(ns_old);
		BUG_ON(ref <= 0);
	}

	return err;
}
#endif

#endif
