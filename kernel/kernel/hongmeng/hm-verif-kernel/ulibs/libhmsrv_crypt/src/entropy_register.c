/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Entropy source register
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 15 14:16:12 2020
 */

#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>
#include <libhmcrypt/hmcrypt.h>
#include <libhmsrv_crypt/sysif/api.h>
#include <libhmsrv_crypt/entropy_register.h>

/* register entropy source with crypto, grant entropy_if actvpool to crypto */
int hm_register_entropy_source(cref_t ap)
{
	/* get crypto cnode index */
	rref_t crypto_rref = ERR_TO_REF(E_HM_INVAL);
	struct sysarg_cnode_statrref statrref = { 0 };
	int ret;

	ret = hm_path_acquire(CRYPTO_PATH, &crypto_rref);
	if (ret != E_HM_OK) {
		hm_error("devhost acquire crypto rref by path failed, ret=%s\n",
			 hmstrerror(ret));
		return ret;
	}

	ret = sysfast_cnode_stat_rref(&statrref, crypto_rref, CNODE_STATRREF_SRC_CNODE_IDX);
	if (ret != E_HM_OK) {
		hm_error("devhost get crypro cnode index failed, ret=%s\n",
			 hmstrerror(ret));
		goto recyle_path;
	}

	/* create uref at devhost */
	struct __actvpool_grant grant;
	grant.credential = 0ULL;
	grant.pre_alloc_actv_cref = 0ULL;
	uref_t new_uref = hm_ucap_export(0, statrref.src_cnode_idx, CNODE_STATRREF_SRC_CNODE_IDX,
					ap, CRIGHT_ACTVPOOL_CALL, ptr_to_ulong(&grant));
	ret = REF_TO_ERR(new_uref);
	if (ret != E_HM_OK) {
		hm_error("devhost export rref for crypto failed, ret=%s\n",
			 hmstrerror(REF_TO_ERR(new_uref)));
		goto recyle_path;
	}

	/* actv register entropy source */
	ret = actvcapcall_cryptcall_crypto_register(crypto_rref, new_uref);
	if (ret != E_HM_OK) {
		hm_error("devhost register entropy source to crypto failed, ret=%s\n",
			 hmstrerror(ret));
		goto recycle_uref;
	}
	goto recyle_path;

recycle_uref:
	(void)hm_ucap_revoke(0, new_uref);
recyle_path:
	(void)hm_path_release(CRYPTO_PATH, crypto_rref);

	return ret;
}
