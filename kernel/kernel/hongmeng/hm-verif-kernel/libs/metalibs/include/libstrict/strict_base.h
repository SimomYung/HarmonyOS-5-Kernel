/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Helpers for static warnings cleanup
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 19 13:58:52 2019
 */
#ifndef ULIBS_LIBSTRICT_STRICT_BASE_H
#define ULIBS_LIBSTRICT_STRICT_BASE_H

#include <errno.h>
#include <securec.h>
#include <stdbool.h>

#include <hongmeng/types.h>

#ifndef __hmbuild__
# ifdef CONFIG_STRICT_FOR_CODEDEX
#define __hmfeature_strict__
# else
#define __hmfeature_no_strict__
# endif
#endif

#ifdef __hmfeature_strict__
#define __STRICT_MUST_HOLD(cond, reason) BUG_ON(!(cond))
#define __STRICT_INITED(v, def, reason)	do { (v) = (def); } while (__false)
#define __STRICT_FILLED(ptr, size, reason) do {		\
	void *__ptr = (void *)(ptr);	\
	size_t __size = (size_t)(size);	\
	NOFAIL(memset_s(__ptr, __size, 0, __size)); \
} while (__false)

#else
#define __STRICT_MUST_HOLD(cond, reason) do { } while (__false)
#define __STRICT_INITED(v, def, reason) do { } while (__false)
#define __STRICT_FILLED(ptr, size, reason) do { } while (__false)
#endif

# define __NR_VARS(ignored, a, b, c, d, e, f, g, h, i, j, k, n, ...) n
# define NR_VARS(...)  __NR_VARS(ignored, ##__VA_ARGS__, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

# define __UNUSED0(a)
# define __UNUSED1(a)  ((void)(a));
# define __UNUSED2(a, ...)  ((void)(a)); __UNUSED1(__VA_ARGS__)
# define __UNUSED3(a, ...)  ((void)(a)); __UNUSED2(__VA_ARGS__)
# define __UNUSED4(a, ...)  ((void)(a)); __UNUSED3(__VA_ARGS__)
# define __UNUSED5(a, ...)  ((void)(a)); __UNUSED4(__VA_ARGS__)
# define __UNUSED6(a, ...)  ((void)(a)); __UNUSED5(__VA_ARGS__)
# define __UNUSED7(a, ...)  ((void)(a)); __UNUSED6(__VA_ARGS__)
# define __UNUSED8(a, ...)  ((void)(a)); __UNUSED7(__VA_ARGS__)
# define __UNUSED9(a, ...)  ((void)(a)); __UNUSED8(__VA_ARGS__)
# define __UNUSED10(a, ...)  ((void)(a)); __UNUSED9(__VA_ARGS__)

# define ____UNUSED(n, ...)  do { __UNUSED##n(__VA_ARGS__) } while (__false)
# define __UNUSED(n, ...)  ____UNUSED(n, __VA_ARGS__)
# define UNUSED(...)  __UNUSED(NR_VARS(__VA_ARGS__), __VA_ARGS__)

#define POSTCONDITION(cond, reason) __STRICT_MUST_HOLD(cond, reason)
#define PRECONDITION(cond, reason) __STRICT_MUST_HOLD(cond, reason)
#define IMPOSSIBLE(cond, reason) __STRICT_MUST_HOLD(!(cond), reason)

#define IS_OUTPUT_PARAM(v, def, reason)	__STRICT_INITED(v, def, reason)
#define IS_FILLED(ptr, size, reason) __STRICT_FILLED(ptr, size, reason)

#endif
