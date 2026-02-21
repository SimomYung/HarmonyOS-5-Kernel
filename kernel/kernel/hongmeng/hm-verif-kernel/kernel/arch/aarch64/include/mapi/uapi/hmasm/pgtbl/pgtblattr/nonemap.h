/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for nonemap
 * Author: Huawei OS Kernel Lab
 * Created: Tue Jun 07 2022
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_NONEMAP_H
#define AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_NONEMAP_H

#include <hmasm/pgtbl/pgtblattr.h>

/*
 * For nonemap, NG bit should be set.
 * Set AP21 to 2 to make it readonly at privilege exception level.
 * See table D5-33 (Stage 1 access permissions for instruction execution for
 * a translation regime that applies to EL0 and a higher Exception level) in ARM64 manual,
 * when XN = 1, PXN = 1, AP21 = 2, access permission is R, not executable for all els.
 */
#define __A64_PGTBLATTR_NONEMAP_BLK1G (____PGTBLATTR_A64BLK1G(0, 0, 1, 1, 0, 0, 0, 1, 1, \
				       __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 2, 0, \
				       __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL, 0, \
				       __PGTBLATTR_A64LV012NMP_MARK))
#define __A64_PGTBLATTR_NONEMAP_BLK2M (____PGTBLATTR_A64BLK2M(0, 0, 1, 1, 0, 0, 0, 1, 1, \
				       __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 2, 0, \
				       __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL, 0, \
				       __PGTBLATTR_A64LV012NMP_MARK))
#define __A64_PGTBLATTR_NONEMAP_PAGE4K (____PGTBLATTR_A64PAGE4K(0, 0, 1, 1, 0, 0, 1, 1, \
					__PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 2, 0, \
					__PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_NORMAL, 0, \
					__PGTBLATTR_A64LV3NONE_MARK))

#endif

#ifndef __PGTBLATTR_MODEL
#define __PGTBLATTR_MODEL(name)
#endif

__PGTBLATTR_MODEL(NONEMAP)
