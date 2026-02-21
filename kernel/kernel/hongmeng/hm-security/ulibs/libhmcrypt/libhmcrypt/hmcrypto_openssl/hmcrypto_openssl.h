/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: Add the init and fini function for crypto lib openssl.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 28 17:10:24 2022
 */
#ifndef ULIBS_HMCRYPTO_OPENSSL_H
#define ULIBS_HMCRYPTO_OPENSSL_H
#include <libhmcrypt/hmcrypto_provider.h>

#define OPENSSL_SUCCESS	(1)

#define ARMV7_NEON      (1<<0)
#define ARMV7_TICK      (1<<1)
#define ARMV8_AES       (1<<2)
#define ARMV8_SHA1      (1<<3)
#define ARMV8_SHA256    (1<<4)
#define ARMV8_PMULL     (1<<5)
#define ARMV8_SHA512    (1<<6)
#define ARMV8_CPUID     (1<<7)

#define HM_SEED	"hm_seed"
#define HM_SEED_PROVIDER	"provider=hm_seed"

int openssl_skcipher_method_init(void);
void openssl_skcipher_method_fini(void);

int openssl_mac_method_init(void);
void openssl_mac_method_fini(void);

int openssl_hash_method_init(void);
void openssl_hash_method_fini(void);

int openssl_drbg_method_init(void);
void openssl_drbg_method_fini(void);

int openssl_method_init(void);
void openssl_method_fini(void);

int openssl_rsacipher_method_init(void);
void openssl_rsacipher_method_fini(void);

int openssl_provider_method_init(void);
void openssl_provider_method_fini(void);
/* declare for internal use */
hmcrypto_provider openssl_provider_get(void);
#endif
