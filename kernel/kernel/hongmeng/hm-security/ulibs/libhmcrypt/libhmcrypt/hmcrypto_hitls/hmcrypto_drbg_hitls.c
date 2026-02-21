/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add drbg of hitls.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 11:10:24 2023
 */

#include <stdint.h>

#include <libhwsecurec/securec.h>
#include <libstrict/strict.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <crypto/crypt_types.h>
#include <crypto/crypt_errno.h>
#include <crypto/crypt_eal_rand.h>
#include <bsl/bsl_sal.h>

#include <libhmcrypt/hmcrypto_drbg.h>
#include "internal_common.h"
#include "hmcrypto_drbg_internal.h"
#include "hmcrypto_hitls.h"

#ifdef __HOST_LLT__
#define INNER
#else
#define INNER static
#endif

static CRYPT_RAND_AlgId hitls_drbg_id_table[] = {
	[HMCRYPTO_DRBG_SHA2_256] = CRYPT_RAND_SHA256,
};

INNER int hitls_drbg_newctx(hmcrypto_drbg_ctx *ctx, hmcrypto_drbg_id id)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	/* id < 0, id is nonnegative enum type */
	if ((unsigned long)id >= ARRAY_SIZE(hitls_drbg_id_table)) {
		hm_debug("invalid drbg id\n");
		return E_HM_INVAL;
	}
	ctx->drbg_id = (void *)(&(hitls_drbg_id_table[id]));

	return E_HM_OK;
}

INNER void hitls_drbg_freectx(hmcrypto_drbg_ctx *ctx)
{
	if (ctx == NULL || ctx->drbg_id == NULL) {
		return;
	}

	ctx->drbg_id = NULL;

	return;
}

INNER int32_t drbg_get_entropy(void *ctx, CRYPT_Data *entropy, uint32_t strength, CRYPT_Range *len_range)
{
	uint32_t entropy_len = strength >> BIT2BYTE;
	uint8_t *entropy_data = NULL;
	size_t entropy_len_tmp = (size_t)entropy_len;
	int err = E_HM_AGAIN;
	hmcrypto_entropy_method_st *get_entropy_method;

	if (ctx == NULL || len_range == NULL || entropy == NULL) {
		return (int32_t)CRYPT_NULL_INPUT;
	}

	get_entropy_method = (hmcrypto_entropy_method_st *)ctx;

	entropy_data = malloc(entropy_len);
	if (entropy_data == NULL) {
		return (int32_t)CRYPT_DRBG_FAIL_GET_ENTROPY;
	}

	err = get_entropy_method->get_entropy_fun(get_entropy_method->entropy_ctx, entropy_data, &entropy_len_tmp);
	entropy->data = entropy_data;
	entropy->len = (uint32_t)entropy_len_tmp;

	if (err != E_HM_OK) {
		return (int32_t)CRYPT_DRBG_FAIL_GET_ENTROPY;
	}

	if (entropy_len_tmp < len_range->min || entropy_len_tmp > len_range->max) {
		return (int32_t)CRYPT_DRBG_FAIL_GET_ENTROPY;
	}

	return CRYPT_SUCCESS;
}

INNER void drbg_clear_entropy(void *ctx, CRYPT_Data *entropy)
{
	UNUSED(ctx);
	if (entropy == NULL) {
		return;
	}

	if (entropy->data != NULL) {
		free(entropy->data);
		entropy->data = NULL;
		entropy->len = 0;
	}

	return;
}

