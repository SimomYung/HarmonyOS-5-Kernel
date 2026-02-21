/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Attributes in arm64 page table - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 07 11:39:16 2021
 */

#ifndef AARCH64_MAPI_ASM_PGTBLATTR_H
#define AARCH64_MAPI_ASM_PGTBLATTR_H

#include <uapi/hmasm/pgtbl/pgtblattr.h>

#define A64_PGTBLATTR(a64setting, ttbrreg, a64tbl512g, a64tbl1g, a64tbl2m, a64blk1g, a64blk2m, a64page4k) \
{ \
	.hlattr.setting		= (a64setting), \
	.tmpl_a64ttbrreg.v	= (ttbrreg), \
	.tmpl_a64tbl512g.v	= (a64tbl512g), \
	.tmpl_a64tbl1g.v	= (a64tbl1g), \
	.tmpl_a64tbl2m.v	= (a64tbl2m), \
	.tmpl_a64blk1g.v	= (a64blk1g), \
	.tmpl_a64blk2m.v	= (a64blk2m), \
	.tmpl_a64page4k.v	= (a64page4k), \
}

#endif
