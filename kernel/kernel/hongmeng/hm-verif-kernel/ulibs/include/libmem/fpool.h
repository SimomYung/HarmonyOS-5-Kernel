/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Fpool function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 10 17:46:01 2023
 */
#ifndef ULIBS_INCLUDE_LIBMEM_FPOOL_H
#define ULIBS_INCLUDE_LIBMEM_FPOOL_H

#include <hongmeng/types.h>

/*
 * By default:
 * non-continuous physical pages,
 * synchronously reclaimed only fs_clean,
 * OOM is triggered only once.
 * Non-PAGE_CACHE page. (slab or anon, not file anyway)
 */

/* Apply for page type */
/* bit for [15:0] */
#define FPOOL_ALLOC_PAGE_TYPE_MASK	0x0000FFFF
/* === PAGE_HUGE === */
/* Apply for a pagecache of the huge page type, (2 MB or 4 MB). */
#define FPOOL_ALLOC_HUGETLB		0x00000001
/* Apply for a huge page for the reserve (only the FPOOL_ALLOC_HUGETLB macro is valid). */
#define FPOOL_ALLOC_CONSUME_RESERVE	0x00000002
/* === PAGE_FILE === */
/* Apply for a pagecache page. */
#define FPOOL_ALLOC_PAGE_CACHE		0x00000004
/* Apply for a anon page(not slab) */
#define FPOOL_ALLOC_PAGE_ANON		0x00000008
/* Apply for movable pagecache page which can be migrated */
#define FPOOL_ALLOC_MOVEABLE		0x00000010
/* Apply for a signed code page for protection */
#define FPOOL_ALLOC_CODE		0x00000020
/*
 * Indicates whether memory reuse is supported. If yes, the cma/kernel zone memory
 * can be used for this application.
 * This flag must be used with FPOOL_ALLOC_MOVEABLE.
 */
#define FPOOL_ALLOC_REUSE		0x00000040

/* Apply for behavior */
/* bit for [31:16] */
#define FPOOL_ALLOC_BEHAVIOR_MASK	0xFFFF0000
/* If the application fails, OOM is not triggered */
#define FPOOL_ALLOC_NO_OOM		0x00010000
/* Do not alloc from slow */
#define FPOOL_ALLOC_NO_SLOW		0x00020000
/* Do not return page until alloc success for page */
#define FPOOL_ALLOC_NOFAIL		0x00040000
/* Alloc page for hkip */
#define FPOOL_ALLOC_HKIP		0x00080000
/* Alloc page for protect lru */
#define FPOOL_ALLOC_PROTECT		0x00100000

#endif /* ULIBS_INCLUDE_LIBMEM_FPOOL_H */
