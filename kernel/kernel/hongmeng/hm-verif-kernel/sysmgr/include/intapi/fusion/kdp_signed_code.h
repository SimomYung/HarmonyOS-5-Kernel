/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Interface of KDP signed code protect
 * Author: Huawei OS Kernel Lab
 * Create: Tue June 14 10:06:00 2024
 */
#ifndef SYSMGR_INTAPI_FUSION_KDP_SIGNED_CODE_H
#define SYSMGR_INTAPI_FUSION_KDP_SIGNED_CODE_H
#include <hmkernel/errno.h>
#include <libstrict/strict.h>

#ifdef CONFIG_VFS_FUSION
int kdp_fs_codepage_protect(uintptr_t page_vaddr);
int kdp_dm_codepage_protect(uintptr_t page_vaddr);
bool kdp_lowmem_check(uintptr_t page_vaddr);
#else
static inline int kdp_fs_codepage_protect(uintptr_t page_vaddr)
{
	UNUSED(page_vaddr);
	return E_HM_OK;
}
static inline int kdp_dm_codepage_protect(uintptr_t page_vaddr)
{
	UNUSED(page_vaddr);
	return E_HM_OK;
}
static inline bool kdp_lowmem_check(uintptr_t page_vaddr)
{
	UNUSED(page_vaddr);
	return true;
}
#endif /* CONFIG_VFS_FUSION */

uint32_t kdp_code_amend_paf(uint32_t ori_paf, uint32_t flags);

#endif /* SYSMGR_INTAPI_FUSION_SIGNED_CODE_H */
