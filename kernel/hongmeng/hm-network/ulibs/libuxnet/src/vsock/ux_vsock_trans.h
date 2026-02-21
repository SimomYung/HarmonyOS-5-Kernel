/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for network vsock transmission
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 24 19:35:43 2021
 */

#ifndef __UX_VSOCK_TRANS_H_
#define __UX_VSOCK_TRANS_H_

#include <hm_vsock.h>

#include "ux_vsock_core.h"

void ux_trans_alloc(ux_vsock_sock_t *vsock);

/**
 * The below operation do not related with data sent, only sent a virtio vsock header
 * @return E_HM_OK means request success, other values means failed
 */
int ux_trans_request(const ux_vsock_sock_t *vsock, const struct sockaddr_vm *dst_addr);
int ux_trans_response(const ux_vsock_sock_t *vsock, const struct virtio_vsock_hdr *req);
int ux_trans_reset(const ux_vsock_sock_t *vsock, const struct virtio_vsock_hdr *hdr);
int ux_trans_shutdown(const ux_vsock_sock_t *vsock, unsigned int mode);
int ux_trans_update_credit(const ux_vsock_sock_t *vsock);

/**
 * write will send real data
 * @return the real length sent
 */
ssize_t ux_trans_write(const ux_vsock_sock_t *vsock, const char *buf, size_t len);

bool ux_trans_has_space(ux_vsock_sock_t *vsock);
uint32_t ux_trans_space_left(ux_vsock_sock_t *vsock);

uint32_t ux_trans_get_buffer_size(const ux_vsock_sock_t *vsock);
void ux_trans_set_buffer_size(ux_vsock_sock_t *vsock, uint32_t buffer_size);

void update_trans_info(ux_vsock_sock_t *vsock, const struct virtio_vsock_hdr *hdr);
int ux_trans_recv_msg(struct vm_msg *msg);
#endif
