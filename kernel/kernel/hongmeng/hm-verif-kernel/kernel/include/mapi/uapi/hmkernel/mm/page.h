/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of page
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 12 20:03:34 2020
 */

#ifndef UAPI_HMKERNEL_PAGE_H
#define UAPI_HMKERNEL_PAGE_H

#include <hmasm/types.h>

#define __PAGE_TYPE_INVALID	0U
#define __PAGE_TYPE_USER	1U
#define __PAGE_TYPE_IOMEM	2U
#define __PAGE_TYPE_RSV		3U
#define __PAGE_TYPE_CONTIG	4U

#define __PAGE_TYPESET_BIT_INVALID	(0UL)
#define __PAGE_TYPESET_BIT_USER		(1UL << __PAGE_TYPE_USER)
#define __PAGE_TYPESET_BIT_IOMEM	(1UL << __PAGE_TYPE_IOMEM)
#define __PAGE_TYPESET_BIT_RSV		(1UL << __PAGE_TYPE_RSV)
#define __PAGE_TYPESET_BIT_CONTIG	(1UL << __PAGE_TYPE_CONTIG)

/* query all type, including IOMEM, USER and RSV */
#define __PAGE_TYPESET_BIT_ALL		(__PAGE_TYPESET_BIT_USER | \
					 __PAGE_TYPESET_BIT_IOMEM | \
					 __PAGE_TYPESET_BIT_RSV | \
					 __PAGE_TYPESET_BIT_CONTIG)
/* only query some range page type */
#define __PAGE_TYPESET_BIT_MASK		(__PAGE_TYPESET_BIT_USER | \
					 __PAGE_TYPESET_BIT_IOMEM | \
					 __PAGE_TYPESET_BIT_RSV | \
					 __PAGE_TYPESET_BIT_CONTIG)

struct __page_range {
	__u64 pfn_start;
	__u64 pfn_end;
	__u32 nid;
	__u32 type;
	__u32 refcnt;
};

#endif
