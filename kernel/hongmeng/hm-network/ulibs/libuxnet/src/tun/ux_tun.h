/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for tuntap socket
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 12 09:37:57 2021
 */

#ifndef __UX_TUN_H
#define __UX_TUN_H

#include "ux_utils.h"
#include "ux_socket.h"
#include "ux_netdev.h"
#include "ux_buf_queue.h"

typedef struct ux_tun_sock ux_tun_sock_t;

typedef struct ux_tun_dev {
	ux_tun_sock_t *tsk;
	bool attached;
	struct dlist_node head;
	ux_netdev_t *dev;
} ux_tun_dev_t;

struct ux_tun_sock {
	ux_sock_t sk;

	int owner;
	char *name;
	bool attached;
	void *epoll_info;
	unsigned long flags;

	struct raw_sem recvsem;
	ux_buf_head_t read_queue;

	ux_tun_dev_t *tdev;
};

static inline void *tdev_from_dev(const ux_netdev_t *dev)
{
	return (char *)dev + sizeof(ux_netdev_t) + UX_NETDEV_PRIV_DEFAULT;
}

static inline ux_tun_sock_t *tsk_from_sk(const ux_sock_t *sk)
{
	return (ux_tun_sock_t *)(void *)sk;
}

static inline bool is_tun_noblock(const ux_sock_t *sk)
{
	if ((sk->noblock & O_NONBLOCK) == 0U) {
		return false;
	}

	return true;
}

int af_tun_init(void);
void af_tun_uninit(void);

#endif
