/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Netlink uevent protocol header
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 24 18:01:07 2020
 */
#ifndef __UX_NETLINK_UEVENT_H_
#define __UX_NETLINK_UEVENT_H_

#include <lib/dlist.h>

#include "hm_netlink.h"
#include "ux_socket.h"

#if defined(CONFIG_UXNET_NETLINK)
int ux_netlink_uevent_init(void);
void ux_netlink_uevent_exit(void);

int ux_uevent_receive(unsigned int dst_group, const char *msg, const size_t len);
#else
static inline int ux_netlink_uevent_init(void)
{
	return E_HM_OK;
}

static inline void ux_netlink_uevent_exit(void) {}

static inline int ux_uevent_receive(unsigned int dst_group, const char *msg, const size_t len)
{
	return E_HM_OK;
}
#endif

#endif
