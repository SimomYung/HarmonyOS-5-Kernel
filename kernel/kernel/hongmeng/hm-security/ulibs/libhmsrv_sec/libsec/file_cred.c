/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Write and read file credentials
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 28 20:23:09 2019
 */
#include "libsec/file_cred.h"
#include <hongmeng/types.h>
#include <hongmeng/compiler.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsysif/sysmgr/api.h>
#include <libhwsecurec/securec.h>

raw_static_assert(sizeof(((struct vfs_file_cred*)(uintptr_t)0)->data) >
		  sizeof(struct sec_file_cred),
		  file_cred_not_enough) __attribute__((unused));

int sec_file_cred_write_mac(struct vfs_file_cred *file_cred,
			    const struct sec_ctx_mac *mac_cred)
{
	int ret = E_HM_OK;

	if (file_cred == NULL) {
		hm_error("cred is NULL\n");
		ret = E_HM_INVAL;
	} else {
		struct sec_file_cred *cred = ptr_to_type(file_cred->data, struct sec_file_cred *);
		cred->mac = *mac_cred;
	}

	return ret;
}

int sec_file_cred_write_dac(struct vfs_file_cred *file_cred,
			    const struct sec_ctx_dac *dac_cred)
{
	int rc = E_HM_OK;

	if (file_cred == NULL) {
		hm_error("cred is NULL\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		struct sec_file_cred *cred = ptr_to_type(file_cred->data, struct sec_file_cred *);
		cred->dac = *dac_cred;
	}

	return rc;
}

int sec_file_cred_read_mac(const struct vfs_file_cred *file_cred,
			   struct sec_ctx_mac *mac_cred)
{
	int rc = E_HM_OK;

	if (file_cred == NULL || mac_cred == NULL) {
		hm_error("cred is NULL\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		const struct sec_file_cred *cred = ptr_to_type(file_cred->data, const struct sec_file_cred *);
		*mac_cred = cred->mac;
	}

	return rc;
}

int sec_file_cred_read_dac(const struct vfs_file_cred *file_cred,
			   struct sec_ctx_dac *dac_cred)
{
	int ret = E_HM_OK;
	const struct sec_file_cred *cred = NULL;
	if (file_cred != NULL) {
		cred = ptr_to_type(file_cred->data, const struct sec_file_cred *);
		*dac_cred = cred->dac;
	} else {
		hm_error("cred is NULL\n");
		ret = E_HM_INVAL;
	}

	return ret;
}

int sec_file_cred_write_sel(struct vfs_file_cred *file_cred,
			    const struct sec_ctx_sel *sel_cred)
{
#ifdef CONFIG_SEHARMONY
	int rc = E_HM_OK;

	if (file_cred == NULL || sel_cred == NULL) {
		hm_error("cred is NULL\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		struct sec_file_cred *cred = ptr_to_type(file_cred->data, struct sec_file_cred *);
		cred->sel = *sel_cred;
	}

	return rc;
#else
	UNUSED(file_cred, sel_cred);
	return E_HM_NOSYS;
#endif
}

int sec_file_cred_read_sel(const struct vfs_file_cred *file_cred,
			   struct sec_ctx_sel *sel_cred)
{
#ifdef CONFIG_SEHARMONY
	int rc = E_HM_OK;

	if (file_cred == NULL || sel_cred == NULL) {
		hm_error("cred is NULL\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		const struct sec_file_cred *cred = ptr_to_type(file_cred->data, const struct sec_file_cred *);
		*sel_cred = cred->sel;
	}

	return rc;
#else
	UNUSED(file_cred, sel_cred);
	return E_HM_NOSYS;
#endif
}
