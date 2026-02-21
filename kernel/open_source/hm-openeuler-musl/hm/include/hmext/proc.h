/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 13 17:43:23 2022
 */

#ifndef HM_INCLUDE_HMEXT_PROC_H
#define HM_INCLUDE_HMEXT_PROC_H

#include <stdint.h>
#include <stddef.h>
#include <hmsyscall.h>

/* Required by Dopra product for hot patching */

#define HM_PROC_CHECK_REG_PC 0u
#define HM_PROC_CHECK_REG_SP 1u

#define HM_PROC_CHECK_MATCH_FAILED  0
#define HM_PROC_CHECK_MATCH_SUCCESS 1

/*
 * hm_proc_check_regs - Check whether SP or PC is in specified address range
 * @which: which register to check
 * @addr: the start address of the check range
 * @size: the size of the check range
 *
 * Return 0 when @which is NOT in the check range
 * Return 1 when @which is matched
 * Return -1 when an error occurred and an error code is stored in errno
 */
int hm_proc_check_regs(unsigned int which, unsigned long addr, size_t size);

#endif
