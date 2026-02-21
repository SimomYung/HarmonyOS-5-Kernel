/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: The ability check interface of fops
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 09 15:01:19 2020
 */

#include <hongmeng/errno.h>

#include "fops.h"
#include "ability.h"

SEC_DEFINE_ABILITY_DEFAULT(fops_sysfs, NO_ARGS(), ab_devhost_fops_sysfs)

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

static int sec_chk_ability_fops_close(struct sec_chk_cred_xact *cred_xact)
{
	return sec_chk_ability(cred_xact, ab_devhost_fops);
}

static int sec_chk_ability_fops_mmap(struct sec_chk_cred_xact *cred_xact)
{
	return sec_chk_ability(cred_xact, ab_devhost_fops_mmap);
}

static int sec_chk_ability_as_vfs(struct sec_chk_cred_xact *cred_xact)
{
	return sec_chk_ability(cred_xact, ab_as_vfs);
}

SEC_DEFINE_COMPONENT(fops, ability,
	sec_define_chk_xact_method(fops_mmap, sec_chk_ability_fops_mmap),
	sec_define_chk_xact_method(fops_close, sec_chk_ability_fops_close),
	sec_define_ability_method(fops_sysfs),
	sec_define_chk_xact_method(fops_transfs, sec_chk_ability_as_vfs),
	sec_define_chk_xact_method(fops_open_sync_sec, sec_chk_ability_as_vfs)
);
#define ENABLE_SEC_MOD_BUCKLE
#include <generated/hmsd/module/devhost/fops.h>
