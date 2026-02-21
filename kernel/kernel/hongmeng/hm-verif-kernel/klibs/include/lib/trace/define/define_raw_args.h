/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Define the raw args pass to tracepoint callback
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 8 15:51:17 2024
 */

#ifdef __HM_TRACEPOINT__
#define ____TP_ARG_NR_N(t1, a1, t2, a2, t3, a3, t4, a4, t5, a5,			\
			t6, a6, t7, a7, t8, a8, t9, a9, t10, a10, t11, a11, t12, a12,	\
			t13, a13, t14, a14, t15, a15, n, x, ...) n
#define __TP_ARG_NR(...)							\
	____TP_ARG_NR_N(__VA_ARGS__,						\
			15, x, 14, x, 13, x, 12, x, 11, x, 10, x, 9, x,	\
			8, x, 7, x, 6, x, 5, x, 4, x, 3, x, 2, x, 1, x, 0, x)

#define __TP_RAW_MAP0(n, m, ...)
#define __TP_RAW_MAP1(n, m, t, a, ...) m(n - 1, t, a);
#define __TP_RAW_MAP2(n, m, t, a, ...) m(n - 2, t, a); __TP_RAW_MAP1(n, m, __VA_ARGS__)
#define __TP_RAW_MAP3(n, m, t, a, ...) m(n - 3, t, a); __TP_RAW_MAP2(n, m, __VA_ARGS__)
#define __TP_RAW_MAP4(n, m, t, a, ...) m(n - 4, t, a); __TP_RAW_MAP3(n, m, __VA_ARGS__)
#define __TP_RAW_MAP5(n, m, t, a, ...) m(n - 5, t, a); __TP_RAW_MAP4(n, m, __VA_ARGS__)
#define __TP_RAW_MAP6(n, m, t, a, ...) m(n - 6, t, a); __TP_RAW_MAP5(n, m, __VA_ARGS__)
#define __TP_RAW_MAP7(n, m, t, a, ...) m(n - 7, t, a); __TP_RAW_MAP6(n, m, __VA_ARGS__)
#define __TP_RAW_MAP8(n, m, t, a, ...) m(n - 8, t, a); __TP_RAW_MAP7(n, m, __VA_ARGS__)
#define __TP_RAW_MAP9(n, m, t, a, ...) m(n - 9, t, a); __TP_RAW_MAP8(n, m, __VA_ARGS__)
#define __TP_RAW_MAP10(n, m, t, a, ...) m(n - 10, t, a); __TP_RAW_MAP9(n, m, __VA_ARGS__)
#define __TP_RAW_MAP11(n, m, t, a, ...) m(n - 11, t, a); __TP_RAW_MAP10(n, m, __VA_ARGS__)
#define __TP_RAW_MAP12(n, m, t, a, ...) m(n - 12, t, a); __TP_RAW_MAP11(n, m, __VA_ARGS__)
#define __TP_RAW_MAP13(n, m, t, a, ...) m(n - 13, t, a); __TP_RAW_MAP12(n, m, __VA_ARGS__)
#define __TP_RAW_MAP(n, ...) __TP_RAW_MAP##n(n, __VA_ARGS__)

#define __TP_SC_U64_DEF(n, t, a)	unsigned long long a

#define TRACE_FUNC_RAW_ARGS(n, ...)	__TP_RAW_MAP(n, __TP_SC_U64_DEF, __VA_ARGS__)
#define TP_FUNC_RAW_ARGS(...)		TRACE_FUNC_RAW_ARGS(__TP_ARG_NR(__VA_ARGS__), __VA_ARGS__)

#define __COUNT_ARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _n, X...) _n
#define COUNT_ARGS(X...) __COUNT_ARGS(, ##X, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define __CONCAT(a, b) a ## b
#define CONCATENATE(a, b) __CONCAT(a, b)

#define UINTTYPE(size) \
	__typeof__(__builtin_choose_expr(size == 1, (__u8)1,	\
		   __builtin_choose_expr(size == 2, (__u16)2,	\
		   __builtin_choose_expr(size == 4, (__u32)3,	\
		   __builtin_choose_expr(size == 8, (__u64)4,	\
					 (void)5)))))

