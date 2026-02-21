/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Implementation of netlink
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 27 09:04:10 2020
 */

#include <libhmlog/hmlog.h>
#include <hongmeng/types.h>

#include "hm_netlink.h"
#include "hm_genetlink.h"
#include "ss_channel.h"

int hm_netlink_register_generic(const void *info, const unsigned int len,
				unsigned long long dh_ap_cref,
				uint32_t *mcgrps_offset,
				uint16_t *familyid)
{
	struct __actvret_netcall_netlink_register_generic netcall_ret;
	struct __actvpool_grant grant;
	uref_t dh_ap_uref;
	rref_t rref;
	uint32_t cnode_idx;
	int ret;

	if (info == NULL || mcgrps_offset == NULL || familyid == NULL) {
		return E_HM_INVAL;
	}

	ret = hm_net_ap_rref_and_cnode(&rref, &cnode_idx);
	if (ret < 0) {
		return ret;
	}

	/* export ap to devmgr */
	grant.credential = 0ull;
	grant.pre_alloc_actv_cref = 0ull;
	dh_ap_uref = hm_ucap_export(0, cnode_idx,
				    /* flags= */
				    CNODE_GRANT_PREPARE_FLAG_QUERY_SRC_CNODE_IDX |
				    CNODE_GRANT_PREPARE_FLAG_QUERY_USLOT_IDX |
				    CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_UREF,
				    dh_ap_cref,
				    /* rights= */
				    CRIGHT_ACTVPOOL_CALL,
				    /* args= */
				    ptr_to_ulong(&grant));
	if (IS_REF_ERR(dh_ap_uref)) {
		ret = REF_TO_ERR(dh_ap_uref);
		goto out;
	}

	mem_zero_s(netcall_ret);
	ret = actvcapcall_netcall_netlink_register_generic(rref,
							   dh_ap_uref,
							   (void *)info,
							   (unsigned int)len,
							   &netcall_ret);
	if (ret != 0) {
		(void)hm_ucap_revoke(0, dh_ap_uref);
		goto out;
	}
	*mcgrps_offset = netcall_ret.mcgrps_offset;
	*familyid = netcall_ret.familyid;
	ret = 0;

out:
	hm_net_put_rref(&rref);
	return ret;
}

int hm_netlink_unregister_generic(uint16_t familyid)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret =  actvcapcall_netcall_netlink_unregister_generic(rref,
									familyid);

	hm_net_put_rref(&rref);
	return ret;
}

int hm_netlink_create_service(int protocol)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_netlink_create_service(rref, protocol);

	hm_net_put_rref(&rref);
	return ret;
}

int hm_netlink_release_service(int protocol)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_netlink_release_service(rref, protocol);

	hm_net_put_rref(&rref);
	return ret;
}

int hm_netlink_unicast(int protocol, unsigned int src_portid,
			 unsigned int dst_port, const char *msg,
			 const size_t len)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_netlink_unicast(rref, protocol,
						     src_portid, dst_port,
						     (char *)msg, (unsigned long)len);

	hm_net_put_rref(&rref);
	return ret;
}

int hm_netlink_unicast_nowait(int protocol, unsigned int src_portid,
			      unsigned int dst_port, const char *msg,
			      const size_t len)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_netlink_unicast_nowait(rref, protocol,
							  src_portid, dst_port,
							  (char *)msg, (unsigned long)len);

	hm_net_put_rref(&rref);
	return ret;
}

int hm_netlink_broadcast(int protocol, unsigned int src_portid,
			 unsigned int dst_group, const char *msg,
			 const size_t len)
{
	rref_t rref;
	int ret;

	ret = hm_net_ap_rref(&rref);
	if (ret < 0) {
		return ret;
	}

	ret = actvcapcall_netcall_netlink_broadcast(rref, protocol,
						     src_portid, dst_group,
						     (char *)msg, (unsigned long)len);

	hm_net_put_rref(&rref);
	return ret;
}
