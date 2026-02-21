/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add drbg of openssl.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 11:10:24 2023
 */

#include <stdint.h>

#include <libhwsecurec/securec.h>
#include <libstrict/strict.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <openssl/evp.h>
#include <openssl/core_names.h>
#include <openssl/params.h>
#include <openssl/types.h>
#include <openssl/rand.h>
#include <openssl/provider.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/proverr.h>

#include <libhmcrypt/hmcrypto_drbg.h>
#include "internal_common.h"
#include "hmcrypto_drbg_internal.h"
#include "hmcrypto_openssl.h"

#ifdef __HOST_LLT__
#define INNER
#else
#define INNER static
#endif

typedef struct {
	const char drbg_type[CRYPTO_NAME_MAX_LEN];
	char based_alg_type[CRYPTO_NAME_MAX_LEN];
	unsigned int strength;
} openssl_drbg_id_table_st;

static openssl_drbg_id_table_st openssl_drbg_id_table[] = {
	[HMCRYPTO_DRBG_SHA2_256] = {"HASH-DRBG", SN_sha256, 256}, // 256 is the strength of DRBG-HASH-SHA256
};

INNER EVP_RAND_CTX *new_rand_parent(void)
{
	OSSL_LIB_CTX *libctx = NULL;
	EVP_RAND *seed = NULL;
	EVP_RAND_CTX *parent = NULL;

	libctx = (OSSL_LIB_CTX *)openssl_provider_get();
	if (libctx == NULL) {
		hm_warn("openssl_provider_get return NULL, use openssl default seed source\n");
		return NULL;
	}

	seed = EVP_RAND_fetch(libctx, HM_SEED, NULL);
	if (seed == NULL) {
		hm_debug("failed to fetch rand seed\n");
		return NULL;
	}

	parent = EVP_RAND_CTX_new(seed, NULL);
	EVP_RAND_free(seed);

	return parent;
}

INNER void free_rand_parent(EVP_RAND_CTX *parent)
{
	if (parent == NULL) {
		return;
	}

	EVP_RAND_CTX_free(parent);
}

INNER int openssl_drbg_newctx(hmcrypto_drbg_ctx *ctx, hmcrypto_drbg_id id)
{
	const char *drbg_type = NULL;
	EVP_RAND_CTX *parent = NULL;
	EVP_RAND *rand = NULL;
	EVP_RAND_CTX *rand_ctx = NULL;

	if (ctx == NULL || ctx->get_entropy_method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	/* id < 0, id is nonnegative enum type */
	if ((unsigned long)id >= ARRAY_SIZE(openssl_drbg_id_table)) {
		hm_debug("invalid drbg id\n");
		return E_HM_INVAL;
	}

	ctx->drbg_id = openssl_drbg_id_table[id].based_alg_type;
	drbg_type = openssl_drbg_id_table[id].drbg_type;
	ctx->strength = openssl_drbg_id_table[id].strength;

	parent = new_rand_parent();

	rand = EVP_RAND_fetch(NULL, drbg_type, NULL);
	if (rand == NULL) {
		hm_debug("failed to fetch rand evp\n");
		free_rand_parent(parent);
		return E_HM_INVAL;
	}

	rand_ctx = EVP_RAND_CTX_new(rand, parent);
	EVP_RAND_free(rand);
	if (rand_ctx == NULL) {
		hm_debug("failed to new rand ctx\n");
		free_rand_parent(parent);
		return E_HM_INVAL;
	}

	ctx->drbg = (void *)rand_ctx;
	ctx->drbg_ctx = (void *)parent;

	return E_HM_OK;
}

INNER void openssl_drbg_freectx(hmcrypto_drbg_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	EVP_RAND_CTX_free((EVP_RAND_CTX *)ctx->drbg);
	ctx->drbg = NULL;
	free_rand_parent((EVP_RAND_CTX *)ctx->drbg_ctx);
	ctx->drbg_ctx = NULL;

	return;
}

INNER int openssl_drbg_instantiate(hmcrypto_drbg_ctx *ctx, const uint8_t *pers, size_t pers_len)
{
	int ret;
	OSSL_PARAM params[2] = {0}; // 2 is the size of params
	OSSL_PARAM *p = params;

	if (ctx == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	*p++ = OSSL_PARAM_construct_utf8_string(OSSL_DRBG_PARAM_DIGEST, (char *)ctx->drbg_id, 0);
	*p = OSSL_PARAM_construct_end();
	ret = EVP_RAND_instantiate((EVP_RAND_CTX *)ctx->drbg, (unsigned int)ctx->strength, 0, pers, pers_len, params);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to instantiate openssl drbg\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int openssl_drbg_uninstantiate(hmcrypto_drbg_ctx *ctx)
{
	int ret;

	if (ctx == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = EVP_RAND_uninstantiate((EVP_RAND_CTX *)ctx->drbg);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to uninstantiate openssl drbg\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int openssl_drbg_generate(hmcrypto_drbg_ctx *ctx, uint8_t *out, size_t out_len,
		uint8_t *addition, size_t addition_len)
{
	int ret;

	if (ctx == NULL || out == NULL || out_len == 0) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	ret = EVP_RAND_generate((EVP_RAND_CTX *)ctx->drbg, out, out_len,
			(unsigned int)ctx->strength, 0, addition, addition_len);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call openssl drbg to generate data\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

#define MAX_ENTROPY_LEN	(32)
INNER int openssl_drbg_reseed(hmcrypto_drbg_ctx *ctx, uint8_t *addition, size_t addition_len)
{
	int ret;
	size_t entropy_len;
	uint8_t entropy[MAX_ENTROPY_LEN] = {0};

	if (ctx == NULL) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	entropy_len = (size_t)((ctx->strength) >> BIT2BYTE);

	ret = ctx->get_entropy_method->get_entropy_fun(ctx->get_entropy_method->entropy_ctx, entropy, &entropy_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to get data for entropy\n");
		return E_HM_INVAL;
	}

	ret = EVP_RAND_reseed((EVP_RAND_CTX *)ctx->drbg, 0, entropy, entropy_len, addition, addition_len);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call openssl drbg to reseed\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int openssl_drbg_method_init(void)
{
	static const hmcrypto_drbg_method openssl_drbg_method = {
		.drbg_newctx_f	      = openssl_drbg_newctx,
		.drbg_freectx_f	      = openssl_drbg_freectx,
		.drbg_instantiate_f   = openssl_drbg_instantiate,
		.drbg_generate_f      = openssl_drbg_generate,
		.drbg_reseed_f	      = openssl_drbg_reseed,
		.drbg_uninstantiate_f = openssl_drbg_uninstantiate,
	};

	return hmcrypto_drbg_method_init(&openssl_drbg_method);
}

void openssl_drbg_method_fini(void)
{
	hmcrypto_drbg_method_fini();
}
