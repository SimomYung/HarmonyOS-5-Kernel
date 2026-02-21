/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for sdev
 * Author: Huawei OS Kernel Lab
 * Created: Sun Apr 24 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SDEV_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SDEV_H

#include <uapi/hmasm/pgtbl/pgtblattr/kdev.h>
#include <uapi/hmasm/pgtbl/pgtblattr/udev.h>

#ifdef CONFIG_PRIVILEGED_SERVICE
#define A64_PGATTR_SDEV_MAPPING A64_PGTBLATTR( \
		__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_KDEV_BLK1G, \
		__A64_PGTBLATTR_KDEV_BLK2M, \
		__A64_PGTBLATTR_KDEV_PAGE4K)
#else
#define A64_PGATTR_SDEV_MAPPING A64_PGTBLATTR ( \
		__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_UDEV_BLK1G, \
		__A64_PGTBLATTR_UDEV_BLK2M, \
		__A64_PGTBLATTR_UDEV_PAGE4K)
#endif

#define PGATTR_SDEV_MAPPING { \
	.a = A64_PGATTR_SDEV_MAPPING,\
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/sdev.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

#ifdef CONFIG_PRIVILEGED_SERVICE

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SDEV, false, true, true, false, false, false, false, true)

#else

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SDEV, false, true, true, false, true, true, false, true)

#endif
