/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: Isolate - Access Control Engine
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 16 10:00:00 2025
 */

#include <libhmisolate/isolate.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsrv_sys/hm_security.h>
#include <libhmsync/raw_scopedptr.h>
#include <libsec/server_env.h>

static int isolate_init(const struct sec_server_env *env)
{
	(void)env;
	return E_HM_OK;
}

static void isolate_fini(void)
{
	return;
}

struct sec_mod sec_mod_isolate = {
	.name = "isolate",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = isolate_init,
		.fini = isolate_fini,
	},
};
