/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK block multi queue definitions
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 12 11:43:30 2023
 */
#ifndef __UDK_BLOCK_FALLBACK_H__
#define __UDK_BLOCK_FALLBACK_H__

#include <udk/block/udk_block.h>

#define NVME_CRYPTO_DATA_UNIT_SIZE 4096
#define NVME_CRYPTO_CIPHERTEXT_SIZE 4096
#define NVME_CRYPTO_PAGE_SIZE_SHIFT 12

#define UDK_BLK_CRYPTO_MAX_IV_SIZE		32
#define UDK_BLK_CRYPTO_DUN_ARRAY_SIZE	(UDK_BLK_CRYPTO_MAX_IV_SIZE / sizeof(uint64_t))

union udk_blk_crypto_iv {
	uint64_t dun[UDK_BLK_CRYPTO_DUN_ARRAY_SIZE];
	uint8_t bytes[UDK_BLK_CRYPTO_MAX_IV_SIZE];
};

inline bool udk_bio_has_crypt_ctx(struct udk_bio *bio)
{
	return bio->crypto_key;
}

bool udk_blk_crypto_fallback_bio_prep(struct udk_bio *bio);

int udk_blk_crypto_fallback_decrypt_endio(struct udk_bio *bio);

void udk_blk_crypto_mempool_mem_free(struct udk_bio *bio);

#endif
