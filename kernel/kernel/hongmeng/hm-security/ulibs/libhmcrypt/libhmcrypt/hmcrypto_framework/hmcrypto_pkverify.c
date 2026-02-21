/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Framework of public key verify.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 10:24:00 2023
 */
#include <stdlib.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <libhwsecurec/securec.h>

#include <libhmcrypt/hmcrypto_pkverify.h>
#include "internal_common.h"
#include "hmcrypto_pkverify_internal.h"

static const hmcrypto_pkverify_method *g_pkverify_method = NULL;

int hmcrypto_pkverify_method_init(const hmcrypto_pkverify_method *pkverify_method)
{
	if (pkverify_method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	g_pkverify_method = pkverify_method;

	return E_HM_OK;
}

void hmcrypto_pkverify_method_fini(void)
{
	g_pkverify_method = NULL;
}

hmcrypto_pkverify_ctx *hmcrypto_pkverify_newctx(hmcrypto_pkverify_info *pkverify_info)
{
	hmcrypto_pkverify_ctx *pkverify_ctx = NULL;
	int ret = E_HM_INVAL;

	if (pkverify_info == NULL) {
		hm_debug("invalid input param\n");
		return NULL;
	}

	pkverify_ctx = (hmcrypto_pkverify_ctx *)malloc(sizeof(hmcrypto_pkverify_ctx));
	if (pkverify_ctx == NULL) {
		hm_debug("failed to malloc mem\n");
		return NULL;
	}

	ret = g_pkverify_method->pkverify_newctx_f(pkverify_info, pkverify_ctx);
	if (ret != E_HM_OK) {
		hm_debug("failed to call pkverify_newctx fucntion, err=%s\n", hmstrerror(ret));
		free(pkverify_ctx);
		return NULL;
	}

	pkverify_ctx->method = g_pkverify_method;

	return pkverify_ctx;
}

void hmcrypto_pkverify_freectx(hmcrypto_pkverify_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	ctx->method->pkverify_freectx_f(ctx);
	ctx->method = NULL;

	free(ctx);
}

int hmcrypto_pkverify_addkey(hmcrypto_pkverify_ctx *ctx, hmcrypto_pkverify_pk *pk)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL || pk == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->pkverify_addkey_f(ctx, pk);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for pkverify init, err=%s\n", hmstrerror(ret));
		return ret;
	}

	return ret;
}

void hmcrypto_pkverify_removekey(hmcrypto_pkverify_ctx *ctx)
{
	if (ctx == NULL || ctx->method == NULL) {
		return;
	}

	ctx->method->pkverify_removekey_f(ctx);
}

int hmcrypto_pkverify_init(hmcrypto_pkverify_ctx *ctx)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->pkverify_init_f(ctx);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for pkverify init, err=%s\n", hmstrerror(ret));
		return ret;
	}

	return ret;
}

int hmcrypto_pkverify_update(hmcrypto_pkverify_ctx *ctx, const uint8_t *data, size_t data_len)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL || !check_valid_data(data, data_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->pkverify_update_f(ctx, data, data_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for pkverify update, err=%s\n", hmstrerror(ret));
		return ret;
	}

	return ret;
}

int hmcrypto_pkverify_final(hmcrypto_pkverify_ctx *ctx, const uint8_t *signature, size_t signature_len)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL || !check_valid_data(signature, signature_len)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->pkverify_final_f(ctx, signature, signature_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for pkverify final, err=%s\n", hmstrerror(ret));
		return ret;
	}

	return ret;
}
