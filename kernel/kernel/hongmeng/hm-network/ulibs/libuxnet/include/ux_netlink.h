/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description:  Header file for netlink genl proto
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 18 13:34:09 2020
 */

#ifndef __LIBUXNET_UX_NETLINK_
#define __LIBUXNET_UX_NETLINK_

#include <stddef.h>
#include <hmkernel/capability.h>

#include "hm_genetlink.h"
#include "../src/netlink/ux_netlink_generic.h"
#include "../src/netlink/ux_netlink_proto.h"
#include "../src/netlink/ux_netlink_core.h"
#include "../src/netlink/ux_netlink_table.h"

#if defined(CONFIG_UXNET_NETLINK)
ux_netlink_generic_t *devhost_register_generic(const ux_reg_fam_info_t *info, uint32_t len, uref_t dh_ap_uref);
int devhost_unregister_generic(uint16_t familyid);
int netlink_genl_unicast(unsigned int dst_port, const char *msg, const size_t len);
int netlink_genl_broadcast(unsigned int dst_group, const char *msg, const size_t len);
#else
static inline ux_netlink_generic_t *devhost_register_generic(
		const ux_reg_fam_info_t *info, uint32_t len, uref_t dh_ap_uref)
{
	UNUSED(info, len, dh_ap_uref);
	return NULL;
}
static inline int devhost_unregister_generic(uint16_t familyid)
{
	UNUSED(familyid);
	return E_HM_OK;
}
static inline int netlink_genl_unicast(unsigned int dst_port, const char *msg, const size_t len)
{
	UNUSED(dst_port, msg, len);
	return E_HM_OK;
}
static inline int netlink_genl_broadcast(unsigned int dst_group, const char *msg, const size_t len)
{
	UNUSED(dst_group, msg, len);
	return E_HM_OK;
}
#endif

#endif
