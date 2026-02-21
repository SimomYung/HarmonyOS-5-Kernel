/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Udk uio sysfs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 3 20:19:18 2021
 */
#include <udk/blk_crypto.h>
#include <udk/log.h>
#include <udk/errno.h>
#include <udk/mutex.h>
#include <libmem/types.h>
#include <intapi/mem/sysmem.h>
#include <intapi/fusion/fusion.h>

static struct crypto_alg_info g_crypto_alg_info;

// crypto alg register
int udk_blk_crypto_register(struct blk_crypto_alg *alg)
{
	int curr_idx = g_crypto_alg_info.idx;

	if (curr_idx > CRYPTO_ALG_MAX_SIZE) {
		udk_error("crypto alg number is full!\n");
		return -EINVAL;
	}

	g_crypto_alg_info.crypto_alg[curr_idx] = alg;
	g_crypto_alg_info.idx = curr_idx + 1;

	return 0;
}

// crypto alg get
struct blk_crypto_alg *udk_blk_crypto_get(const char *alg_name)
{
	struct blk_crypto_alg *alg;
	int curr_idx = g_crypto_alg_info.idx;

	for (int i = 0; i < curr_idx; i++) {
		alg = g_crypto_alg_info.crypto_alg[i];
		if (!strcmp(alg->cra_name, alg_name))
			return alg;
	}

	return NULL;
}