/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2024. All rights reserved.
 * Description: Implementations of hongmeng log encryption api
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 14:45:15 2024
 */
#include <stdbool.h>

#include <libhwsecurec/securec.h>
#include <hongmeng/compiler.h>
#include <libhmcrypt/hmbase64.h>
#include <libhmcrypt/hmcrypto_skcipher.h>
#include <libhmcrypt/hmcrypto_rsacipher.h>
#include <libhmlog/hmlog.h>
#include <libhmelog/hmelog.h>

#include "internel.h"

static hmcrypto_skcipher_ctx *hmelog_skcipher_init(const uint8_t *key, const uint8_t *iv)
{
	int err = E_HM_OK;
	hmcrypto_skcipher_ctx *cipctx = NULL;

	if (err == E_HM_OK) {
		cipctx = hmcrypto_skcipher_newctx(HMELOG_ENCRYPT_MODE, NULL);
		if (cipctx == NULL) {
			err = E_HM_NOMEM;
		}
	}
	if (err == E_HM_OK) {
		err = hmcrypto_skcipher_encinit(cipctx,
						key, HMELOG_ENCRYPT_KEY_LEN,
						iv, HMELOG_ENCRYPT_IV_LEN);
	}

	if (err != E_HM_OK) {
		hmcrypto_skcipher_freectx(cipctx);
		cipctx = NULL;
	}

	return cipctx;
}

static int hmelog_pub_encrypt(const uint8_t *input, size_t input_len, uint8_t **output, size_t *output_len)
{
	int ret = E_HM_OK;
	hmcrypto_rsacipher_id id = HMCRYPTO_RSAES_PKCS1_OAEP_MGF1_SHA256;
	hmcrypto_oaep_attr oaep_attr = {0};
	const hmcrypto_data data_in = {(uint8_t *)input, input_len};
	const hmcrypto_data public_key = {(uint8_t *)HMELOG_PUBLIC_KEY, sizeof(HMELOG_PUBLIC_KEY)};
	hmcrypto_data data_out;

	data_out.len = HMELOG_PUBLIC_CIPHERTEXT_LEN;
	data_out.data = malloc(HMELOG_PUBLIC_CIPHERTEXT_LEN);
	if (data_out.data == NULL) {
		ret = E_HM_NOMEM;
	}

	oaep_attr.mgf1_hash = HMCRYPTO_DIGEST_SHA256;
	oaep_attr.label_buffer = 0;

	if (ret == E_HM_OK) {
		ret = hmcrypto_rsacipher_encrypt(id, &oaep_attr, &public_key, &data_in, &data_out);
		if (ret != E_HM_OK) {
			hm_error("hmelog: public encrypt failed: %d\n", ret);
			free(data_out.data);
		}
	}

	if (ret == E_HM_OK) {
		*output = data_out.data;
		*output_len = data_out.len;
	}
	return ret;
}

static int hmelog_key_init(struct elog_ctx *ctx, const uint8_t *key, const uint8_t *iv)
{
	int err = E_HM_OK;
	uint8_t *ekey = NULL, *eiv = NULL;
	size_t ekey_size = 0, eiv_size = 0;

	if (err == E_HM_OK) {
		err = hmelog_pub_encrypt(key, HMELOG_ENCRYPT_KEY_LEN, &ekey, &ekey_size);
	}
	if (err == E_HM_OK) {
		err = hmelog_pub_encrypt(iv, HMELOG_ENCRYPT_IV_LEN, &eiv, &eiv_size);
	}
	if (err == E_HM_OK) {
		ctx->key_encrypted.data = ekey;
		ctx->key_encrypted.len = ekey_size;
		ctx->iv_encrypted.data = eiv;
		ctx->iv_encrypted.len = eiv_size;
	}

	return err;
}

struct elog_ctx *hmelog_newctx(size_t session, const uint8_t *key, const uint8_t *iv, uint64_t tag)
{
	int err = E_HM_OK;
	struct elog_ctx *ctx = NULL;