#ifdef __KERNEL__
#define __CAST_TO_U64(x) ({ \
	typeof(x) __src = (x); \
	UINTTYPE(sizeof(x)) __dst; \
	memcpy(&__dst, &__src, sizeof(__dst)); \
	(__u64)__dst; })
#else
#define __CAST_TO_U64(x) ({ \
	typeof(x) __src = (x); \
	UINTTYPE(sizeof(x)) __dst; \
	memcpy_s(&__dst, sizeof(__dst), &__src, sizeof(__src)); \
	(__u64)__dst; })
#endif

#define __CAST1(a,...) __CAST_TO_U64(a)
#define __CAST2(a,...) __CAST_TO_U64(a), __CAST1(__VA_ARGS__)
#define __CAST3(a,...) __CAST_TO_U64(a), __CAST2(__VA_ARGS__)
#define __CAST4(a,...) __CAST_TO_U64(a), __CAST3(__VA_ARGS__)
#define __CAST5(a,...) __CAST_TO_U64(a), __CAST4(__VA_ARGS__)
#define __CAST6(a,...) __CAST_TO_U64(a), __CAST5(__VA_ARGS__)
#define __CAST7(a,...) __CAST_TO_U64(a), __CAST6(__VA_ARGS__)
#define __CAST8(a,...) __CAST_TO_U64(a), __CAST7(__VA_ARGS__)
#define __CAST9(a,...) __CAST_TO_U64(a), __CAST8(__VA_ARGS__)
#define __CAST10(a,...) __CAST_TO_U64(a), __CAST9(__VA_ARGS__)
#define __CAST11(a,...) __CAST_TO_U64(a), __CAST10(__VA_ARGS__)
#define __CAST12(a,...) __CAST_TO_U64(a), __CAST11(__VA_ARGS__)
#define CAST_TO_U64(...) CONCATENATE(__CAST, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define UNPACK(...)			__VA_ARGS__
#define REPEAT_1(FN, DL, X, ...)	FN(X)
#define REPEAT_2(FN, DL, X, ...)	FN(X) UNPACK DL REPEAT_1(FN, DL, __VA_ARGS__)
#define REPEAT_3(FN, DL, X, ...)	FN(X) UNPACK DL REPEAT_2(FN, DL, __VA_ARGS__)
#define REPEAT_4(FN, DL, X, ...)	FN(X) UNPACK DL REPEAT_3(FN, DL, __VA_ARGS__)
#define REPEAT_5(FN, DL, X, ...)	FN(X) UNPACK DL REPEAT_4(FN, DL, __VA_ARGS__)
#define REPEAT_6(FN, DL, X, ...)	FN(X) UNPACK DL REPEAT_5(FN, DL, __VA_ARGS__)
#define REPEAT_7(FN, DL, X, ...)	FN(X) UNPACK DL REPEAT_6(FN, DL, __VA_ARGS__)
#define REPEAT_8(FN, DL, X, ...)	FN(X) UNPACK DL REPEAT_7(FN, DL, __VA_ARGS__)
#define REPEAT_9(FN, DL, X, ...)	FN(X) UNPACK DL REPEAT_8(FN, DL, __VA_ARGS__)
#define REPEAT_10(FN, DL, X, ...)	FN(X) UNPACK DL REPEAT_9(FN, DL, __VA_ARGS__)
#define REPEAT_11(FN, DL, X, ...)	FN(X) UNPACK DL REPEAT_10(FN, DL, __VA_ARGS__)
#define REPEAT_12(FN, DL, X, ...)	FN(X) UNPACK DL REPEAT_11(FN, DL, __VA_ARGS__)
#define REPEAT(X, FN, DL, ...)		REPEAT_##X(FN, DL, __VA_ARGS__)

#define SARG(X)		__u64 arg##X
#define COPY(X)		args[X] = arg##X

#define __DL_COM	(,)
#define __DL_SEM	(;)

#define __SEQ_0_11	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11

#define COPY_TRACE_ARGS_DEFN_x(x)							\
static inline void copy_trace_args##x(__u64 *args, REPEAT(x, SARG, __DL_COM, __SEQ_0_11))\
{								\
	REPEAT(x, COPY, __DL_SEM, __SEQ_0_11);			\
}
#else
#define COPY_TRACE_ARGS_DEFN_x(x)
#define TP_FUNC_RAW_ARGS(...)
#endif
