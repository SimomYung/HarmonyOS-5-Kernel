/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: ulwt kernel object uapi
 * Author: Huawei OS Kernel Lab
 * Created: Thu Aug 31 15:16:48 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_ULWT_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_ULWT_H

#define __CTRLMEM_KOBJ_ULWT_SIZE 1024UL

struct __ctrlmem_kobj_ulwt_s {
	__u8 padding[__CTRLMEM_KOBJ_ULWT_SIZE];
};

#endif
