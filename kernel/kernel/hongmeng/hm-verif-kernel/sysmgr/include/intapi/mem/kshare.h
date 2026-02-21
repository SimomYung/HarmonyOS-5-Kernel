/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Kshare interface management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 20 06:05:26 2021
 */
#ifndef SYSMGR_INTAPI_MEM_KSHARE_H
#define SYSMGR_INTAPI_MEM_KSHARE_H

#include <stdlib.h>
#include <stdint.h>
#include <intapi/mem/vspace.h>
#include <hmkernel/capability.h>

/*
 * kshare_alloc() - alloc continuous kshare memory
 *
 * @cref:           [I] sysctrl_cref
 *
 * @size:           [I] size of kshare memory to alloc
 *
 * @kshare_type:    [I] kshare type of kshare memory to alloc
 *
 * @paddr:          [O] paddr for kshare memory
 *
 * Returns:          0 if succeed, E_HM_* otherwise
 */
int kshare_alloc(cref_t cref, size_t size,
		 enum __sysctrl_kshare_type kshare_type,
		 uint64_t *paddr);
/*
 * kshare_free() - free continuous kshare memory
 *
 * @cref:           [I] sysctrl_cref
 *
 * @paddr:          [I] paddr of kshare memory to free
 *
 * @size:           [I] size of kshare memory to free
 *
 * Returns:          0 if succeed, E_HM_* otherwise
 */
int kshare_free(cref_t cref, uint64_t paddr, size_t size);

/*
 * kshare_map_paddr() - map continuous kshare memory
 *
 * @vspace:         [I] vspace of process to map
 *
 * @paddr:          [I] paddr to map for kshare memory
 *
 * @size:           [I] size of kshare memory to map
 *
 * @start:          [I] advise start address to map
 *
 * @prot:           [I] prot to map
 *
 * Returns:         return address for kshare map
 */
uintptr_t kshare_map_paddr(struct vspace_s *vspace,
			  uint64_t paddr,
			  size_t size,
			  uintptr_t start,
			  uint32_t prot);

uintptr_t kshare_hmpsfsh_map_paddr(struct vspace_s *vspace, uint64_t paddr);

#endif
