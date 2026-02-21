/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: ctrlmem tlbgather related functions in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 10 15:15:51 2023
 */
#ifndef INTAPI_CTRLMEM_TLBGATHER_H
#define INTAPI_CTRLMEM_TLBGATHER_H

#include <hmasm/memory.h>

void ctrlmem_tlbgather_page_init(__paddr_t pa);

#endif
