/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Page table attribute for uvde
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 01 11:38:55 2023
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_UVDEV_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_UVDEV_H

#include <uapi/hmasm/pgtbl/pgtblattr/uvdev.h>

#define A64_PGATTR_UVDEV_MAPPING A64_PGTBLATTR ( \
	__PGTBLHDLR_HLATTR_A64_SETTING_USER, \
	__PGTBLATTR_DEFAULT_A64TTBRREG, \
	__PGTBLATTR_DEFAULT_A64TBL512G, \
	__PGTBLATTR_DEFAULT_A64TBL1G, \
	__PGTBLATTR_DEFAULT_A64TBL2M, \
	__A64_PGTBLATTR_UVDEV_BLK1G, \
	__A64_PGTBLATTR_UVDEV_BLK2M, \
	__A64_PGTBLATTR_UVDEV_PAGE4K)

#define PGATTR_UVDEV_MAPPING { \
	.a = A64_PGATTR_UVDEV_MAPPING, \
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/uvdev.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(UVDEV, false, false, false, false, false, false, false, true)
