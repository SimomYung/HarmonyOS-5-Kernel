/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header file for ux vhost
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 23 15:16:13 2021
 */

#ifndef __UX_VHOST_H__
#define __UX_VHOST_H__

#include <libvhost_user/vhost_server.h>

int ux_vhost_init(void);
void ux_vhost_release(void);

int ux_vhost_recv_msg(uint32_t caller_cnode, struct vhost_user_msg_s *req,
		      struct vhost_user_msg_s *rsp);
int ux_vhost_register_backend(struct vhost_backend_s *backend);

#ifdef CONFIG_VIRTIO_VSOCK
int ux_vhost_vsock_init(void);
#else
static inline int ux_vhost_vsock_init(void)
{
	return E_HM_OK;
}
#endif

#ifdef CONFIG_VHOST_VNET
int ux_vhost_vnet_init(void);
#else
static inline int ux_vhost_vnet_init(void)
{
	return E_HM_OK;
}
#endif

#endif
