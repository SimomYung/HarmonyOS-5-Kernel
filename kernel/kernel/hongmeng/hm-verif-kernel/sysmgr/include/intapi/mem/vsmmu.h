/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Description
 * Author: Huawei OS Kernel Lab
 * Create: Sat Apr 01 10:18:17 2023
 */
#ifndef SYSMGR_INTAPI_MEM_VSMMU_H
#define SYSMGR_INTAPI_MEM_VSMMU_H

#include <hmkernel/strict.h>

struct process_s;

#ifdef CONFIG_VSMMU
unsigned long vsmmu_s2pgd_of(struct process_s *process);
void vsmmu_destory(struct process_s *process);
#else
static inline unsigned long vsmmu_s2pgd_of(struct process_s *process)
{
	UNUSED(process);
	return 0UL;
}
#endif

#endif /* SYSMGR_INTAPI_VSMMU_VSMMU_H */
