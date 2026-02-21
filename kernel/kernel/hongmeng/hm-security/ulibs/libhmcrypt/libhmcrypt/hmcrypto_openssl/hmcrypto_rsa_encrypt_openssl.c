/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add rsa cipher of openssl.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 25 11:10:24 2024
 */

#include <stdint.h>

#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <libhmcrypt/hmcrypto_algid.h>
#include "internal_common.h"
#include "hmcrypto_openssl.h"
#include "hmcrypto_rsacipher_internel.h"

#ifdef __HOST_LLT__
#define INNER
#else
#define INNER static
#endif

#define SHA256_LEN                 32
#define SHA384_LEN                 48
#define SHA512_LEN                 64

typedef struct {
	uint32_t hash_len;
	int32_t padding;
	int32_t hash_nid;
} openssl_rsacipher_id_table_st;

static openssl_rsacipher_id_table_st openssl_rsacipher_id_table[] = {
	[HMCRYPTO_RSAES_PKCS1_V1_5] = {0, RSA_PKCS1_PADDING, 0},
	[HMCRYPTO_RSAES_PKCS1_OAEP_MGF1_SHA256] = {SHA256_LEN, RSA_PKCS1_OAEP_PADDING, NID_sha256},
	[HMCRYPTO_RSAES_PKCS1_OAEP_MGF1_SHA384] = {SHA384_LEN, RSA_PKCS1_OAEP_PADDING, NID_sha384},
	[HMCRYPTO_RSAES_PKCS1_OAEP_MGF1_SHA512] = {SHA512_LEN, RSA_PKCS1_OAEP_PADDING, NID_sha512},
	[HMCRYPTO_RSA_NO_PADDING] = {0, RSA_NO_PADDING, 0},
};

INNER void* openssl_rsacipher_newevpkey(const uint8_t* public_key, size_t key_len)
{
	if (!check_valid_data(public_key, key_len)) {
		hm_error("public_key error\n");
		return NULL;
	}
	BIO *mem = BIO_new_mem_buf(public_key, (int)key_len);
	if (mem == NULL) {
		hm_error("get bio mem buf failed\n");
		return NULL;
	}
	RSA *rsa_key = PEM_read_bio_RSA_PUBKEY(mem, NULL, NULL, NULL);
	BIO_free(mem);
	mem = NULL;
	if (rsa_key == NULL) {
		hm_error("get rsa pub key failed\n");
		return NULL;
	}
	EVP_PKEY *evp_key =  EVP_PKEY_new();
	if (evp_key == NULL) {
		hm_error("Create rsa evp key failed\n");
		RSA_free(rsa_key);
		rsa_key = NULL;
		return NULL;
	}
	int32_t rc = EVP_PKEY_assign_RSA(evp_key, rsa_key);
	if (rc != OPENSSL_SUCCESS) {
		hm_error("Evp assign rsa key failed\n");
		RSA_free(rsa_key);
		rsa_key = NULL;
		EVP_PKEY_free(evp_key);
		evp_key = NULL;
		return NULL;
	}
	return (void *)(uintptr_t)evp_key;
}

INNER void openssl_rsacipher_freeevpkey(void *evp_key)
{
	if (evp_key == NULL) {
		return;
	}
	EVP_PKEY_free((EVP_PKEY *)evp_key); /* rsa_key will free together with evp_key, can not free it alone */
	evp_key = NULL;
}

