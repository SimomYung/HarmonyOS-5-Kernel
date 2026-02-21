/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for hwcode
 * Author: Huawei OS Kernel Lab
 * Created: Mon Mar 21 2022
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_HWCODE_H
#define AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_HWCODE_H

#include <hmasm/pgtbl/pgtblattr.h>

/*
 * For hypervisor, NG bit should not be set.
 * Set AP21 to 0 to make it read/write at privilege exception level.
 * See table D5-33 (Stage 1 access permissions for instruction execution for 
 * a translation regime that applies to EL0 and a higher Exception level) in ARM64 manual,
 * when XN = 0, PXN = 0, AP21 = 0, access permission is RW for privilege els, executable for all els.
 */
#define __A64_PGTBLATTR_HWCODE_BLK1G	(__PGTBLATTR_A64BLK1G(0, 0, 0, 0, 0, 0, 0, 0, 1, \
					 __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 0, 0, \
					 __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL, 0))
#define __A64_PGTBLATTR_HWCODE_BLK2M	(__PGTBLATTR_A64BLK2M(0, 0, 0, 0, 0, 0, 0, 0, 1, \
					 __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 0, 0, \
					 __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL, 0))
#define __A64_PGTBLATTR_HWCODE_PAGE4K	(__PGTBLATTR_A64PAGE4K(0, 0, 0, 0, 0, 0, 0, 1, \
					 __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 0, 0, \
					 __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL, 0))

#endif

#ifndef __PGTBLATTR_MODEL
#define __PGTBLATTR_MODEL(name)
#endif

__PGTBLATTR_MODEL(HWCODE)
