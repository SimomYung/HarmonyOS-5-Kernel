/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: ctrlmem iofast related functions in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Sat Apr 27 16:00:23 2024
 */
#ifndef INTAPI_CTRLMEM_IOFAST_H
#define INTAPI_CTRLMEM_IOFAST_H

#include <hmasm/memory.h>
#include <page/page.h>

void ctrlmem_normal_type_trans_iofast(__paddr_t pa, struct page_s *pg);
void ctrlmem_normal_type_trans_smmu_pt(__paddr_t pa, struct page_s *pg);
void ctrlmem_normal_type_trans_smmu_pgd(__paddr_t pa, struct page_s *pg);
void ctrlmem_io_type_trans_normal(__paddr_t pa, struct page_s *pg);
void ctrlmem_io_type_trans_ctg(__paddr_t pa, struct page_s *pg);
#endif
