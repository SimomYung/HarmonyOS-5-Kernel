/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of creating or verifying system file checksums
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 21 20:32:56 2021
 */

#ifndef LIBHMSRV_CRT_VER_CKSUM_H
#define LIBHMSRV_CRT_VER_CKSUM_H

#include <limits.h>
#include <libhmcrypt/hmcrypto_hash.h>

#define SHA256SUM_RAW_LEN 32
#define SHA256SUM_STR_LEN ((SHA256SUM_RAW_LEN) * (2))
#define BIG_BUF_SIZE (SHA256SUM_STR_LEN + PATH_MAX + 1)

typedef struct {
	hmcrypto_hash_ctx *(*hash_newctx)(hmcrypto_hash_id id);
	void (*hash_freectx)(hmcrypto_hash_ctx *ctx);
	int (*hash_init)(hmcrypto_hash_ctx *ctx);
	int (*hash_update)(hmcrypto_hash_ctx *ctx, const uint8_t *data, size_t data_len);
	int (*hash_final)(hmcrypto_hash_ctx *ctx, uint8_t *out, size_t *out_len);
} hmcert_hash_ctx;

void hmcert_hash_ctx_init(hmcert_hash_ctx *hash_ctx);
void hmcert_hash_ctx_fini(void);

int compare_file_chksum(const char *path);

#endif
