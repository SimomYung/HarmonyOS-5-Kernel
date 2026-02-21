/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 15 23:55:42 2020
 */
#ifndef AARCH64_MAPI_ASM_MEMORY_H
#define AARCH64_MAPI_ASM_MEMORY_H

#include <uapi/hmasm/memory.h>

typedef __vaddr_t vaddr_t;
typedef __paddr_t paddr_t;

#define VADDR_T_MAX __VADDR_T_MAX
#define PADDR_T_MAX __PADDR_T_MAX

#ifdef CONFIG_AARCH64_ADDR_TAGGING
#define untagged_addr(addr) ({				\
		u64 _addr = (u64)(addr);		\
		_addr = __untagged_addr(_addr);		\
		(__typeof__(addr))_addr;		\
})
#else
#define untagged_addr(addr) (addr)
#endif

#endif
