/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Declare interface of iomem mmap/unmap for udk
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 19 10:20:01 UTC 2024
 */

#ifndef __SYSMGR_INTAPI_MEM_SYSMGR_IOMEM_H__
#define __SYSMGR_INTAPI_MEM_SYSMGR_IOMEM_H__

struct mem_raw_ranges;
/*
 * To map the page allocated by udk to the contiguous virtual memory of ldk
 *
 * @param paddr_array	[I] Specified page physical address
 * @param cnt		[I] Length of paddr_array
 * @param prot		[I] Memory accessing attributes
 * @param hint		[I] Mapping attributes
 * @param cnode_idx	[I] The index of the caller process we are going to map mem into
 *			    Caller should make sure this process doesn't exit
 * @param ret_vaddr	[O] The returned mapped virtual address,
 *			    can't be used if returned value is not E_HM_OK
 *
 * @Return E_HM_OK if map success, error number if fail.
*/
int udkmgr_iomem_vmap_to_devhost(struct mem_raw_ranges *pfn_array, size_t cnt,
				 unsigned int prot, uint64_t hint, uint32_t cnode_idx, uintptr_t *ret_vaddr);

/* unmap the page mapped to ldk */
int udkmgr_iomem_vunmap_from_devhost(uintptr_t vaddr, size_t size, uint32_t cnode_idx);

int udkmgr_vmap(struct mem_raw_ranges *pfn_array, int array_len,
		unsigned int prot, uint64_t hint, void *ret_vaddr);

int udkmgr_vunmap(unsigned long va, unsigned long size);

#endif /* __SYSMGR_INTAPI_MEM_SYSMGR_IOMEM_H__ */
