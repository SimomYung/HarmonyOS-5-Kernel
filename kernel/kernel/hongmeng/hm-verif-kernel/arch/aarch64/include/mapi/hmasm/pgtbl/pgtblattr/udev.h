/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for udev
 * Author: Huawei OS Kernel Lab
 * Created: Sun Apr 24 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_UDEV_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_UDEV_H

#include <uapi/hmasm/pgtbl/pgtblattr/udev.h>

#define A64_PGATTR_UDEV_MAPPING A64_PGTBLATTR( \
		__PGTBLHDLR_HLATTR_A64_SETTING_USER, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_UDEV_BLK1G, \
		__A64_PGTBLATTR_UDEV_BLK2M, \
		__A64_PGTBLATTR_UDEV_PAGE4K)

#define PGATTR_UDEV_MAPPING { \
	.a = A64_PGATTR_UDEV_MAPPING,\
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/udev.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(UDEV, false, true, true, false, true, true, false, true)
