/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: This is the header of udk_slab interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu June 01 10:09:28 2023
 */
#ifndef __LINUX_SLAB_H
#define __LINUX_SLAB_H

#include <udk/types.h>

#ifndef	MAX_ORDER
#define	MAX_ORDER	11
#endif

#ifndef	PAGE_SHIFT
#define	PAGE_SHIFT	12
#endif

#define	KMALLOC_SHIFT_HIGH	(PAGE_SHIFT + 1)
#define	KMALLOC_SHIFT_MAX	(MAX_ORDER + PAGE_SHIFT - 1)
#ifndef	KMALLOC_SHIFT_LOW
#define	KMALLOC_SHIFT_LOW	3
#endif

#define	KMALLOC_MAX_SIZE		(1UL << KMALLOC_SHIFT_MAX)
#define	KMALLOC_MAX_CACHE_SIZE	(1UL << KMALLOC_SHIFT_HIGH)
#define	KMALLOC_MAX_ORDER		(KMALLOC_SHIFT_MAX - PAGE_SHIFT)

#ifndef	KMALLOC_MIN_SIZE
#define	KMALLOC_MIN_SIZE	(1UL << KMALLOC_SHIFT_LOW)
#endif

#define	SLAB_OBJ_MIN_SIZE	(KMALLOC_MIN_SIZE < 16 ? \
							(KMALLOC_MIN_SIZE) : 16)

#endif
