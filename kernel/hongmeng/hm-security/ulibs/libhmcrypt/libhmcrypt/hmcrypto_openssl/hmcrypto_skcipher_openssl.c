/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add skcipher of openssl.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 8 11:10:24 2023
 */

#include <stdint.h>

#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>

#include <openssl/evp.h>
#include <openssl/core_names.h>

#include <libhmcrypt/hmcrypto_skcipher.h>
#include <libhmcrypt/hmcrypto_algid.h>
#include "internal_common.h"
#include "hmcrypto_skcipher_internal.h"
#include "hmcrypto_openssl.h"

#ifdef __HOST_LLT__
#define INNER
#else
#define INNER static
#endif

typedef struct {
	const char skcipher_type[CRYPTO_NAME_MAX_LEN];
	skcipher_mode_id mode_id;
	EVP_CIPHER *evp_cipher;
} openssl_skcipher_id_table_st;

static openssl_skcipher_id_table_st openssl_skcipher_id_table[] = {
	[HMCRYPTO_SKCIPHER_AES128_ECB] = {"AES-128-ECB", SKCIPHER_ECB_MODE, NULL},
	[HMCRYPTO_SKCIPHER_AES128_CBC] = {"AES-128-CBC", SKCIPHER_CBC_MODE, NULL},
	[HMCRYPTO_SKCIPHER_AES256_XTS] = {"AES-256-XTS", SKCIPHER_XTS_MODE, NULL},
	[HMCRYPTO_SKCIPHER_AES128_CTS] = {"AES-128-CBC-CTS", SKCIPHER_CTS_MODE, NULL},
	[HMCRYPTO_SKCIPHER_AES256_CTS] = {"AES-256-CBC-CTS", SKCIPHER_CTS_MODE, NULL},
	[HMCRYPTO_SKCIPHER_AES256_GCM] = {"AES-256-GCM", SKCIPHER_GCM_MODE, NULL},
	[HMCRYPTO_SKCIPHER_AES256_CTR] = {"AES-256-CTR", SKCIPHER_CTR_MODE, NULL},
};

INNER int openssl_skcipher_newctx(hmcrypto_skcipher_id id, hmcrypto_skcipher_ctx *ctx)
{
	EVP_CIPHER *cipher = NULL;
	EVP_CIPHER_CTX *cipher_ctx = NULL;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	/* id < 0, id is nonnegative enum type */
	if ((unsigned long)id >= ARRAY_SIZE(openssl_skcipher_id_table)) {
		hm_debug("invalid skcipher id\n");
		return E_HM_INVAL;
	}

	cipher = openssl_skcipher_id_table[id].evp_cipher;
	if (cipher == NULL) {
		hm_debug("skcipher ctx from table is NULL\n");
		return E_HM_INVAL;
	}

	cipher_ctx = EVP_CIPHER_CTX_new();
	if (cipher_ctx == NULL) {
		hm_debug("failed to new skcipher ctx from openssl\n");
		return E_HM_INVAL;
	}

	ctx->skcipher = (void *)cipher;
	ctx->skcipher_ctx = (void *)cipher_ctx;
	ctx->mode_id = openssl_skcipher_id_table[id].mode_id;

	return E_HM_OK;
}

INNER void openssl_skcipher_freectx(hmcrypto_skcipher_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	EVP_CIPHER_CTX_free((EVP_CIPHER_CTX *)ctx->skcipher_ctx);
	ctx->skcipher_ctx = NULL;
	/* do not need to free skcipher */
	ctx->skcipher = NULL;
}

