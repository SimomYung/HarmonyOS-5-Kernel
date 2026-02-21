/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: hmpsfmap kernel object uapi
 * Author: Huawei OS Kernel Lab
 * Created: Thu Nov 9 15:02:50 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_HMPSFMAP_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_HMPSFMAP_H

#include <hmasm/memory.h>
#include <hmasm/types.h>
#include <lib/hmpsf/hmpsf_def.h>

struct __ctrlmem_kobj_hmpsfmap_s {
	union {
		/*
		 * hmpsfmap can only be accessed as a whole by user space. So, when using ctrlmem
		 * syscalls to access a hmpsfmap kobj, the offset must be zero.
		 */
		__u32 op;
		unsigned char padding[256];
	};
};

enum __ctrlmem_kobj_hmpsfmap_op {
	__CTRLMEM_KOBJ_HMPSFMAP_MAP_CREATE,
	__CTRLMEM_KOBJ_HMPSFMAP_MAP_DELETE,

	__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_LOAD,
	__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_STORE,
	__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_EXCHANGE,
	__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_COMPARE_EXCHANGE,
	__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_ADD_FETCH,
	__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_SUB_FETCH,
	__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_NEXT_KEY,
	__CTRLMEM_KOBJ_HMPSFMAP_INT_HASH_MAP_CLEAR,

	__CTRLMEM_KOBJ_HMPSFMAP_BYTES_HASH_MAP_GET,
	__CTRLMEM_KOBJ_HMPSFMAP_BYTES_HASH_MAP_SET,
	__CTRLMEM_KOBJ_HMPSFMAP_BYTES_HASH_MAP_NEXT_KEY,
	__CTRLMEM_KOBJ_HMPSFMAP_BYTES_HASH_MAP_CLEAR,
};

#define __CTRLMEM_KOBJ_HMPSFMAP_FLAGS_SHARED	0x1

struct __ctrlmem_kobj_hmpsfmap_spawn_args_s {
	void *pages_pgstr;
	__paddr_t pages_pa;
	__u32 pages_num;
	__u64 flags;
};

#endif
