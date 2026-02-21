/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: This file add the rsa cipher method and internal ctx.
 * Author: Huawei OS Kernel Lab
 * Create: Thur Apr 25 15:08:00 2024
 */
#ifndef ULIBS_HMCRYPTO_RSACIPHER_INTERNAL_H
#define ULIBS_HMCRYPTO_RSACIPHER_INTERNAL_H

#include <libhmcrypt/hmcrypto_rsacipher.h>

#define MAX_DIGEST_LEN	(64)

typedef struct {
	void* (*rsacipher_newevpkey_f)(const uint8_t* public_key, size_t key_len);
	void (*rsacipher_freeevpkey_f)(void *evp_key);
	int (*rsacipher_newctx_f)(hmcrypto_rsacipher_id id, void *evp_key, hmcrypto_rsacipher_ctx *ctx);
	void (*rsacipher_freectx_f)(hmcrypto_rsacipher_ctx *ctx);
	int32_t (*rsacipher_setrsactx_mode_f)(hmcrypto_rsacipher_ctx *ctx);
	int32_t (*rsacipher_set_rsapadding_f)(hmcrypto_rsacipher_ctx *ctx);
	int32_t (*rsacipher_set_oaep_padding_hash_f)(const hmcrypto_oaep_attr *oaep_attr, int32_t hash_nid,
		hmcrypto_rsacipher_ctx *originctx, int32_t padding);
	int32_t (*rsacipher_set_oaep_label_f)(hmcrypto_rsacipher_ctx *ctx, const hmcrypto_oaep_attr *oaep_attr);
	int32_t (*rsacipher_do_rsa_encrypt_f)(hmcrypto_rsacipher_ctx *ctx, const hmcrypto_data *data_in,
		hmcrypto_data *data_out);
} hmcrypto_rsacipher_method;

struct hmcrypto_rsacipher_ctx_internal {
	void *rsacipher_ctx;
	hmcrypto_rsacipher_id id;
	uint32_t hash_len;
	int32_t padding;
	int32_t hash_nid;
	bool enc;
	const hmcrypto_rsacipher_method *method;
};

int hmcrypto_rsacipher_method_init(const hmcrypto_rsacipher_method *rsacipher_method);

void hmcrypto_rsacipher_method_fini(void);

#endif
