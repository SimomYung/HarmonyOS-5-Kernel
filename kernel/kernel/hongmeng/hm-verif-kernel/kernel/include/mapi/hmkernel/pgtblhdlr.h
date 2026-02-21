/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: HMKernel page table handler - mapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 06 18:02:09 2021
 */

#ifndef MAPI_HMKERNEL_PGTBLHDLR_H
#define MAPI_HMKERNEL_PGTBLHDLR_H

#include <hmasm/memory.h>

#define __PGTBLHDLR_BASICTYPES_KEY	__vaddr_t
#define __PGTBLHDLR_BASICTYPES_KEY_MAX	__VADDR_T_MAX
#define __PGTBLHDLR_BASICTYPES_VAL	__paddr_t
#define __PGTBLHDLR_BASICTYPES_INVAL	((__paddr_t)(0ul))

#include <hmkernel/pgtblhdlr/types.h>
#include <hmkernel/pgtblhdlr/populate.h>

#undef __PGTBLHDLR_BASICTYPES_INVAL
#undef __PGTBLHDLR_BASICTYPES_VAL
#undef __PGTBLHDLR_BASICTYPES_KEY_MAX
#undef __PGTBLHDLR_BASICTYPES_KEY

#endif
