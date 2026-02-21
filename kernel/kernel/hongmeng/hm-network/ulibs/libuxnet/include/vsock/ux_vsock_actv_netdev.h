/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for vsock actv netdev interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun  2 14:21:46 2021
 */

#ifndef __UX_VSOCK_ACTV_NETDEV_H__
#define __UX_VSOCK_ACTV_NETDEV_H__

#include <stddef.h>
#include <stdint.h>
#include <sys/uio.h>

/**
 * vsock netdev registered by hongmeng actv-call, and the other operations was
 * also implemented by actv-call e.g xmit and recv vsock packages.
 */

int vsock_register_actv_netdev(unsigned int cid, unsigned int dev_group_id,
			       uint32_t uvmm_cnode_idx);
void vsock_unregister_actv_netdev(uint32_t uvmm_cnode_idx, unsigned int cid);

/**
 * vm transmit data, hmnetd is the receiver.
 */
int vsock_actv_netdev_tx(uint32_t uvmm_cnode_idx, unsigned int cid,
			 const struct iovec *caller_iov, size_t nr_iov);
int vsock_actv_netdev_vq_ready(uint32_t uvmm_cnode_idx, unsigned int cid);
void vsock_actv_netdev_revoke(uint32_t cnode_idx);
#endif
