/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for sncdata
 * Author: Huawei OS Kernel Lab
 * Created: Sun Apr 24 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SNCDATA_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SNCDATA_H

#include <uapi/hmasm/pgtbl/pgtblattr/kncdata.h>
#include <uapi/hmasm/pgtbl/pgtblattr/uncdata.h>

#ifdef CONFIG_PRIVILEGED_SERVICE
#define A64_PGATTR_SNCDATA_MAPPING A64_PGTBLATTR( \
		__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_KNCDATA_BLK1G, \
		__A64_PGTBLATTR_KNCDATA_BLK2M, \
		__A64_PGTBLATTR_KNCDATA_PAGE4K)
#else
#define A64_PGATTR_SNCDATA_MAPPING A64_PGTBLATTR ( \
		__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_UNCDATA_BLK1G, \
		__A64_PGTBLATTR_UNCDATA_BLK2M, \
		__A64_PGTBLATTR_UNCDATA_PAGE4K)
#endif

#define PGATTR_SNCDATA_MAPPING { \
	.a = A64_PGATTR_SNCDATA_MAPPING,\
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/sncdata.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

#ifdef CONFIG_PRIVILEGED_SERVICE

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SNCDATA, false, true, true, false, false, false, false, false)

#else

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SNCDATA, false, true, true, false, true, true, false, false)

#endif
