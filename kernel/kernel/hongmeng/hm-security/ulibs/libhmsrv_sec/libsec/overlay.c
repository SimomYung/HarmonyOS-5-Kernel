/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Init the module of overlay
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 06 11:27:14 2023
 */
#include "libsec/utility.h"
#include "libsec/overlay.h"

#include <lib/dlist.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsec/server_env.h>
#include <libsec/module.h>
#include <libhmsync/raw_mutex_guard.h>

static int overlay_init(const struct sec_server_env *env)
{
	UNUSED(env);
	return E_HM_OK;
}

static void overlay_fini(void)
{
}

struct sec_mod sec_mod_overlay = {
	.name = "overlay",
	.ref_cnt = SEC_MOD_REFCNT_NR_INITIALIZER,
	.lock = RAW_MUTEX_INITIALIZER,
	.ops = {
		.init = overlay_init,
		.fini = overlay_fini,
	},
};
