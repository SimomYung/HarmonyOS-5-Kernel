/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Framework of rsa cipher.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 10:24:00 2023
 */
#include <stdlib.h>

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <libhwsecurec/securec.h>

#include <libhmcrypt/hmcrypto_rsacipher.h>
#include "internal_common.h"
#include "hmcrypto_rsacipher_internel.h"

static const hmcrypto_rsacipher_method *g_rsacipher_method = NULL;

int hmcrypto_rsacipher_method_init(const hmcrypto_rsacipher_method *rsacipher_method)
{
	if (rsacipher_method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	g_rsacipher_method = rsacipher_method;

	return E_HM_OK;
}

void hmcrypto_rsacipher_method_fini(void)
{
	g_rsacipher_method = NULL;
}

hmcrypto_rsacipher_ctx* hmcrypto_rsacipher_newctx(hmcrypto_rsacipher_id id, void *evp_key)
{
	hmcrypto_rsacipher_ctx *rsacipher_ctx = NULL;
	int ret = E_HM_INVAL;

	if (evp_key  == NULL) {
		hm_debug("invalid input param\n");
		return NULL;
	}

	rsacipher_ctx = (hmcrypto_rsacipher_ctx *)malloc(sizeof(hmcrypto_rsacipher_ctx));
	if (rsacipher_ctx == NULL) {
		hm_debug("failed to malloc mem\n");
		return NULL;
	}

	ret = g_rsacipher_method->rsacipher_newctx_f(id, evp_key, rsacipher_ctx);
	if (ret != E_HM_OK) {
		hm_debug("failed to call rsacipher_newctx fucntion, err=%s\n", hmstrerror(ret));
		free(rsacipher_ctx);
		return NULL;
	}

	rsacipher_ctx->method = g_rsacipher_method;

	return rsacipher_ctx;
}

void hmcrypto_rsacipher_freectx(hmcrypto_rsacipher_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}
	ctx->method->rsacipher_freectx_f(ctx);
	ctx->method = NULL;

	free(ctx);
	ctx = NULL;
}

void* hmcrypto_rsacipher_newevpkey(uint8_t *public_key, size_t key_len)
{
	if (!check_valid_data(public_key, key_len)) {
		hm_debug("invalid input param\n");
		return NULL;
	}

	void *evp_key = g_rsacipher_method->rsacipher_newevpkey_f(public_key, key_len);
	if (evp_key == NULL) {
		return evp_key;
	}

	return evp_key;
}

void hmcrypto_rsacipher_freeevpkey(void *evp_key)
{
	if (evp_key == NULL) {
		return;
	}

	g_rsacipher_method->rsacipher_freeevpkey_f(evp_key);
}

int hmcrypto_rsacipher_enc_init(hmcrypto_rsacipher_ctx *ctx)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ctx->enc = true;

	ret = ctx->method->rsacipher_setrsactx_mode_f(ctx);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for rsacipher setrsaactx mode, err=%s\n", hmstrerror(ret));
		return ret;
	}

	return ret;
}

int hmcrypto_rsacipher_set_rsapadding(hmcrypto_rsacipher_ctx *ctx)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->rsacipher_set_rsapadding_f(ctx);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for rsacipher setrsaactx mode, err=%s\n", hmstrerror(ret));
		return ret;
	}
	return ret;
}

int hmcrypto_rsacipher_set_rsaoaepattr(hmcrypto_rsacipher_ctx *ctx, const hmcrypto_oaep_attr *oaep_attr)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->rsacipher_set_oaep_padding_hash_f(oaep_attr, ctx->hash_nid, ctx, ctx->padding);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for rsacipher set oaep padding hash, err=%s\n", hmstrerror(ret));
		return ret;
	}
	ret = ctx->method->rsacipher_set_oaep_label_f(ctx, oaep_attr);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for rsacipher set oaep label, err=%s\n", hmstrerror(ret));
		return ret;
	}
	return ret;
}

int hmcrypto_rsacipher_do_rsa_enc(hmcrypto_rsacipher_ctx *ctx, const hmcrypto_data *data_in, hmcrypto_data *data_out)
{
	int ret = E_HM_OK;

	if (ctx == NULL || ctx->method == NULL || !check_hmcrypto_data(data_in)) {
		hm_debug("invalid input param\n");
		return E_HM_INVAL;
	}

	ret = ctx->method->rsacipher_do_rsa_encrypt_f(ctx, data_in, data_out);
	if (ret != E_HM_OK) {
		hm_debug("failed to call function for rsacipher update, err=%s\n", hmstrerror(ret));
		return ret;
	}
	return ret;
}

int hmcrypto_rsacipher_encrypt(hmcrypto_rsacipher_id id, const hmcrypto_oaep_attr *oaep_attr,
	const hmcrypto_data *public_key, const hmcrypto_data *data_in, hmcrypto_data *data_out)
{
	int ret = E_HM_OK;
	hmcrypto_rsacipher_ctx *ctx = NULL;
	void *evp_key = NULL;

	if (!check_hmcrypto_data(public_key) || !check_hmcrypto_data(data_in) || !check_hmcrypto_data(data_out)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		evp_key = hmcrypto_rsacipher_newevpkey(public_key->data, public_key->len);
		if (evp_key == NULL) {
			ret = E_HM_INVAL;
		}
	}
	if (ret == E_HM_OK) {
		ctx = hmcrypto_rsacipher_newctx(id, evp_key);
		if (ctx == NULL) {
			ret = E_HM_INVAL;
		}
	}
	if (ret == E_HM_OK) {
		ret = hmcrypto_rsacipher_enc_init(ctx);
	}
	if (ret == E_HM_OK) {
		ret = hmcrypto_rsacipher_set_rsapadding(ctx);
	}
	if (ret == E_HM_OK) {
		ret = hmcrypto_rsacipher_set_rsaoaepattr(ctx, oaep_attr);
	}
	if (ret == E_HM_OK) {
		ret = hmcrypto_rsacipher_do_rsa_enc(ctx, data_in ,data_out);
	}
	hmcrypto_rsacipher_freeevpkey(evp_key);
	hmcrypto_rsacipher_freectx(ctx);
	return ret;
}
