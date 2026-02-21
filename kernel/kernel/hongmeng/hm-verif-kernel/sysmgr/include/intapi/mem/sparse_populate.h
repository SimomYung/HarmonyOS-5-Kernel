/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Populate page array interface
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 27 15:28:26 2021
 */

#ifndef SYSMGR_MEM_SPARSE_POPULATE_H
#define SYSMGR_MEM_SPARSE_POPULATE_H

#include <hongmeng/types.h>

/*
 * Why pt_pfn array size is 8? When MAX_MAP_SIZE is set to 2MB,
 * it consumes 4 page tables at most. Since slot size is 2 and
 * each pt pfn slot just stores one page, pt_pfn array_size is 8.
 */
#define MAX_MAP_SIZE	0x800000U
#define PFN_SLOT_SIZE	2U
#define CACHE_DEPTH	8U
#define PG_PFN_SIZE	2U
#define PT_PFN_SIZE	16U

/*
 * WARNING: The following interfaces are designed as memmgr internal interfaces
 * and can not be used by other modules except kasan since kasan needs to stub
 * these interfaces.
 */
void populate_page_array(uintptr_t start, size_t sz);

#endif
