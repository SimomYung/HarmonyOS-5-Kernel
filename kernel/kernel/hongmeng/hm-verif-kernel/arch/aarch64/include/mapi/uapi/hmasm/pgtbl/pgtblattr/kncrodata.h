/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for kncrodata
 * Author: Huawei OS Kernel Lab
 * Created: Mon Jul 11 2022
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_KNCRODATA_H
#define AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_KNCRODATA_H

#include <hmasm/pgtbl/pgtblattr.h>

/*
 * For kernel, NG bit should not be set.
 * For serveice, NG bit should be set and should be adjusted if PRIVILEGED_SERVICE is configured.
 * Set AP21 to 2 to make it readonly at privilege exception level.
 * See table D5-33 (Stage 1 access permissions for instruction execution for 
 * a translation regime that applies to EL0 and a higher Exception level) in ARM64 manual,
 * when XN = 1, PXN = 1, AP21 = 2, access permission is R, not executable for all els.
 */
#define __A64_PGTBLATTR_KNCRODATA_BLK1G (__PGTBLATTR_A64BLK1G(0, 0, 1, 1, 0, 0, 0, 0, 1, \
					 __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 2, 0, \
					 __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL_NC, 0))
#define __A64_PGTBLATTR_KNCRODATA_BLK2M (__PGTBLATTR_A64BLK2M(0, 0, 1, 1, 0, 0, 0, 0, 1, \
					 __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 2, 0, \
					 __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL_NC, 0))
#define __A64_PGTBLATTR_KNCRODATA_PAGE4K (__PGTBLATTR_A64PAGE4K(0, 0, 1, 1, 0, 0, 0, 1, \
					  __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 2, 0, \
					  __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL_NC, 0))

#endif

#ifndef __PGTBLATTR_MODEL
#define __PGTBLATTR_MODEL(name)
#endif

__PGTBLATTR_MODEL(KNCRODATA)
