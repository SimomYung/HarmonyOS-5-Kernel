/*
* Copyright (C) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
* Description: Hkids sec module
* Author: Huawei OS Kernel Lab
* Create: Sun Mar 09 17:32:55 2025
*/

#include <libhmhkids/hkids.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsrv_sys/hm_security.h>
#include <libhmsync/raw_scopedptr.h>
#include <libsec/server_env.h>
#include <libsec/chk_cred.h>

static int hkids_init(const struct sec_server_env *env)
{
	UNUSED(env);
	return E_HM_OK;
}

static void hkids_fini(void)
{
}

struct sec_mod sec_mod_hkids = {
	.name = "hkids",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = hkids_init,
		.fini = hkids_fini,
	},
};
