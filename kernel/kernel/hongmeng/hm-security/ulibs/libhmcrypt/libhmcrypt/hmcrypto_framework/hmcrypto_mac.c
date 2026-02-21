/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Framework of mac ciphers.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 16:10:24 2023
 */
#include <stdlib.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <libhwsecurec/securec.h>

#include <libhmcrypt/hmcrypto_mac.h>
#include <libhmcrypt/hmcrypto_algid.h>
#include "internal_common.h"
#include "hmcrypto_mac_internal.h"

static hmcrypto_mac_method *g_mac_method = NULL;

int hmcrypto_mac_method_init(hmcrypto_mac_method *mac_method)
{
	if (mac_method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	g_mac_method = mac_method;

	return E_HM_OK;
}

void hmcrypto_mac_method_fini(void)
{
	g_mac_method = NULL;
}

hmcrypto_mac_ctx *hmcrypto_mac_newctx(hmcrypto_mac_id id)
{
	hmcrypto_mac_ctx *mac_ctx = NULL;
	int ret;

	mac_ctx = (hmcrypto_mac_ctx *)malloc(sizeof(hmcrypto_mac_ctx));
	if (mac_ctx == NULL) {
		hm_debug("failed to malloc mem\n");
		return NULL;
	}

	ret = g_mac_method->mac_newctx_f(id, mac_ctx);
	if (ret != E_HM_OK) {
		free(mac_ctx);
		return NULL;
	}

	mac_ctx->id = id;
	mac_ctx->method = g_mac_method;

	return mac_ctx;
}

void hmcrypto_mac_freectx(hmcrypto_mac_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	ctx->method->mac_freectx_f(ctx);
	ctx->method = NULL;

	free(ctx);
}

int hmcrypto_mac_addkey(hmcrypto_mac_ctx *ctx, const uint8_t *key, size_t key_len)
{
	int ret;

	if (ctx == NULL || key == NULL || key_len == 0) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ctx->key_len = key_len;
	ctx->key = (uint8_t *)malloc(ctx->key_len);
	if (ctx->key == NULL) {
		hm_debug("failed to malloc memory for mac key\n");
		return E_HM_INVAL;
	}

	ret = memcpy_s(ctx->key, key_len, key, key_len);
	if (ret != EOK) {
		hm_debug("failed to assign vaule for mac key\n");
		hmcrypto_mac_removekey(ctx);
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

void hmcrypto_mac_removekey(hmcrypto_mac_ctx *ctx)
{
	if (ctx == NULL || ctx->key == NULL) {
		return;
	}

	NOFAIL(memset_s(ctx->key, ctx->key_len, 0, ctx->key_len));
	ctx->key_len = 0;
	free(ctx->key);
	ctx->key = NULL;
}

int hmcrypto_mac_init(hmcrypto_mac_ctx *ctx)
{
	int ret;

	if (ctx == NULL || !check_valid_data(ctx->key, ctx->key_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->mac_init_f(ctx, ctx->key, ctx->key_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for mac init\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_mac_update(hmcrypto_mac_ctx *ctx, const uint8_t *data, size_t data_len)
{
	int ret;

	if (ctx == NULL || !check_valid_data(data, data_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->mac_update_f(ctx, data, data_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for mac update\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_mac_final(hmcrypto_mac_ctx *ctx, uint8_t *out, size_t *out_len)
{
	int ret;

	if (ctx == NULL || out_len == NULL || !check_valid_data(out, *out_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->mac_final_f(ctx, out, out_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for skcipher final\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

int hmcrypto_mac(hmcrypto_mac_id id, const hmcrypto_data *key, const hmcrypto_data *data, hmcrypto_data *output)
{
	int ret = E_HM_OK;
	hmcrypto_mac_ctx *ctx = NULL;

	if (!check_hmcrypto_data(key) || !check_hmcrypto_data(data) || !check_hmcrypto_data(output)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ctx = hmcrypto_mac_newctx(id);
		if (ctx == NULL) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_mac_addkey(ctx, key->data, key->len);
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_mac_init(ctx);
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_mac_update(ctx, data->data, data->len);
	}

	if (ret == E_HM_OK) {
		ret = hmcrypto_mac_final(ctx, output->data, &output->len);
	}

	hmcrypto_mac_removekey(ctx);
	hmcrypto_mac_freectx(ctx);

	return ret;
}
