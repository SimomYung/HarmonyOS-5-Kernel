/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add hash cipher of hitls.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 11:10:24 2023
 */

#include <stdint.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <crypto/crypt_algid.h>
#include <crypto/crypt_eal_md.h>
#include <crypto/crypt_errno.h>

#include <libhmcrypt/hmcrypto_hash.h>
#include "internal_common.h"
#include "hmcrypto_hash_internal.h"
#include "hmcrypto_hitls.h"

#ifdef __HOST_LLT__
#define INNER
#else
#define INNER static
#endif

static CRYPT_MD_AlgId hitls_hash_id_table[] = {
	[HMCRYPTO_HASH_SHA2_256] = CRYPT_MD_SHA256,
};

INNER int hitls_hash_newctx(hmcrypto_hash_id id, hmcrypto_hash_ctx *ctx)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	/* id < 0, id is nonnegative enum type */
	if ((unsigned long)id >= ARRAY_SIZE(hitls_hash_id_table)) {
		hm_debug("invalid drbg id\n");
		return E_HM_INVAL;
	}

	ctx->hash_ctx = (void *)CRYPT_EAL_MdNewCtx(hitls_hash_id_table[id]);
	if (ctx->hash_ctx == NULL) {
		hm_debug("failed to get cipher ctx from hitls\n");
		return E_HM_INVAL;
	}

	ctx->hash = NULL;
	ctx->id = id;

	return E_HM_OK;
}

INNER void hitls_hash_freectx(hmcrypto_hash_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	CRYPT_EAL_MdFreeCtx((CRYPT_EAL_MdCTX *)ctx->hash_ctx);
}

INNER int hitls_hash_init(hmcrypto_hash_ctx *ctx)
{
	int32_t ret = CRYPT_SUCCESS;

	if (ctx == NULL || ctx->hash_ctx == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = CRYPT_EAL_MdInit((CRYPT_EAL_MdCTX *)ctx->hash_ctx);
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to call function CRYPT_EAL_MdInit, ret = %ld\n", (long)ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int hitls_hash_update(hmcrypto_hash_ctx *ctx, const uint8_t *data, size_t data_len)
{
	int32_t ret = CRYPT_SUCCESS;
	uint32_t data_len_t;

	if (ctx == NULL || ctx->hash_ctx == NULL || !check_valid_data(data, data_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	data_len_t = (uint32_t)data_len;
	ret = CRYPT_EAL_MdUpdate((CRYPT_EAL_MdCTX *)ctx->hash_ctx, data, data_len_t);
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to call function CRYPT_EAL_MdUpdate, ret = %ld\n", (long)ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int hitls_hash_final(hmcrypto_hash_ctx *ctx, uint8_t *out, size_t *out_len)
{
	int32_t ret = CRYPT_SUCCESS;
	uint32_t out_len_t;

	if (ctx == NULL || ctx->hash_ctx == NULL || out_len == NULL || !check_valid_data(out, *out_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	out_len_t = (uint32_t)*out_len;
	ret = CRYPT_EAL_MdFinal((CRYPT_EAL_MdCTX *)ctx->hash_ctx, out, &out_len_t);
	if (ret != CRYPT_SUCCESS) {
		hm_debug("failed to call function CRYPT_EAL_MdFinal, ret = %ld\n", (long)ret);
		return E_HM_INVAL;
	}

	*out_len = (size_t)out_len_t;

	return E_HM_OK;
}

int hitls_hash_method_init(void)
{
	static hmcrypto_hash_method hitls_hash_method = {
		.hash_newctx_f	= hitls_hash_newctx,
		.hash_freectx_f	= hitls_hash_freectx,
		.hash_init_f	= hitls_hash_init,
		.hash_update_f	= hitls_hash_update,
		.hash_final_f	= hitls_hash_final,
	};

	return hmcrypto_hash_method_init(&hitls_hash_method);
}

void hitls_hash_method_fini(void)
{
	hmcrypto_hash_method_fini();
}
