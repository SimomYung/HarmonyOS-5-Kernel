/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: smmu ctrl page
 * Date: 2024/12/07
 */

#ifndef INTAPI_CTRLMEM_SMMUPAGE_H
#define INTAPI_CTRLMEM_SMMUPAGE_H

#include <hmasm/memory.h>

int ctrlmem_smmu_page_init(__paddr_t pa);
int ctrlmem_smmu_page_finalize(__paddr_t pa);

#endif