INNER int openssl_rsacipher_newctx(hmcrypto_rsacipher_id id, void *evp_key, hmcrypto_rsacipher_ctx *ctx)
{
	OSSL_LIB_CTX *libctx;
	EVP_PKEY_CTX *rsacipher_ctx = NULL;
	if (ctx == NULL || evp_key == NULL) {
		hm_error("ctx is NULL or evp_key is not set!");
		return E_HM_INVAL;
	}

	/* id < 0, id is nonnegative enum type */

	if ((unsigned long)id >= ARRAY_SIZE(openssl_rsacipher_id_table)) {
		hm_debug("invalid rsacipher id\n");
		return E_HM_INVAL;
	}
	ctx->id = id;
	ctx->hash_nid = openssl_rsacipher_id_table[id].hash_nid;
	ctx->padding = openssl_rsacipher_id_table[id].padding;
	ctx->hash_len = openssl_rsacipher_id_table[id].hash_len;

	libctx = (OSSL_LIB_CTX *)openssl_provider_get();
	if (libctx == NULL) {
		hm_warn("openssl_provider_get return NULL, use openssl default seed source\n");
	}

	rsacipher_ctx = EVP_PKEY_CTX_new_from_pkey(libctx, (EVP_PKEY *)evp_key, NULL);
	if (rsacipher_ctx == NULL) {
		hm_debug("failed to new rsacipher_ctx ctx from openssl\n");
		return E_HM_INVAL;
	}
	ctx->rsacipher_ctx = (void *)rsacipher_ctx;

	return E_HM_OK;
}

INNER void openssl_rsacipher_freectx(hmcrypto_rsacipher_ctx *ctx)
{
	if (ctx == NULL) {
		return;
	}
	EVP_PKEY_CTX_free((EVP_PKEY_CTX *)ctx->rsacipher_ctx);
	ctx->rsacipher_ctx = NULL;
}

INNER int32_t openssl_set_rsactx_mode(hmcrypto_rsacipher_ctx *ctx)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}
	if (ctx->enc) {
		int32_t rc = EVP_PKEY_encrypt_init((EVP_PKEY_CTX *)ctx->rsacipher_ctx);
		if (rc != OPENSSL_SUCCESS) {
			hm_error("Evp rsa encrypt init failed\n");
			return E_HM_INVAL;
		}
	} else {
		int32_t rc = EVP_PKEY_decrypt_init((EVP_PKEY_CTX *)ctx->rsacipher_ctx);
		if (rc != OPENSSL_SUCCESS) {
			hm_error("Evp rsa decrypt init failed\n");
			return E_HM_INVAL;
		}
	}

	return E_HM_OK;
}