INNER int openssl_skcipher_set_nopadding(hmcrypto_skcipher_ctx *ctx)
{
	int ret;

	ret = EVP_CIPHER_CTX_set_padding((EVP_CIPHER_CTX *)ctx->skcipher_ctx, 0);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to set skcipher no padding\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

#define OPENSSL_CTS_MODE_PARAM_LEN	(2)
INNER int openssl_skcipher_set_cts_mode_param(hmcrypto_skcipher_ctx *ctx)
{
	int ret;
	OSSL_PARAM params[OPENSSL_CTS_MODE_PARAM_LEN];
	hmcrypto_skcipher_cts_mode_id cts_mode_id;
	if (ctx->params == NULL) {
		cts_mode_id = HMCRYPTO_SKCIPHER_CTS_CS3;
	} else {
		cts_mode_id = ctx->params->cts_mode_id;
	}

	if (cts_mode_id == HMCRYPTO_SKCIPHER_CTS_CS1) {
		params[0] = OSSL_PARAM_construct_utf8_string(OSSL_CIPHER_PARAM_CTS_MODE, "CS1", 0);
	} else if (cts_mode_id == HMCRYPTO_SKCIPHER_CTS_CS2) {
		params[0] = OSSL_PARAM_construct_utf8_string(OSSL_CIPHER_PARAM_CTS_MODE, "CS2", 0);
	} else {
		params[0] = OSSL_PARAM_construct_utf8_string(OSSL_CIPHER_PARAM_CTS_MODE, "CS3", 0);
	}

	params[1] = OSSL_PARAM_construct_end();
	ret = EVP_CIPHER_CTX_set_params((EVP_CIPHER_CTX *)ctx->skcipher_ctx, params);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to set skcipher cts mode param\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int openssl_skcipher_handle_params(hmcrypto_skcipher_ctx *ctx)
{
	int ret = E_HM_OK;

	if (ctx == NULL) {
		return E_HM_INVAL;
	}

	if (ctx->mode_id == SKCIPHER_CTS_MODE) {
		ret = openssl_skcipher_set_cts_mode_param(ctx);
	}

	if (ret == E_HM_OK && ctx->params != NULL && ctx->params->flag_no_padding) {
		ret = openssl_skcipher_set_nopadding(ctx);
	}

	return ret;
}

INNER int openssl_skcipher_encinit(hmcrypto_skcipher_ctx *ctx, const uint8_t *key,
		size_t key_len, const uint8_t *iv, size_t iv_len)
{
	int ret;
	OSSL_PARAM params[2] = {OSSL_PARAM_END, OSSL_PARAM_END};

	if (ctx == NULL || !check_valid_data(key, key_len) || !check_valid_data(iv, iv_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	if (ctx->mode_id == SKCIPHER_GCM_MODE) {
		params[0] = OSSL_PARAM_construct_size_t(OSSL_CIPHER_PARAM_AEAD_IVLEN, &iv_len);
		ret = EVP_CIPHER_CTX_set_params((EVP_CIPHER_CTX *)ctx->skcipher_ctx, params);
		if (ret != OPENSSL_SUCCESS) {
			hm_debug("failed to set iv len for gcm mode\n");
			return E_HM_INVAL;
		}
	}

	ret = EVP_EncryptInit_ex((EVP_CIPHER_CTX *)ctx->skcipher_ctx, (EVP_CIPHER *)ctx->skcipher, NULL, key, iv);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function EVP_EncryptInit_ex, err = %d\n", ret);
		return E_HM_INVAL;
	}

	ret = openssl_skcipher_handle_params(ctx);
	if (ret != E_HM_OK) {
		hm_debug("failed to handle skcipher params\n");
		return E_HM_INVAL;
	}

	ctx->enc = true;

	return E_HM_OK;
}

INNER int openssl_skcipher_decinit(hmcrypto_skcipher_ctx *ctx, const uint8_t *key,
		size_t key_len, const uint8_t *iv, size_t iv_len)
{
	OSSL_PARAM params[2] = {OSSL_PARAM_END, OSSL_PARAM_END};
	int ret;

	if (ctx == NULL || !check_valid_data(iv, iv_len) || !check_valid_data(key, key_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	if (ctx->mode_id == SKCIPHER_GCM_MODE) {
		params[0] = OSSL_PARAM_construct_size_t(OSSL_CIPHER_PARAM_AEAD_IVLEN, &iv_len);
		ret = EVP_CIPHER_CTX_set_params((EVP_CIPHER_CTX *)ctx->skcipher_ctx, params);
		if (ret != OPENSSL_SUCCESS) {
			hm_debug("failed to set iv len for gcm mode in decrypt\n");
			return E_HM_INVAL;
		}
	}

	ret = EVP_DecryptInit_ex((EVP_CIPHER_CTX *)ctx->skcipher_ctx, (EVP_CIPHER *)ctx->skcipher, NULL, key, iv);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function cipher init, err = %d\n", ret);
		return E_HM_INVAL;
	}

	ret = openssl_skcipher_handle_params(ctx);
	if (ret != E_HM_OK) {
		hm_debug("failed to handle skcipher params\n");
		return E_HM_INVAL;
	}

	ctx->enc = false;

	return E_HM_OK;
}

INNER int openssl_skcipher_init(hmcrypto_skcipher_ctx *ctx, const uint8_t *iv, size_t iv_len, bool enc)
{
	int ret;
	OSSL_PARAM params[2] = {OSSL_PARAM_END, OSSL_PARAM_END};
	OSSL_PARAM *p = NULL;

	if (ctx == NULL || !check_valid_data(iv, iv_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	if (ctx->mode_id == SKCIPHER_GCM_MODE) {
		params[0] = OSSL_PARAM_construct_size_t(OSSL_CIPHER_PARAM_AEAD_IVLEN, &iv_len);
		p = params;
	}

	if (enc) {
		ret = EVP_EncryptInit_ex2((EVP_CIPHER_CTX *)ctx->skcipher_ctx, (EVP_CIPHER *)ctx->skcipher, ctx->key, iv, p);
	} else {
		ret = EVP_DecryptInit_ex2((EVP_CIPHER_CTX *)ctx->skcipher_ctx, (EVP_CIPHER *)ctx->skcipher, ctx->key, iv, p);
	}
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function EVP_EncryptInit_ex, err = %d\n", ret);
		return E_HM_INVAL;
	}

	ret = openssl_skcipher_handle_params(ctx);
	if (ret != E_HM_OK) {
		hm_debug("failed to handle skcipher params\n");
		return E_HM_INVAL;
	}

	ctx->enc = enc;

	return E_HM_OK;
}

INNER void openssl_skcipher_fini(hmcrypto_skcipher_ctx *ctx)
{
	(void)ctx;
	return;
}

static int openssl_skcipher_update_gcm(hmcrypto_skcipher_ctx *ctx)
{
	int ret;
	int out_len = 0;

	if (ctx->enc) {
		ret = EVP_EncryptUpdate((EVP_CIPHER_CTX *)ctx->skcipher_ctx, NULL, &out_len, ctx->aad, (int)(ctx->aad_len));
	} else {
		ret = EVP_DecryptUpdate((EVP_CIPHER_CTX *)ctx->skcipher_ctx, NULL, &out_len, ctx->aad, (int)(ctx->aad_len));
	}

	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to add aad for gcm mode, err=%d\n", ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

#define TMP_HATA_LEN	(16)
INNER int openssl_skcipher_update(hmcrypto_skcipher_ctx *ctx, const uint8_t *input, size_t input_len,
		uint8_t *output, size_t *output_len)
{
	int ret;
	int inlen;
	int outlen;
	uint8_t tmp_input[TMP_HATA_LEN] = {0};

	if (ctx == NULL || !check_valid_data(input, input_len) || output_len == NULL ||
			!check_valid_data(output, *output_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	if (ctx->mode_id == SKCIPHER_GCM_MODE && ctx->aad != NULL) {
		ret = openssl_skcipher_update_gcm(ctx);
		if (ret != E_HM_OK) {
			hm_debug("failed to add aad for gcm mode\n");
			return E_HM_INVAL;
		}
	}

	inlen = (int)input_len;
	outlen = (int)*output_len;
	if (ctx->enc) {
		if ((ctx->mode_id == SKCIPHER_CTS_MODE) && (inlen < TMP_HATA_LEN)) {
			NOFAIL(memcpy_s(tmp_input, TMP_HATA_LEN, input, input_len));
			ret = EVP_EncryptUpdate((EVP_CIPHER_CTX *)ctx->skcipher_ctx, output, &outlen, tmp_input, TMP_HATA_LEN);
		} else {
			ret = EVP_EncryptUpdate((EVP_CIPHER_CTX *)ctx->skcipher_ctx, output, &outlen, input, inlen);
		}
	} else {
		ret = EVP_DecryptUpdate((EVP_CIPHER_CTX *)ctx->skcipher_ctx, output, &outlen, input, inlen);
	}
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function skcipher update, err=%d\n", ret);
		return E_HM_INVAL;
	}
	*output_len = (size_t)(unsigned int)outlen;

	return E_HM_OK;
}

INNER int openssl_skcipher_final(hmcrypto_skcipher_ctx *ctx, uint8_t *output, size_t *output_len)
{
	int ret;
	int outlen;

	if (ctx == NULL || output_len == NULL || !check_valid_data(output, *output_len)) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	outlen = (int)*output_len;
	if (ctx->enc) {
		ret = EVP_EncryptFinal_ex((EVP_CIPHER_CTX *)ctx->skcipher_ctx, output, &outlen);
	} else {
		ret = EVP_DecryptFinal_ex((EVP_CIPHER_CTX *)ctx->skcipher_ctx, output, &outlen);
	}
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to call function skcipher final, err = %d\n", ret);
		return E_HM_INVAL;
	}

	if ((ctx->mode_id == SKCIPHER_CTS_MODE) && (!ctx->enc) && (outlen == TMP_HATA_LEN)) {
		while (output[outlen - 1] == 0) {
			outlen --;
		}
	}

	*output_len = (size_t)(unsigned int)outlen;

	return E_HM_OK;
}

INNER int openssl_skcipher_settag(hmcrypto_skcipher_ctx *ctx, uint8_t *tag, size_t tag_len)
{
	int ret;
	OSSL_PARAM params[2] = {OSSL_PARAM_END, OSSL_PARAM_END};

	if (ctx == NULL || tag == NULL || tag_len == 0) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	params[0] = OSSL_PARAM_construct_octet_string(OSSL_CIPHER_PARAM_AEAD_TAG, (void*)tag, tag_len);
	ret = EVP_CIPHER_CTX_set_params((EVP_CIPHER_CTX *)ctx->skcipher_ctx, params);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to set gcm tag, err=%d\n", ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int openssl_skcipher_gettag(hmcrypto_skcipher_ctx *ctx, uint8_t *tag, size_t tag_len)
{
	int ret;
	OSSL_PARAM params[2] = {OSSL_PARAM_END, OSSL_PARAM_END};

	if (ctx == NULL || tag == NULL || tag_len == 0) {
		hm_debug("invalid input params\n");
		return E_HM_INVAL;
	}

	params[0] = OSSL_PARAM_construct_octet_string(OSSL_CIPHER_PARAM_AEAD_TAG, (void*)tag, tag_len);
	ret = EVP_CIPHER_CTX_get_params((EVP_CIPHER_CTX *)ctx->skcipher_ctx, params);
	if (ret != OPENSSL_SUCCESS) {
		hm_debug("failed to set gcm tag, err=%d\n", ret);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}
int openssl_skcipher_method_init(void)
{
	size_t i;
	static hmcrypto_skcipher_method openssl_skcipher_method = {
		.skcipher_newctx_f  = openssl_skcipher_newctx,
		.skcipher_freectx_f = openssl_skcipher_freectx,
		.skcipher_encinit_f = openssl_skcipher_encinit,
		.skcipher_decinit_f = openssl_skcipher_decinit,
		.skcipher_init_f    = openssl_skcipher_init,
		.skcipher_fini_f    = openssl_skcipher_fini,
		.skcipher_update_f  = openssl_skcipher_update,
		.skcipher_final_f   = openssl_skcipher_final,
		.skcipher_settag_f  = openssl_skcipher_settag,
		.skcipher_gettag_f  = openssl_skcipher_gettag,
	};

	for (i = 0; i < sizeof(openssl_skcipher_id_table) / sizeof(openssl_skcipher_id_table_st); i++) {
		if (openssl_skcipher_id_table[i].evp_cipher != NULL) {
			continue;
		}
		openssl_skcipher_id_table[i].evp_cipher =
			EVP_CIPHER_fetch(NULL, openssl_skcipher_id_table[i].skcipher_type, NULL);
		BUG_ON(openssl_skcipher_id_table[i].evp_cipher == NULL);
	}

	return hmcrypto_skcipher_method_init(&openssl_skcipher_method);
}

void openssl_skcipher_method_fini(void)
{
	size_t i;
	for (i = 0; i < sizeof(openssl_skcipher_id_table) / sizeof(openssl_skcipher_id_table_st); i++) {
		EVP_CIPHER_free((EVP_CIPHER *)openssl_skcipher_id_table[i].evp_cipher);
		openssl_skcipher_id_table[i].evp_cipher = NULL;
	}
	hmcrypto_skcipher_method_fini();
}
