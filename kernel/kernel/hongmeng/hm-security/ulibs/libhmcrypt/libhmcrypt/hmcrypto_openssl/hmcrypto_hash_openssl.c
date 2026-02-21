/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add hash cipher of hitls.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 11:10:24 2023
 */

#include <stdint.h>

#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>

#include <openssl/evp.h>

#include <libhmcrypt/hmcrypto_hash.h>
#include "internal_common.h"
#include "hmcrypto_hash_internal.h"
#include "hmcrypto_openssl.h"

#ifdef __HOST_LLT__
#define INNER
#else
#define INNER static
#endif

typedef struct {
	const char hash_type[CRYPTO_NAME_MAX_LEN];
	EVP_MD *evp_hash;
} openssl_hash_id_table_st;

static openssl_hash_id_table_st openssl_hash_id_table[] = {
	[HMCRYPTO_HASH_SHA2_256] = {"sha256", NULL},
	[HMCRYPTO_HASH_SM3] = {"sm3", NULL},
};

INNER int openssl_hash_newctx(hmcrypto_hash_id id, hmcrypto_hash_ctx *ctx)
{
	EVP_MD *hash = NULL;
	EVP_MD_CTX *hash_ctx = NULL;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	/* id < 0, id is nonnegative enum type */
	if ((unsigned long)id >= ARRAY_SIZE(openssl_hash_id_table)) {
		hm_debug("invalid hash id\n");
		return E_HM_INVAL;
	}

	hash = openssl_hash_id_table[id].evp_hash;
	if (hash == NULL) {
		hm_debug("hash ctx from table is NULL\n");
		return E_HM_INVAL;
	}

	hash_ctx = EVP_MD_CTX_new();
	if (hash_ctx == NULL) {
		hm_debug("failed to new hash ctx from openssl\n");
		return E_HM_INVAL;
	}

	ctx->hash_ctx = (void *)hash_ctx;
	ctx->hash = (void *)hash;

	return E_HM_OK;
}

INNER void openssl_hash_freectx(hmcrypto_hash_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	EVP_MD_CTX_free((EVP_MD_CTX *)ctx->hash_ctx);
	ctx->hash_ctx = NULL;
	/* do not need to free skcipher */
	ctx->hash = NULL;
}

INNER int openssl_hash_init(hmcrypto_hash_ctx *ctx)
{
	int ret;

	if (ctx == NULL || ctx->hash == NULL || ctx->hash_ctx == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = EVP_DigestInit_ex((EVP_MD_CTX *)ctx->hash_ctx, (EVP_MD *)ctx->hash, NULL);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function EVP_DigestInit_ex\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int openssl_hash_update(hmcrypto_hash_ctx *ctx, const uint8_t *data, size_t data_len)
{
	int ret;

	if (ctx == NULL || ctx->hash_ctx == NULL || !check_valid_data(data, data_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = EVP_DigestUpdate((EVP_MD_CTX *)ctx->hash_ctx, data, data_len);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function EVP_DigestUpdate\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int openssl_hash_final(hmcrypto_hash_ctx *ctx, uint8_t *out, size_t *out_len)
{
	int ret;
	unsigned int len;

	if (ctx == NULL || ctx->hash_ctx == NULL || out_len == NULL || !check_valid_data(out, *out_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	len = (unsigned int)*out_len;
	ret = EVP_DigestFinal_ex((EVP_MD_CTX *)ctx->hash_ctx, out, &len);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function EVP_DigestFinal_ex\n");
		return E_HM_INVAL;
	}
	*out_len = (size_t)len;

	return E_HM_OK;
}

int openssl_hash_method_init(void)
{
	size_t i;
	static hmcrypto_hash_method openssl_hash_method = {
		.hash_newctx_f	= openssl_hash_newctx,
		.hash_freectx_f	= openssl_hash_freectx,
		.hash_init_f	= openssl_hash_init,
		.hash_update_f	= openssl_hash_update,
		.hash_final_f	= openssl_hash_final,
	};

	for (i = 0; i < sizeof(openssl_hash_id_table) / sizeof(openssl_hash_id_table_st); i++) {
		if (openssl_hash_id_table[i].evp_hash != NULL) {
			continue;
		}
		openssl_hash_id_table[i].evp_hash = EVP_MD_fetch(NULL, openssl_hash_id_table[i].hash_type, NULL);
		BUG_ON(openssl_hash_id_table[i].evp_hash == NULL);
	}

	return hmcrypto_hash_method_init(&openssl_hash_method);
}

void openssl_hash_method_fini(void)
{
	size_t i;
	for (i = 0; i < sizeof(openssl_hash_id_table) / sizeof(openssl_hash_id_table_st); i++) {
		EVP_MD_free((EVP_MD *)openssl_hash_id_table[i].evp_hash);
		openssl_hash_id_table[i].evp_hash = NULL;
	}
	hmcrypto_hash_method_fini();
}
