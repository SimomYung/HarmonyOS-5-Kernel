/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Inner interfaces for kshm
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 19 16:18:30 2023
 */
#ifndef SYSMGR_INCLUDE_INTAPI_CTRLMEM_KSHMOP_H
#define SYSMGR_INCLUDE_INTAPI_CTRLMEM_KSHMOP_H

#include <intapi/ctrlmem/pgstr.h>
#include <hmkernel/types.h>

struct page_s;
void ctrlmem_kshm_page_trans(struct page_s *pg, __u32 pgstr_type);

#endif
