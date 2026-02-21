/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Framework of drbg.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 10:24:00 2023
 */
#include <stdlib.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <libhwsecurec/securec.h>

#include <libhmcrypt/hmcrypto_drbg.h>
#include "internal_common.h"
#include "hmcrypto_drbg_internal.h"

const hmcrypto_drbg_method *g_drbg_method = NULL;

int hmcrypto_drbg_method_init(const hmcrypto_drbg_method *drbg_method)
{
	if (drbg_method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	g_drbg_method = drbg_method;

	return E_HM_OK;
}

void hmcrypto_drbg_method_fini(void)
{
	g_drbg_method = NULL;
}

hmcrypto_drbg_ctx *hmcrypto_drbg_newctx(hmcrypto_drbg_id id, void *entropy_ctx, get_entropy_ft get_entropy_fun)
{
	hmcrypto_drbg_ctx *drbg_ctx = NULL;
	hmcrypto_entropy_method_st *entropy_method = NULL;
	int ret = E_HM_INVAL;

	drbg_ctx = (hmcrypto_drbg_ctx *)malloc(sizeof(hmcrypto_drbg_ctx));
	if (drbg_ctx == NULL) {
		hm_debug("failed to malloc memory for drbg ctx\n");
		return NULL;
	}

	entropy_method = (hmcrypto_entropy_method_st *)malloc(sizeof(hmcrypto_entropy_method_st));
	if (entropy_method == NULL) {
		hm_debug("failed to malloc memory for entropy method\n");
		free(drbg_ctx);
		return NULL;
	}

	drbg_ctx->get_entropy_method = entropy_method;
	ret = g_drbg_method->drbg_newctx_f(drbg_ctx, id);
	if (ret != E_HM_OK) {
		hm_debug("failed to call drbg newctx function, err=%s\n", hmstrerror(ret));
		free(entropy_method);
		free(drbg_ctx);
		return NULL;
	}

	drbg_ctx->method = g_drbg_method;
	drbg_ctx->get_entropy_method->entropy_ctx = entropy_ctx;
	drbg_ctx->get_entropy_method->get_entropy_fun = get_entropy_fun;

	return drbg_ctx;
}

void hmcrypto_drbg_freectx(hmcrypto_drbg_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}

	if (ctx->get_entropy_method != NULL) {
		ctx->get_entropy_method->entropy_ctx = NULL;
		ctx->get_entropy_method->get_entropy_fun = NULL;
		free(ctx->get_entropy_method);
		ctx->get_entropy_method = NULL;
	}

	if (ctx->method != NULL) {
		ctx->method->drbg_freectx_f(ctx);
		ctx->method = NULL;
	}

	free(ctx);
}

int hmcrypto_drbg_instantiate(hmcrypto_drbg_ctx *ctx, const uint8_t *pers, size_t pers_len)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->drbg_instantiate_f(ctx, pers, pers_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call drbg instantiate, err=%s\n", hmstrerror(ret));
		return ret;
	}

	return ret;
}

int hmcrypto_drbg_generate(hmcrypto_drbg_ctx *ctx, uint8_t *out, size_t out_len, uint8_t *addition, size_t addition_len)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->drbg_generate_f(ctx, out, out_len, addition, addition_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call drbg generate, err=%s\n", hmstrerror(ret));
		return ret;
	}

	return ret;
}

int hmcrypto_drbg_reseed(hmcrypto_drbg_ctx *ctx, uint8_t *addition, size_t addition_len)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->drbg_reseed_f(ctx, addition, addition_len);
	if (ret != E_HM_OK) {
		hm_debug("failed to call drbg reseed, err=%s\n", hmstrerror(ret));
		return ret;
	}

	return ret;
}

int hmcrypto_drbg_uninstantiate(hmcrypto_drbg_ctx *ctx)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->drbg_uninstantiate_f(ctx);
	if (ret != E_HM_OK) {
		hm_debug("failed to call drbg uninstantiate, err=%s\n", hmstrerror(ret));
		return ret;
	}

	return ret;
}
