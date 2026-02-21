/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add init and fini fun of hitls method.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 11:10:24 2023
 */

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>
#include "hmcrypto_hitls.h"

int hitls_method_init(void)
{
	int ret;

#ifdef CONFIG_CRYPTO_HITLS
	ret = hitls_hash_method_init();
	BUG_ON(ret != E_HM_OK);
#endif
#if defined(CONFIG_CRYPTO_HITLS) || defined(CONFIG_CRYPTO_PART_HITLS)
	ret = hitls_drbg_method_init();
	BUG_ON(ret != E_HM_OK);
#endif
	return E_HM_OK;
}

void hitls_method_fini(void)
{
#ifdef CONFIG_CRYPTO_HITLS
	hitls_hash_method_fini();
#endif
#if defined(CONFIG_CRYPTO_HITLS) || defined(CONFIG_CRYPTO_PART_HITLS)
	hitls_drbg_method_fini();
#endif
	return;
}
