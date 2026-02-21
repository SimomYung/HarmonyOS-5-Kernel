/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Defs for ctrlmem of kdp verity hash
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 6 11:51:00 2024
 */

#ifndef INTAPI_CTRLMEM_KDP_VERITY_H
#define INTAPI_CTRLMEM_KDP_VERITY_H

#include <hmasm/memory.h>
#include <hmasm/types.h>
#include <hmkernel/pgstr/pgstr.h>
#include <hmkernel/errno.h>
#include <libstrict/strict.h>

int ctrlmem_kdp_verity_type_trans(__paddr_t pa);
void ctrlmem_kdp_verity_clear(__paddr_t pa);
#endif
