/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description:  Walk pgstrtbl tree in hierarchical table structure
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 01 18:23:53 2023
 */

#ifndef SYSMGR_INCLUDE_INTAPI_CTRLMEM_PGSTRTBLHIER_H
#define SYSMGR_INCLUDE_INTAPI_CTRLMEM_PGSTRTBLHIER_H

#include <hmasm/memory.h>
#include <hmkernel/pgstr/pgstr.h>

struct pgstr_s *pgstrtblhier_pa_to_pgstr(__paddr_t pa);

#endif
