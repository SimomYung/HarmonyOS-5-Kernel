/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 18 11:45:21 2019
 */
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmpm/suspend.h>
#include <libsysif/pwrmgr/api.h>

#include "pm_sysif.h"

int hm_suspend2mem(void)
{
	int err;
	rref_t rref;

	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		return err;
	}
	err = actvcapcall_pmcall_suspend_set_state(rref, SUSPEND_STATE_MEM);
	if (err != E_HM_OK) {
		hm_error("suspend set state failed: %s\n",
			 hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

int hm_suspend_disk_restore(void)
{
	int err;
	rref_t rref;

	rref = pm_sysif_get_rref();
	if (IS_REF_ERR(rref)) {
		err = REF_TO_ERR(rref);
		hm_error("pm sysif get rref failed: %s\n", hmstrerror(err));
		return err;
	}
	err = actvcapcall_pmcall_suspend_disk_restore(rref);
	if (err != E_HM_OK) {
		if (err == E_HM_NOSYS) {
			return E_HM_OK;
		}
		hm_error("suspend disk restore failed: %s\n",
			 hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}
