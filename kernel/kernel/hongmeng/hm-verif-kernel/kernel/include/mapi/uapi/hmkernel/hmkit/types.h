/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: basic frame work for types
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 15:20:51 2024
*/

#ifndef HMKERNEL_HMKIT_TYPES_H
#define HMKERNEL_HMKIT_TYPES_H
#include <hmkernel/compiler.h>

#ifndef __ASSEMBLY__

typedef signed char hmk_s8_t;
typedef unsigned char hmk_u8_t;

typedef signed short hmk_s16_t;
typedef unsigned short hmk_u16_t;

typedef int hmk_s32_t;
typedef unsigned int hmk_u32_t;

typedef long long hmk_s64_t;
typedef unsigned long long hmk_u64_t;

typedef unsigned long hmk_uptr_t;
typedef signed long hmk_sptr_t;

typedef unsigned long hmk_usize_t;

typedef _Bool hmk_bool_t;

#define HMK_TRUE (bool_t)(1 == 1)
#define HMK_FALSE (bool_t)(1 != 1)

#define hmk_NULL  ((void *)(0ul))

static_assert_by_typedef(sizeof(hmk_s8_t) == 1, s8_not_aligned);
static_assert_by_typedef(sizeof(hmk_u8_t) == 1, u8_not_aligned);

static_assert_by_typedef(sizeof(hmk_s16_t) == 2, s16_not_aligned);
static_assert_by_typedef(sizeof(hmk_u16_t) == 2, u16_not_aligned);

static_assert_by_typedef(sizeof(hmk_s32_t) == 4, s32_not_aligned);
static_assert_by_typedef(sizeof(hmk_u32_t) == 4, u32_not_aligned);

static_assert_by_typedef(sizeof(hmk_s64_t) == 8, s64_not_aligned);
static_assert_by_typedef(sizeof(hmk_u64_t) == 8, u64_not_aligned);

#endif

#endif
