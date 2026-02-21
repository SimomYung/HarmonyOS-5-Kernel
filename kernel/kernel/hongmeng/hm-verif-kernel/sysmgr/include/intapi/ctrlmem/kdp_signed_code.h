/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Defs for ctrlmem of kdp signed code
 * Author: Huawei OS Kernel Lab
 * Create: THU May 9 15:29:00 2024
 */

#ifndef INTAPI_CTRLMEM_KDP_SIGNED_CODE_H
#define INTAPI_CTRLMEM_KDP_SIGNED_CODE_H

#include <hmasm/memory.h>
#include <hmasm/types.h>
#include <hmkernel/pgstr/pgstr.h>
#include <hmkernel/errno.h>
#include <libstrict/strict.h>

int ctrlmem_kdp_signed_code_type_trans(__paddr_t pa);
int ctrlmem_kdp_dm_code_type_trans(__paddr_t pa);
bool ctrlmem_kdp_lowmem_check(__paddr_t pa);
#endif