	if (key == NULL || iv == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		ctx = malloc(sizeof(struct elog_ctx));
		if (ctx == NULL) {
			err = E_HM_NOMEM;
		}
	}

	if (err == E_HM_OK) {
		mem_zero_b(ctx, sizeof(struct elog_ctx));
		ctx->ctx = hmelog_skcipher_init(key, iv);
		if (ctx->ctx == NULL) {
			err = E_HM_FAULT;
			hm_error("hmelog: init new cipher failed\n");
		}
	}

	if (err == E_HM_OK) {
		err = hmelog_key_init(ctx, key, iv);
	}

	if (err != E_HM_OK) {
		hmelog_freectx(ctx);
		ctx = NULL;
	} else {
		ctx->session = session;
		ctx->cnt = 0;
		ctx->tag = tag;
	}

	return ctx;
}

int hmelog_reset(struct elog_ctx *ctx, const uint8_t *key, const uint8_t *iv, uint64_t tag)
{
	int err = E_HM_OK;
	hmcrypto_skcipher_ctx *cipctx = NULL;
	uint8_t *ekey = NULL, *eiv = NULL;
	size_t ekey_size = 0, eiv_size = 0;

	if (key == NULL || iv == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		cipctx = hmelog_skcipher_init(key, iv);
		if (cipctx == NULL) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		err = hmelog_pub_encrypt(key, HMELOG_ENCRYPT_KEY_LEN, &ekey, &ekey_size);
	}
	if (err == E_HM_OK) {
		err = hmelog_pub_encrypt(iv, HMELOG_ENCRYPT_IV_LEN, &eiv, &eiv_size);
	}
	if (err == E_HM_OK) {
		if (ctx->ctx != NULL) {
			hmcrypto_skcipher_freectx(ctx->ctx);
		}
		ctx->ctx = cipctx;

		if (ctx->key_encrypted.data != NULL) {
			free(ctx->key_encrypted.data);
		}
		ctx->key_encrypted.data = ekey;
		ctx->key_encrypted.len = ekey_size;

		if (ctx->iv_encrypted.data != NULL) {
			free(ctx->iv_encrypted.data);
		}
		ctx->iv_encrypted.data = eiv;
		ctx->iv_encrypted.len = eiv_size;
		ctx->cnt = 0;
		ctx->tag = tag;
	}

	if (err != E_HM_OK) {
		if (cipctx != NULL) {
			hmcrypto_skcipher_freectx(cipctx);
		}
		if (ekey != NULL) {
			free(ekey);
		}
		if (eiv != NULL) {
			free(eiv);
		}
	}

	return err;
}

void hmelog_freectx(struct elog_ctx *ctx)
{
	if (ctx != NULL) {
		if (ctx->ctx != NULL) {
			hmcrypto_skcipher_freectx(ctx->ctx);
			ctx->ctx = NULL;
		}

		if (ctx->key_encrypted.data != NULL) {
			free(ctx->key_encrypted.data);
			ctx->key_encrypted.data = NULL;
		}

		if (ctx->iv_encrypted.data != NULL) {
			free(ctx->iv_encrypted.data);
			ctx->iv_encrypted.data = NULL;
		}

		free(ctx);
	}
}

#define get_common_available_part_len(head_size)	\
	((HMELOG_ENCRYPT_BASE64_BUF_LEN - (head_size) - sizeof(HMELOG_COMMON_PART_TAIL)) / 4 * 4)
#define get_available_full_len(head_size, tail_size)	\
	((HMELOG_ENCRYPT_BASE64_BUF_LEN - (head_size) - (tail_size)) / 4 * 4)

static int hmelog_pubkey_version_first(struct elog_ctx *ctx, char *log, size_t *size, size_t *pos)
{
	int ret = snprintf_s(log, HMELOG_ENCRYPT_BASE64_BUF_LEN, HMELOG_ENCRYPT_BASE64_BUF_LEN - 1,
			     HMELOG_PUBLIC_KEY_VERSION_FIRST_FMT, ctx->session, HMELOG_PUBLIC_KEY_VERSION, ctx->tag);
	if (ret >= 0) {
		*size = *pos = ret;
		ret = E_HM_OK;
	} else {
		*size = *pos = 0;
		ret = E_HM_INVAL;
	}
	return ret;
}

