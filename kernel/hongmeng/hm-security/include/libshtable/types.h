/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Types for shared hash table head file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:49:00 2019
 */
#ifndef ULIBS_LIBSHTABLE_TYPES_H
#define ULIBS_LIBSHTABLE_TYPES_H

#include <stdint.h>

typedef uint16_t sht_uint_t;
#define PRIX_SHT_U PRIx16

typedef sht_uint_t sht_idx_t;

#define SHT_INVALID ((sht_idx_t)-1)

#define SHT_TYPE_BITS (sizeof(sht_idx_t) * 8U)

#define SHT_MAX ((((uint32_t)1) << SHT_TYPE_BITS) - (uint32_t)1)

static inline sht_idx_t sht_idx_read(const volatile sht_idx_t *idx)
{
	return *(const volatile sht_idx_t *)idx;
}

#endif /* ifndef ULIBS_LIBSHTABLE_TYPES_H */
