/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 09 17:50:54 2021
 */

#include <libsec/keyring.h>
#include <libsec/server_env.h>

static int keyring_init(const struct sec_server_env *env)
{
	UNUSED(env);
	return E_HM_OK;
}

static void keyring_fini(void)
{
}

struct sec_mod sec_mod_keyring = {
	.name = "keyring",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = keyring_init,
		.fini = keyring_fini,
	}
};
