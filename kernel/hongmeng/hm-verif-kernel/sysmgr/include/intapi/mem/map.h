/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Map interface management
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 06 06:05:26 2020
 */
#ifndef SYSMGR_INTAPI_MEM_MAP_H
#define SYSMGR_INTAPI_MEM_MAP_H

#include <stdint.h>
#include <libmem/types.h>

struct vspace_s;

/*
 * MAP_FAILED_UNMAPPED_VADDR means that the existed vregions,
 * which intersect with the specific range of mmap with
 * MAP_FIXED/MFLAG_MAP_FIXED flag, have been unmapped in
 * failure cases, so that invokers should not touch or
 * reunmap the same range.
 */
#ifndef MAP_FAILED_UNMAPPED_VADDR
#define MAP_FAILED_UNMAPPED_VADDR (INVALID_VADDR - 1UL) /* -2 */
#endif

/*
 * Trans mmap flags to hm_flags.
 */
uint32_t map_prepare_hm_flags(const struct vspace_s *vspace, uint32_t flags);

int sysmgr_vmap(struct mem_raw_ranges *pfn_array, int array_len, unsigned int prot, void *ret_vaddr);

int sysmgr_vunmap(unsigned long va, unsigned long size);
int map_reserve_virtual_range(struct vspace_s *vspace,
		  uintptr_t start,
		  size_t length,
		  uintptr_t *ret_vaddr);

#endif /* SYSMGR_INTAPI_MEM_MAP_H */
