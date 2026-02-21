/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Interface of KDP verity
 * Author: Huawei OS Kernel Lab
 * Create: Tue June 4 10:06:00 2024
 */
#ifndef SYSMGR_INTAPI_FUSION_KDP_VERITY_H
#define SYSMGR_INTAPI_FUSION_KDP_VERITY_H
#include <hmkernel/errno.h>
#include <libstrict/strict.h>

#ifdef CONFIG_VFS_FUSION
struct page_s;
int kdp_verity_protect(uintptr_t page_vaddr);
void kdp_verity_expose(struct page_s *pg);
#else
static inline int kdp_verity_protect(uintptr_t page_vaddr)
{
	UNUSED(page_vaddr)
	return E_HM_OK;
}
struct page_s;
static inline void kdp_verity_expose(struct page_s *pg)
{
	UNUSED(pg);
}
#endif /* CONFIG_VFS_FUSION */
#endif /* SYSMGR_INTAPI_FUSION_VERITY_H */
