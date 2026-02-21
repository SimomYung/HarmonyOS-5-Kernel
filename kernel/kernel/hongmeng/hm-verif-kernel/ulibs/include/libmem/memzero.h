/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Interfaces of memset operations
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 21 14:19:23 CST 2023
 */

#ifndef ULIBS_INCLUDE_LIBMEM_MEMSET_H
#define ULIBS_INCLUDE_LIBMEM_MEMSET_H

#include <stdint.h>
#include <lib/utils.h>
#include <hmasm/page.h>
#include <hongmeng/types.h>
#include <hongmeng/panic.h>

#if defined(__HOST_LLT__) || defined(__HOST_UT__)
#include <libhwsecurec/securec.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define __MEMZERO_ADDR_ALIGNMENT	64
#define __MEMZERO_SIZE_ALIGNMENT	256

/*
 * Zero a large memory chunk in a fast way. The @addr must
 * be 64 bytes aligned, and the @size must be multiple of 256,
 * otherwise unexpected behaviour would happen.
 */
void __arch_memzero(void *const addr, const size_t size);

#if !defined(__HOST_LLT__) && !defined(__HOST_UT__)
/* Do not use this for small memory chunks. */
static inline void hm_mem_zero(void *const addr, const size_t size)
{
	BUG_ON_D(!IS_ALIGNED((uintptr_t)addr, __MEMZERO_ADDR_ALIGNMENT) ||
		 !IS_ALIGNED(size, __MEMZERO_SIZE_ALIGNMENT) ||
		 size < __MEMZERO_SIZE_ALIGNMENT);
	__arch_memzero(addr, size);
}

static inline void hm_page_zero(void *const page)
{
	BUG_ON_D(!IS_ALIGNED((uintptr_t)page, PAGE_SIZE));
	__arch_memzero(page, PAGE_SIZE);
}
#else
static inline void hm_mem_zero(void *const addr, const size_t size)
{
	NOFAIL(memset_s(addr, size, 0, size));
}

static inline void hm_page_zero(void *const page)
{
	hm_mem_zero(page, PAGE_SIZE);
}
#endif

#ifdef __cplusplus
}
#endif

#endif
