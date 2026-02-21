/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Definitions for kobj actv binder context
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 17 01:36:17 2024
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_ACTV_BINDER_CONTEXT_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_ACTV_BINDER_CONTEXT_H

#include <hmkernel/types.h>

#define __CTRLMEM_KOBJ_ACTV_BINDER_CONTEXT_SIZE		64UL

struct __ctrlmem_kobj_actv_binder_context_s {
	__u8 padding[__CTRLMEM_KOBJ_ACTV_BINDER_CONTEXT_SIZE];
};

#endif /* MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_ACTV_BINDER_CONTEXT_H */
