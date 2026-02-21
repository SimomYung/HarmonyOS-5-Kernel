/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: hmpsfmod kernel object uapi
 * Author: Huawei OS Kernel Lab
 * Created: Thu Aug 31 15:16:48 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_HMPSFMOD_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_HMPSFMOD_H

#include <hmkernel/ctrlmem/kobjs/hmpsfmap.h>
#include <lib/hmpsf/hmpsf_def.h>

struct __ctrlmem_kobj_hmpsfmod_s {
	union {
		/*
		 * hmpsfmod can only be accessed as a whole by user space. So, when using ctrlmem
		 * syscalls to access a hmpsfmod kobj, the offset must be zero.
		 */
		__u32 op;
		unsigned char padding[256];
	};
};

enum __ctrlmem_kobj_hmpsfmod_op {
	__CTRLMEM_KOBJ_HMPSFMOD_LOAD,
	__CTRLMEM_KOBJ_HMPSFMOD_UNLOAD,
	__CTRLMEM_KOBJ_HMPSFMOD_INVALIDATE,
};

/*
 * hmpsf attributes for loading to kernel
 */
struct hmpsf_module_kernel_attr_s {
	struct hmpsf_module_proxy_attr_s proxy_attr;
	unsigned int module_id;

	struct __ctrlmem_kobj_hmpsfmap_s *map_kobjs[HMPSF_MODULE_MAX_MAPS];
	unsigned int map_cnt;
};

#endif
