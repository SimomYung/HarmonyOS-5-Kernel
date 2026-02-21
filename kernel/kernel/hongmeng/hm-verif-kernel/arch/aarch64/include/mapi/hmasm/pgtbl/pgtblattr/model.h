/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: pgtblattr model related definitions
 * Author: Huawei OS Kernel Lab
 * Created: Thu Feb 10 2022
 */

#ifndef AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_MODEL_H
#define AARCH64_MAPI_ASM_PGTBL_PGTBLATTR_MODEL_H

#include <uapi/hmasm/pgtbl/pgtblattr/model.h>
#include <hmkernel/types.h>

#define PGTBLATTR_MODELID(name) __PGTBLATTR_MODELID(name)
#define PGTBLATTR_MODELID_CNT __A64_PGTBLATTR_MODELID_INVAL

struct __pgtblattr_model_property {
	/* basic properties */
	__bool stage2;
	__bool kread;
	__bool kwrite;
	__bool kexec;
	__bool uread;
	__bool uwrite;
	__bool uexec;
	__bool device;
};

#endif
