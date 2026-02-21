/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The ability check interface of reserved port range
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 23 21:59:19 2019
 */

#include <hongmeng/errno.h>

#include "security/ability.h"
#include "security/reserved_port_range.h"
#include <generated/hmsd/hook/net/reserved_port_range.h>

static int ability_reserved_port_range(const struct sec_chk_cred *chk_cred,
				       const sec_plc_port_range_t *port_range)
{
	if (sec_plc_mode_is_disable()) {
		return E_HM_OK;
	}

	if (!sec_plc_check_port_range(sid_of_sec_cred(chk_cred), port_range)) {
		if (!sec_plc_type_is_no_log_subj(sid_of_sec_cred(chk_cred), NULL)) {
			hm_info("Access denied. (alias=reserved_port_range, proc=[0x%x], " \
				"ret=E_HM_PERM(-9), start=%"PRIu64", end=%"PRIu64")\n",
				chk_cred->cnode_idx, port_range->start, port_range->end);
		}

		if (!sec_plc_mode_is_permissive()) {
			return E_HM_PERM;
		}
	}
	return E_HM_OK;
}

SEC_DEFINE_COMPONENT(reserved_port_range, ability,
		     sec_define_chk_method(reserved_port_range, (sec_chk_reserved_port_range_ft)ability_reserved_port_range)
		    );
#define ENABLE_SEC_MOD_ABILITY
#include <generated/hmsd/module/net/reserved_port_range.h>
