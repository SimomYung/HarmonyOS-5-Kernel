/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for scode
 * Author: Huawei OS Kernel Lab
 * Created: Tue Mar 08 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SCODE_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SCODE_H

#include <uapi/hmasm/pgtbl/pgtblattr/kcode.h>
#include <uapi/hmasm/pgtbl/pgtblattr/ucode.h>

#ifdef CONFIG_PRIVILEGED_SERVICE
#define A64_PGATTR_SCODE_MAPPING A64_PGTBLATTR ( \
	__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
	__PGTBLATTR_DEFAULT_A64TTBRREG, \
	__PGTBLATTR_DEFAULT_A64TBL512G, \
	__PGTBLATTR_DEFAULT_A64TBL1G, \
	__PGTBLATTR_DEFAULT_A64TBL2M, \
	__A64_PGTBLATTR_KCODE_BLK1G, \
	__A64_PGTBLATTR_KCODE_BLK2M, \
	__A64_PGTBLATTR_KCODE_PAGE4K)
#else
#define A64_PGATTR_SCODE_MAPPING A64_PGTBLATTR ( \
	__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
	__PGTBLATTR_DEFAULT_A64TTBRREG, \
	__PGTBLATTR_DEFAULT_A64TBL512G, \
	__PGTBLATTR_DEFAULT_A64TBL1G, \
	__PGTBLATTR_DEFAULT_A64TBL2M, \
	__A64_PGTBLATTR_UCODE_BLK1G, \
	__A64_PGTBLATTR_UCODE_BLK2M, \
	__A64_PGTBLATTR_UCODE_PAGE4K)
#endif

#define PGATTR_SCODE_MAPPING { \
	.a = A64_PGATTR_SCODE_MAPPING \
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/scode.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

#ifdef CONFIG_PRIVILEGED_SERVICE

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SCODE, false, true, false, true, false, false, false, false)

#else

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SCODE, false, true, false, false, true, false, true, false)

#endif
