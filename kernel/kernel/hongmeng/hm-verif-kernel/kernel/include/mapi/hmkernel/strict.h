/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Export kernel strict.h to mapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 08 10:38:40 2019
 */
#ifndef MAPI_HMKERNEL_STRICT_H
#define MAPI_HMKERNEL_STRICT_H

#include <uapi/hmkernel/strict.h>

#ifdef CONFIG_STRICT_UNUSED
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
# define __UNUSED11(a, ...)  ((void)(a)); __UNUSED10(__VA_ARGS__)

# define ____UNUSED(n, ...)  do { __UNUSED##n(__VA_ARGS__) } while (false)
# define __UNUSED(n, ...)  ____UNUSED(n, __VA_ARGS__)
# define UNUSED(...)  __UNUSED(NR_VARS(__VA_ARGS__), __VA_ARGS__)

#else
# define UNUSED(...)
#endif

#define __IS(x) ____IS(x)
#define __NOT(x) ____NOT(x)

#endif
