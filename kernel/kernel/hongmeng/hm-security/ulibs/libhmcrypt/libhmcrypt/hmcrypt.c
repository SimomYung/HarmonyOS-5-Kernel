/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: This file offer the implementation hmcrypt_init that should be invoking before using crypto function.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jnu 15 06:10:24 2022
 */
#include "libhmcrypt/hmcrypt.h"

#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <string.h>

#include <bsl/bsl_sal.h>
#include <bsl/bsl_errno.h>

#ifdef CONFIG_CRYPTO_OPENSSL
#include "hmcrypto_openssl.h"
#endif
#if defined(CONFIG_CRYPTO_HITLS) || defined(CONFIG_CRYPTO_PART_HITLS)
#include "hmcrypto_hitls.h"
#endif

#if defined(CONFIG_CRYPTO_HITLS) || defined(CONFIG_CRYPTO_PART_HITLS)
static void *hitls_malloc(uint32_t size)
{
	return malloc(size);
}

static int hitls_mem_init(void)
{
	BSL_SAL_MemCallback callback = {
		.pfMalloc = hitls_malloc,
		.pfFree = free
	};

	if (BSL_SAL_RegMemCallback(&callback) != BSL_SUCCESS) {
		hm_debug("failed to register callback for crypto lib hitls\n");
		return E_HM_INVAL;
	}

	return E_HM_OK;
}
#endif

int hmcrypt_init(void)
{
	int rc;

#ifdef CONFIG_CRYPTO_OPENSSL
	rc = openssl_method_init();
	if (rc != E_HM_OK) {
		hm_debug("failed to initialize openssl method, err=%s\n", hmstrerror(rc));
		return rc;
	}
#endif
#if defined(CONFIG_CRYPTO_HITLS) || defined(CONFIG_CRYPTO_PART_HITLS)
	rc = hitls_mem_init();
	if (rc != E_HM_OK) {
		hm_debug("failed to initialize hitls memory, err=%s\n", hmstrerror(rc));
		return rc;
	}

	rc = hitls_method_init();
	if (rc != E_HM_OK) {
		hm_debug("failed to initialize hitls method, err=%s\n", hmstrerror(rc));
		return rc;
	}
#endif

	return E_HM_OK;
}

void hmcrypt_fini(void)
{
#ifdef CONFIG_CRYPTO_OPENSSL
	openssl_method_fini();
#endif
#if defined(CONFIG_CRYPTO_HITLS) || defined(CONFIG_CRYPTO_PART_HITLS)
	hitls_method_fini();
#endif
	return;
}
