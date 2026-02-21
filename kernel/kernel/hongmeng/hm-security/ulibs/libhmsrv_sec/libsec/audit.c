/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Definitions of audit module
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 25 11:46:05 2020
 */
#include "libsec/audit.h"
#include "libsec/server_env.h"
#include <libstrict/strict.h>

static int audit_init(const struct sec_server_env *env)
{
	UNUSED(env);
	return E_HM_OK;
}

static void audit_fini(void)
{
}

struct sec_mod sec_mod_audit = {
	.name = "audit",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = audit_init,
		.fini = audit_fini,
	},
};
