/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Secure module base functions
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:27:25 2019
 */
#include "libsec/module.h"

#include <stddef.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

int sec_mod_init(struct sec_mod *mod, const struct sec_server_env *env)
{
	int ret = E_HM_OK;

	if (mod == NULL) {
		hm_error("mod is NULL\n");
		ret = E_HM_INVAL;
	} else {
		if (mod->ops.init != NULL) {
			raw_mutex_lock(&mod->lock);
			if (raw_refcnt_nr_counter(&mod->ref_cnt) == SEC_MOD_UNINITIALIZED) {
				ret = mod->ops.init(env);
			}
			if (ret == E_HM_OK) {
				(void)raw_refcnt_nr_get(&mod->ref_cnt);
			}
			raw_mutex_unlock(&mod->lock);
		}
	}

	return ret;
}

void sec_mod_fini(struct sec_mod *mod)
{
	if (mod != NULL && mod->ops.fini != NULL) {
		int nr = raw_refcnt_nr_counter(&mod->ref_cnt);
		BUG_ON(nr <= SEC_MOD_UNINITIALIZED);
		if (raw_refcnt_nr_put(&mod->ref_cnt) == SEC_MOD_UNINITIALIZED) {
			mod->ops.fini();
		}
	} else {
		hm_error("mod or fini is NULL\n");
	}
}
