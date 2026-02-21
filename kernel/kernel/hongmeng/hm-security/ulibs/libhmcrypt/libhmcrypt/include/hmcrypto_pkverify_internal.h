/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: This file add the pk verify method and internal ctx.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 10:24:00 2023
 */
#ifndef ULIBS_HMCRYPTO_PKVERIFY_INTERNAL_H
#define ULIBS_HMCRYPTO_PKVERIFY_INTERNAL_H

#include <libhmcrypt/hmcrypto_pkverify.h>

#define MAX_DIGEST_LEN	(64)

typedef struct {
	int (*pkverify_newctx_f)(hmcrypto_pkverify_info *pkverify_info, hmcrypto_pkverify_ctx *ctx);
	void (*pkverify_freectx_f)(hmcrypto_pkverify_ctx *ctx);

	int (*pkverify_addkey_f)(hmcrypto_pkverify_ctx *ctx, hmcrypto_pkverify_pk *pk);
	void (*pkverify_removekey_f)(hmcrypto_pkverify_ctx *ctx);

	int (*pkverify_init_f)(hmcrypto_pkverify_ctx *ctx);
	int (*pkverify_update_f)(hmcrypto_pkverify_ctx *ctx, const uint8_t *data, size_t data_len);
	int (*pkverify_final_f)(hmcrypto_pkverify_ctx *ctx, const uint8_t *signature, size_t signature_len);
} hmcrypto_pkverify_method;

struct hmcrypto_pkverify_ctx_internal {
	hmcrypto_pkverify_info *verify_info;
	const hmcrypto_pkverify_method *method;
	void *private_info;
};

int hmcrypto_pkverify_method_init(const hmcrypto_pkverify_method *pkverify_method);

void hmcrypto_pkverify_method_fini(void);

#endif
