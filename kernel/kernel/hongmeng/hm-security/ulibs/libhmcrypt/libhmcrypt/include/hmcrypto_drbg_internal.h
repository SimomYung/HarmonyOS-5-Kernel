/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: This file add the drbg method and internal ctx.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 10:24:00 2023
 */
#ifndef ULIBS_HMCRYPTO_DRBG_INTERNAL_H
#define ULIBS_HMCRYPTO_DRBG_INTERNAL_H

#include <libhmcrypt/hmcrypto_drbg.h>

#define BIT2BYTE	(3)

typedef struct {
	int (*drbg_newctx_f)(hmcrypto_drbg_ctx *ctx, hmcrypto_drbg_id id);
	void (*drbg_freectx_f)(hmcrypto_drbg_ctx *ctx);

	int (*drbg_instantiate_f)(hmcrypto_drbg_ctx *ctx, const uint8_t *pers, size_t pers_len);
	int (*drbg_generate_f)(hmcrypto_drbg_ctx *ctx, uint8_t *out, size_t out_len, uint8_t *addition, size_t addition_len);
	int (*drbg_reseed_f)(hmcrypto_drbg_ctx *ctx, uint8_t *addition, size_t addition_len);
	int (*drbg_uninstantiate_f)(hmcrypto_drbg_ctx *ctx);
} hmcrypto_drbg_method;

struct hmcrypto_drbg_ctx_internal {
	void *drbg;
	void *drbg_ctx;
	void *drbg_id;
	unsigned int strength;
	hmcrypto_entropy_method_st *get_entropy_method;
	const hmcrypto_drbg_method *method;
};

int hmcrypto_drbg_method_init(const hmcrypto_drbg_method *drbg_method);

void hmcrypto_drbg_method_fini(void);

#endif
