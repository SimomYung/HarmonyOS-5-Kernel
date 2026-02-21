/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: The ability check interface of rpc
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 19 15:01:19 2024
 */

#include <hongmeng/errno.h>

#include "common.h"
#include "ability.h"

static int sec_chk_ability(struct sec_chk_cred_xact *cred_xact,
			   enum ability_def ability)
{
	const struct sec_chk_cred *cred = NULL;
	struct sec_cred_mac mac;
	int rc;

	mem_zero_s(mac);

	/* get cred of xact src */
	cred = sec_chk_src_cred(cred_xact);
	if (cred == NULL) {
		return E_HM_SRCH;
	}

	rc = sec_check_ability(cred, mac_cred_of_sec_cred(cred), ability,
				sec_chk_show_obj_name, NULL);

	return rc;
}

#define DEFINE_SEC_HOOK_AB_SRC(NAME, _ARG, ABILITY) \
static int sec_chk_ability_##NAME(struct sec_chk_cred_xact *cred PARA(_ARG))	\
{ \
	UNUSED_VAR(_ARG); \
	return sec_chk_ability(cred, ABILITY); \
}

DEFINE_SEC_HOOK_AB_SRC(src_actv_is_vfs, NO_ARGS(), ab_as_vfs)

SEC_DEFINE_COMPONENT(common, ability,
	sec_define_chk_xact_method(src_actv_is_vfs, sec_chk_ability_src_actv_is_vfs),
);
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/devhost/common.h>
