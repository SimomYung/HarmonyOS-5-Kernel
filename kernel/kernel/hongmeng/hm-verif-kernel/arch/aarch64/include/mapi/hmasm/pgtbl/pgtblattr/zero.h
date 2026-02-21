/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Page table attribute for zero
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 07 16:01:48 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_ZERO_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_ZERO_H

#include <hmasm/pgtbl/pgtblattr.h>

/*
 * This scenario is used for temporary init pgattr_t to suppress warning. Doesn't care everything...
 * Set to setting to __PGTBLHDLR_HLATTR_A64_SETTING_KERNEL because value of it is 0.
 */
#define A64_PGATTR_ZERO A64_PGTBLATTR(	\
		__PGTBLHDLR_HLATTR_A64_SETTING_KERNEL, \
		(__u64)0u,	\
		(__u64)0u,	\
		(__u64)0u,	\
		(__u64)0u,	\
		(__u64)0u,	\
		(__u64)0u,	\
		(__u64)0u)

/*
 * Not _MAPPING suffix because this is not a mapping scenario.
 */
#define PGATTR_ZERO { \
	.a = A64_PGATTR_ZERO,\
}

#endif