static int hmelog_pubkey_version_second(struct elog_ctx *ctx, char *log, size_t *size, size_t *pos)
{
	int ret = snprintf_s(log, HMELOG_ENCRYPT_BASE64_BUF_LEN, HMELOG_ENCRYPT_BASE64_BUF_LEN - 1,
			     HMELOG_PUBLIC_KEY_VERSION_SECOND_FMT, ctx->session, HMELOG_PUBLIC_KEY_VERSION, ctx->tag);
	if (ret >= 0) {
		*size = *pos = ret;
		ret = E_HM_OK;
	} else {
		*size = *pos = 0;
		ret = E_HM_INVAL;
	}
	return ret;
}

#define GEN_META_PRINT_FUNCTION(FUNCNAME, META_MACRO, META_STRUCT)						       \
static int FUNCNAME(struct elog_ctx *ctx, char *log, size_t *size, size_t *pos)					       \
{														       \
	int ret, err = E_HM_OK;											       \
	size_t p = 0, b64_size = 0, raw_len, input_len, output_len;						       \
	bool is_full;												       \
														       \
	/* head tag */												       \
	ret = snprintf_s(log, HMELOG_ENCRYPT_BASE64_BUF_LEN, HMELOG_ENCRYPT_BASE64_BUF_LEN - 1,			       \
			 HMELOG_TAG_##META_MACRO##_HEAD, ctx->session);						       \
	if (ret > 0) {												       \
		p = ret;											       \
	} else {												       \
		err = E_HM_INVAL;										       \
	}													       \
														       \
	size_t available_raw_len_full = base64_decode_out_size(							       \
		get_available_full_len(p, sizeof(HMELOG_TAG_##META_MACRO##_TAIL)));				       \
	if (ctx->META_STRUCT.len - *pos > available_raw_len_full) {						       \
		is_full = false;										       \
	} else {												       \
		is_full = true;											       \
	}													       \
														       \
	/* content */												       \
	if (err == E_HM_OK) {											       \
		if (is_full) {											       \
			raw_len = base64_decode_out_size(							       \
					get_available_full_len(p, sizeof(HMELOG_TAG_##META_MACRO##_TAIL)));	       \
		} else {											       \
			raw_len = base64_decode_out_size(get_common_available_part_len(p));			       \
		}												       \
		input_len = min(raw_len, ctx->META_STRUCT.len - *pos);						       \
		output_len = base64_encode_out_size(input_len) - 1; /* not include \0 */			       \
		err = base64_encode(ctx->META_STRUCT.data + *pos,						       \
				    input_len,									       \
				    log + p,									       \
				    &b64_size);									       \
		BUG_ON_D(b64_size != output_len);								       \
		*pos += input_len;										       \
		p += output_len;										       \
	}													       \
														       \
	/* tail */												       \
	if (err == E_HM_OK) {											       \
		if (is_full) {											       \
			NOFAIL(memcpy_s(log + p, HMELOG_ENCRYPT_BASE64_BUF_LEN - p,				       \
					HMELOG_TAG_##META_MACRO##_TAIL, sizeof(HMELOG_TAG_##META_MACRO##_TAIL)));      \
			p += sizeof(HMELOG_TAG_##META_MACRO##_TAIL) - 1; /* not include \0 */			       \
		} else {											       \
			NOFAIL(memcpy_s(log + p, HMELOG_ENCRYPT_BASE64_BUF_LEN - p,				       \
					HMELOG_COMMON_PART_TAIL, sizeof(HMELOG_COMMON_PART_TAIL)));		       \
			p += sizeof(HMELOG_COMMON_PART_TAIL) - 1; /* not include \0 */				       \
		}												       \
		*size = p;											       \
	}													       \
														       \
	if (err == E_HM_OK && !is_full) {									       \
		err = E_HM_AGAIN;										       \
	}													       \
														       \
	return err;												       \
}

GEN_META_PRINT_FUNCTION(hmelog_key_first, KEY_FIRST, key_encrypted)
GEN_META_PRINT_FUNCTION(hmelog_key_second, KEY_SECOND, key_encrypted)
GEN_META_PRINT_FUNCTION(hmelog_iv_first, IV_FIRST, iv_encrypted)
GEN_META_PRINT_FUNCTION(hmelog_iv_second, IV_SECOND, iv_encrypted)

static int hmelog_meta_stat_update(struct elog_meta_stat *stat, size_t pos, int ret)
{
	int new_ret = ret;

	/*
	 * internel function use size_t record pos, but elog_meta_stat only use uint32_t.
	 * currently pos will not large than UINT32_MAX
	 */
	if (pos > UINT32_MAX) {
		new_ret = E_HM_OVERFLOW;
	}

	if (ret == E_HM_AGAIN) {
		/* this phase in going, only update pos */
		stat->pos = (uint32_t)pos;
	} else if (ret == E_HM_OK) {
		/* this phase finished, update phase and reset pos */
		stat->phase += 1;
		if (stat->phase != hmelog_phase_max) {
			stat->pos = 0;
			/* phase finished, but process does not */
			new_ret = E_HM_AGAIN;
		}
	}

	return new_ret;
}

int hmelog_meta_first(struct elog_ctx *ctx, char *log, size_t *size, struct elog_meta_stat *stat)
{
	int ret;
	size_t pos;

	if (ctx == NULL || log == NULL || size == NULL || stat == NULL) {
		return E_HM_INVAL;
	}

	pos = stat->pos;
	switch (stat->phase) {
		case (uint16_t)hmelog_phase_pubkey_version:
			ret = hmelog_pubkey_version_first(ctx, log, size, &pos);
			break;
		case (uint16_t)hmelog_phase_key:
			ret = hmelog_key_first(ctx, log, size, &pos);
			break;
		case (uint16_t)hmelog_phase_iv:
			ret = hmelog_iv_first(ctx, log, size, &pos);
			break;
		default:
			hm_error("hmelog: unknow meta print phase: %hu\n", stat->phase);
			return E_HM_INVAL;
	}

	return hmelog_meta_stat_update(stat, pos, ret);
}

int hmelog_meta_second(struct elog_ctx *ctx, char *log, size_t *size, struct elog_meta_stat *stat)
{
	int ret;
	size_t pos;

	if (ctx == NULL || log == NULL || size == NULL || stat == NULL) {
		return E_HM_INVAL;
	}

	pos = stat->pos;
	switch (stat->phase) {
		case (uint16_t)hmelog_phase_pubkey_version:
			ret = hmelog_pubkey_version_second(ctx, log, size, &pos);
			break;
		case (uint16_t)hmelog_phase_key:
			ret = hmelog_key_second(ctx, log, size, &pos);
			break;
		case (uint16_t)hmelog_phase_iv:
			ret = hmelog_iv_second(ctx, log, size, &pos);
			break;
		default:
			hm_error("hmelog: unknow meta print phase: %hu\n", stat->phase);
			return E_HM_INVAL;
	}

	return hmelog_meta_stat_update(stat, pos, ret);
}

static int hmelog_encrypt_headtag(struct elog_ctx *ctx)
{
	int ret = snprintf_s(ctx->buf, HMELOG_ENCRYPT_BASE64_BUF_LEN, HMELOG_ENCRYPT_BASE64_BUF_LEN - 1,
			     HMELOG_TAG_LOG_HEAD, ctx->session, ctx->cnt);

	return ret > 0 ? ret : E_HM_INVAL;
}

/* base64encode(aes_encrypt(input)) */
static size_t encrypt_content(hmcrypto_skcipher_ctx *ctx,
			 uint8_t *input,
			 size_t input_size,
			 char *output)
{
	int err = E_HM_OK;
	uint8_t cipher_buf[ENCRYPT_BLOCK_SIZE];
	size_t block_size;
	size_t size;
	size_t input_pos = 0, output_pos = 0;

	while (input_pos < input_size) {
		block_size = min(ENCRYPT_BLOCK_SIZE, input_size - input_pos);
		err = hmcrypto_skcipher_update(ctx, input + input_pos, block_size, cipher_buf, &size);
		BUG_ON(err != E_HM_OK);
		BUG_ON_D(size != block_size);
		input_pos += block_size;

		err = base64_encode(cipher_buf, block_size, output + output_pos, &size);
		BUG_ON(err != E_HM_OK);
		output_pos += size;
	}

	return output_pos;
}

static int hmelog_encrypt_content(struct elog_ctx *ctx,
		   		  const struct elog_params *plain,
				  size_t *plain_pos,
				  size_t buf_pos,
				  bool is_full)
{
	size_t raw_len, input_len;
	size_t size = 0;

	if (is_full) {
		raw_len = base64_decode_out_size(get_available_full_len(buf_pos, sizeof(HMELOG_TAG_LOG_TAIL)));
	} else {
		raw_len = base64_decode_out_size(get_common_available_part_len(buf_pos));
	}

	input_len = min(raw_len, plain->size - *plain_pos);
	size = encrypt_content(ctx->ctx, (uint8_t *)plain->data + *plain_pos, input_len, ctx->buf + buf_pos);
	*plain_pos += input_len;
	ctx->cnt += input_len;

	return (int)size;
}

static int hmelog_encrypt_tail(struct elog_ctx *ctx,
			       size_t buf_pos,
			       bool is_full)
{
	int ret;

	if (is_full) {
		NOFAIL(memcpy_s(ctx->buf + buf_pos, HMELOG_ENCRYPT_BASE64_BUF_LEN - buf_pos,
				HMELOG_TAG_LOG_TAIL, sizeof(HMELOG_TAG_LOG_TAIL)));
		ret = (int)sizeof(HMELOG_TAG_LOG_TAIL);
	} else {
		NOFAIL(memcpy_s(ctx->buf + buf_pos, HMELOG_ENCRYPT_BASE64_BUF_LEN - buf_pos,
				HMELOG_COMMON_PART_TAIL, sizeof(HMELOG_COMMON_PART_TAIL)));
		ret = (int)sizeof(HMELOG_COMMON_PART_TAIL);
	}

	return ret;
}

int hmelog_encrypt(struct elog_ctx *ctx,
		   const struct elog_params *plain, size_t *plain_pos,
		   struct elog_params *cipher)
{
	int ret;
	size_t p = 0;
	bool is_full;

	if (ctx == NULL || plain_pos == NULL || cipher == NULL ||
	    plain == NULL || plain->data == NULL || plain->size == 0) {
		return E_HM_INVAL;
	    }

	/* head tag */
	ret = hmelog_encrypt_headtag(ctx);
	if (ret < E_HM_OK) {
		return ret;
	}
	p += (size_t)ret;

	size_t available_raw_len_full = base64_decode_out_size(
		get_available_full_len(p, sizeof(HMELOG_TAG_LOG_TAIL)));
	if (plain->size - *plain_pos > available_raw_len_full) {
		is_full = false;
	} else {
		is_full = true;
	}

	/* content */
	ret = hmelog_encrypt_content(ctx, plain, plain_pos, p, is_full);
	if (ret < E_HM_OK) {
		return ret;
	}
	p += (size_t)ret;

	/* tail */
	ret = hmelog_encrypt_tail(ctx, p, is_full);
	if (ret < E_HM_OK) {
		return ret;
	}
	p += (size_t)ret;

	cipher->data = ctx->buf;
	cipher->size = p;

	return is_full ? E_HM_OK : E_HM_AGAIN;
}
