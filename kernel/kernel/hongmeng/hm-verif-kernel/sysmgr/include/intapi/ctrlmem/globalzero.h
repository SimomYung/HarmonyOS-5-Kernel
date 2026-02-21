/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Defs for global zero page
 * Author: Huawei OS Kernel Lab
 * Created: Thu Jun 01 2023
 */

#ifndef INTAPI_CTRLMEM_GLOBALZEO_H
#define INTAPI_CTRLMEM_GLOBALZEO_H

#include <hmasm/memory.h>

extern void ctrlmem_global_zero_page_init(void *gzpage);

extern void ctrlmem_hhp_global_zero_page_init(__paddr_t pa);
#endif
