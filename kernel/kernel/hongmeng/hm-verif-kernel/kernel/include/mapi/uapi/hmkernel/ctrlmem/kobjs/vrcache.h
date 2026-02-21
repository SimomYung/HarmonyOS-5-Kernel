/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Definitions for vregion cache object
 * Author: Huawei OS Kernel Lab
 * Created: Tue Apr 18 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_VRCACHE_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_VRCACHE_H

#include <hmkernel/types.h>
#include <hmasm/memory.h>
#include <hmasm/page.h>

#define __CTRLMEM_KOBJ_VRCACHE_BITS	2
#define __CTRLMEM_KOBJ_VRCACHE_NUM	(1 << __CTRLMEM_KOBJ_VRCACHE_BITS)

/*
 * the index of proper vrcache is determined by the hash of the va which trigers the page fault
 * shift is used to get the hash of va
 * step is used to get the next slot
 */
#define __CTRLMEM_KOBJ_VRCACHE_SHIFT			__PAGE_SHIFT
#define __CTRLMEM_KOBJ_VRCACHE_MASK			(__CTRLMEM_KOBJ_VRCACHE_NUM - 1)
#define __CTRLMEM_KOBJ_VRCACHE_HASH(va)			(((va) >> __CTRLMEM_KOBJ_VRCACHE_SHIFT) & __CTRLMEM_KOBJ_VRCACHE_MASK)

struct __ctrlmem_kobj_vrcache_ent_s {
	union {
		struct __ctrlmem_kobj_vregion_s *vrcache;
		__u64 padding;
	};
};

struct __ctrlmem_kobj_vrcache_s {
	struct __ctrlmem_kobj_vrcache_ent_s entries[__CTRLMEM_KOBJ_VRCACHE_NUM];
};

#endif
