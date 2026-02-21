/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Implementation of shmem_lf_hash_map_utils
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 22 10:15:58 2024
 */
#include <libhmlog/hmlog.h>
#include <libhmsrv_crypt/crypt_rng.h>
#include <libhmsrv_crypt/crypt_server.h>
#include "shmem_lf_hash_map_utils.h"

static rref_t g_crypto_rref = 0;
static rref_t get_crpyto_rref(void)
{
	if (IS_REF_ERR(g_crypto_rref)) {
		g_crypto_rref = crypt_get_crypto_rref();
	}
	return g_crypto_rref;
}

unsigned int hmpsf_test_rand(void)
{
	int err = E_HM_OK;
	unsigned int random = 0;

	err = hmsrv_crypt_urandom_bytes(get_crpyto_rref(), CRYPT_RNG_SYSTEM,
					(unsigned char*)&random, sizeof(unsigned int));
	if (err != E_HM_OK) {
		hm_error("hmpsf_test_rand: Failed to get random bytes, err=%d\n", err);
		random = 0;
	}

	return random;
}
