/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: The ulibs for quality of service
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 06 17:17:49 2022
 */

#include "pm_sysif.h"
#include <libhmpm/pmqos.h>
#include <libsysif/pwrmgr/api.h>

int hm_pmqos_request_add(struct hm_pmqos_request *request, int value)
{
	int err = E_HM_OK;
	rref_t rref = 0;
	struct __actvret_pmcall_pmqos_request_add add_req_ret;

	if (request == NULL) {
		hm_warn("invalid request\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		rref = pm_sysif_get_rref();
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
			hm_error("pmqos sysif get rref failed: %s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		err = actvcapcall_pmcall_pmqos_request_add(rref, (unsigned int)(request->type),
							   value, &add_req_ret);
		if (err == E_HM_OK) {
			request->id = add_req_ret.id;
		} else {
			hm_debug("pmqos add request errno: %s\n", hmstrerror(err));
		}
	}

	return err;
}

int hm_pmqos_request_remove(struct hm_pmqos_request *request)
{
	int err = E_HM_OK;
	rref_t rref = 0;

	if (request == NULL) {
		hm_warn("invalid request\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		rref = pm_sysif_get_rref();
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
			hm_error("pmqos sysif get rref failed: %s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		err = actvcapcall_pmcall_pmqos_request_remove(rref, *request);
		if (err != E_HM_OK) {
			hm_error("pmqos remove request failed: %s\n", hmstrerror(err));
		}
	}

	return err;
}

int hm_pmqos_request_update(struct hm_pmqos_request *request, int value)
{
	int err = E_HM_OK;
	rref_t rref = 0;

	if (request == NULL) {
		hm_warn("invalid request\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		rref = pm_sysif_get_rref();
		if (IS_REF_ERR(rref)) {
			err = REF_TO_ERR(rref);
			hm_error("pmqos sysif get rref failed: %s\n", hmstrerror(err));
		}
	}
	if (err == E_HM_OK) {
		err = actvcapcall_pmcall_pmqos_request_update(rref, *request, value);
		if (err != E_HM_OK) {
			hm_error("pmqos update request failed: %s\n", hmstrerror(err));
		}
	}

	return err;
}
