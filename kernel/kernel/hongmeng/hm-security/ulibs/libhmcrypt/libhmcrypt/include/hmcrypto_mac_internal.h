/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: The internal definition of mac.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 16:10:24 2023
 */
#ifndef ULIBS_HMCRYPTO_MAC_INTERNAL_H
#define ULIBS_HMCRYPTO_MAC_INTERNAL_H

#include <libhmcrypt/hmcrypto_mac.h>
#include <libhmcrypt/hmcrypto_algid.h>
#include "internal_common.h"

typedef struct {
	int (*mac_newctx_f)(hmcrypto_mac_id id, hmcrypto_mac_ctx *ctx);
	void (*mac_freectx_f)(hmcrypto_mac_ctx *ctx);

	int (*mac_init_f)(hmcrypto_mac_ctx *ctx, const uint8_t *key, size_t key_len);
	int (*mac_update_f)(hmcrypto_mac_ctx *ctx, const uint8_t *data, size_t data_len);
	int (*mac_final_f)(hmcrypto_mac_ctx *ctx, uint8_t *out, size_t *out_len);
} hmcrypto_mac_method;

struct hmcrypto_mac_ctx_internal {
	void *mac;
	void *mac_ctx;
	hmcrypto_mac_id id;
	uint8_t *key;
	size_t key_len;
	// An mac algorithm may be hmac, siphash and so on, thus we add mac_type to indicate different mac algorithm.
	const char *mac_type;
	/*
	 * For a specific mac algorithm, it may be based on different algorithm, for example, the hmac may be based on
	 * sha2-256(named hmac-sha2-256) or sha3-512(named hmac-sha3-512) and so on. Here, we use the based_alg_type to
	 * represent different based algorithm.
	 */
	char *based_alg_type;
	hmcrypto_mac_method *method;
};

int hmcrypto_mac_method_init(hmcrypto_mac_method *mac_method);
void hmcrypto_mac_method_fini(void);

#endif
