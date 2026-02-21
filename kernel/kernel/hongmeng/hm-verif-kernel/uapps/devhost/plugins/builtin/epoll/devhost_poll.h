/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Prototypes for device poll support
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 21 16:03:29 2023
 */
#ifndef __DEVHOST_POLL_H__
#define __DEVHOST_POLL_H__

#include <lib/dlist.h>
#include <devhost/poll.h>
#include <libhmsrv_sys/hm_select_common.h>

struct filp_node;

struct devhost_poll_target_info {
	struct ppoll_info_args poll_args;
	unsigned int cnode_idx;
	struct filp_node *fnode;
};

struct devhost_poll_node {
	struct dlist_node node;
	struct devhost_poll_wqueues_info pwq_info;
	struct devhost_poll_target_info target;
};

int devhost_poll_add(struct filp_node *fnode,
		     struct devhost_poll_target_info *tinfo,
		     int need_add,
		     unsigned int *revents);

void devhost_poll_remove(struct filp_node *fnode,
			 unsigned long long tcb_key);

int devhost_epoll_ctl(struct filp_node *fnode, struct epoll_info_arg *arg);
void devhost_epoll_remove(struct filp_node *fnode, cref_t ep_cref);
int devhost_epoll_recheck(struct filp_node *fnode, unsigned int *revents);

#endif /* __DEVHOST_POLL_H__ */
