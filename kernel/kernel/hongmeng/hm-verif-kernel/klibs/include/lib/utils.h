/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Common Macro implement
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 24 16:52:49 2018
 */
#ifndef KLIBS_LIB_UTILS_H
#define KLIBS_LIB_UTILS_H

#include <hmasm/types.h>

#ifdef __KERNEL__

#define ____OFFSET_OF_START_PAGE ((uptr_t)0x1000)
/* always use strict mode, avoiding warnings of zero address/pointer */
#define offset_of(type, member) \
	(((uptr_t)&((type *)____OFFSET_OF_START_PAGE)->member) - ____OFFSET_OF_START_PAGE)

#define container_of(ptr, type, member) \
	((type *)(((uptr_t)(ptr)) - offset_of(type, member)))
#else

#include <stdint.h>
#define ____OFFSET_OF_START_PAGE ((uintptr_t)0x1000)
#define offset_of(type, member) \
	(((uintptr_t)&((type *)____OFFSET_OF_START_PAGE)->member) - ____OFFSET_OF_START_PAGE)

#define container_of(ptr, type, member) \
	((type *)(((uintptr_t)(ptr)) - offset_of(type, member)))

#endif /* __KERNEL__ */

#define sizeof_field(type, member) sizeof(((type *)____OFFSET_OF_START_PAGE)->member)

#define offset_of_end(type, member) \
	(offset_of(type, member) + sizeof_field(type, member))

#define ARRAY_SIZE(a)	(sizeof(a) / sizeof ((a)[0]))
#define ALIGN_MASK(x, m) ({typeof(x) __mask = (typeof(x))(m);\
			  (((x) + (__mask)) & ~(__mask));})
#define ALIGN_UP(x, a)	 ALIGN_MASK((x), ((typeof(x))(a) - (typeof(x))(1)))
#define ALIGN_DOWN(x, a) ALIGN_MASK((x) - ((typeof(x))(a) - (typeof(x))(1)), ((typeof(x))(a) - (typeof(x))(1)))
#define IS_ALIGNED(x, a) (((x) & (((typeof(x))(a) - (typeof(x))(1)))) == (typeof(x))(0))

/*
 * Same as ALIGN_MASK but do not use gcc extension. Can be used in decleration
 * and staic assertion. Expand 'm' twice, so m must be const expression.
 */
#define ALIGN_MASK_CONST(x, m)	(((x) + ((typeof(x))(m))) & ~((typeof(x))(m)))
#define ALIGN_UP_CONST(x, a)	ALIGN_MASK_CONST((x), ((typeof(x))(a) - (typeof(x))(1)))
#define ALIGN_DOWN_CONST(x, a)	ALIGN_MASK_CONST((x) - ((typeof(x))(a) - (typeof(x))(1)), \
						 ((typeof(x))(a) - (typeof(x))(1)))

#define member_size(type, member) sizeof(((type *)0)->member)

#define sizeof_uint(x)	((unsigned int)sizeof(x))

#endif
