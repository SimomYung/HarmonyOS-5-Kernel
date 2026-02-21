/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: pgtblattr model related definitions
 * Author: Huawei OS Kernel Lab
 * Created: Mon Apr 25 2022
 */

#ifndef AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_MODEL_H
#define AARCH64_MAPI_UAPI_ASM_PGTBL_PGTBLATTR_MODEL_H

#define __PGTBLATTR_MODELID(name) __A64_PGTBLATTR_MODELID_##name

#include <hmasm/pgtbl/pgtblattr/all_noguard.h>
enum __pgtblattr_modelid_e {
#undef __PGTBLATTR_MODEL
#define __PGTBLATTR_MODEL(name) __PGTBLATTR_MODELID(name),
#include <hmasm/pgtbl/pgtblattr/all_noguard.h>
	__A64_PGTBLATTR_MODELID_INVAL,
#undef __PGTBLATTR_MODEL
};

/*
 * MODEL_REF are reference models used for page table walker. When we don't care
 * content of a page table entry but care about the structure of page table, use
 * model ref.
 */
#define __PGTBLATTR_MODEL_REF_UDECODE()			__PGTBLATTR_MODELID(UDATA)
#define __PGTBLATTR_MODEL_REF_KDECODE()			__PGTBLATTR_MODELID(KDATA)
#define __PGTBLATTR_MODEL_REF_USCAN(privileged)		((privileged) ? \
								__PGTBLATTR_MODELID(SDATA) : \
								__PGTBLATTR_MODELID(UDATA))
#define __PGTBLATTR_MODEL_REF_KSCAN()			__PGTBLATTR_MODELID(KDATA)

#endif
