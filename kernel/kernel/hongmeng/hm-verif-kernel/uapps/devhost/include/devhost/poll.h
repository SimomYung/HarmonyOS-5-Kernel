/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Prototypes for devhost poll APIs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 21 15:33:53 2023
 */

/*
 * CAUSION: This file will be used in linux source,
 *          **DO NOT** include any HMOS headers.
 */

#ifndef __DEVHOST_API_POLL_H__
#define __DEVHOST_API_POLL_H__

#if defined(MODULE)
#include <linux/types.h>
#elif defined(UDK_MODULE) || defined(UDK_FRAMEWORK)
#include <stddef.h>
#else
#include <stddef.h>
#endif

/* This object is used to clear poll-related resources of the devhost backend */
struct devhost_poll_wqueues_info {
	void *pwq;      /* the poll wait queue of device */
	void (*release)(void *pwq); /* release func of pwq */
};

static inline
void devhost_init_pwq_info(struct devhost_poll_wqueues_info *info,
			   void *pwq, void (*release)(void *))
{
	info->pwq = pwq;
	info->release = release;
}

static inline
void devhost_release_pwq(struct devhost_poll_wqueues_info *info)
{
	if (info->release != NULL) {
		info->release(info->pwq);
	}
}

/* Transfer to devhost backend */
struct devhost_poll_info {
	struct devhost_poll_wqueues_info *pwq_info;
	void *target;  /* wakeup target */
	int (*wakeup)(void *target, unsigned int revents);
};

struct filp_node;
void devhost_poll_close(struct filp_node *fnode);
void devhost_epoll_close2(struct filp_node *fnode);
void devhost_reject_fdtable_rref(unsigned int cnode_idx);

void devhost_sync_revents_to_hmkobj_trad(struct filp_node *fnode, unsigned int revents);
struct hmkobj_ctx;
void devhost_sync_revents_to_hmkobj(struct hmkobj_ctx *kobj_ctx, unsigned int revents);
void *devhost_sync_open(struct hmkobj_ctx *kobj_ctx);
void devhost_sync_close(struct hmkobj_ctx *kobj_ctx);
#endif /* __DEVHOST_API_POLL_H__ */
