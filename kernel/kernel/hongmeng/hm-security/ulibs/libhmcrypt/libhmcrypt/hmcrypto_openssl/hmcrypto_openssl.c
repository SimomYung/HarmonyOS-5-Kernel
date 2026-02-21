/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add init and fini fun of openssl method.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 11:10:24 2023
 */

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>
#include <openssl/crypto.h>
#include <stdlib.h>
#include "hmcrypto_openssl.h"

int openssl_method_init(void)
{
	int ret;

	/* keep init first */
	ret = openssl_provider_method_init();
	if (ret != E_HM_OK) {
		hm_panic("openssl_provider_method_init failed , ret : %d\n", ret);
	}

	ret = openssl_hash_method_init();
	BUG_ON(ret != E_HM_OK);

	ret = openssl_mac_method_init();
	BUG_ON(ret != E_HM_OK);

	ret = openssl_skcipher_method_init();
	BUG_ON(ret != E_HM_OK);

	ret = openssl_drbg_method_init();
	BUG_ON(ret != E_HM_OK);

	ret = openssl_rsacipher_method_init();
	BUG_ON(ret != E_HM_OK);

	return E_HM_OK;
}

void openssl_method_fini(void)
{
	openssl_hash_method_fini();
	openssl_mac_method_fini();
	openssl_skcipher_method_fini();
	openssl_drbg_method_fini();
	openssl_rsacipher_method_fini();
	/* keep fini at lastest */
	openssl_provider_method_fini();

	return;
}
