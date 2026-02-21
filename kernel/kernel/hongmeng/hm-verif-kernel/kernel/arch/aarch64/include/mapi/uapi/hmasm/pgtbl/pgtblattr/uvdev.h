/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Page table attribute for uvdev
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 01 11:42:58 2023
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_UVDEV_H
#define AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_UVDEV_H

#include <hmasm/pgtbl/pgtblattr.h>

/*
 * For nonemap, NG bit should be set.
 * Set AP21 to 2 to make it readonly at privilege exception level, and read from
 * privilege level should be avoided as physical memory not really exist.
 * See table D5-33 (Stage 1 access permissions for instruction execution for
 * a translation regime that applies to EL0 and a higher Exception level) in ARM64 manual,
 * when XN = 1, PXN = 1, AP21 = 2, access permission is R, not executable for all els.
 */
#define __A64_PGTBLATTR_UVDEV_BLK1G (__PGTBLATTR_A64BLK1G(0, 0, 1, 1, 0, 0, 0, 1, 1, \
				       __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 2, 0, \
				       __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRnE, 0))
#define __A64_PGTBLATTR_UVDEV_BLK2M (__PGTBLATTR_A64BLK2M(0, 0, 1, 1, 0, 0, 0, 1, 1, \
				       __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 2, 0, \
				       __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRnE, 0))
#define __A64_PGTBLATTR_UVDEV_PAGE4K (__PGTBLATTR_A64PAGE4K(0, 0, 1, 1, 0, 0, 1, 1, \
				       __PGTBLATTR_A64LOWERBLKATTR_SH_INNER_SHAREABLE, 2, 0, \
				       __PGTBLATTR_A64LOWERBLKATTR_ATTRIDX_DEVICE_nGnRnE, 0))
#endif

#ifndef __PGTBLATTR_MODEL
#define __PGTBLATTR_MODEL(name)
#endif

__PGTBLATTR_MODEL(UVDEV)
