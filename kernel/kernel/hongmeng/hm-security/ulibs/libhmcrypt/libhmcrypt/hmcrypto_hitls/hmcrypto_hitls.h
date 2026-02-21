/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Add the init and fini function for crypto lib hitls.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 6 16:10:24 2023
 */
#ifndef ULIBS_HMCRYPTO_HITLS_H
#define ULIBS_HMCRYPTO_HITLS_H

int hitls_hash_method_init(void);
void hitls_hash_method_fini(void);

int hitls_pkverify_method_init(void);
void hitls_pkverify_method_fini(void);

int hitls_drbg_method_init(void);
void hitls_drbg_method_fini(void);

int hitls_method_init(void);
void hitls_method_fini(void);

#endif