INNER int32_t openssl_set_rsapadding(hmcrypto_rsacipher_ctx *ctx)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}
	int ret = EVP_PKEY_CTX_set_rsa_padding((EVP_PKEY_CTX *)ctx->rsacipher_ctx, ctx->padding);
	if (ret != OPENSSL_SUCCESS) {
		hm_error("Evp set rsa ctx padding failed\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static const EVP_MD *get_mgf1_algorithm(uint32_t mgf1_hash)
{
	switch (mgf1_hash) {
	case HMCRYPTO_DIGEST_SHA256:
		return EVP_sha256();
	case HMCRYPTO_DIGEST_SHA384:
		return EVP_sha384();
	case HMCRYPTO_DIGEST_SHA512:
		return EVP_sha512();
	default:
		return NULL;
	}
	return NULL;
}

INNER int32_t openssl_set_oaep_padding_hash(const hmcrypto_oaep_attr *oaep_attr, int32_t hash_nid,
	hmcrypto_rsacipher_ctx *originctx, int32_t padding)
{
	if (padding != RSA_PKCS1_OAEP_PADDING)
		return E_HM_OK;

	if (hash_nid == 0) {
		hm_error("Get hash nid not vaild for oaep\n");
		return E_HM_INVAL;
	}
	int ret;
	const EVP_MD *md = EVP_get_digestbynid(hash_nid);
	if (md == NULL) {
		hm_error("Get evp digest by nid failed, hash_nid=%d\n", hash_nid);
		return E_HM_INVAL;
	}
	EVP_PKEY_CTX *ctx = (EVP_PKEY_CTX *)originctx->rsacipher_ctx;
	ret = EVP_PKEY_CTX_set_rsa_oaep_md(ctx, (void *)md);
	if (ret != OPENSSL_SUCCESS) {
		hm_error("Evp rsa set oaep md failed\n");
		return E_HM_INVAL;
	}

	/* The mgf1 hash is fixed sha1 in dx, so use sha1 for compatible in here */
	const EVP_MD *evp_md = get_mgf1_algorithm(oaep_attr->mgf1_hash);
	if (evp_md == NULL) {
		hm_error("invalid mgf1 md fuction\n");
		return E_HM_INVAL;
	}
	ret = EVP_PKEY_CTX_set_rsa_mgf1_md(ctx, evp_md);
	if (ret != OPENSSL_SUCCESS) {
		hm_error("Evp rsa set mgf1 md failed\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

INNER int32_t openssl_set_oaep_label(hmcrypto_rsacipher_ctx *ctx, const hmcrypto_oaep_attr *oaep_attr)
{
	if (ctx == NULL) {
		return E_HM_INVAL;
	}
	if (ctx->padding != RSA_PKCS1_OAEP_PADDING || oaep_attr == NULL || oaep_attr->label_len == 0) {
		return E_HM_OK;
	}
	int32_t ret =  E_HM_OK;
	uint64_t label_buf = oaep_attr->label_buffer;
	uint32_t label_len = oaep_attr->label_len;
	if (label_len == 0) {
		hm_error("label_len not vaild\n");
	}
	uint8_t *temp_label = (uint8_t *)malloc(label_len);
	if (temp_label == NULL) {
		hm_error("Malloc label buf failed, label_len=%u\n", label_len);
		return E_HM_INVAL;
	}
	if (memcpy_s(temp_label, label_len, (uint8_t *)(uintptr_t)label_buf, label_len) != EOK) {
		hm_error("Copy label buff ctx failed\n");
		free(temp_label);
		return E_HM_INVAL;
	}
	ret = EVP_PKEY_CTX_set0_rsa_oaep_label((EVP_PKEY_CTX *)ctx->rsacipher_ctx, (void *)temp_label,
		(int)label_len);
	if (ret != OPENSSL_SUCCESS) {
		hm_error("EVP set rsa oaep label failed, ret = %d\n", ret);
		free(temp_label);
		return E_HM_INVAL;
	}
	return E_HM_OK;
}

INNER int32_t openssl_do_rsa_encrypt(hmcrypto_rsacipher_ctx *ctx, const hmcrypto_data *data_in, hmcrypto_data *data_out)
{
	if (ctx == NULL || !check_hmcrypto_data(data_in) || !check_hmcrypto_data(data_out)) {
		return E_HM_INVAL;
	}
	size_t data_out_size = data_out->len;
	int32_t ret = EVP_PKEY_encrypt((EVP_PKEY_CTX *)ctx->rsacipher_ctx, (uint8_t *)(uintptr_t)data_out->data,
		&data_out_size, (uint8_t *)(uintptr_t)data_in->data, data_in->len);
	if (ret != OPENSSL_SUCCESS || data_out_size > UINT32_MAX) {
		hm_error("Evp rsa encrypt failed\n");
		return E_HM_INVAL;
	}
	data_out->len = data_out_size;
	return E_HM_OK;
}

int openssl_rsacipher_method_init(void)
{
	static hmcrypto_rsacipher_method openssl_rsacipher_method = {
		.rsacipher_newevpkey_f		   = openssl_rsacipher_newevpkey,
		.rsacipher_freeevpkey_f		   = openssl_rsacipher_freeevpkey,
		.rsacipher_newctx_f		   = openssl_rsacipher_newctx,
		.rsacipher_freectx_f		   = openssl_rsacipher_freectx,
		.rsacipher_setrsactx_mode_f	   = openssl_set_rsactx_mode,
		.rsacipher_set_rsapadding_f	   = openssl_set_rsapadding,
		.rsacipher_set_oaep_padding_hash_f = openssl_set_oaep_padding_hash,
		.rsacipher_set_oaep_label_f	   = openssl_set_oaep_label,
		.rsacipher_do_rsa_encrypt_f	   = openssl_do_rsa_encrypt,
	};

	return hmcrypto_rsacipher_method_init(&openssl_rsacipher_method);
}

void openssl_rsacipher_method_fini(void)
{
	hmcrypto_rsacipher_method_fini();
}
