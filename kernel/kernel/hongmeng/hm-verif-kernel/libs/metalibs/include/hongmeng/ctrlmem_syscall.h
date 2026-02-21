/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: API of ctrlmem syscall
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 28 12:08:46 2022
 */

#ifndef ULIBS_HONGMENG_CTRLMEM_SYSCALL_H
#define ULIBS_HONGMENG_CTRLMEM_SYSCALL_H

#include <hmkernel/types.h>

static inline int sysfast___ctrlmem_write_r32_v32_arg0(void *, unsigned long);
static inline int sysfast___ctrlmem_write_r32_v64_arg0(void *, unsigned long, unsigned long);
static inline int sysfast___ctrlmem_write_r64_v32_arg0(void *, unsigned long);
static inline int sysfast___ctrlmem_write_r64_v64_arg0(void *, unsigned long);

static inline int
sysfast_ctrlmem_write32_arg0(void *dest, __u32 v)
{
	return sysfast___ctrlmem_write_r32_v32_arg0(dest, (unsigned long)(v));
}

static inline int
sysfast_ctrlmem_write64_arg0(void *dest, __u64 v)
{
#if (__SIZEOF_LONG__ == 8)
	return sysfast___ctrlmem_write_r64_v64_arg0(dest, (unsigned long)(v));
#else
	unsigned long hi = (unsigned long)((v >> 32u) & 0xffffffffull);
	unsigned long lo = (unsigned long)((v) & 0xffffffffull);

	return sysfast___ctrlmem_write_r32_v64_arg0(dest, lo, hi);
#endif
}

#endif
