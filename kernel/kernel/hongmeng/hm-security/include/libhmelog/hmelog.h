/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2024. All rights reserved.
 * Description: Definitions of hongmeng log encryption api
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 14:45:15 2024
 */
#ifndef ULIBS_INCLUDE_LIBHMELOG_HMELOG_H
#define ULIBS_INCLUDE_LIBHMELOG_HMELOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hmkernel/klog_level.h>
#include <libhmlog/hmlog.h>
#include <libhmcrypt/hmcrypto_skcipher.h>
#include <libhmcrypt/hmbase64.h>

#define HMELOG_ENCRYPT_KEY_LEN                   32U
#define HMELOG_ENCRYPT_IV_LEN                    16U
#define HMELOG_ENCRYPT_BASE64_BUF_LEN            512U

/*
 * not thread safe, make sure only one thread use it
 */
struct elog_ctx {
	size_t session;
	size_t cnt;
	uint64_t tag;
	hmcrypto_skcipher_ctx *ctx;
	hmcrypto_data key_encrypted;
	hmcrypto_data iv_encrypted;
	char buf[HMELOG_ENCRYPT_BASE64_BUF_LEN]; /* only use for log encrypt */
};

struct elog_params {
	char *data;
	size_t size;
};

struct elog_ctx *hmelog_newctx(size_t session, const uint8_t *key, const uint8_t *iv, uint64_t tag);
int hmelog_reset(struct elog_ctx *ctx, const uint8_t *key, const uint8_t *iv, uint64_t tag);
void hmelog_freectx(struct elog_ctx *ctx);

struct elog_meta_stat {
	uint32_t phase; /* real type is hmelog_meta_phase */
	uint32_t pos;
};

/*
 * get ctx metadatas: public key version, tag, key, iv
 * Arguments:
 *   ctx: elog ctx
 *   log: log buf, restore formated metadata. need length >= HMELOG_ENCRYPT_BASE64_BUF_LEN
 *   size: written log buf size
 *   stat: printing state
 * Return:
 *   E_HM_OK when metadata write done
 *   E_HM_AGAIN when metadata too long, need calling again to get remain metadata
 *   E_HM_INVAL when any pointer is invalid
 */
int hmelog_meta_first(struct elog_ctx *ctx, char *log, size_t *size, struct elog_meta_stat *stat);
int hmelog_meta_second(struct elog_ctx *ctx, char *log, size_t *size, struct elog_meta_stat *stat);

/*
 * encrypt given log msg
 * if ciphertext length reach max len(HMELOG_ENCRYPT_BASE64_BUF_LEN), this function will return E_HM_AGAIN,
 * and caller need calling again to get remain ciphertext
 */
int hmelog_encrypt(struct elog_ctx *ctx,
		   const struct elog_params *plain, size_t *plain_pos,
		   struct elog_params *cipher);
#endif
