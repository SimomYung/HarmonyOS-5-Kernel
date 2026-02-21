/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: The internal definition of skcipher.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 1 10:10:24 2023
 */
#ifndef ULIBS_HMCRYPTO_SKCIPHER_INTERNAL_H
#define ULIBS_HMCRYPTO_SKCIPHER_INTERNAL_H

#include <libhmcrypt/hmcrypto_skcipher.h>
#include <libhmcrypt/hmcrypto_algid.h>

typedef enum {
	SKCIPHER_ECB_MODE,
	SKCIPHER_CBC_MODE,
	SKCIPHER_CTS_MODE,
	SKCIPHER_XTS_MODE,
	SKCIPHER_GCM_MODE,
	SKCIPHER_CTR_MODE,
} skcipher_mode_id;

typedef struct {
	int (*skcipher_newctx_f)(hmcrypto_skcipher_id id, hmcrypto_skcipher_ctx *ctx);
	void (*skcipher_freectx_f)(hmcrypto_skcipher_ctx *ctx);

	int (*skcipher_encinit_f)(hmcrypto_skcipher_ctx *ctx, const uint8_t *key, size_t key_len,
			const uint8_t *iv, size_t iv_len);

	int (*skcipher_decinit_f)(hmcrypto_skcipher_ctx *ctx, const uint8_t *key, size_t key_len,
			const uint8_t *iv, size_t iv_len);

	int (*skcipher_init_f)(hmcrypto_skcipher_ctx *ctx, const uint8_t *iv, size_t iv_len, bool enc);

	void (*skcipher_fini_f)(hmcrypto_skcipher_ctx *ctx);

	int (*skcipher_update_f)(hmcrypto_skcipher_ctx *ctx, const uint8_t *input, size_t input_len,
			uint8_t *output, size_t *output_len);

	int (*skcipher_final_f)(hmcrypto_skcipher_ctx *ctx, uint8_t *output, size_t *output_len);

	int (*skcipher_settag_f)(hmcrypto_skcipher_ctx *ctx, uint8_t *tag, size_t tag_len);
	int (*skcipher_gettag_f)(hmcrypto_skcipher_ctx *ctx, uint8_t *tag, size_t tag_len);
} hmcrypto_skcipher_method;

struct hmcrypto_skcipher_ctx_internal {
	void *skcipher;
	void *skcipher_ctx;
	bool enc;
	uint8_t *key;
	size_t key_len;
	uint8_t *aad;
	size_t aad_len;
	skcipher_mode_id mode_id;
	hmcrypto_skcipher_id id;
	hmcrypto_skcipher_params *params;
	hmcrypto_skcipher_method *method;
};

int hmcrypto_skcipher_method_init(hmcrypto_skcipher_method *skcipher_method);
void hmcrypto_skcipher_method_fini(void);

#endif
