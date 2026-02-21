/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: Implement the common function for crypto function.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 28 16:10:24 2022
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <libhmcrypt/hmcrypto_common.h>
#include "internal_common.h"

bool check_valid_data(const uint8_t *data, size_t data_len)
{
	return (data == NULL && data_len == 0) || (data != NULL);
}

bool check_hmcrypto_data(const hmcrypto_data *c)
{
	return (c != NULL) && check_valid_data(c->data, c->len);
}
