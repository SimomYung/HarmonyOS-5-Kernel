/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Provide interfaces of crypto server
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 17 18:16:06 2019
 */
#ifndef LIBHMSRV_CRYPT_SERVER_H
#define LIBHMSRV_CRYPT_SERVER_H

#include <hmkernel/capability.h>

rref_t crypt_get_crypto_rref(void);
int crypt_release_crypto_rref(rref_t rref);

#endif
