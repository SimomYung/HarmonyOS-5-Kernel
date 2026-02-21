/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 09 10:48:33 2024
 */

#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsrv_sys/hm_security.h>
#include <libhmactv/actv.h>

#include "libsec/server_env.h"
#include <libsec/service_management.h>

#include <libkcov/kcov.h>
#include <libseckcov/seckcov.h>

static int hm_secs_mgmt_kcov(unsigned int command, const struct actvpool_recv *data, unsigned int service)
{
	UNUSED(command, data, service);
	kcov_enable();
	return E_HM_OK;
}

static int kcov_init(const struct sec_server_env *env)
{
	UNUSED(env);
	return hm_secs_mgmt_register(HM_SEC_MODULE_KCOV, hm_secs_mgmt_kcov);
}

static void kcov_fini(void)
{
	return;
}

struct sec_mod sec_mod_kcov = {
	.name = "kcov",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = kcov_init,
		.fini = kcov_fini,
	},
};
