/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Interface of mm
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jun 28 17:43:23 2022
 */

#ifndef HM_INCLUDE_HMEXT_MM_H
#define HM_INCLUDE_HMEXT_MM_H

#include <stdint.h>
#include <stddef.h>

#include <hmext/ext.h>

/*
 * hm_mm_va2pa - Transfer virtual address to physical address
 */
static inline int hm_mm_va2pa(void *va, uint64_t *pa)
{
	return hm_ext_syscall(HM_EXT_CMD_MM_VA2PA,
			      va, 0U, pa, 0U);
}

#endif
