/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Declare interface of page for udk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 13 16:10:12 UTC 2024
 */

#ifndef __SYSMGR_INTAPI_MEM_SYSMGR_PAGE_H__
#define __SYSMGR_INTAPI_MEM_SYSMGR_PAGE_H__

#include <lib/dlist.h>

uint64_t sysmgr_alloc_pages(uint32_t flags, uint32_t order);
void sysmgr_free_pages(uint64_t pa);

#endif /* __SYSMGR_INTAPI_MEM_SYSMGR_PAGE_H__ */
