/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implement of security service management
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 27 09:56:54 2023
 */
#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libsec/module.h>
#include <libsec/service_management.h>

static hm_secs_mgmt_func secs_mgmt_funcs[HM_SEC_MODULE_MAX + 1] = {0};

int hm_secs_mgmt_register(unsigned int module, hm_secs_mgmt_func func)
{
	if ((func == NULL) || (module == 0) ||
	    (module > HM_SEC_MODULE_MAX)) {
		hm_error("invalid input parameter\n");
		return E_HM_INVAL;
	}

	if (secs_mgmt_funcs[module] != NULL) {
		/* Repeated registration occurs in the vfs_fusion scenario */
		hm_debug("module %u callback funtion exist\n", module);
		return E_HM_OK;
	}

	secs_mgmt_funcs[module] = func;
	return E_HM_OK;
}

int hm_secs_mgmt(unsigned int module, unsigned int command,
		 const struct actvpool_recv *data, unsigned int service)
{
	hm_secs_mgmt_func func = NULL;

	if ((module == 0) || (module > HM_SEC_MODULE_MAX)) {
		hm_error("invalid module type %u\n", module);
		return E_HM_INVAL;
	}

	func = secs_mgmt_funcs[module];
	if (func == NULL) {
		hm_error("failed to find func, module=%u, command=%u\n",
			 module, command);
		return E_HM_OPNOTSUPP;
	}

	return func(command, data, service);
}
