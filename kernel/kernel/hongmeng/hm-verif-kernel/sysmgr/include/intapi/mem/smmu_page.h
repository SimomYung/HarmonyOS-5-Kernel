/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: smmu page operate
 * Date: 2024/12/07
 */

#ifndef SYSMGR_MEM_SMMU_PAGE_H
#define SYSMGR_MEM_SMMU_PAGE_H

#include <stdlib.h>
#include <libmem/utils.h>
#include <hmkernel/page_pool/page_pool.h>

#define SMMU_STATISTIC_COUNT 3
int handle_smmu_page_alloc_and_insert(uint32_t count);
uint32_t handle_smmu_page_free(uint64_t pfn);
int smmu_page_init(void);
void get_smmu_pool_info(uint32_t *data, uint32_t size);

#endif /* SYSMGR_MEM_SMMU_PAGE_H */