/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: The ulibs for tppmgr init event notifier
 * Author: Huawei OS Kernel Lab
 * Create: Thu June 04 16:27:45 2024
 */

#include "pm_sysif.h"
#include <libhmpm/pm_init_event.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsysif/pwrmgr/api.h>

int hm_pm_init_notify(enum pm_init_event event)
{
	int err = E_HM_OK;
	rref_t rref = 0;

	if (event >= INIT_EVENT_NR) {
		hm_warn("invalid init event\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		rref = pm_sysif_get_rref();
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
			hm_error("pm init event sysif get rref failed: %s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		err = actvcapcall_pmcall_pm_init_notify(rref, event);
		if (err != E_HM_OK) {
			hm_error("module %d init failed: %s\n", event, hmstrerror(err));
		}
	}

	return err;
}
