/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony source file for policydb
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include "policydb.h"

int hmsel_policydb_read(struct hmsel_policydb *policydb, struct hmsel_policy_file *policy_file)
{
	int rc = E_HM_OK;
	if (policydb == NULL || policy_file == NULL) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rc = adapter_policydb_read(&policydb->adapter_policydb, policy_file);
		if (rc == E_HM_OK) {
			policydb_read_feedback(policydb);
		}
	}
	return rc;
}

int hmsel_policydb_sid_bounded_transition(struct hmsel_policydb *policydb, uint32_t stype, uint32_t ttype)
{
	int rc = E_HM_OK;
	if (policydb == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = adapter_policydb_sid_bounded_transition(&policydb->adapter_policydb, stype, ttype);
	}
	return rc;
}
