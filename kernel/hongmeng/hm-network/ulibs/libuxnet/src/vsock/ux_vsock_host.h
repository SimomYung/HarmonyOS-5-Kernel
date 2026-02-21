/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: interface for network vsock socket
 * Author: Huawei OS Kernel Lab
 * Create: Mon June 17 19:35:43 2024
 */

#ifndef __UX_VSOCK_HOST_H_
#define __UX_VSOCK_HOST_H_

#include "ux_vsock_core.h"
#include "vsock/ux_vsock_netdev.h"

#define DEEPCOPY_INTO_VM_MSG_OK 0

typedef struct ux_host_vsock_sock {
	ux_sock_t sk;

	unsigned int cid;
	ux_netdev_t *host_netdev;
} ux_host_vsock_sock_t;

static inline ux_host_vsock_sock_t *host_vsock_sk(ux_sock_t *sk)
{
	return (ux_host_vsock_sock_t *)(void *)sk;
}

static inline ux_host_vsock_sock_t *host_vsock_from_socket(ux_socket_t *socket)
{
	return (ux_host_vsock_sock_t *)(void *)ux_socket_get_sock(socket);
}

static inline void lock_host_vsock(ux_host_vsock_sock_t *vsock)
{
	raw_mutex_lock(&vsock->sk.lock);
}

static inline void unlock_host_vsock(ux_host_vsock_sock_t *vsock)
{
	raw_mutex_unlock(&vsock->sk.lock);
}

int ux_host_vsock_create(ux_socket_t *socket, int domain, int type, int protocol, int flags);
#endif
