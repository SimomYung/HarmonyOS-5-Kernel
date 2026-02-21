/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for stage2 rodata
 * Author: Huawei OS Kernel Lab
 * Created: Tue Jun 14 2022
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_RODATA_S2_H
#define AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_RODATA_S2_H

#include <hmasm/pgtbl/pgtblattr.h>

/*
 * Set S2AP to 1 to make it read only at non-secure el1 and non-secure el0
 * See Table (D5-27 Data access permissions for stage 2 of the Secure or Non-secure EL1&0,
 * when EL2 is enabled, translation regime) in ARM64 manual,
 * when XN10 = 2, S2AP = 1, access permission is R, not executable for el0 and el1.
 */
#define __A64_PGTBLATTR_RODATA_S2_BLK1G (__PGTBLATTR_STAGE2_A64BLK1G(0, 2, 0, 0, 0, 1, \
					 __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0xf, 1, 0, 0))
#define __A64_PGTBLATTR_RODATA_S2_BLK2M (__PGTBLATTR_STAGE2_A64BLK2M(0, 2, 0, 0, 0, 1, \
					 __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0xf, 1, 0, 0))
#define __A64_PGTBLATTR_RODATA_S2_PAGE4K (__PGTBLATTR_STAGE2_A64PAGE4K(0, 2, 0, 0, 1, \
					  __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0xf, 1, 0, 0))

#endif

#ifndef __PGTBLATTR_MODEL
#define __PGTBLATTR_MODEL(name)
#endif

__PGTBLATTR_MODEL(RODATA_S2)
