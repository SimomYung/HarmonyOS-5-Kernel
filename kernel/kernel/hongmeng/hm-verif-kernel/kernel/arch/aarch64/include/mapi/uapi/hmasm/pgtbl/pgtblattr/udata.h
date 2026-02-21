/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for udata
 * Author: Huawei OS Kernel Lab
 * Created: Tue Jan 25 2022
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_UDATA_H
#define AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_UDATA_H

#include <hmasm/pgtbl/pgtblattr.h>

/*
 * For user, NG bit should be set.
 * Set AP21 to 1 to make it read/write at all els.
 * See table D5-33 (Stage 1 access permissions for instruction execution for 
 * a translation regime that applies to EL0 and a higher Exception level) in ARM64 manual,
 * when XN = 1, PXN = 1, AP21 = 1, access permission is RW, not executable for all els.
 */
#define __A64_PGTBLATTR_UDATA_BLK1G (__PGTBLATTR_A64BLK1G(0, 0, 1, 1, 0, 0, 0, 1, 1, \
				     __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0, \
				     __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL, 0))
#define __A64_PGTBLATTR_UDATA_BLK2M (__PGTBLATTR_A64BLK2M(0, 0, 1, 1, 0, 0, 0, 1, 1, \
				     __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0, \
				     __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL, 0))
#define __A64_PGTBLATTR_UDATA_PAGE4K (__PGTBLATTR_A64PAGE4K(0, 0, 1, 1, 0, 0, 1, 1, \
				     __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0, \
				     __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL, 0))

#endif

#ifndef __PGTBLATTR_MODEL
#define __PGTBLATTR_MODEL(name)
#endif

__PGTBLATTR_MODEL(UDATA)