INNER int32_t drbg_get_nonce(void *ctx, CRYPT_Data *nonce, uint32_t strength, CRYPT_Range *len_range)
{
	uint8_t *nonce_data = NULL;
	uint32_t nonce_len = strength >> BIT2BYTE;
	size_t nonce_len_tmp = (size_t)nonce_len;
	int err = E_HM_AGAIN;
	hmcrypto_entropy_method_st *get_entropy_method;

	if (ctx == NULL || nonce == NULL || len_range == NULL) {
		return (int32_t)CRYPT_NULL_INPUT;
	}

	get_entropy_method = (hmcrypto_entropy_method_st *)ctx;

	nonce_data = malloc(nonce_len);
	if (nonce_data == NULL) {
		return (int32_t)CRYPT_DRBG_FAIL_GET_NONCE;
	}

	err = get_entropy_method->get_entropy_fun(get_entropy_method->entropy_ctx, nonce_data, &nonce_len_tmp);

	nonce->data = nonce_data;
	nonce->len = (uint32_t)nonce_len_tmp;

	if (err != E_HM_OK) {
		return (int32_t)CRYPT_DRBG_FAIL_GET_NONCE;
	}

	if (nonce_len_tmp < len_range->min || nonce_len_tmp > len_range->max) {
		return (int32_t)CRYPT_DRBG_FAIL_GET_NONCE;
	}

	return CRYPT_SUCCESS;
}

INNER void drbg_clear_nonce(void *ctx, CRYPT_Data *nonce)
{
	UNUSED(ctx);
	if (nonce == NULL) {
		return;
	}

	if (nonce->data != NULL) {
		free(nonce->data);
		nonce->data = NULL;
		nonce->len = 0;
	}

	return;
}

INNER int hitls_drbg_instantiate(hmcrypto_drbg_ctx *ctx, const uint8_t *pers, size_t pers_len)
{
	int32_t ret = CRYPT_SUCCESS;
	CRYPT_RandSeedMethod seed_method;
	CRYPT_RAND_AlgId drbg_id;
	uint32_t tmp_pers_len = (uint32_t)pers_len;

	if (ctx == NULL || ctx->drbg_id == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	seed_method.getEntropy = drbg_get_entropy;
	seed_method.cleanEntropy = drbg_clear_entropy;
	seed_method.getNonce = drbg_get_nonce;
	seed_method.cleanNonce = drbg_clear_nonce;

	drbg_id = *(CRYPT_RAND_AlgId *)(ctx->drbg_id);

	ret = CRYPT_EAL_RandInit(drbg_id, &seed_method, ctx->get_entropy_method, pers, tmp_pers_len);
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to initialize hitls drbg, err=%ld\n", (long)ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int hitls_drbg_uninstantiate(hmcrypto_drbg_ctx *ctx)
{
	UNUSED(ctx);
	CRYPT_EAL_RandDeinit();

	return E_HM_OK;
}

INNER int hitls_drbg_generate(hmcrypto_drbg_ctx *ctx, uint8_t *out, size_t out_len,
		uint8_t *addition, size_t addition_len)
{
	int32_t ret;
	uint32_t tmp_addition_len = (uint32_t)addition_len;
	uint32_t tmp_out_len = (uint32_t)out_len;

	if (ctx == NULL || out == NULL || out_len == 0) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = CRYPT_EAL_RandbytesWithAdin(out, tmp_out_len, addition, tmp_addition_len);
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to generate hitls drbg, err=%ld\n", (long)ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int hitls_drbg_reseed(hmcrypto_drbg_ctx *ctx, uint8_t *addition, size_t addition_len)
{
	int32_t ret;
	uint32_t tmp_addition_len = (uint32_t)addition_len;

	if (ctx == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = CRYPT_EAL_RandSeedWithAdin(addition, tmp_addition_len);
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to reseed hitls drbg, err=%ld\n", (long)ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hitls_drbg_method_init(void)
{
	static const hmcrypto_drbg_method hitls_drbg_method = {
		.drbg_newctx_f	      = hitls_drbg_newctx,
		.drbg_freectx_f	      = hitls_drbg_freectx,
		.drbg_instantiate_f   = hitls_drbg_instantiate,
		.drbg_generate_f      = hitls_drbg_generate,
		.drbg_reseed_f	      = hitls_drbg_reseed,
		.drbg_uninstantiate_f = hitls_drbg_uninstantiate,
	};

	return hmcrypto_drbg_method_init(&hitls_drbg_method);
}

void hitls_drbg_method_fini(void)
{
	hmcrypto_drbg_method_fini();
}
