/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Definitions for kobj actv frame
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 19 14:05:56 2024
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_ACTV_FRAME_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_ACTV_FRAME_H

#include <hmkernel/types.h>

#define __CTRLMEM_KOBJ_ACTV_FRAME_SIZE		128UL

struct __ctrlmem_kobj_actv_frame_s {
	__u8 padding[__CTRLMEM_KOBJ_ACTV_FRAME_SIZE];
};

#endif /* MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_ACTV_FRAME_H */
