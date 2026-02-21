/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Library of crypto
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 22 11:11:07 2019
 */
#ifndef LIBHMCRYPT_H
#define LIBHMCRYPT_H

/* Converge definition of CRYPTO_PATH */
#define CRYPTO_PATH	"crypto"

int hmcrypt_init(void);
void hmcrypt_fini(void);
#endif
