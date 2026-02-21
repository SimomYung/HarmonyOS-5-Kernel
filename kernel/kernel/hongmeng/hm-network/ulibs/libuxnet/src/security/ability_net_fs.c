/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: The ability check interface of net_fs
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 23 21:59:19 2019
 */

#include <hongmeng/errno.h>
#include "security/net_fs.h"
#include <generated/hmsd/hook/net/net_fs.h>
#include "security/ability.h"

static int sec_check_xact_ability_net_fs(struct sec_chk_cred_xact *cred)
{
	struct sec_chk_cred *src_cred = NULL;
	int rc;

	if (cred == NULL) {
		return E_HM_ACCES;
	}

	src_cred = sec_chk_src_cred(cred);
	if (src_cred == NULL) {
		return E_HM_ACCES;
	}

	rc = sec_check_ability(src_cred, mac_cred_of_sec_cred(src_cred), ab_net_fs,
			       sec_chk_show_obj_name, NULL);

	return rc;
}

SEC_DEFINE_COMPONENT(net_fs, ability,
		     sec_define_chk_xact_method(net_fs, sec_check_xact_ability_net_fs)
		    );
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/net/net_fs.h>
