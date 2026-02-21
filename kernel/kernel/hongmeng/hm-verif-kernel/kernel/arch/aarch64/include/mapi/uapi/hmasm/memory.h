/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: memory.h for uapi - aarch64
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 24 17:55:08 2021
 */

#ifndef AARCH64_MAPI_UAPI_ASM_MEMORY_H
#define AARCH64_MAPI_UAPI_ASM_MEMORY_H

#include <hmasm/types.h>

typedef unsigned long __vaddr_t;
typedef __u64 __paddr_t;

#define __VADDR_T_MAX	0xffffffffffffffff
#define __PADDR_T_MAX	0xffffffffffffffff

#define __PRIxvaddr "016lx"
#define __PRIxpaddr "016"__PRIx64

#define __TAG_BITS 8U
#define __untagged_addr(addr) (__typeof__(addr))((__s64)(addr << __TAG_BITS) >> __TAG_BITS)

#endif
