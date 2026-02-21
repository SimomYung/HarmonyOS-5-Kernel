/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 29 12:44:05 2018
 */
#ifndef AARCH64_DIV_H
#define AARCH64_DIV_H

#include <hmkernel/types.h>
#include <hmkernel/panic.h>

static inline u32
div_u64_u32(u64 *val, u32 base)
{
	u64 v = *val;

	BUG_ON(base == 0);
	*val = v / base;
	return (u32)(v % (u64)base);
}

#endif
