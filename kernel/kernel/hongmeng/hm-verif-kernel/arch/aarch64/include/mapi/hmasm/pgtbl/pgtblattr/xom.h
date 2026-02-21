/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for xom
 * Author: Huawei OS Kernel Lab
 * Created: Sun Oct 09 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_XOM_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_XOM_H

#include <uapi/hmasm/pgtbl/pgtblattr/xom.h>

#define A64_PGATTR_XOM_MAPPING A64_PGTBLATTR ( \
	__PGTBLHDLR_HLATTR_A64_SETTING_USER, \
	__PGTBLATTR_DEFAULT_A64TTBRREG, \
	__PGTBLATTR_DEFAULT_A64TBL512G, \
	__PGTBLATTR_DEFAULT_A64TBL1G, \
	__PGTBLATTR_DEFAULT_A64TBL2M, \
	__A64_PGTBLATTR_XOM_BLK1G, \
	__A64_PGTBLATTR_XOM_BLK2M, \
	__A64_PGTBLATTR_XOM_PAGE4K)

#define PGATTR_XOM_MAPPING { \
	.a = A64_PGATTR_XOM_MAPPING \
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/xom.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(XOM, false, true, false, false, false, false, true, false)
