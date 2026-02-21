/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: Framework of symmetric key ciphers.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 28 16:10:24 2022
 */
#include <stdlib.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <libhwsecurec/securec.h>

#include <libhmcrypt/hmcrypto_skcipher.h>
#include <libhmcrypt/hmcrypto_algid.h>
#include "internal_common.h"
#include "hmcrypto_skcipher_internal.h"

DEFINE_ULIBS_MCACHE(hmcrypto_skcipher_ctx, sizeof(hmcrypto_skcipher_ctx))
DEFINE_ULIBS_MCACHE_VARIANT(security_variant)

static hmcrypto_skcipher_method *g_skcipher_method = NULL;

int hmcrypto_skcipher_method_init(hmcrypto_skcipher_method *skcipher_method)
{
	if (skcipher_method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	g_skcipher_method = skcipher_method;

	return E_HM_OK;
}

void hmcrypto_skcipher_method_fini(void)
{
	g_skcipher_method = NULL;
}

hmcrypto_skcipher_ctx *hmcrypto_skcipher_newctx(hmcrypto_skcipher_id id, hmcrypto_skcipher_params *params)
{
	hmcrypto_skcipher_ctx *skcipher_ctx = NULL;
	int ret = E_HM_INVAL;

	skcipher_ctx = (hmcrypto_skcipher_ctx *)mcache_hmcrypto_skcipher_ctx_malloc();
	if (skcipher_ctx == NULL) {
		hm_debug("failed to malloc mem\n");
		return NULL;
	}

	ret = g_skcipher_method->skcipher_newctx_f(id, skcipher_ctx);
	if (ret != E_HM_OK) {
		mcache_hmcrypto_skcipher_ctx_free(skcipher_ctx);
		return NULL;
	}

	skcipher_ctx->id = id;
	skcipher_ctx->params = params;
	skcipher_ctx->method = g_skcipher_method;
	skcipher_ctx->aad = NULL;
	skcipher_ctx->key = NULL;

	return skcipher_ctx;
}

void hmcrypto_skcipher_freectx(hmcrypto_skcipher_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	hmcrypto_skcipher_removeaad(ctx);
	hmcrypto_skcipher_removekey(ctx);

	ctx->method->skcipher_freectx_f(ctx);
	ctx->method = NULL;
	mcache_hmcrypto_skcipher_ctx_free(ctx);
}

int hmcrypto_skcipher_encinit(hmcrypto_skcipher_ctx *ctx, const uint8_t *key,
			      size_t key_len, const uint8_t *iv, size_t iv_len)
{
	int ret;

	if (ctx == NULL || !check_valid_data(key, key_len) || !check_valid_data(iv, iv_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->skcipher_encinit_f(ctx, key, key_len, iv, iv_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for encrypt init\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_skcipher_decinit(hmcrypto_skcipher_ctx *ctx, const uint8_t *key,
			      size_t key_len, const uint8_t *iv, size_t iv_len)
{
	int ret;

	if (ctx == NULL || !check_valid_data(key, key_len) || !check_valid_data(iv, iv_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->skcipher_decinit_f(ctx, key, key_len, iv, iv_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for decrypt init\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_skcipher_addkey(hmcrypto_skcipher_ctx *ctx, const uint8_t *key, size_t key_len)
{
	int ret;

	if (ctx == NULL || key == NULL || key_len == 0) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ctx->key_len = key_len;
	ctx->key = (uint8_t *)mcache_security_variant_malloc(ctx->key_len);
	if (ctx->key == NULL) {
		hm_debug("failed to malloc memory for skcipher key\n");
		return E_HM_INVAL;
	}

	ret = memcpy_s(ctx->key, key_len, key, key_len);
	if (ret != EOK) {
		hm_debug("failed to assign vaule for skcipher key\n");
		hmcrypto_skcipher_removekey(ctx);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

void hmcrypto_skcipher_removekey(hmcrypto_skcipher_ctx *ctx)
{
	if (ctx == NULL || ctx->key == NULL) {
		return;
	}

	NOFAIL(memset_s(ctx->key, ctx->key_len, 0, ctx->key_len));
	mcache_security_variant_free(ctx->key);
	ctx->key = NULL;
	ctx->key_len = 0;
}

int hmcrypto_skcipher_addaad(hmcrypto_skcipher_ctx *ctx, const uint8_t *aad, size_t aad_len)
{
	int ret;

	if (ctx == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	if (aad == NULL) {
		return E_HM_OK;
	}

	ctx->aad_len = aad_len;
	ctx->aad = (uint8_t *)malloc(ctx->aad_len);
	if (ctx->aad == NULL || ctx->aad_len == 0) {
		hm_debug("failed to malloc memory for skcipher aad\n");
		return E_HM_INVAL;
	}

	ret = memcpy_s(ctx->aad, aad_len, aad, aad_len);
	if (ret != EOK) {
		hm_debug("failed to assign vaule for skcipher aad\n");
		hmcrypto_skcipher_removeaad(ctx);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

void hmcrypto_skcipher_removeaad(hmcrypto_skcipher_ctx *ctx)
{
	if (ctx == NULL || ctx->aad == NULL) {
		return;
	}

	NOFAIL(memset_s(ctx->aad, ctx->aad_len, 0, ctx->aad_len));
	free(ctx->aad);
	ctx->aad = NULL;
	ctx->aad_len = 0;
}

int hmcrypto_skcipher_init(hmcrypto_skcipher_ctx *ctx, const uint8_t *iv, size_t iv_len, bool enc)
{
	int ret;

	if (ctx == NULL || !check_valid_data(iv, iv_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->skcipher_init_f(ctx, iv, iv_len, enc);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for downinit\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

void hmcrypto_skcipher_fini(hmcrypto_skcipher_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	ctx->method->skcipher_fini_f(ctx);
}

int hmcrypto_skcipher_update(hmcrypto_skcipher_ctx *ctx, const uint8_t *input,
			     size_t input_len, uint8_t *output, size_t *output_len)
{
	int ret;

	if (ctx == NULL || !check_valid_data(input, input_len) || output_len == NULL ||
			!check_valid_data(output, *output_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->skcipher_update_f(ctx, input, input_len, output, output_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for skcipher update\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_skcipher_final(hmcrypto_skcipher_ctx *ctx, uint8_t *output, size_t *output_len)
{
	int ret;

	if (ctx == NULL || output_len == NULL || !check_valid_data(output, *output_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->skcipher_final_f(ctx, output, output_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for skcipher final\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_skcipher_settag(hmcrypto_skcipher_ctx *ctx, uint8_t *tag, size_t tag_len)
{
	int ret;

	if (ctx == NULL || tag == NULL || tag_len == 0) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->skcipher_settag_f(ctx, tag, tag_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to set tag for skcipher gcm mode\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_skcipher_gettag(hmcrypto_skcipher_ctx *ctx, uint8_t *tag, size_t tag_len)
{
	int ret;

	if (ctx == NULL || tag == NULL || tag_len == 0) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->skcipher_gettag_f(ctx, tag, tag_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to set tag for skcipher gcm mode\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_skcipher_encrypt(hmcrypto_skcipher_id id, const hmcrypto_data *key,
			      const hmcrypto_data *iv, const hmcrypto_data *input, hmcrypto_data *output)
{
	int ret = E_HM_OK;
	size_t len = 0;
	size_t final_len = 0;
	hmcrypto_skcipher_ctx *ctx = NULL;

	if (!check_hmcrypto_data(key) || !check_hmcrypto_data(iv) || !check_hmcrypto_data(input) ||
			!check_hmcrypto_data(output)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ctx = hmcrypto_skcipher_newctx(id, NULL);
		if (ctx == NULL) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_skcipher_encinit(ctx, key->data, key->len, iv->data, iv->len);
	}

	if (ret == E_HM_OK) {
		len = output->len;
		ret = hmcrypto_skcipher_update(ctx, input->data, input->len, output->data, &len);
	}

	if (ret == E_HM_OK) {
		final_len = output->len - len;
		ret = hmcrypto_skcipher_final(ctx, output->data + len, &final_len);
		output->len = len + final_len;
	}

	hmcrypto_skcipher_fini(ctx);
	hmcrypto_skcipher_freectx(ctx);

	return ret;
}

int hmcrypto_skcipher_decrypt(hmcrypto_skcipher_id id, const hmcrypto_data *key,
			      const hmcrypto_data *iv, const hmcrypto_data *input, hmcrypto_data *output)
{
	int err = E_HM_OK;
	size_t len = 0;
	size_t final_len = 0;
	hmcrypto_skcipher_ctx *ctx = NULL;

	if (!check_hmcrypto_data(iv) || !check_hmcrypto_data(key) || !check_hmcrypto_data(input) ||
			!check_hmcrypto_data(output)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		ctx = hmcrypto_skcipher_newctx(id, NULL);
		err = (ctx == NULL) ? E_HM_INVAL : E_HM_OK;
	}

	if (err == E_HM_OK) {
		err = hmcrypto_skcipher_decinit(ctx, key->data, key->len, iv->data, iv->len);
	}

	if (err == E_HM_OK) {
		len = output->len;
		err = hmcrypto_skcipher_update(ctx, input->data, input->len, output->data, &len);
	}

	if (err == E_HM_OK) {
		final_len = output->len - len;
		err = hmcrypto_skcipher_final(ctx, output->data + len, &final_len);
		output->len = len + final_len;
	}

	hmcrypto_skcipher_fini(ctx);
	hmcrypto_skcipher_freectx(ctx);

	return err;
}

int hmcrypto_skcipher_encrypt_aead(hmcrypto_skcipher_id id, const hmcrypto_aead_input *input,
		hmcrypto_data *output, hmcrypto_data *tag)
{
	int ret = E_HM_OK;
	size_t len = 0;
	size_t final_len = 0;
	hmcrypto_skcipher_ctx *ctx = NULL;

	if (!check_hmcrypto_data(input->key) || !check_hmcrypto_data(input->iv) || !check_hmcrypto_data(input->in_data) ||
			!check_hmcrypto_data(output) || !check_hmcrypto_data(tag)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ctx = hmcrypto_skcipher_newctx(id, NULL);
		if (ctx == NULL) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_skcipher_addkey(ctx, input->key->data, input->key->len);
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_skcipher_addaad(ctx, input->aad->data, input->aad->len);
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_skcipher_init(ctx, input->iv->data, input->iv->len, true);
	}

	if (ret == E_HM_OK) {
		len = output->len;
		ret = hmcrypto_skcipher_update(ctx, input->in_data->data, input->in_data->len, output->data, &len);
	}

	if (ret == E_HM_OK) {
		final_len = output->len - len;
		ret = hmcrypto_skcipher_final(ctx, output->data + len, &final_len);
		output->len = len + final_len;
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_skcipher_gettag(ctx, tag->data, tag->len);
	}

	hmcrypto_skcipher_fini(ctx);
	hmcrypto_skcipher_removeaad(ctx);
	hmcrypto_skcipher_removekey(ctx);
	hmcrypto_skcipher_freectx(ctx);

	return ret;
}

int hmcrypto_skcipher_decrypt_aead(hmcrypto_skcipher_id id, const hmcrypto_aead_input *input,
		hmcrypto_data *output, hmcrypto_data *tag)
{
	int ret = E_HM_OK;
	size_t len = 0;
	size_t final_len = 0;
	hmcrypto_skcipher_ctx *ctx = NULL;

	if (!check_hmcrypto_data(input->key) || !check_hmcrypto_data(input->iv) || !check_hmcrypto_data(input->in_data) ||
			!check_hmcrypto_data(output) || !check_hmcrypto_data(tag)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ctx = hmcrypto_skcipher_newctx(id, NULL);
		if (ctx == NULL) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_skcipher_addkey(ctx, input->key->data, input->key->len);
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_skcipher_addaad(ctx, input->aad->data, input->aad->len);
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_skcipher_init(ctx, input->iv->data, input->iv->len, false);
	}

	if (ret == E_HM_OK) {
		len = output->len;
		ret = hmcrypto_skcipher_update(ctx, input->in_data->data, input->in_data->len, output->data, &len);
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_skcipher_settag(ctx, tag->data, tag->len);
	}

	if (ret == E_HM_OK) {
		final_len = output->len - len;
		ret = hmcrypto_skcipher_final(ctx, output->data + len, &final_len);
		output->len = len + final_len;
	}

	hmcrypto_skcipher_fini(ctx);
	hmcrypto_skcipher_removeaad(ctx);
	hmcrypto_skcipher_removekey(ctx);
	hmcrypto_skcipher_freectx(ctx);

	return ret;
}
