/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK block multi queue definitions
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 12 11:43:30 2023
 */
#ifndef __UDK_BLOCK_FALLBACK_H__
#define __UDK_BLOCK_FALLBACK_H__

#include <udk/block/udk_block.h>
#include <udk/blk_crypto.h>

#define UDK_BLK_CRYPTO_DATA_UNIT_SIZE 4096
#define UDK_BLK_CRYPTO_CIPHERTEXT_SIZE 4096
#define UDK_BLK_CRYPTO_PAGE_SIZE_SHIFT 12

#define UDK_BLK_CRYPTO_MAX_IV_SIZE		32
#define UDK_BLK_CRYPTO_DUN_ARRAY_SIZE	(UDK_BLK_CRYPTO_MAX_IV_SIZE / sizeof(uint64_t))

#define UDK_CRYPTO_MEMPOOL_FREE_SIZE 512

union udk_blk_crypto_iv {
	uint64_t dun[UDK_BLK_CRYPTO_DUN_ARRAY_SIZE];
	uint8_t bytes[UDK_BLK_CRYPTO_MAX_IV_SIZE];
};

struct udk_crypto_io_info {
	int bi_vec_num;
	unsigned long long *pfn_arr;
	struct dlist_node queuelist;
};

inline bool udk_bio_has_crypt_ctx(struct udk_bio *bio)
{
	return bio->crypto_key;
}

bool udk_blk_crypto_fallback_bio_prep(struct udk_bio *bio);
bool udk_blk_crypto_fallback_decrypt_endio(struct udk_bio *bio);
void udk_blk_crypto_fallback_encrypt_endio(struct udk_bio *bio);

// mempool init
int udk_crypto_mempool_init(struct blk_crypto_info *info);

// mempool exit
int udk_crypto_mempool_exit(struct blk_crypto_info *info);

int udk_crypto_workqueue_init(void);

void *udk_crypto_mempool_mem_alloc(struct blk_crypto_info *info, unsigned int size, unsigned int times, bool add_work);

int crypto_aes_endecrypt_for_fs(void *src_vec, void *dst_vec, unsigned long vec_cnt, void* crypt_ctx, bool enc,
								bool endecrypt_for_fs);

#endif
