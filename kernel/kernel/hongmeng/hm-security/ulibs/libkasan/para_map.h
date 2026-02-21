/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Parameters map
 * Author: Huawei OS Kernel Lab
 * Create: Sat Nov 16 11:49:29 2018
 */

#ifndef __PARA_MAP_H_
#define __PARA_MAP_H_

#define PACK(...) __VA_ARGS__

#define __NUM(t1, a1, t2, a2, t3, a3, t4, a4, t5, a5, t6, a6, t7, a7, t8, a8, n, ...) n
#define _NUM(...) __NUM(__VA_ARGS__, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 0, 0)
#define ARG_NUM(...) _NUM(__VA_ARGS__)

/* prototype of SUFFIX_XXX_XXX is SUFFIX__(MAP_TYPE)_(IS_LASTARG) */
#define SUFFIX___PARA_N ,
#define SUFFIX___PARA_Y void
#define SUFFIX___ARG_N ,
#define SUFFIX___ARG_Y

#define __PARA(t, v) t v
#define __ARG(t, v) v

#define ARGS(...) __VA_ARGS__
#define NO_ARG()

#define MAP0(map, t) SUFFIX_##map##_Y
#define MAP1(map, t, v, ...) map(t, v)
#define MAP2(map, t, v, ...) map(t, v)SUFFIX_##map##_N MAP1(map, __VA_ARGS__)
#define MAP3(map, t, v, ...) map(t, v)SUFFIX_##map##_N MAP2(map, __VA_ARGS__)
#define MAP4(map, t, v, ...) map(t, v)SUFFIX_##map##_N MAP3(map, __VA_ARGS__)
#define MAP5(map, t, v, ...) map(t, v)SUFFIX_##map##_N MAP4(map, __VA_ARGS__)
#define MAP6(map, t, v, ...) map(t, v)SUFFIX_##map##_N MAP5(map, __VA_ARGS__)
#define MAP7(map, t, v, ...) map(t, v)SUFFIX_##map##_N MAP6(map, __VA_ARGS__)
#define MAP8(map, t, v, ...) map(t, v)SUFFIX_##map##_N MAP7(map, __VA_ARGS__)
#define _MAP(n, map, ...) MAP##n(map, __VA_ARGS__)
#define MAP(n, map, ARG) _MAP(n, map, ARG)

#define _PARA(ARG) MAP(ARG_NUM(ARG), __PARA, PACK(ARG))
#define _ARGS(ARG) MAP(ARG_NUM(ARG), __ARG, PACK(ARG))

#endif

