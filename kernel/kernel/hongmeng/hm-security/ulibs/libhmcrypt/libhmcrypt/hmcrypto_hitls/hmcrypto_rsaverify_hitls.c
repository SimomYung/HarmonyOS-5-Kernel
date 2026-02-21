/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add rsa verify of hitls.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 11:10:24 2023
 */

#include <stdint.h>

#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <crypto/crypt_algid.h>
#include <crypto/crypt_eal_md.h>
#include <crypto/crypt_eal_pkey.h>
#include <crypto/crypt_errno.h>

#include <libhmcrypt/hmcrypto_pkverify.h>
#include "internal_common.h"
#include "hmcrypto_pkverify_internal.h"
#include "hmcrypto_hitls.h"

#ifdef __HOST_LLT__
#define INNER
#else
#define INNER static
#endif

static CRYPT_MD_AlgId hitls_hash_id_table[] = {
	[HMCRYPTO_HASH_SHA2_256] = CRYPT_MD_SHA256,
};

typedef struct {
	CRYPT_PKEY_AlgId pk_id;
	CRYPT_MD_AlgId md_id;
	CRYPT_MD_AlgId mgf_id;
	int32_t salt_len;
	CRYPT_EAL_PkeyCtx *pk_ctx;
	CRYPT_EAL_MdCTX *hash_ctx;
} hitls_rsaverify_private_info;

