/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add hash cipher interface.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 11:10:24 2023
 */
#ifndef ULIBS_HMCRYPTO_HASH_H
#define ULIBS_HMCRYPTO_HASH_H

#include <stdint.h>
#include <string.h>
#include <libhmcrypt/hmcrypto_common.h>

#define HMCRYPTO_HASH_SHA2_256_DIGEST	(32)
#define HMCRYPTO_HASH_SHA2_256_BLOCK    (64)

typedef enum {
	HMCRYPTO_HASH_SHA2_256,
	HMCRYPTO_HASH_SM3,
} hmcrypto_hash_id;

/*
 * The hash is used to generate a data block of a fixed size to authenticate
 * the integrity of a message. A hash can be repressed as out = HASH(m),
 * where m is the message and out is the data block of a fixed size to
 * authenticate the integrity.
 *
 * In detail, given the input(data, data_len), you can choose an hash id
 * to get the hash value and store the result in an output(out, out_len) as
 * follows. Note that, the output_len should be set as needed.
 * 1) new ctx: hmcrypto_hash_ctx *ctx = hmcrypto_hash_newctx(id);
 * 2) init: hmcrypto_hash_init(ctx);
 * 3) update: hmcrypto_hash_update(ctx, data, data_len), this can be called multi times;
 * 4) final: hmcrypto_hash_final(ctx, out, &out_len);
 * 5) free ctx: hmcrypto_hash_freectx(ctx)
 */

typedef struct hmcrypto_hash_ctx_internal hmcrypto_hash_ctx;

hmcrypto_hash_ctx *hmcrypto_hash_newctx(hmcrypto_hash_id id);
void hmcrypto_hash_freectx(hmcrypto_hash_ctx *ctx);

int hmcrypto_hash_init(hmcrypto_hash_ctx *ctx);
int hmcrypto_hash_update(hmcrypto_hash_ctx *ctx, const uint8_t *data, size_t data_len);
int hmcrypto_hash_final(hmcrypto_hash_ctx *ctx, uint8_t *out, size_t *out_len);

/*
 * For the convenience, you can call the following api to get the hash result directly, but this
 * api call hmcrypto_hash_update only once.
 */
int hmcrypto_hash(hmcrypto_hash_id id, const uint8_t *data, size_t data_len, uint8_t *out, size_t *out_len);

#endif
