/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for network vsock socket
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 24 19:35:43 2021
 */

#ifndef __UX_VSOCK_SOCKET_H_
#define __UX_VSOCK_SOCKET_H_

#include "ux_vsock_core.h"

int af_vsock_init(void);

ux_vsock_sock_t *vsock_alloc_and_init(ux_socket_t *sock, int domain, int type, int protocol);
void install_vsock_to_socket(ux_socket_t *sock, ux_vsock_sock_t *vsock);

ux_vsock_buf_t *alloc_vsock_buf(const char *buf, size_t len);
void free_vsock_buf(ux_vsock_buf_t *buf);

/**
 * when the vm is killed or exit abnormally and can not response us anymore,
 * the blocked uapp should be notified about this event.
 */
void vsock_handle_cid_revoke(unsigned int cid);

#endif
