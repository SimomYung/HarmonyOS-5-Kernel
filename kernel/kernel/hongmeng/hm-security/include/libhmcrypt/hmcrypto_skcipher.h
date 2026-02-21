/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: Add symmetric key cipher interface.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 28 16:10:24 2022
 */
#ifndef ULIBS_HMCRYPTO_SKCIPHER_H
#define ULIBS_HMCRYPTO_SKCIPHER_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <libmem/mcache.h>
#include <libhmcrypt/hmcrypto_common.h>
#include <libhmcrypt/hmcrypto_algid.h>

/*
 * Symmetric key ciphers(skciphers) are used to protect data confidentiality, it consists of encryption
 * and decryption algorithms. The encryption and decryption use the same key. Normally, for a skcipher,
 * given the key, iv(initial vector) and input, the encryption can be represented as
 * output = E(key, iv, input).
 * Meanwhile, given the key, iv and input, the decryption can be represented as
 * output = D(key, iv, input).
 * Natural, input = D(key, iv, E(key, iv, input)).
 *
 * Generally speaking, there are two types of skciphers:
 * 1) block ciphers + mode, for example "AES128-CBC", where "AES128" represents the block cipher AES with
 * 128 bits key, and "CBC" represents the mode.
 * 2) stream cipher, such as ChaCha20, Trivium and so on.
 * Here, the basic concepts are briefly described as follows.
 * 1) block cipher, the block cipher encrypt or decrypt a data by "block". For example, AES is a block
 * cipher, its block lenght is 128 bits, that is, it can encrypt or decrypt data whose length no more that
 * 128 bits.
 * 2) mode, the block length usually a small value, thus, for input of any length, the mode
 * 2-1) divide input into a pieces of block;
 * 2-2) encrypt or decrypt each block;
 * 2-3) combinate the results to get the result.
 * Note that, when the length of the input is not the multiples of block length, the mode may padding the
 * input to be the multiples of block length.
 * 3) stream cipher, the stream cipher generates the key stream and xor with input to generate the result.
 */

/*
 * Given the key(key_data, key_len) and iv(iv_data, iv_len), you can choose an skcipher id to encyrpt or
 * decrypt a messages input(input_data, input_len) and store the result in an output(output_data, output_len)
 * as follows. Note that, the output_len should be set as needed.
 * 1) new ctx: hmcrypto_skcipher_ctx ctx = hmcrypto_skcipher_newctx(id);
 * 2) init: if encrypt: hmcrypto_skcipher_encinit(ctx, key_data, key_len, iv_data, iv_len);
 * 	    if decrypt: hmcrypto_skcipher_decinit(ctx, key_data, key_len, iv_data, iv_len);
 * 3) update: hmcrypto_skcipher_update(ctx, input_data, input_len, output_data, &tmp_output_len), where
 * tmp_output_len can be set as output_len;
 * 4) final: hmcrypto_skcipher_final(ctx, output_data + tmp_output_len, &final_output_len), where
 * final_output_len = output_len - tmp_output_len;
 * 5) fini: hmcrypto_skcipher_fini(ctx);
 * 6) free ctx: hmcrypto_skcipher_freectx(ctx);
 * As a result, the encrypt or decrypt is output_data with data len = tmp_output_len + final_output_len.
 *
 * In some case, you may want to add the key separately, you can take the following steps.
 * 1*) new ctx: hmcrypto_skcipher_ctx ctx = hmcrypto_skcipher_newctx(id);
 * 2*) add key: hmcrypto_skcipher_addkey(ctx, key, key_len);
 * 3*) init: if encrypt: hmcrypto_skcipher_init(ctx, iv_data, iv_len, 1);
 * 	     if decrypt: hmcrypto_skcipher_init(ctx, iv_data, iv_len, 0);
 * 4*) update: hmcrypto_skcipher_update(ctx, input_data, input_len, output_data, &tmp_output_len), where
 * tmp_output_len can be set as output_len;
 * 5*) final: hmcrypto_skcipher_final(ctx, output_data + tmp_output_len, &final_output_len), where
 * final_output_len = output_len - tmp_output_len;
 * 6*) fini: hmcrypto_skcipher_fini(ctx);
 * 7*) remove key: hmcrypto_skcipher_removekey(ctx);
 * 8*) free ctx: hmcrypto_skcipher_freectx(ctx);
 * Note that, you must call the add key befor init.
 */

