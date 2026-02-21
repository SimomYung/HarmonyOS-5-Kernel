/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Definitions for kobj actv binder refer
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 17 01:36:17 2024
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_ACTV_BINDER_REFER_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_ACTV_BINDER_REFER_H

#include <hmkernel/types.h>

#define __CTRLMEM_KOBJ_ACTV_BINDER_REFER_SIZE		64UL

struct __ctrlmem_kobj_actv_binder_refer_s {
	union {
		struct {
			unsigned int op;
		};
		__u8 padding[__CTRLMEM_KOBJ_ACTV_BINDER_REFER_SIZE];
	};
};

enum __ctrlmem_kobj_actv_binder_refer_op {
	__CTRLMEM_KOBJ_AB_REFER_LINK,
	__CTRLMEM_KOBJ_AB_REFER_UNLINK,
	__CTRLMEM_KOBJ_AB_REFER_REVOKE,
};

#endif /* MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_ACTV_BINDER_REFER_H */
