/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add mac cipher of openssl.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 11:10:24 2023
 */

#include <stdint.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <openssl/evp.h>
#include <openssl/core_names.h>

#include <libhmcrypt/hmcrypto_mac.h>
#include <libhmcrypt/hmcrypto_algid.h>
#include "internal_common.h"
#include "hmcrypto_mac_internal.h"
#include "hmcrypto_openssl.h"

#ifdef __HOST_LLT__
#define INNER
#else
#define INNER static
#endif

typedef struct {
	const char mac_type[CRYPTO_NAME_MAX_LEN];
	char based_alg_type[CRYPTO_NAME_MAX_LEN];
} openssl_mac_id_table_st;

static openssl_mac_id_table_st openssl_mac_id_table[] = {
	[HMCRYPTO_HMAC_SHA2_256] = {"HMAC", "sha256"},
	[HMCRYPTO_HMAC_SHA2_512] = {"HMAC", "sha512"},
	[HMCRYPTO_SIPHASH_2_4_8] = {"SIPHASH", "siphash"},
};

INNER int openssl_mac_newctx(hmcrypto_mac_id id, hmcrypto_mac_ctx *ctx)
{
	EVP_MAC *mac = NULL;
	EVP_MAC_CTX *mctx = NULL;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	/* id < 0, id is nonnegative enum type */
	if ((unsigned long)id >= ARRAY_SIZE(openssl_mac_id_table)) {
		hm_debug("invalid mac id\n");
		return E_HM_INVAL;
	}

	ctx->mac_type = openssl_mac_id_table[id].mac_type;
	ctx->based_alg_type = openssl_mac_id_table[id].based_alg_type;
	mac = EVP_MAC_fetch(NULL, openssl_mac_id_table[id].mac_type, NULL);
	if (mac == NULL) {
		hm_debug("failed to fetch mac ctx from openssl\n");
		return E_HM_INVAL;
	}

	mctx = EVP_MAC_CTX_new(mac);
	if (mctx == NULL) {
		EVP_MAC_free(mac);
		hm_debug("failed to new mac ctx from openssl\n");
		return E_HM_INVAL;
	}

	ctx->mac = (void *)mac;
	ctx->mac_ctx = (void *)mctx;

	return E_HM_OK;
}

INNER void openssl_mac_freectx(hmcrypto_mac_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	ctx->mac_type = NULL;
	ctx->based_alg_type = NULL;
	EVP_MAC_CTX_free((EVP_MAC_CTX *)ctx->mac_ctx);
	ctx->mac_ctx = NULL;
	EVP_MAC_free((EVP_MAC *)ctx->mac);
	ctx->mac = NULL;
}

#define HMAC_PARAMS_LEN	(2)
static int openssl_mac_init_hmac(hmcrypto_mac_ctx *ctx, const uint8_t *key, size_t key_len)
{
	OSSL_PARAM params[HMAC_PARAMS_LEN];
	OSSL_PARAM *p = params;
	int ret;

	*p++ = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST,
			ctx->based_alg_type, strlen(ctx->based_alg_type));
	*p = OSSL_PARAM_construct_end();

	ret = EVP_MAC_init((EVP_MAC_CTX *)ctx->mac_ctx, key, key_len, params);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function EVP_MAC_init\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

#define SIPHASH_PARAMS_LEN (4)
static int openssl_mac_init_siphash(hmcrypto_mac_ctx *ctx, const uint8_t *key, size_t key_len)
{
	OSSL_PARAM params[SIPHASH_PARAMS_LEN];
	OSSL_PARAM *p = params;
	int ret;

	/*
	 * The siphash is a special MAC algorithm, it can be expressed in general
	 * as siphash-c-d(k, m) and the digest len may be 8 or 16 bytes. In default
	 * case, c = 2, d = 4 and digest len is 8.
	 */
	unsigned int c_rounds = 2; // 2 is the default param c in siphash
	unsigned int d_rounds = 4; // 4 is the default param d in siphash
	unsigned int digest_len = 8; // 8 is the default digest len in siphash

	*p++ = OSSL_PARAM_construct_uint(OSSL_MAC_PARAM_SIZE, &digest_len);
	*p++ = OSSL_PARAM_construct_uint(OSSL_MAC_PARAM_C_ROUNDS, &c_rounds);
	*p++ = OSSL_PARAM_construct_uint(OSSL_MAC_PARAM_D_ROUNDS, &d_rounds);
	*p = OSSL_PARAM_construct_end();

	ret = EVP_MAC_init((EVP_MAC_CTX *)ctx->mac_ctx, key, key_len, params);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function EVP_MAC_init\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int openssl_mac_init(hmcrypto_mac_ctx *ctx, const uint8_t *key, size_t key_len)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->mac_ctx == NULL || !check_valid_data(key, key_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	if (strcmp(ctx->mac_type, "HMAC") == 0) {
		ret = openssl_mac_init_hmac(ctx, key, key_len);
	} else if (strcmp(ctx->mac_type, "SIPHASH") == 0) {
		ret = openssl_mac_init_siphash(ctx, key, key_len);
	} else {
		ret = E_HM_INVAL;
		hm_debug("unsupport MAC type\n");
	}

	return ret;
}

INNER int openssl_mac_update(hmcrypto_mac_ctx *ctx, const uint8_t *data, size_t data_len)
{
	int ret;

	if (ctx == NULL || ctx->mac_ctx == NULL || !check_valid_data(data, data_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = EVP_MAC_update((EVP_MAC_CTX *)ctx->mac_ctx, data, data_len);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function EVP_MAC_update\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int openssl_mac_final(hmcrypto_mac_ctx *ctx, uint8_t *out, size_t *out_len)
{
	int ret;

	if (ctx == NULL || ctx->mac_ctx == NULL || out_len == NULL || !check_valid_data(out, *out_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = EVP_MAC_final((EVP_MAC_CTX *)ctx->mac_ctx, out, out_len, *out_len);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function EVP_MAC_final\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int openssl_mac_method_init(void)
{
	static hmcrypto_mac_method openssl_mac_method = {
		.mac_newctx_f  = openssl_mac_newctx,
		.mac_freectx_f = openssl_mac_freectx,
		.mac_init_f    = openssl_mac_init,
		.mac_update_f  = openssl_mac_update,
		.mac_final_f   = openssl_mac_final,
	};

	return hmcrypto_mac_method_init(&openssl_mac_method);
}

void openssl_mac_method_fini(void)
{
	hmcrypto_mac_method_fini();
}
