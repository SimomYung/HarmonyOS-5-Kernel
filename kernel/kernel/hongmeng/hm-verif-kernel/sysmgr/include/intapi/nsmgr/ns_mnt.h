/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: mount namespace header file
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 30 14:12:47 2020
 */

#ifndef SYSMGR_INCLUDE_NSMGR_NS_MNT_H
#define SYSMGR_INCLUDE_NSMGR_NS_MNT_H

#include <lib/utils.h>
#include <intapi/thread/thread.h>
#include <intapi/nsmgr/ns_common.h>

struct ns_group;
struct mnt_ns {
	struct ns_common ns_common;
};

static inline int
mnt_ns_get(struct mnt_ns *ns)
{
	return ns_common_get(&ns->ns_common);
}

static inline void
mnt_ns_put(struct mnt_ns *ns)
{
	return ns_common_put(&ns->ns_common);
}

static inline struct mnt_ns *
common_to_mnt_ns(struct ns_common *ns)
{
	return container_of(ns, struct mnt_ns, ns_common);
}

int copy_mnt_ns(struct thread_s *new,
			   struct mnt_ns *ns_old,
			   unsigned int flags, struct mnt_ns **ns_new);
struct mnt_ns *create_mnt_ns(void);

#endif
