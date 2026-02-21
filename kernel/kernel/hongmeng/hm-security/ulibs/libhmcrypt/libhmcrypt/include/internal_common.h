/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: Define the common function for crypto function.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 28 16:10:24 2022
 */
#ifndef ULIBS_INTERNAL_COMMON_H
#define ULIBS_INTERNAL_COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <libhmcrypt/hmcrypto_common.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)	(sizeof(a) / sizeof ((a)[0]))
#endif

#define CRYPTO_NAME_MAX_LEN	(16)

bool check_valid_data(const uint8_t *data, size_t data_len);
bool check_hmcrypto_data(const hmcrypto_data *c);

#endif
