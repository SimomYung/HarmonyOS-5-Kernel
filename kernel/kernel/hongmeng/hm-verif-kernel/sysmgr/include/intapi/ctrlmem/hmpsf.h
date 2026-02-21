/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Defs for ctrlmem of programmable system framework
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 18 22:02:50 2023
 */

#ifndef INTAPI_CTRLMEM_HMPSF_H
#define INTAPI_CTRLMEM_HMPSF_H

#include <hmasm/memory.h>
#include <hmasm/types.h>
#include <hmkernel/pgstr/pgstr.h>

void ctrlmem_hmpsf_detach_pages(__paddr_t pages_pa, unsigned int pages_num);
void ctrlmem_hmpsf_attach_pages(__paddr_t pages_pa, unsigned int pages_num);

#endif
