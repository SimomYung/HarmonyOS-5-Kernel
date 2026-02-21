/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: syspgarr metadata as kobject - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 01 11:12:02 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_SYSPGARR_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_SYSPGARR_H

#include <hmkernel/types.h>

#define __CTRLMEM_KOBJ_SYSPGARR_PADDING_N	4
struct __ctrlmem_kobj_syspgarr_s {
	__u64 padding[__CTRLMEM_KOBJ_SYSPGARR_PADDING_N];
};

#define __CTRLMEM_KOBJ_SYSPGARR_TYPE_SPARSE	0 /* sparse model */
#define __CTRLMEM_KOBJ_SYSPGARR_TYPE_DPA	1 /* double page array */

#endif
