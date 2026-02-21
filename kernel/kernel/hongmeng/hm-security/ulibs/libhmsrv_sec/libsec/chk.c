/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Secure check functions
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 01 17:29:37 2019
 */
#include "libsec/chk_cred.h"

#include <stddef.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include "librequester/requester.h"

int sec_chk_cred_init_requester(struct sec_chk_cred *cred)
{
	struct requester req = {0};
	int rc = E_HM_OK;

	if (cred == NULL) {
		hm_error("cred is NULL\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = requester_init(&req);
		if (rc != E_HM_OK) {
			hm_error("failed to init the requester, (rc=%s)\n", hmstrerror(rc));
		}
	}

	if (rc == E_HM_OK) {
		sec_chk_cocred_init(cred, req.cnode_idx, &req);
	}

	return rc;
}

int sec_chk_cred_init_requester_simple(struct sec_chk_cred *cred)
{
	struct requester req = {0};
	int rc = requester_init_simple(&req);
	if (rc != E_HM_OK) {
		hm_error("failed to init the requester, (rc=%s)\n", hmstrerror(rc));
	}

	if (rc == E_HM_OK) {
		sec_chk_cocred_init(cred, req.cnode_idx, &req);
	}

	return rc;
}

int sec_chk_src_cred_init_requester_simple(struct sec_chk_cred *src_cred)
{
	struct requester req = {0};
	int rc = requester_init_simple_src(&req);
	if (rc != E_HM_OK) {
		hm_error("failed to init the requester, (rc=%s)\n", hmstrerror(rc));
	}

	if (rc == E_HM_OK) {
		sec_chk_cocred_init_src(src_cred, req.cnode_idx, &req);
	}

	return rc;
}

