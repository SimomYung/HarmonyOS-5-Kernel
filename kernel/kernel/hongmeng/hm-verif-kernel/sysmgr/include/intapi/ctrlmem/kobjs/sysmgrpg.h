/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: APIs for sysmgrpg
 * Author: Huawei OS Kernel Lab
 * Created: Mon May 08 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_SYSMGRPG_H
#define INTAPI_CTRLMEM_KOBJS_SYSMGRPG_H

#include <hmkernel/types.h>
#include <hmasm/memory.h>
#include <stdint.h>

extern int sysmgrpg_nml2spg_trans(__paddr_t pa);
extern int sysmgrpg_spg2nml_trans(__paddr_t pa);

#endif
