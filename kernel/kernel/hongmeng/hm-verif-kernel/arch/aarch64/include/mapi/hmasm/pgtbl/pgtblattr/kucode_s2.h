/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for stage2 kucode
 * Author: Huawei OS Kernel Lab
 * Created: Tue Apr 26 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_KUCODE_S2_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_KUCODE_S2_H

#include <uapi/hmasm/pgtbl/pgtblattr/kucode_s2.h>

#define A64_PGATTR_KUCODE_S2_MAPPING A64_PGTBLATTR ( \
	__PGTBLHDLR_HLATTR_A64_SETTING_S2_NORMAL, \
	__PGTBLATTR_DEFAULT_A64TTBRREG, \
	__PGTBLATTR_DEFAULT_A64TBL512G, \
	__PGTBLATTR_DEFAULT_A64TBL1G, \
	__PGTBLATTR_DEFAULT_A64TBL2M, \
	__A64_PGTBLATTR_KUCODE_S2_BLK1G, \
	__A64_PGTBLATTR_KUCODE_S2_BLK2M, \
	__A64_PGTBLATTR_KUCODE_S2_PAGE4K)

#define PGATTR_KUCODE_S2_MAPPING { \
	.a = A64_PGATTR_KUCODE_S2_MAPPING \
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/kucode_s2.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(KUCODE_S2, true, true, false, true, true, false, true, false)
