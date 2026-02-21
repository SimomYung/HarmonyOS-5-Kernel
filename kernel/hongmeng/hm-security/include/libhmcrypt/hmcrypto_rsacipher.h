/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Add rsa ciphter interface.
 * Author: Huawei OS Kernel Lab
 * Create: Thur Apr 25 17:10:24 2024
 */
#ifndef ULIBS_HMCRYPTO_RSACIPHER_H
#define ULIBS_HMCRYPTO_RSACIPHER_H

#include <stdint.h>
#include <string.h>
#include <libhmcrypt/hmcrypto_common.h>
#include <libhmcrypt/hmcrypto_algid.h>
typedef struct hmcrypto_rsacipher_ctx_internal hmcrypto_rsacipher_ctx;

typedef struct {
    uint32_t mgf1_hash;
    uint32_t label_len;
    uint64_t label_buffer;
} hmcrypto_oaep_attr;

hmcrypto_rsacipher_ctx *hmcrypto_rsacipher_newctx(hmcrypto_rsacipher_id id, void *evp_key);
void hmcrypto_rsacipher_freectx(hmcrypto_rsacipher_ctx *ctx);
void *hmcrypto_rsacipher_newevpkey(uint8_t *public_key, size_t key_len);
void hmcrypto_rsacipher_freeevpkey(void *evp_key);
int hmcrypto_rsacipher_enc_init(hmcrypto_rsacipher_ctx *ctx);
int hmcrypto_rsacipher_set_rsapadding(hmcrypto_rsacipher_ctx *ctx);
int hmcrypto_rsacipher_set_rsaoaepattr(hmcrypto_rsacipher_ctx *ctx, const hmcrypto_oaep_attr *oaep_attr);
int hmcrypto_rsacipher_do_rsa_enc(hmcrypto_rsacipher_ctx *ctx, const hmcrypto_data *data_in, hmcrypto_data *data_out);
int hmcrypto_rsacipher_encrypt(hmcrypto_rsacipher_id id, const hmcrypto_oaep_attr *oaep_attr,
    const hmcrypto_data *public_key, const hmcrypto_data *data_in, hmcrypto_data *data_out);
#endif
