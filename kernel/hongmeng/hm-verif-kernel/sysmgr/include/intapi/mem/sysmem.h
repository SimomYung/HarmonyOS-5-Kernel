/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Sysmgr memory management
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 15 09:26:39 2019
 */
#ifndef SYSMGR_MEM_SYSMEM_H
#define SYSMGR_MEM_SYSMEM_H

#include <intapi/mem/vspace.h>

/*
 * Currently, kasan does not wrap sysmgr_free(). The memory applied for
 * using sysmgr_internal_malloc() is released by sysmgr_free_wrapper().
 * After wrapping sysmgr_free(), replace sysmgr_free_wrapper() with sysmgr_free().
 */
#define sysmgr_free_wrapper(p) free(p)

void *sysmem_malloc_ign_wm(size_t size);
void sysmem_free_ign_wm(uintptr_t p);

void *sysmgr_internal_malloc(size_t size, uint32_t paf);
void sysmgr_free(void *ptr);
uintptr_t sysmem_palloc(unsigned long size);
void sysmem_pfree(uintptr_t vaddr);
#endif
