/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kobj vspace extension - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 04 21:55:06 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_VSPACEEXT_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_VSPACEEXT_H

#include <hmkernel/ctrlmem/kobjctnr.h>

struct __ctrlmem_kobj_vspaceext_s {
	__u64 kptr;
	struct {
		__u64 pvoffset;
		__u64 area_start;
		__u64 area_end;
	} linear;
};

#endif
