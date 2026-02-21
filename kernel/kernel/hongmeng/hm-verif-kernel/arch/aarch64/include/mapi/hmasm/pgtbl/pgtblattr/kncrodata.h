/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for kncrodata
 * Author: Huawei OS Kernel Lab
 * Created: Mon Jul 11 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_KNCRODATA_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_KNCRODATA_H

#include <uapi/hmasm/pgtbl/pgtblattr/kncrodata.h>

#define A64_PGATTR_KNCRODATA_MAPPING A64_PGTBLATTR ( \
	__PGTBLHDLR_HLATTR_A64_SETTING_KERNEL, \
	__PGTBLATTR_DEFAULT_A64TTBRREG, \
	__PGTBLATTR_DEFAULT_A64TBL512G, \
	__PGTBLATTR_DEFAULT_A64TBL1G, \
	__PGTBLATTR_DEFAULT_A64TBL2M, \
	__A64_PGTBLATTR_KNCRODATA_BLK1G, \
	__A64_PGTBLATTR_KNCRODATA_BLK2M, \
	__A64_PGTBLATTR_KNCRODATA_PAGE4K)

#define PGATTR_KNCRODATA_MAPPING { \
	.a = A64_PGATTR_KNCRODATA_MAPPING \
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/kncrodata.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(KNCRODATA, false, true, false, false, false, false, false, false)
