/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: sec_mod components definition
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 20 11:28:21 2023
 */

#include <libsec/usr_ns.h>
#include <libsec/server_env.h>

static int userns_init(const struct sec_server_env *env)
{
	UNUSED(env);
	return E_HM_OK;
}

static void userns_fini(void)
{
}

struct sec_mod sec_mod_userns = {
	.name = "userns",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = userns_init,
		.fini = userns_fini,
	}
};
