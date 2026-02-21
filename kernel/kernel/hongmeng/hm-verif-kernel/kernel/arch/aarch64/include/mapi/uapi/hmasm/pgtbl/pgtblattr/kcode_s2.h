/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for stage2 kcode
 * Author: Huawei OS Kernel Lab
 * Created: Mon Jun 13 2022
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_KCODE_S2_H
#define AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_KCODE_S2_H

#include <hmasm/pgtbl/pgtblattr.h>

/*
 * Set S2AP to 1 to make it read only at non-secure el1 and non-secure el0
 * See Table (D5-27 Data access permissions for stage 2 of the Secure or Non-secure EL1&0,
 * when EL2 is enabled, translation regime) in ARM64 manual,
 * when XN10 = 3, S2AP = 1, access permission is RE, executable for el1.
 */
#define __A64_PGTBLATTR_KCODE_S2_BLK1G (__PGTBLATTR_STAGE2_A64BLK1G(0, 3, 0, 0, 0, 1, \
					__PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0xf, 1, 0, 0))
#define __A64_PGTBLATTR_KCODE_S2_BLK2M (__PGTBLATTR_STAGE2_A64BLK2M(0, 3, 0, 0, 0, 1, \
					__PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0xf, 1, 0, 0))
#define __A64_PGTBLATTR_KCODE_S2_PAGE4K (__PGTBLATTR_STAGE2_A64PAGE4K(0, 3, 0, 0, 1, \
					 __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0xf, 1, 0, 0))

#endif

#ifndef __PGTBLATTR_MODEL
#define __PGTBLATTR_MODEL(name)
#endif

__PGTBLATTR_MODEL(KCODE_S2)
