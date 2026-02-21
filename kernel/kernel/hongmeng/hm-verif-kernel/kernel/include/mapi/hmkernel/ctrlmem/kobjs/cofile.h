/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Definitions for cofile object
 * Author: Huawei OS Kernel Lab
 * Created: Wed Jun 28 01:47:59 2023
 */

#ifndef MAPI_HMKERNEL_CTRLMEM_KOBJS_COFILE_H
#define MAPI_HMKERNEL_CTRLMEM_KOBJS_COFILE_H

#include <uapi/hmkernel/ctrlmem/kobjs/cofile.h>
#include <hmkernel/kernel.h>

ENUM(1, kobjcof_type, KOBJCOF_TYPE_,
	PROCFS,
);

ENUM(2, kobjcof_fdata_status, KOBJCOF_FDATA_,
	CAN_READ,
	NEED_FETCH,
);

#endif
