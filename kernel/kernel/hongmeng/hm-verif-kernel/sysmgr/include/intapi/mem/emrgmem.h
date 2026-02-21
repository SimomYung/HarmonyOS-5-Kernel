/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: snapshot malloc free declaration
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 26 14:27:10 2024
 */

#ifndef __EMRGMEM_H
#define __EMRGMEM_H

#ifdef CONFIG_EMERGENCY_MEMORY_ALLOC
bool emrgmem_malloc_inited(void);
int emrgmem_malloc_init(bool alloc_paddr);
void *emrgmem_malloc(size_t size);
void emrgmem_free(uintptr_t ptr);
#else
#include <intapi/mem/paf.h>
#include <intapi/mem/sysmem.h>
static inline bool emrgmem_malloc_inited(void)
{
	return false;
}
static inline int emrgmem_malloc_init(bool alloc_paddr)
{
	UNUSED(alloc_paddr);
	return E_HM_OK;
}
static inline void *emrgmem_malloc(size_t size)
{
	return sysmgr_internal_malloc(size, PAF_SNAPSHOT);
}
static inline void emrgmem_free(uintptr_t ptr)
{
	sysmgr_free_wrapper(ptr_to_void(ptr));
}
#endif

#endif
