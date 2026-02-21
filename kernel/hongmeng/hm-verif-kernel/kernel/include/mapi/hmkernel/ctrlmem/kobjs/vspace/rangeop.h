/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Rangeop is kobj_vspace method with va-range as argument - mapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 15 23:00:23 2023
 */

#ifndef MAPI_HMKERNEL_CTRLMEM_KOBJS_VSPACE_RANGEOP_H
#define MAPI_HMKERNEL_CTRLMEM_KOBJS_VSPACE_RANGEOP_H

#include <uapi/hmkernel/ctrlmem/kobjs/vspace/rangeop.h>

#define __ctrlmem_kobj_vspace_rangeop_unmap_urecord_mkparec(_pa, _is_present, _is_fragile)	\
	(((_pa) | ((_is_present) ? __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_PRESENT : (__u64)(0UL))) \
		| ((_is_fragile) ? __CTRLMEM_KOBJ_VSPACE_RANGEOP_UNMAP_URECORD_PAREC_FRAGILE : (__u64)(0ULL)))

#endif
