/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Stub interface for netlink
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 11 10:15:31 2022
 */

#include "hmnet/hm_netlink.h"
#include "hmnet/hm_genetlink.h"

#include <errno.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_select_common.h>

int hm_netlink_broadcast(int protocol, unsigned int src_portid,
			 unsigned int dst_group, const char *msg,
			 const size_t len)
{
	UNUSED(protocol, src_portid, dst_group, msg, len);
	return E_HM_NOSYS;
}

int hm_netlink_unicast(int protocol, unsigned int src_portid,
			 unsigned int dst_port, const char *msg,
			 const size_t len)
{
	UNUSED(protocol, src_portid, dst_port, msg, len);
	return E_HM_NOSYS;
}

int hm_netlink_register_generic(const void *info, const unsigned int len,
				unsigned long long dh_ap_cref,
				uint32_t *mcgrps_offset,
				uint16_t *familyid)
{
	UNUSED(info, len, dh_ap_cref, mcgrps_offset, familyid);
	return E_HM_NOSYS;
}

int hm_netlink_unregister_generic(uint16_t familyid)
{
	UNUSED(familyid);
	return E_HM_NOSYS;
}
