/*
* Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
* Description: dec sec module
* Author: Huawei OS Kernel Lab
* Create: Wed Apr 24 01:19:14 2024
*/

#include <libhmdec/dec.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsrv_sys/hm_security.h>
#include <libhmsync/raw_scopedptr.h>
#include <libsec/server_env.h>

static int dec_init(const struct sec_server_env *env)
{
    (void)env;
	return E_HM_OK;
}

static void dec_fini(void)
{
	return;
}

struct sec_mod sec_mod_dec = {
	.name = "dec",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = dec_init,
		.fini = dec_fini,
	},
};
