/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description:  Header file for network can raw protocol
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 09:49:09 2019
 */

#ifndef __UX_CAN_RAW_H_
#define __UX_CAN_RAW_H_

#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_sem.h>

#include "ux_socket.h"
#include "ux_buf_queue.h"

typedef struct ux_can_raw_sock {
	ux_sock_t sk;
	int can_ifindex;
	int can_bind;
	int can_loop;
	int can_fdcap;
	struct can_filter *filter;
	struct dlist_node raw_sock_list_node;
	ux_buf_head_t recv_queue;
	struct raw_sem recvsem;
} ux_can_raw_sock_t;

static inline ux_can_raw_sock_t *can_raw_sock(ux_socket_t *s)
{
	ux_sock_t *sk = ux_socket_get_sock(s);

	if (sk != NULL) {
		return container_of(sk, ux_can_raw_sock_t, sk);
	} else {
		return NULL;
	}
}

int ux_can_raw_socket_init(void);
void ux_can_raw_socket_exit(void);

#endif
