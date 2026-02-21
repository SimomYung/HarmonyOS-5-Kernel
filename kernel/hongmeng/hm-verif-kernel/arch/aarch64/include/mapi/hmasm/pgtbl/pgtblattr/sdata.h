/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for sdata
 * Author: Huawei OS Kernel Lab
 * Created: Mon Mar 07 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SDATA_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SDATA_H

#include <uapi/hmasm/pgtbl/pgtblattr/kdata.h>
#include <uapi/hmasm/pgtbl/pgtblattr/udata.h>

#ifdef CONFIG_PRIVILEGED_SERVICE
#define A64_PGATTR_SDATA_MAPPING A64_PGTBLATTR( \
		__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_KDATA_BLK1G, \
		__A64_PGTBLATTR_KDATA_BLK2M, \
		__A64_PGTBLATTR_KDATA_PAGE4K)
#else
#define A64_PGATTR_SDATA_MAPPING A64_PGTBLATTR ( \
		__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_UDATA_BLK1G, \
		__A64_PGTBLATTR_UDATA_BLK2M, \
		__A64_PGTBLATTR_UDATA_PAGE4K)
#endif

#define PGATTR_SDATA_MAPPING { \
	.a = A64_PGATTR_SDATA_MAPPING,\
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/sdata.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

#ifdef CONFIG_PRIVILEGED_SERVICE

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SDATA, false, true, true, false, false, false, false, false)

#else

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SDATA, false, true, true, false, true, true, false, false)

#endif
