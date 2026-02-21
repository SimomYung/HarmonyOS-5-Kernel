/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: struct reserved_region_s
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 16 11:47:43 2024
 */

#ifndef INTAPI_MEM_RESERVED_REGION_H
#define INTAPI_MEM_RESERVED_REGION_H

struct reserved_region_s {
	__sec_data(sysmgr, cntx) uintptr_t addr_start;
	__sec_data(sysmgr, cntx) uintptr_t addr_end;
};

struct vspace_s;

#ifdef CONFIG_RESERVED_REGION
struct reserved_region_s *vspace_als_region_of(struct vspace_s *vspace);
#else
static inline struct reserved_region_s *vspace_als_region_of(struct vspace_s *vspace)
{
	UNUSED(vspace);
	return NULL;
}
#endif

#endif
