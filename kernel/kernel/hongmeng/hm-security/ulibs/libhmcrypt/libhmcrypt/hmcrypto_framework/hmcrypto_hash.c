/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Framework of hash ciphers.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 10:24:00 2023
 */
#include <stdlib.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <libhwsecurec/securec.h>

#include <libhmcrypt/hmcrypto_hash.h>
#include "internal_common.h"
#include "hmcrypto_hash_internal.h"

static hmcrypto_hash_method *g_hash_method = NULL;

int hmcrypto_hash_method_init(hmcrypto_hash_method *hash_method)
{
	if (hash_method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	g_hash_method = hash_method;

	return E_HM_OK;
}

void hmcrypto_hash_method_fini(void)
{
	g_hash_method = NULL;
}

hmcrypto_hash_ctx *hmcrypto_hash_newctx(hmcrypto_hash_id id)
{
	hmcrypto_hash_ctx *hash_ctx = NULL;
	int ret = E_HM_INVAL;

	hash_ctx = (hmcrypto_hash_ctx *)malloc(sizeof(hmcrypto_hash_ctx));
	if (hash_ctx == NULL) {
		hm_debug("failed to malloc mem\n");
		return NULL;
	}

	ret = g_hash_method->hash_newctx_f(id, hash_ctx);
	if (ret != E_HM_OK) {
		free(hash_ctx);
		return NULL;
	}

	hash_ctx->id = id;
	hash_ctx->method = g_hash_method;

	return hash_ctx;
}

void hmcrypto_hash_freectx(hmcrypto_hash_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	ctx->method->hash_freectx_f(ctx);
	ctx->method = NULL;

	free(ctx);
}

int hmcrypto_hash_init(hmcrypto_hash_ctx *ctx)
{
	int ret;

	if (ctx == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->hash_init_f(ctx);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for hash init\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_hash_update(hmcrypto_hash_ctx *ctx, const uint8_t *data, size_t data_len)
{
	int ret;

	if (ctx == NULL || !check_valid_data(data, data_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->hash_update_f(ctx, data, data_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for hash update\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_hash_final(hmcrypto_hash_ctx *ctx, uint8_t *out, size_t *out_len)
{
	int ret;

	if (ctx == NULL || out_len == NULL || !check_valid_data(out, *out_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->hash_final_f(ctx, out, out_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for hash final\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_hash(hmcrypto_hash_id id, const uint8_t *data, size_t data_len, uint8_t *out, size_t *out_len)
{
	int ret = E_HM_OK;
	hmcrypto_hash_ctx *ctx = NULL;

	if (!check_valid_data(data, data_len) || out_len == NULL || !check_valid_data(out, *out_len)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ctx = hmcrypto_hash_newctx(id);
		if (ctx == NULL) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_hash_init(ctx);
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_hash_update(ctx, data, data_len);
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_hash_final(ctx, out, out_len);
	}

	hmcrypto_hash_freectx(ctx);

	return ret;
}
