/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for ux sock filter
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 10 09:31:16 CST 2020
 */

#ifndef _SOCK_FILTER_H_
#define _SOCK_FILTER_H_

#include <stdint.h>

typedef int64_t s64_t;
typedef uint64_t u64_t;

typedef uint64_t u64_t;
typedef int64_t s64_t;

typedef uint32_t u32_t;
typedef int32_t s32_t;

typedef uint16_t u16_t;
typedef int16_t s16_t;

typedef uint8_t u8_t;
typedef int8_t s8_t;

struct ux_sock_filter {
	u16_t  code;
	u8_t   jt;
	u8_t   jf;
	u32_t  k;
};

struct ux_sock_fprog {
	u16_t                  len;
	struct ux_sock_filter  *filter;
};

#endif
