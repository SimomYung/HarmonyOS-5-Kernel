/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Hash functions for shtable head file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:49:44 2019
 */
#ifndef ULIBS_LIBSHTABLE_HASH_H
#define ULIBS_LIBSHTABLE_HASH_H

#include <stddef.h>
#include <stdint.h>

struct shtable;
uint32_t sht_hash(const struct shtable *sht, const void *key, size_t size);

#endif /* ifndef ULIBS_LIBSHTABLE_HASH_H */
