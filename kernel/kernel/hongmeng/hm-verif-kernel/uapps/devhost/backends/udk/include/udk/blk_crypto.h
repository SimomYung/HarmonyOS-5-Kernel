/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Interfaces of udk uio sysfs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 3 20:18:56 2021
 */
#ifndef __BLK_CRYPTO_H__
#define __BLK_CRYPTO_H__

#include <udk/lib/scatterlist.h>
#include <udk/mempool.h>
#include <udk/mutex.h>

#define CRYPTO_MAX_ALG_NAME 128
#define CRYPTO_ALG_MAX_SIZE 5

extern struct blk_crypto_info g_blk_crypto;

struct blk_crypto_info {
	struct udk_mempool *mempool;
	void *vaddr; // mempool vaddr
	struct udk_mutex mutex;
	int insert_num;
	int max_num;
	int one_size;
};

struct blk_crypto_request {
	bool endecrypt_for_fs;
	unsigned int key_slot_id;
	uint8_t *iv;
	uint8_t *metadata;

	struct udk_sg_table src;
	struct udk_sg_table dst;
};

struct blk_crypto_alg {
	int (*encrypt)(struct blk_crypto_request *req);
	int (*decrypt)(struct blk_crypto_request *req);
	char cra_name[CRYPTO_MAX_ALG_NAME];
};

struct crypto_alg_info {
	struct blk_crypto_alg *crypto_alg[CRYPTO_ALG_MAX_SIZE];
	int idx;
};

// crypto alg register
int udk_blk_crypto_register(struct blk_crypto_alg *alg);

// crypto alg get
struct blk_crypto_alg *udk_blk_crypto_get(const char *alg_name);

#endif /* __BLK_CRYPTO_H__ */
