/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: dm-verity table signature verification
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 12 09:58:42 2022
 */

#include <bsl/bsl_sal.h>
#include <bsl/bsl_errno.h>
#include <crypto/crypt_errno.h>
#include <crypto/crypt_eal_pkey.h>

#include <hongmeng/errno.h>
#include <hmkernel/klog_level.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>

#include <libhmcrypt/hmpem.h>
#include <libhmcrypt/hmcrypt.h>
#include <libhmdmverity/hmdmverity.h>

static void free_pubkey(CRYPT_EAL_PkeyPub *pubkey)
{
	if (pubkey->key.rsaPub.n != NULL) {
		free(pubkey->key.rsaPub.n);
		pubkey->key.rsaPub.n = NULL;
	}
	pubkey->key.rsaPub.nLen = 0;

	if (pubkey->key.rsaPub.e != NULL) {
		free(pubkey->key.rsaPub.e);
		pubkey->key.rsaPub.e = NULL;
	}
	pubkey->key.rsaPub.eLen = 0;

	return;
}

int dm_sig_verity(char *pubkey_path, uint8_t *data, uint32_t dataLen, uint8_t *sign, uint32_t signLen)
{
	int ret = E_HM_OK;

	ret = hmcrypt_init();
	if (ret != E_HM_OK) {
		return ret;
	}

	CRYPT_EAL_PkeyCtx *pkey = NULL;
	pkey = CRYPT_EAL_PkeyNewCtx(CRYPT_PKEY_RSA);

	/* fill with RSA PSS */
	CRYPT_RSA_PssPara pkey_psspad;
	pkey_psspad.saltLen = -1;
	pkey_psspad.mdId = CRYPT_MD_SHA256;
	pkey_psspad.mgfId = CRYPT_MD_SHA256;

	CRYPT_EAL_PkeyPub pubkey = {0};
	ret = pem_parse_hitls_pubkey(pubkey_path, &pubkey);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = CRYPT_EAL_PkeySetPub(pkey, &pubkey);
	if (ret != CRYPT_SUCCESS) {
		hm_error("failed to set crypt eal pkey, ret=%d\n", ret);
		goto error;
	}

	ret = CRYPT_EAL_PkeyCtrl(pkey, CRYPT_CTRL_SET_RSA_EMSA_PSS,
				 &pkey_psspad, sizeof(CRYPT_RSA_PssPara));
	if (ret != CRYPT_SUCCESS) {
		hm_error("failed to ctrl crypt eal pkey, ret=%d\n", ret);
		goto error;
	}

	ret = CRYPT_EAL_PkeyVerify(pkey, CRYPT_MD_SHA256, data, dataLen, sign, signLen);
	if (ret != CRYPT_SUCCESS) {
		hm_error("failed to verify crypt eal pkey, ret=%d\n", ret);
		goto error;
	}

	free_pubkey(&pubkey);
	CRYPT_EAL_PkeyFreeCtx(pkey);
	return E_HM_OK;

error:
	free_pubkey(&pubkey);
	CRYPT_EAL_PkeyFreeCtx(pkey);
	return E_HM_INVAL;
}