INNER int hitls_rsaverify_newctx(hmcrypto_pkverify_info *verify_info, hmcrypto_pkverify_ctx *ctx)
{
	hitls_rsaverify_private_info *private_info = NULL;

	if (verify_info == NULL || verify_info->rsaverify_info == NULL || ctx == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	private_info = (hitls_rsaverify_private_info *)malloc(sizeof(hitls_rsaverify_private_info));
	if (private_info == NULL) {
		hm_debug("failed to malloc mem\n");
		return E_HM_INVAL;
	}

	/* id < 0, id is nonnegative enum type */
	if ((unsigned long)verify_info->rsaverify_info->md_id  >= ARRAY_SIZE(hitls_hash_id_table) ||
	    (unsigned long)verify_info->rsaverify_info->mgf_id >= ARRAY_SIZE(hitls_hash_id_table)) {
		free(private_info);
		hm_debug("invalid hash id\n");
		return E_HM_INVAL;
	}
	private_info->md_id = hitls_hash_id_table[verify_info->rsaverify_info->md_id];
	private_info->mgf_id = hitls_hash_id_table[verify_info->rsaverify_info->mgf_id];
	private_info->pk_id = CRYPT_PKEY_RSA;
	private_info->salt_len = (int32_t)verify_info->rsaverify_info->salt_len;
	private_info->pk_ctx = CRYPT_EAL_PkeyNewCtx(private_info->pk_id);
	if (private_info->pk_ctx == NULL) {
		free(private_info);
		hm_debug("failed to get rsa ctx from hitls\n");
		return E_HM_INVAL;
	}
	private_info->hash_ctx = CRYPT_EAL_MdNewCtx(private_info->md_id);
	if (private_info->hash_ctx == NULL) {
		CRYPT_EAL_PkeyFreeCtx(private_info->pk_ctx);
		free(private_info);
		hm_debug("failed to get hash ctx from hitls\n");
		return E_HM_INVAL;
	}

	ctx->private_info = (void *)private_info;

	return E_HM_OK;
}

INNER void hitls_rsaverify_freectx(hmcrypto_pkverify_ctx *ctx)
{
	hitls_rsaverify_private_info *private_info = NULL;

	if (ctx == NULL || ctx->private_info == NULL) {
		return;
	}

	private_info = (hitls_rsaverify_private_info *)ctx->private_info;
	ctx->private_info = NULL;

	CRYPT_EAL_MdFreeCtx(private_info->hash_ctx);
	CRYPT_EAL_PkeyFreeCtx(private_info->pk_ctx);
	free(private_info);

	return;
}

static int add_rsa_pk(CRYPT_EAL_PkeyPub *rsa_pub, hmcrypto_rsaverify_pk *rsaverify_pk)
{
	int ret;

	rsa_pub->id = CRYPT_PKEY_RSA;
	rsa_pub->key.rsaPub.nLen = (uint32_t)rsaverify_pk->rsa_n_len;
	rsa_pub->key.rsaPub.n = malloc(rsa_pub->key.rsaPub.nLen);
	if (rsa_pub->key.rsaPub.n == NULL) {
		hm_debug("failed to malloc memory for RSA param N, the length of N is %zu\n", rsaverify_pk->rsa_n_len);
		return E_HM_INVAL;
	}

	ret = memcpy_s(rsa_pub->key.rsaPub.n, rsa_pub->key.rsaPub.nLen, rsaverify_pk->rsa_n, rsaverify_pk->rsa_n_len);
	if (ret != EOK) {
		hm_debug("failed to copy value for RSA param N\n");
		ret = E_HM_INVAL;
		goto free_n;
	}

	rsa_pub->key.rsaPub.eLen = (uint32_t)rsaverify_pk->rsa_e_len;
	rsa_pub->key.rsaPub.e = malloc(rsa_pub->key.rsaPub.eLen);
	if (rsa_pub->key.rsaPub.e == NULL) {
		hm_debug("failed to malloc memory for RSA param E, the length of E is %zu\n", rsaverify_pk->rsa_e_len);
		ret = E_HM_INVAL;
		goto free_n;
	}

	ret = memcpy_s(rsa_pub->key.rsaPub.e, rsa_pub->key.rsaPub.eLen, rsaverify_pk->rsa_e, rsaverify_pk->rsa_e_len);
	if (ret != EOK) {
		hm_debug("failed to copy value for RSA param E\n");
		ret = E_HM_INVAL;
		goto free_e;
	}

	return E_HM_OK;

free_e:
	free(rsa_pub->key.rsaPub.e);
	rsa_pub->key.rsaPub.e = NULL;
	rsa_pub->key.rsaPub.eLen = 0;
free_n:
	free(rsa_pub->key.rsaPub.n);
	rsa_pub->key.rsaPub.n = NULL;
	rsa_pub->key.rsaPub.nLen = 0;

	return ret;
}

static void remove_rsa_pk(CRYPT_EAL_PkeyPub *rsa_pub)
{
	if (rsa_pub == NULL) {
		return;
	}

	if (rsa_pub->key.rsaPub.n != NULL) {
		free(rsa_pub->key.rsaPub.n);
		rsa_pub->key.rsaPub.n = NULL;
		rsa_pub->key.rsaPub.nLen = 0;
	}

	if (rsa_pub->key.rsaPub.e != NULL) {
		free(rsa_pub->key.rsaPub.e);
		rsa_pub->key.rsaPub.e = NULL;
		rsa_pub->key.rsaPub.eLen = 0;
	}

	return;
}

INNER int hitls_rsaverify_addkey(hmcrypto_pkverify_ctx *ctx, hmcrypto_pkverify_pk *pk)
{
	CRYPT_EAL_PkeyPub rsa_pub = {0};
	int32_t ret = CRYPT_SUCCESS;
	CRYPT_RSA_PssPara pkey_pad = {0};
	hitls_rsaverify_private_info *private_info = NULL;

	if (ctx == NULL || ctx->private_info == NULL || pk == NULL || pk->rsaverify_pk == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	if (add_rsa_pk(&rsa_pub, pk->rsaverify_pk) != E_HM_OK) {
		hm_debug("failed to set rsa pk\n");
		return E_HM_INVAL;
	}

	private_info = (hitls_rsaverify_private_info *)ctx->private_info;
	ret = CRYPT_EAL_PkeySetPub(private_info->pk_ctx, &rsa_pub);
	remove_rsa_pk(&rsa_pub);
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to call function CRYPT_EAL_PkeySetPub, err=%ld\n", (long)ret);
		return E_HM_INVAL;
	}

	pkey_pad.saltLen = private_info->salt_len;
	pkey_pad.mdId = private_info->md_id;
	pkey_pad.mgfId = private_info->mgf_id;

	ret = CRYPT_EAL_PkeyCtrl(private_info->pk_ctx, CRYPT_CTRL_SET_RSA_EMSA_PSS,
			&pkey_pad, sizeof(CRYPT_RSA_PssPara));
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to call function CRYPT_EAL_PkeyCtrl, err=%ld\n", (long)ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER void hitls_rsaverify_removekey(hmcrypto_pkverify_ctx *ctx)
{
	(void)ctx;
}

INNER int hitls_rsaverify_init(hmcrypto_pkverify_ctx *ctx)
{
	int32_t ret = CRYPT_SUCCESS;
	hitls_rsaverify_private_info *private_info = NULL;

	if (ctx == NULL || ctx->private_info == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	private_info = (hitls_rsaverify_private_info *)ctx->private_info;
	if (private_info->hash_ctx == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = CRYPT_EAL_MdInit(private_info->hash_ctx);
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to call function CRYPT_EAL_MdInit, err=%ld\n", (long)ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int hitls_rsaverify_update(hmcrypto_pkverify_ctx *ctx, const uint8_t *data, size_t data_len)
{
	int32_t ret = CRYPT_SUCCESS;
	uint32_t data_len_t;
	hitls_rsaverify_private_info *private_info = NULL;

	if (ctx == NULL || ctx->private_info == NULL || !check_valid_data(data, data_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	private_info = (hitls_rsaverify_private_info *)ctx->private_info;
	if (private_info->hash_ctx == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	data_len_t = (uint32_t)data_len;
	ret = CRYPT_EAL_MdUpdate(private_info->hash_ctx, data, data_len_t);
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to call function CRYPT_EAL_MdUpdate, err=%ld\n", (long)ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int hitls_rsaverify_final(hmcrypto_pkverify_ctx *ctx, const uint8_t *signature, size_t signature_len)
{
	int32_t ret = CRYPT_SUCCESS;
	uint8_t digest[MAX_DIGEST_LEN] = {0};
	uint32_t digest_len = MAX_DIGEST_LEN;
	uint32_t tmp_signature_len = (uint32_t)signature_len;
	hitls_rsaverify_private_info *private_info = NULL;

	if (ctx == NULL || ctx->private_info == NULL || !check_valid_data(signature, signature_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	private_info = (hitls_rsaverify_private_info *)ctx->private_info;
	if (private_info->hash_ctx == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = CRYPT_EAL_MdFinal(private_info->hash_ctx, digest, &digest_len);
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to call function CRYPT_EAL_MdFinal, err=%ld\n", (long)ret);
		return E_HM_INVAL;
	}

	ret = CRYPT_EAL_PkeyVerifyData(private_info->pk_ctx, digest, digest_len, signature, tmp_signature_len);
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to call function CRYPT_EAL_PkeyVerifyData, err=%ld\n", (long)ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hitls_pkverify_method_init(void)
{
	static const hmcrypto_pkverify_method hitls_rsaverify_method = {
		.pkverify_newctx_f    = hitls_rsaverify_newctx,
		.pkverify_freectx_f   = hitls_rsaverify_freectx,
		.pkverify_addkey_f    = hitls_rsaverify_addkey,
		.pkverify_removekey_f = hitls_rsaverify_removekey,
		.pkverify_init_f      = hitls_rsaverify_init,
		.pkverify_update_f    = hitls_rsaverify_update,
		.pkverify_final_f     = hitls_rsaverify_final,
	};

	return hmcrypto_pkverify_method_init(&hitls_rsaverify_method);
}

void hitls_pkverify_method_fini(void)
{
	hmcrypto_pkverify_method_fini();
}
