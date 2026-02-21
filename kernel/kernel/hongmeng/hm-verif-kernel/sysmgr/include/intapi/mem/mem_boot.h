/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Memory boot management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 28 12:09:52 2021
 */

#ifndef SYSMGR_MEM_MEM_BOOT_H
#define SYSMGR_MEM_MEM_BOOT_H

#include <stdint.h>
#include <hmkernel/capability.h>
#include <hmkernel/mm/page.h>

/*
 * WARNING: The following interfaces are designed as memmgr internal interfaces
 * and can not be used by other modules except kasan since kasan needs to stub
 * these interfaces.
 */
/*
 * mem_scan_first_free
 *
 * enter the kernel to scan a free memory segment.
 * the range are input as the input parameter.
 * the one scanned free memory node is output as the output parameter r_range.
 */
bool mem_scan_first_free(cref_t cref,
			 uint64_t pfn_start, uint64_t pfn_end,
			 struct __page_range *r_range);
#endif
