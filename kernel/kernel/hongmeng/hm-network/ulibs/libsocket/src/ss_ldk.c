/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Implementation for hmnet ldk
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 10:45:31 2022
 */

#include <libsysif_net/api.h>
#include <hongmeng/types.h>
#include <libhmlog/hmlog.h>
#include <hmkernel/capability.h>

#include "hmnet_ldk.h"
#include "ss_channel.h"

int hm_net_ldk_domain_register(int family, cref_t dh_ap_cref)
{
	struct __actvpool_grant grant;
	rref_t rref;
	uint32_t cnode_idx;
	uref_t dh_ap_uref;
	int ret;

	/* export ap to hmnetd */
	grant.credential = 0ull;
	grant.pre_alloc_actv_cref = 0ull;

	ret = hm_net_ap_rref_and_cnode(&rref, &cnode_idx);
	if (ret < 0) {
		return ret;
	}

	dh_ap_uref = hm_ucap_export(0, cnode_idx,
					CNODE_GRANT_PREPARE_FLAG_QUERY_SRC_CNODE_IDX |
					CNODE_GRANT_PREPARE_FLAG_QUERY_USLOT_IDX |
					CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_UREF,
					dh_ap_cref,
					CRIGHT_ACTVPOOL_CALL,
					ptr_to_ulong(&grant));
	if (IS_REF_ERR(dh_ap_uref)) {
		hm_error("failed to export cref: %d\n", REF_TO_ERR(dh_ap_uref));
		ret = REF_TO_ERR(dh_ap_uref);
		goto out;
	}

	hm_debug("grant devhost uref to hmnetd: 0x%llu\n", dh_ap_uref);
	ret = actvcapcall_netcall_ldk_domain_register(rref, family, dh_ap_uref);

out:
	hm_net_put_rref(&rref);
	return ret;
}
