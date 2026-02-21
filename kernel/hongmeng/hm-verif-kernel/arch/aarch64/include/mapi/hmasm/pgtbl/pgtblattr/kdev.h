/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for kdev - mapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 18 16:58:43 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_KDEV_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_KDEV_H

#include <uapi/hmasm/pgtbl/pgtblattr/kdev.h>

#define A64_PGATTR_KDEV_MAPPING A64_PGTBLATTR( \
		__PGTBLHDLR_HLATTR_A64_SETTING_KERNEL, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_KDEV_BLK1G, \
		__A64_PGTBLATTR_KDEV_BLK2M, \
		__A64_PGTBLATTR_KDEV_PAGE4K)

#define PGATTR_KDEV_MAPPING { \
	.a = A64_PGATTR_KDEV_MAPPING,\
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/kdev.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(KDEV, false, true, true, false, false, false, false, true)
