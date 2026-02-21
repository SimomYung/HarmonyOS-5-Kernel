/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Definitions for vregion cache object
 * Author: Huawei OS Kernel Lab
 * Created: Tue Apr 18 2023
 */

#ifndef MAPI_HMKERNEL_CTRLMEM_KOBJS_VRCACHE_H
#define MAPI_HMKERNEL_CTRLMEM_KOBJS_VRCACHE_H

#include <uapi/hmkernel/ctrlmem/kobjs/vrcache.h>
#include <hmkernel/ctrlmem/kobjs/syspa.h>

/*
 * Do not include vregion.h to make dependency one-way.
 * vregion-shared on vregion.h will include vrcache.
 */
struct ctrlmem_kobj_vregion_s;

#define CTRLMEM_KOBJ_VRCACHE_BITS	__CTRLMEM_KOBJ_VRCACHE_BITS
#define CTRLMEM_KOBJ_VRCACHE_NUM	__CTRLMEM_KOBJ_VRCACHE_NUM

#define CTRLMEM_KOBJ_VRCACHE_SHIFT	__CTRLMEM_KOBJ_VRCACHE_SHIFT
#define CTRLMEM_KOBJ_VRCACHE_MASK	__CTRLMEM_KOBJ_VRCACHE_MASK
#define CTRLMEM_KOBJ_VRCACHE_HASH(va)	__CTRLMEM_KOBJ_VRCACHE_HASH(va)

struct ctrlmem_kobj_vrcache_ent_s {
	union {
		ctrlmem_syspa(struct ctrlmem_kobj_vregion_s) vregion;

		u64 padding;
	};
};

struct ctrlmem_kobj_vrcache_s {
	struct ctrlmem_kobj_vrcache_ent_s entries[CTRLMEM_KOBJ_VRCACHE_NUM];
};

#endif
