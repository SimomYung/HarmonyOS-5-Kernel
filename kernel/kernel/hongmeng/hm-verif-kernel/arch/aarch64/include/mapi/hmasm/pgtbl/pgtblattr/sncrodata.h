/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Page table attribute for sncrodata
 * Author: Huawei OS Kernel Lab
 * Created: Mon Jul 11 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SNCRODATA_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_SNCRODATA_H

#include <uapi/hmasm/pgtbl/pgtblattr/krodata.h>
#include <uapi/hmasm/pgtbl/pgtblattr/urodata.h>

#ifdef CONFIG_PRIVILEGED_SERVICE
#define A64_PGATTR_SNCRODATA_MAPPING A64_PGTBLATTR( \
		__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_KRODATA_BLK1G, \
		__A64_PGTBLATTR_KRODATA_BLK2M, \
		__A64_PGTBLATTR_KRODATA_PAGE4K)
#else
#define A64_PGATTR_SNCRODATA_MAPPING A64_PGTBLATTR ( \
		__PGTBLHDLR_HLATTR_A64_SETTING_SERVICE, \
		__PGTBLATTR_DEFAULT_A64TTBRREG, \
		__PGTBLATTR_DEFAULT_A64TBL512G, \
		__PGTBLATTR_DEFAULT_A64TBL1G, \
		__PGTBLATTR_DEFAULT_A64TBL2M, \
		__A64_PGTBLATTR_URODATA_BLK1G, \
		__A64_PGTBLATTR_URODATA_BLK2M, \
		__A64_PGTBLATTR_URODATA_PAGE4K)
#endif

#define PGATTR_SNCRODATA_MAPPING { \
	.a = A64_PGATTR_SNCRODATA_MAPPING,\
}

#endif

#include <uapi/hmasm/pgtbl/pgtblattr/sncrodata.h>

#ifndef __PGTBLATTR_MODEL_PROPERTY
#define __PGTBLATTR_MODEL_PROPERTY(name, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device)
#endif

#ifdef CONFIG_PRIVILEGED_SERVICE

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SNCRODATA, false, true, false, false, false, false, false, false)

#else

/* property format: (model, stage2, kread, kwrite, kexec, uread, uwrite, uexec, device) */
__PGTBLATTR_MODEL_PROPERTY(SNCRODATA, false, true, false, false, true, false, false, false)

#endif
