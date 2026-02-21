/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: load pubkey from pem file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 06 13:22:40 2022
 */
#ifndef ULIBS_HMPEM_H
#define ULIBS_HMPEM_H
#include <crypto/crypt_eal_pkey.h>

/*
 * read and parse pem public key.
 * on success the function return hitls pubkey
 * return E_HM_INVAL if load pubkey failed, return E_HM_POSIX_FAULT if parse pem failed.
 */
int pem_parse_hitls_pubkey(const char *path, CRYPT_EAL_PkeyPub *pkey);

#endif
