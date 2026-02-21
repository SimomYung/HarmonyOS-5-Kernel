/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for dax code
 * Author: Huawei OS Kernel Lab
 * Created: Wed Jul 13 2022
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_DAXCODE_H
#define AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_DAXCODE_H

#include <hmasm/pgtbl/pgtblattr.h>

/*
 * For user, NG bit should be set.
 * Set AP21 to 3 to make it read only at all els.
 * See table D5-33 (Stage 1 access permissions for instruction execution for 
 * a translation regime that applies to EL0 and a higher Exception level) in ARM64 manual,
 * when XN = 0, PXN = 1, AP21 = 3, access permission is RE, executable for el0.
 */
#define __A64_PGTBLATTR_DAXCODE_BLK1G	(__PGTBLATTR_A64BLK1G(0, 0, 0, 1, 0, 0, 0, 1, 1, \
					 __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 3, 0, \
					 __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRnE, 0))
#define __A64_PGTBLATTR_DAXCODE_BLK2M	(__PGTBLATTR_A64BLK2M(0, 0, 0, 1, 0, 0, 0, 1, 1, \
					 __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 3, 0, \
					 __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRnE, 0))
#define __A64_PGTBLATTR_DAXCODE_PAGE4K	(__PGTBLATTR_A64PAGE4K(0, 0, 0, 1, 0, 0, 1, 1, \
					 __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 3, 0, \
					 __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRnE, 0))
#endif

#ifndef __PGTBLATTR_MODEL
#define __PGTBLATTR_MODEL(name)
#endif

__PGTBLATTR_MODEL(DAXCODE)
