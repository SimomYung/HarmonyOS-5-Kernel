/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description:  Header file for netlink audit
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 18 14:10:16 2020
 */

#ifndef __UX_NETLINK_AUDIT_H_
#define __UX_NETLINK_AUDIT_H_
#include "ux_netdev.h"
#include "hm_netlink.h"

#ifdef CONFIG_AUDIT
int ux_netlink_audit_init(void);
void ux_netlink_audit_exit(void);
int netlink_audit_unicast(unsigned int dst_port, const char *msg, const size_t len);
#else
static inline int ux_netlink_audit_init(void)
{
	return 0;
}

static inline void ux_netlink_audit_exit(void) {}

static inline int netlink_audit_unicast(unsigned int dst_port, const char *msg, const size_t len)
{
	UNUSED(dst_port, msg, len);
	return E_HM_OK;
}
#endif

#endif
