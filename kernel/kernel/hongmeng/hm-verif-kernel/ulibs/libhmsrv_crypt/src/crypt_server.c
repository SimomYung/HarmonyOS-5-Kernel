/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide interfaces of crypto server
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 17 18:05:08 2019
 */
#include <hongmeng/errno.h>
#include <libhmcrypt/hmcrypt.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_crypt/crypt_server.h>

rref_t crypt_get_crypto_rref(void)
{
	rref_t rref;
	int err = E_HM_OK;
	err = hm_path_acquire(CRYPTO_PATH, &rref);
	if (err != E_HM_OK) {
		rref = ERR_TO_REF(err);
	}
	return rref;
}

int crypt_release_crypto_rref(rref_t rref)
{
	return hm_path_release(CRYPTO_PATH, rref);
}
