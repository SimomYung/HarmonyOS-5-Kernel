/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: Add the common use structure for each crypto function.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 28 16:10:24 2022
 */
#ifndef ULIBS_HMCRYPTO_COMMON_H
#define ULIBS_HMCRYPTO_COMMON_H

#include <stdio.h>

typedef struct {
	uint8_t *data;
	size_t len;
} hmcrypto_data;

typedef struct {
	hmcrypto_data *in_data;
	hmcrypto_data *key;
	hmcrypto_data *iv;
	hmcrypto_data *aad;
} hmcrypto_aead_input;

#endif
