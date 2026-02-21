/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: The internal definition of hash.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 10:24:00 2023
 */
#ifndef ULIBS_HMCRYPTO_HASH_REGISTER_H
#define ULIBS_HMCRYPTO_HASH_REGISTER_H

#include <libhmcrypt/hmcrypto_hash.h>

typedef struct {
	int (*hash_newctx_f)(hmcrypto_hash_id id, hmcrypto_hash_ctx *ctx);
	void (*hash_freectx_f)(hmcrypto_hash_ctx *ctx);

	int (*hash_init_f)(hmcrypto_hash_ctx *ctx);
	int (*hash_update_f)(hmcrypto_hash_ctx *ctx, const uint8_t *data, size_t data_len);
	int (*hash_final_f)(hmcrypto_hash_ctx *ctx, uint8_t *output, size_t *output_len);
} hmcrypto_hash_method;

struct hmcrypto_hash_ctx_internal {
	void *hash;
	void *hash_ctx;
	hmcrypto_hash_id id;
	hmcrypto_hash_method *method;
};

int hmcrypto_hash_method_init(hmcrypto_hash_method *hash_method);
void hmcrypto_hash_method_fini(void);

#endif
