/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Map macro define head file 
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:19:54 2019
 */
#ifndef ULIBS_LIBSEC_METHOD_MAP_H
#define ULIBS_LIBSEC_METHOD_MAP_H

#undef ULIBS_LIBSEC_METHOD_MAP_UNDEF_H

#ifdef __PACKED_ARGS
# error __PACKED_ARGS should not be defined
#endif

#ifdef __PACKED_NO_ARG
# error __PACKED_NO_ARG should not be defined
#endif

#ifdef __PACKED_ARG_NUM
# error __PACKED_ARG_NUM should not be defined
#endif

#define NO_ARGS() __PACKED_NO_ARGS()
#define ARGS(...) __PACKED_ARGS(__VA_ARGS__)
#define __UNPACK___PACKED_ARGS(...) _, __VA_ARGS__
#define __UNPACK___PACKED_NO_ARGS() _
#define __UNPACK___PACKED_ARG_NUM(_ARG) __ARG_NUM(__UNPACK_##_ARG)

#define __ARG_NUM(...)							\
	___ARG_NUM(__VA_ARGS__, 8, _, 7, _, 6, _, 5, _,			\
				4, _, 3, _, 2, _, 1, _, 0)
#define ___ARG_NUM(_, T1, V1, T2, V2, T3, V3, T4, V4,			\
		      T5, V5, T6, V6, T7, V7, T8, V8, N, ...) N

#define __IS_0(N) __IS_0_##N
#define __IS_0_0 Y
#define __IS_0_1 N
#define __IS_0_2 N
#define __IS_0_3 N
#define __IS_0_4 N
#define __IS_0_5 N
#define __IS_0_6 N
#define __IS_0_7 N
#define __IS_0_8 N

#define ___CAT(X, Y) X##Y
#define __CAT(X, Y) ___CAT(X, Y)
#define ___CAT_(X, Y) X##_##Y
#define __CAT_(X, Y) ___CAT_(X, Y)

/*
 * __MAP_N_UNPACK(M, N, _ARG) ->
 * N is __PACKED_ARG_NUM(__PACKED_ARGS(...))
 * __UNPACK_##N is __UNPACK___PACKED_ARG_NUM(__PACKED_ARGS(...))
 *		 __ARG_NUM(__UNPACK___PACKED_ARGS(...))
 *		 __ARG_NUM(...)
 */
#define __MAP(M, _ARG) __MAP_N_UNPACK(M, __PACKED_ARG_NUM(_ARG), _ARG)
#define __MAP_N_UNPACK(M, N, _ARG) __MAP_N_EXT(M, __UNPACK_##N, __UNPACK_##_ARG)
#define __MAP_N_EXT(M, N, _ARG) __MAP_N(M, N, _ARG)
#define __MAP_N(M, N, _, _ARG...) __PREFIX(M, N) __MAP_##N(M, _ARG) __SUFFIX(M, N)
#define __MAP_0(M, _) M##0()
#define __MAP_1(M, T, V) M##1(T, V)
#define __MAP_2(M, T, V, ...) M(T, V) __MAP_1(M, __VA_ARGS__)
#define __MAP_3(M, T, V, ...) M(T, V) __MAP_2(M, __VA_ARGS__)
#define __MAP_4(M, T, V, ...) M(T, V) __MAP_3(M, __VA_ARGS__)
#define __MAP_5(M, T, V, ...) M(T, V) __MAP_4(M, __VA_ARGS__)
#define __MAP_6(M, T, V, ...) M(T, V) __MAP_5(M, __VA_ARGS__)
#define __MAP_7(M, T, V, ...) M(T, V) __MAP_6(M, __VA_ARGS__)
#define __MAP_8(M, T, V, ...) M(T, V) __MAP_7(M, __VA_ARGS__)
#define __PREFIX(M, N) __CAT_(M, __CAT_(PREFIX, __IS_0(N)))
#define __SUFFIX(M, N) __CAT_(M, __CAT_(SUFFIX, __IS_0(N)))

#endif
