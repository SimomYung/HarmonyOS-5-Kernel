/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description:
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 15 16:04:19 2022
 */
#ifndef H_SYSMGR_INCLUDE_HMPSF_PAGE_H
#define H_SYSMGR_INCLUDE_HMPSF_PAGE_H

#include <hmasm/memory.h>
#include <hmkernel/pgstr/pgstr.h>

__paddr_t hmpsf_alloc_page_frame_for_kernel(unsigned int page_order, struct pgstr_s **pages_pgstr);
int hmpsf_reclaim_from_kernel(__paddr_t pages_pa, unsigned int pages_num);

#endif
