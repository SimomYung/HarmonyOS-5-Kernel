/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Stdint
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 5 01:36:17 2018
 */
#ifndef _LIBFDT_STDINT_H
#define _LIBFDT_STDINT_H

#include <hmkernel/types.h>

typedef u8 uint8_t;
typedef u16 uint16_t;
typedef u32 uint32_t;
typedef u64 uint64_t;
typedef s64 int64_t;
typedef uptr_t uintptr_t;

#if defined(__HOST_LLT__) || defined(__clang__)
/* Required type definitions by stdatomic */
typedef u16 wchar_t;
typedef s8 int_least8_t;
typedef u8 uint_least8_t;
typedef s16 int_least16_t;
typedef u16 uint_least16_t;
typedef s32 int_least32_t;
typedef u32 uint_least32_t;
typedef s64 int_least64_t;
typedef u64 uint_least64_t;
typedef s8 int_fast8_t;
typedef u8 uint_fast8_t;
typedef s16 int_fast16_t;
typedef u16 uint_fast16_t;
typedef s32 int_fast32_t;
typedef u32 uint_fast32_t;
typedef s64 int_fast64_t;
typedef u64 uint_fast64_t;
typedef ptr_t intptr_t;

/* The type stores the ptrdiff result via signed ptr type.
 * See: https://www.gnu.org/software/libc/manual/html_node/Important-Data-Types.html */
typedef ptr_t ptrdiff_t;

/* Maximum-width signed integer type */
typedef s64 intmax_t;

/* Maximum-width unsigned integer type */
typedef u64 uintmax_t;
#endif

#define RSIZE_MAX (size_t)((size_t)(~((size_t)0)) / ((size_t)2))

#endif
