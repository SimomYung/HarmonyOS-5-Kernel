/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of crypt in udk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 21 18:01:36 2021
 */
#include <udk/crypt.h>

#include <libhmsrv_crypt/crypt.h>

static rref_t g_crypto_rref = 0;

int udk_get_random_bytes(void *buf, unsigned int nbytes)
{
	int ret;

	if (buf == NULL) {
		return -EINVAL;
	}

	if (IS_REF_ERR(g_crypto_rref)) {
		g_crypto_rref = crypt_get_crypto_rref();
	}
	ret = hmsrv_crypt_urandom_bytes(g_crypto_rref, CRYPT_RNG_SYSTEM, (unsigned char *)buf, nbytes);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}

	return 0;
}
