/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: ulibs for idle
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 18 11:45:21 2022
 */

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmpm/idle.h>
#include <libsysif/pwrmgr/api.h>

#include "pm_sysif.h"

struct idlemgr_stat_config {
	rref_t rref;
};

static int idlemgr_stat_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct idlemgr_stat_config *config = (struct idlemgr_stat_config *)data;

	return actvxactcapcall_pmcall_idlemgr_stat(false, true, config->rref, *attr);
}

int hm_idle_stat(struct bunch *bunch)
{
	int err = E_HM_OK;
	rref_t rref = 0;
	struct bunch_ipc_reader_ctx ctx;
	struct idlemgr_stat_config config;

	if (bunch == NULL) {
		hm_error("invalid bunch\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		rref = pm_sysif_get_rref();
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
			hm_error("idle: pm sysif get rref failed: %s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		mem_zero_s(config);
		config.rref = rref;
		ctx.ipc = idlemgr_stat_ipc;
		ctx.private_data = ptr_to_void(&config);
		ctx.target_info = rref;
		err = bunch_ipc_request(bunch, &ctx);
		if (err == E_HM_NOSYS) {
			hm_info("bunch ipc request is not supported\n");
		} else if (err < 0) {
			hm_error("bunch ipc request failed: %s\n", hmstrerror(err));
		}
	}

	return err;
}
