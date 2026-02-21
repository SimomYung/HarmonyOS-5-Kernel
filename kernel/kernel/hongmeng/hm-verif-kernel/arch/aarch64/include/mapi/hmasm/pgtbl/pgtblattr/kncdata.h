/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for kncdata
 * Author: Huawei OS Kernel Lab
 * Created: Tue Jan 25 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_KNCDATA_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_KNCDATA_H

#include <uapi/hmasm/pgtbl/pgtblattr/kncdata.h>

#define A64_PGATTR_KNCDATA_MAPPING A64_PGTBLATTR ( \
	__PGTBLHDLR_HLATTR_A64_SETTING_KERNEL, \
	__PGTBLATTR_DEFAULT_A64TTBRREG, \
	__PGTBLATTR_DEFAULT_A64TBL512G, \
	__PGTBLATTR_DEFAULT_A64TBL1G, \
	__PGTBLATTR_DEFAULT_A64TBL2M, \
	__A64_PGTBLATTR_KNCDATA_BLK1G, \
	__A64_PGTBLATTR_KNCDATA_BLK2M, \
	__A64_PGTBLATTR_KNCDATA_PAGE4K)

#define PGATTR_KNCDATA_MAPPING { \
	.a = A64_PGATTR_KNCDATA_MAPPING \
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/kncdata.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(KNCDATA, false, true, true, false, false, false, false, false)
