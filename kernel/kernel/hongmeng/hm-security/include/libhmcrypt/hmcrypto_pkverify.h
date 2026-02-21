/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add pk verify interface.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 11:10:24 2023
 */
#ifndef ULIBS_HMCRYPTO_PKVERIFY_H
#define ULIBS_HMCRYPTO_PKVERIFY_H

#include <stdint.h>
#include <string.h>
#include <libhmcrypt/hmcrypto_common.h>
#include <libhmcrypt/hmcrypto_hash.h>

typedef enum {
	HMCRYPTO_PK_RSA,
} hmcrypto_pk_id;

typedef struct {
	hmcrypto_pk_id pk_id;
	hmcrypto_hash_id md_id;
	hmcrypto_hash_id mgf_id;
	int salt_len;
	int bit_number;
} hmcrypto_rsaverify_info;

typedef struct {
	hmcrypto_rsaverify_info *rsaverify_info;
} hmcrypto_pkverify_info;

typedef struct {
	uint8_t *rsa_n;
	size_t rsa_n_len;
	uint8_t *rsa_e;
	size_t rsa_e_len;
} hmcrypto_rsaverify_pk;

typedef struct {
	hmcrypto_rsaverify_pk *rsaverify_pk;
} hmcrypto_pkverify_pk;

typedef struct hmcrypto_pkverify_ctx_internal hmcrypto_pkverify_ctx;

hmcrypto_pkverify_ctx *hmcrypto_pkverify_newctx(hmcrypto_pkverify_info *pkverify_info);
void hmcrypto_pkverify_freectx(hmcrypto_pkverify_ctx *ctx);

int hmcrypto_pkverify_addkey(hmcrypto_pkverify_ctx *ctx, hmcrypto_pkverify_pk *pk);
void hmcrypto_pkverify_removekey(hmcrypto_pkverify_ctx *ctx);

int hmcrypto_pkverify_init(hmcrypto_pkverify_ctx *ctx);
int hmcrypto_pkverify_update(hmcrypto_pkverify_ctx *ctx, const uint8_t *data, size_t data_len);
int hmcrypto_pkverify_final(hmcrypto_pkverify_ctx *ctx, const uint8_t *signature, size_t signature_len);

#endif
