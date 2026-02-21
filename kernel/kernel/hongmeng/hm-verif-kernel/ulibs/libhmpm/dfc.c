/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Implementation of dfc native api
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 6 14:02:59 2023
 */
#include <libhmpm/dfc.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsysif/freqmgr/api.h>
#include <libhwsecurec/securec.h>

#include "pm_sysif.h"

struct freq_stat_config {
	rref_t rref;
	struct dfc_component_name component;
};

static int dfc_freq_stat_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct freq_stat_config *config = (struct freq_stat_config *)data;

	return actvcapcall_fmcall_dfc_freq_stat(config->rref,
						config->component, *attr);
}

static int set_component(struct dfc_component_name *comp, const char *name)
{
	int err = E_HM_OK;
	size_t len;

	len = strnlen(name, DFC_COMP_NAME_MAX);
	if (len >= DFC_COMP_NAME_MAX || len == 0) {
		hm_warn("invalid name len=%zu\n", len);
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		NOFAIL(strncpy_s(comp->string, DFC_COMP_NAME_MAX, name, len));
	}

	return err;
}

int hm_dfc_freq_stat(const char *component, struct bunch *bunch)
{
	int err = E_HM_OK;
	rref_t rref = 0;
	struct sysarg_cnode_statrref statrref;
	struct freq_stat_config config;
	struct bunch_ipc_reader_ctx ctx;

	if (component == NULL || bunch == NULL) {
		hm_warn("invalid component name or bunch\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		rref = fm_sysif_get_rref();
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
			if (err == E_HM_NOSYS) {
				hm_info("dfc freq stat is not supported\n");
			} else if (err < 0) {
				hm_error("freqmgr sysif get rref failed: %s\n",
					 hmstrerror(err));
			}
		}
	}
	if (err == E_HM_OK) {
		/* query tppmgr cnode_idx by rref */
		mem_zero_s(statrref);
		err = syscap_CNodeStatRRef(0, &statrref, rref,
					   CNODE_STATRREF_SRC_CNODE_IDX);
		if (err < 0) {
			hm_error("query tppmgr cnode_idx failed: %s\n",
				 hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		mem_zero_s(config);
		err = set_component(&config.component, component);
	}
	if (err == E_HM_OK) {
		config.rref = rref;
		mem_zero_s(ctx);
		bunch_ipc_ctx_init(&ctx, (uint64_t)rref, dfc_freq_stat_ipc,
				   ptr_to_void(&config));
		err = bunch_ipc_request(bunch, &ctx);
		if (err < 0) {
			hm_error("bunch ipc try request data failed: %s\n",
				 hmstrerror(err));
		}
	}

	return err;
}
