/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Page table attribute for stage2 rodata
 * Author: Huawei OS Kernel Lab
 * Created: Thu Mar 16 2023
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBLA64S2_PGTBLATTR_RODATA_S2_H
#define AARCH64_MAPI_UAPI_ASM_PGTBLA64S2_PGTBLATTR_RODATA_S2_H

#include <hmasm/pgtbla64s2/attr.h>

/*
 * arm manual:
 * https://developer.arm.com/documentation/ddi0487/ia
 * Document number ID081822, Document version I.a
 *
 * Set S2AP to 1 to make it read only at non-secure el1 and non-secure el0
 * See Table (D8-40 Data access permissions for a stage 2 translation) in ARM64 manual,
 * See Chapter (D8.4.4 Instruction execution permissions) in ARM64 manual,
 * when XN10 = 2, S2AP = 1, access permission is R, not executable for el0 and el1.
 */
#define __A64S2_PGTBLATTR_RODATA_S2_BLK1G (__PGTBLATTR_STAGE2_A64S2BLK1G(0, 2, 0, 0, 0, 0, 1, \
					 __PGTBLATTR_A64S2LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0xf, 1, 0, 0))
#define __A64S2_PGTBLATTR_RODATA_S2_BLK2M (__PGTBLATTR_STAGE2_A64S2BLK2M(0, 2, 0, 0, 0, 0, 1, \
					 __PGTBLATTR_A64S2LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0xf, 1, 0, 0))
#define __A64S2_PGTBLATTR_RODATA_S2_PAGE4K (__PGTBLATTR_STAGE2_A64S2PAGE4K(0, 2, 0, 0, 0, 1, \
					  __PGTBLATTR_A64S2LOWERBLKATTR_SH_INNER_SHAREABLE, 1, 0xf, 1, 0, 0))

#define A64S2_PGATTR_RODATA_S2_MAPPING \
{ \
	.a64s2ttbrreg	= __PGTBLATTR_DEFAULT_A64S2TTBRREG, \
	.a64s2tbl512g	= __PGTBLATTR_DEFAULT_A64S2TBL512G, \
	.a64s2tbl1g	= __PGTBLATTR_DEFAULT_A64S2TBL1G, \
	.a64s2tbl2m	= __PGTBLATTR_DEFAULT_A64S2TBL2M, \
	.a64s2blk1g	= __A64S2_PGTBLATTR_RODATA_S2_BLK1G, \
	.a64s2blk2m	= __A64S2_PGTBLATTR_RODATA_S2_BLK2M, \
	.a64s2page4k	= __A64S2_PGTBLATTR_RODATA_S2_PAGE4K, \
}

#endif


#ifndef __PGTBLATTR_MODEL
#define __PGTBLATTR_MODEL(name)
#endif

__PGTBLATTR_MODEL(RODATA_S2)
