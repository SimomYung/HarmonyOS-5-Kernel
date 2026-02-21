/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Operations of verification methods of cms
 * Author: Huawei OS Kernel Lab
 * Create: Mon June 28 10:51:16 2021
 */
#include "cms_sig_verify.h"
#include <fcntl.h>
#include <stdint.h>
#include <sys/param.h>

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>

#include <libhwsecurec/securec.h>

#include <cmscbb/cmscbb_types.h>
#include <cmscbb/cmscbb_sdk.h>
#include <cmscbb/cmscbb_verify.h>
#include <cmscbb/cmscbb_cms_vrf.h>

#ifdef __HOST_LLT__
#define INNER
#else
#define INNER static
#endif

int cms_verify_sig(struct hmsig_shm *shm, const void *data, size_t size, const void *sig, size_t sig_size)
{
	int rc = E_HM_OK;
	CMSCBB_ERROR_CODE ret = 0;
	CmscbbVerifyInfo info;
	mem_zero_s(info);

	info.src = (CVB_VOID *)data;
	info.srcSz = (CVB_UINT32)size;
	info.sig = (CVB_VOID *)sig;
	info.sigSz = (CVB_UINT32)sig_size;
	if (shm != NULL) {
		info.crl = (CVB_VOID *)shm->base;
		info.crlSz = (CVB_UINT32)shm->size;
	}

	ret = CmscbbVerifyCmsData(&info);
	if (ret != CVB_SUCCESS) {
		rc = E_HM_ACCES;
	}

	return rc;
}

INNER int parse_compare_result(CmscbbCrlPeriodStat stat)
{
	int ret = E_HM_ACCES;
	switch (stat) {
	case SCPS_SAME:
		hm_debug("the update crl is same with the original one\n");
		ret = E_HM_EXIST;
		break;
	case SCPS_NEW:
		hm_debug("the update crl is newer than the original one\n");
		ret = E_HM_OK;
		break;
	case SCPS_OLD:
		hm_debug("the update crl is older than the original one\n");
		break;
	case SCPS_MIX:
		/* There are different publishers or there are both new and old ones */
		hm_debug("cannot assess the update and original crl\n");
		ret = E_HM_OK;
		break;
	default:
		break;
	}
	return ret;
}

INNER CMSCBB_ERROR_CODE cms_create_ctx_for_decode(CMSCBB_VRF_CTX *vrf_ctx, CMSCBB_VRF_CTX *backup_ctx)
{
	CMSCBB_ERROR_CODE ret = CVB_SUCCESS;
	/* create context of verification */
	ret = CmscbbCreateVerfContext(vrf_ctx);
	if (ret != CVB_SUCCESS) {
		hm_error("failed to create vrf_ctx context, err=%08lx\n", (unsigned long)ret);
	} else {
		ret = CmscbbCreateVerfContext(backup_ctx);
		if (ret != CVB_SUCCESS) {
			(void)CmscbbVrfCtxFree(*vrf_ctx);
			hm_error("failed to create backup_ctx context, err=%08lx\n", (unsigned long)ret);
		}
	}

	return ret;
}

INNER int cms_compare_crl(const void *old_crl, size_t old_sz, const void *new_crl, size_t new_sz)
{
	int rc = E_HM_RESFAULT;
	CMSCBB_ERROR_CODE ret = 0;
	CMSCBB_VRF_CTX vrf_ctx = CVB_NULL;
	CMSCBB_VRF_CTX backup_ctx = CVB_NULL;
	/* storage the result of crl parsing */
	CmscbbCrlBundleInfo *crl_update = NULL;
	CmscbbCrlBundleInfo *crl_original = NULL;
	/* storage the result of crl compare */
	CmscbbCrlPeriodStat crl_stat = SCPS_MIX;

	/* create context of verification */
	ret = cms_create_ctx_for_decode(&vrf_ctx, &backup_ctx);
	if (ret != CVB_SUCCESS) {
		hm_error("failed to create verification context, err=%08lx\n", (unsigned long)ret);
	} else {
		/* check the crl file format and parse it */
		ret = CmscbbDecodeCrl(vrf_ctx, (const CVB_BYTE *)new_crl, (CVB_UINT32)new_sz, &crl_update);
		if (ret != CVB_SUCCESS) {
			hm_debug("failed to decode crl update file, err=%08lx\n", (unsigned long)ret);
			goto err;
		}

		ret = CmscbbDecodeCrl(backup_ctx, (const CVB_BYTE *)old_crl, (CVB_UINT32)old_sz, &crl_original);
		if (ret != CVB_SUCCESS) {
			hm_error("failed to decode crl original file, err=%08lx\n", (unsigned long)ret);
			(void)CmscbbCrlFree(vrf_ctx, crl_update);
			goto err;
		}

		/* compare crl */
		ret = CmscbbCrlCompare(vrf_ctx, crl_update, crl_original, &crl_stat);
		if (ret != CVB_SUCCESS) {
			hm_error("failed to compare crl, err=%08lx\n", (unsigned long)ret);
		} else {
			rc = parse_compare_result(crl_stat);
		}

		(void)CmscbbCrlFree(vrf_ctx, crl_update);
		(void)CmscbbCrlFree(vrf_ctx, crl_original);
err:
		(void)CmscbbVrfCtxFree(backup_ctx);
		(void)CmscbbVrfCtxFree(vrf_ctx);
	}
	return rc;
}

int cms_check_crl(const void *old_crl, size_t old_sz, const void *new_crl, size_t new_sz)
{
	int ret;

	if (old_crl == NULL || old_sz == 0 || new_crl == NULL || new_sz == 0) {
		hm_error("parameters are invalid\n");
		ret = E_HM_INVAL;
	} else {
		ret = cms_compare_crl(old_crl, old_sz, new_crl, new_sz);
		if (ret != E_HM_OK) {
			hm_debug("failed to compare crl, error=%s\n", hmstrerror(ret));
		}
	}

	return ret;
}

INNER int _crl_check_format(const void *crl, size_t sz)
{
	int rc = E_HM_OK;
	CMSCBB_ERROR_CODE ret = CVB_SUCCESS;
	CMSCBB_VRF_CTX vrf_ctx = CVB_NULL;
	CmscbbCrlBundleInfo *crl_update = NULL;
	/* create context of verification */
	ret = CmscbbCreateVerfContext(&vrf_ctx);
	if (ret != CVB_SUCCESS) {
		hm_error("failed to create vrf_ctx context, error=%08lx\n", (unsigned long)ret);
		rc = E_HM_RESFAULT;
	} else {
		/* check the crl file format */
		ret = CmscbbDecodeCrl(vrf_ctx, (const CVB_BYTE *)crl, (CVB_UINT32)sz, &crl_update);
		if (ret != CVB_SUCCESS) {
			hm_debug("failed to decode crl file, error=%08lx\n", (unsigned long)ret);
			rc = E_HM_INVAL;
		} else {
			(void)CmscbbCrlFree(vrf_ctx, crl_update);
		}
	}

	if (vrf_ctx != CVB_NULL) {
		(void)CmscbbVrfCtxFree(vrf_ctx);
	}
	return rc;
}

int cms_check_crl_format(const void *crl, size_t sz)
{
	int ret = E_HM_OK;

	if (crl == NULL || sz == 0) {
		hm_error("parameters are invalid\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = _crl_check_format(crl, sz);
	}

	return ret;
}