/*
 * The CTS mode is only used for fscrypt and encrypt the file names. This is a trade-off scheme since the based crypto
 * lib openssl has some limitation, that is the CTS mode only work with input data >= bolck size (16 byte for AES).
 */
typedef enum {
	HMCRYPTO_SKCIPHER_CTS_CS1 = 1,
	HMCRYPTO_SKCIPHER_CTS_CS2,
	HMCRYPTO_SKCIPHER_CTS_CS3,
} hmcrypto_skcipher_cts_mode_id;

typedef struct {
	bool flag_no_padding;
	hmcrypto_skcipher_cts_mode_id cts_mode_id;
} hmcrypto_skcipher_params;


typedef struct hmcrypto_skcipher_ctx_internal hmcrypto_skcipher_ctx;

DECLARE_ULIBS_MCACHE(hmcrypto_skcipher_ctx)
DECLARE_ULIBS_MCACHE_VARIANT(security_variant)

hmcrypto_skcipher_ctx *hmcrypto_skcipher_newctx(hmcrypto_skcipher_id id, hmcrypto_skcipher_params *params);
void hmcrypto_skcipher_freectx(hmcrypto_skcipher_ctx *ctx);
int hmcrypto_skcipher_addkey(hmcrypto_skcipher_ctx *ctx, const uint8_t *key, size_t key_len);
void hmcrypto_skcipher_removekey(hmcrypto_skcipher_ctx *ctx);
int hmcrypto_skcipher_encinit(hmcrypto_skcipher_ctx *ctx, const uint8_t *key,
			      size_t key_len, const uint8_t *iv, size_t iv_len);
int hmcrypto_skcipher_decinit(hmcrypto_skcipher_ctx *ctx, const uint8_t *key,
			      size_t key_len, const uint8_t *iv, size_t iv_len);
int hmcrypto_skcipher_init(hmcrypto_skcipher_ctx *ctx, const uint8_t *iv, size_t iv_len, bool enc);
void hmcrypto_skcipher_fini(hmcrypto_skcipher_ctx *ctx);

int hmcrypto_skcipher_update(hmcrypto_skcipher_ctx *ctx, const uint8_t *input,
			     size_t input_len, uint8_t *output, size_t *output_len);
int hmcrypto_skcipher_final(hmcrypto_skcipher_ctx *ctx, uint8_t *output, size_t *output_len);

int hmcrypto_skcipher_encrypt(hmcrypto_skcipher_id id, const hmcrypto_data *key, const hmcrypto_data *iv,
			      const hmcrypto_data *input, hmcrypto_data *output);
int hmcrypto_skcipher_decrypt(hmcrypto_skcipher_id id, const hmcrypto_data *key, const hmcrypto_data *iv,
			      const hmcrypto_data *input, hmcrypto_data *output);
size_t hmcrypto_skcipher_ivlen(hmcrypto_skcipher_id id);

int hmcrypto_skcipher_addaad(hmcrypto_skcipher_ctx *ctx, const uint8_t *aad, size_t aad_len);
void hmcrypto_skcipher_removeaad(hmcrypto_skcipher_ctx *ctx);

int hmcrypto_skcipher_settag(hmcrypto_skcipher_ctx *ctx, uint8_t *tag, size_t tag_len);
int hmcrypto_skcipher_gettag(hmcrypto_skcipher_ctx *ctx, uint8_t *tag, size_t tag_len);

int hmcrypto_skcipher_encrypt_aead(hmcrypto_skcipher_id id, const hmcrypto_aead_input *input,
		hmcrypto_data *output, hmcrypto_data *tag);
int hmcrypto_skcipher_decrypt_aead(hmcrypto_skcipher_id id, const hmcrypto_aead_input *input,
		hmcrypto_data *output, hmcrypto_data *tag);

#endif
