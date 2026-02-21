/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Page table attribute for upgd - mapi
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 07 14:19:53 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_UPGD_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_UPGD_H

#include <hmasm/pgtbl/pgtblattr.h>

/*
 * This scenario is used for populate PGD only. doesn't care other levels.
 */
#define A64_PGATTR_UPGD A64_PGTBLATTR(	\
		__PGTBLHDLR_HLATTR_A64_SETTING_USER,	\
		__PGTBLATTR_DEFAULT_A64TTBRREG,		\
		__PGTBLATTR_A64TBL512G(0, 0, 0, 0, 0),	\
		__PGTBLATTR_A64TBL1G(0, 0, 0, 0, 0),	\
		__PGTBLATTR_A64TBL2M(0, 0, 0, 0, 0),	\
		__PGTBLATTR_A64BLK1G(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), \
		__PGTBLATTR_A64BLK2M(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0), \
		__PGTBLATTR_A64PAGE4K(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0))

/*
 * Not mapping, no _MAPPING suffix.
 */
#define PGATTR_UPGD { \
	.a = A64_PGATTR_UPGD,\
}

#endif
