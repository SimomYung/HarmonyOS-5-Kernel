/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for swcode
 * Author: Huawei OS Kernel Lab
 * Created: Tue Mar 08 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SWCODE_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SWCODE_H

#include <uapi/hmasm/pgtbl/pgtblattr/kwcode.h>
#include <uapi/hmasm/pgtbl/pgtblattr/uwcode.h>

#ifdef CONFIG_PRIVILEGED_SERVICE
#define A64_PGATTR_SWCODE_MAPPING A64_PGTBLATTR( \
		__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_KWCODE_BLK1G, \
		__A64_PGTBLATTR_KWCODE_BLK2M, \
		__A64_PGTBLATTR_KWCODE_PAGE4K)
#else
#define A64_PGATTR_SWCODE_MAPPING A64_PGTBLATTR ( \
		__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_UWCODE_BLK1G, \
		__A64_PGTBLATTR_UWCODE_BLK2M, \
		__A64_PGTBLATTR_UWCODE_PAGE4K)
#endif

#define PGATTR_SWCODE_MAPPING { \
	.a = A64_PGATTR_SWCODE_MAPPING, \
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/swcode.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

#ifdef CONFIG_PRIVILEGED_SERVICE

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SWCODE, false, true, true, true, false, false, false, false)

#else

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SWCODE, false, true, true, false, true, true, true, false)

#endif
