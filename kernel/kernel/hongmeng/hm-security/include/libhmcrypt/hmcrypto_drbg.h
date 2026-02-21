/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add drbg interface.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 11:10:24 2023
 */
#ifndef ULIBS_HMCRYPTO_DRBG_H
#define ULIBS_HMCRYPTO_DRBG_H

#include <stdint.h>
#include <string.h>
#include <libhmcrypt/hmcrypto_common.h>

typedef enum {
	HMCRYPTO_DRBG_SHA2_256,
} hmcrypto_drbg_id;

typedef int (*get_entropy_ft)(void *entropy_ctx, uint8_t *entropy, size_t *entropy_len);

typedef struct {
	void *entropy_ctx;
	get_entropy_ft get_entropy_fun;
} hmcrypto_entropy_method_st;

typedef struct hmcrypto_drbg_ctx_internal hmcrypto_drbg_ctx;

hmcrypto_drbg_ctx *hmcrypto_drbg_newctx(hmcrypto_drbg_id id, void *entropy_ctx, get_entropy_ft get_entropy_fun);
void hmcrypto_drbg_freectx(hmcrypto_drbg_ctx *ctx);

int hmcrypto_drbg_instantiate(hmcrypto_drbg_ctx *ctx, const uint8_t *pers, size_t pers_len);
int hmcrypto_drbg_generate(hmcrypto_drbg_ctx *ctx, uint8_t *out, size_t out_len,
		uint8_t *addition, size_t addition_len);
int hmcrypto_drbg_reseed(hmcrypto_drbg_ctx *ctx, uint8_t *addition, size_t addition_len);
int hmcrypto_drbg_uninstantiate(hmcrypto_drbg_ctx *ctx);

#endif
