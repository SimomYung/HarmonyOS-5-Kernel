/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: PAL implement for module operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <devhost/backend.h>

#include <hongmeng/errno.h>
#include <libsysif/devmgr/api.h>
#include <libhwsecurec/securec.h>

#include "devhost.h"

int devhost_pal_request_module_args(const char *name, const char *mod_args)
{
	int ret = E_HM_OK;
	struct hm_actv_buf modname;
	struct hm_actv_buf modargs;

	if (name == NULL) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&modname, name, strlen(name) + 1U);
	hm_actv_buf_init(&modargs, mod_args, (mod_args == NULL) ? 0 : (strlen(mod_args) + 1U));
	ret = actvxactcapcall_drvcall_devmgr_core_request_module(false, true, libdh_get_devmgr_ap(),
							     modname,
							     modargs);
	if (ret != E_HM_OK) {
		return ret;
	}
	return E_HM_OK;
}

int devhost_pal_request_module(const char *name)
{
	return devhost_pal_request_module_args(name, NULL);
}
