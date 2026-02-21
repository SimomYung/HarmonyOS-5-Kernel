/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: arguments list helper
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 1 15:12:15 2020
 */
#ifndef DEVHOST_PLUGIN_HELPER_H
#define DEVHOST_PLUGIN_HELPER_H

#include <stdint.h>

#define __ARG_N(a0, a1, a2, a3, a4, a5, a6, a7,	\
		a8, a9, a10, a11, a12, a13, a14, a15, a16, N, ...)  N
#define COUNT_ARG(...)  __ARG_N(0, ##__VA_ARGS__,		\
				16, 15, 14, 13, 12, 11, 10, 9,	\
				8, 7, 6, 5, 4, 3, 2, 1, 0)

#define __ARGS_TYPEOF_OF(p)	typeof(p)
#define __ARGS_TYPEOF_P(p)	p
#define __ARGS_TYPEOF_NONE(p)

#define __ARGS_COMMA_SEP_NONE(arg)	,
#define __ARGS_COMMA_SEP_NONE0(arg)

#define __ARGS_SEMI_SEP(arg)	arg;
#define __ARGS_SEMI_SEP0(arg)	arg;

#define __ARGS_COMMA_SEP(arg)	(arg),
#define __ARGS_COMMA_SEP0(arg)	arg

#define __ARGS_FIELD0(my_typeof, sep, a0, ...)
#define __ARGS_FIELD1(my_typeof, sep, a1, ...)	my_typeof(a1) sep##0(_filed1)
#define __ARGS_FIELD2(my_typeof, sep, a2, ...)	my_typeof(a2) sep(_filed2) __ARGS_FIELD1(my_typeof, sep, __VA_ARGS__)
#define __ARGS_FIELD3(my_typeof, sep, a3, ...)	my_typeof(a3) sep(_filed3) __ARGS_FIELD2(my_typeof, sep, __VA_ARGS__)
#define __ARGS_FIELD4(my_typeof, sep, a4, ...)	my_typeof(a4) sep(_filed4) __ARGS_FIELD3(my_typeof, sep, __VA_ARGS__)
#define __ARGS_FIELD5(my_typeof, sep, a5, ...)	my_typeof(a5) sep(_filed5) __ARGS_FIELD4(my_typeof, sep, __VA_ARGS__)
#define __ARGS_FIELD6(my_typeof, sep, a6, ...)	my_typeof(a6) sep(_filed6) __ARGS_FIELD5(my_typeof, sep, __VA_ARGS__)
#define __ARGS_FIELD7(my_typeof, sep, a7, ...)	my_typeof(a7) sep(_filed7) __ARGS_FIELD6(my_typeof, sep, __VA_ARGS__)
#define __ARGS_FIELD8(my_typeof, sep, a8, ...)	my_typeof(a8) sep(_filed8) __ARGS_FIELD7(my_typeof, sep, __VA_ARGS__)
#define __ARGS_FIELD9(my_typeof, sep, a9, ...)	my_typeof(a9) sep(_filed9) __ARGS_FIELD8(my_typeof, sep, __VA_ARGS__)
#define __ARGS_FIELD10(my_typeof, sep, a10, ...)	\
	my_typeof(a10) sep(_filed10) __ARGS_FIELD9(my_typeof, sep, __VA_ARGS__)

#define __ARGS_FIELD11(my_typeof, sep, a11, ...)	\
	my_typeof(a11) sep(_filed11) __ARGS_FIELD10(my_typeof, sep, __VA_ARGS__)

#define __ARGS_FIELD12(my_typeof, sep, a12, ...)	\
	my_typeof(a12) sep(_filed12) __ARGS_FIELD11(my_typeof, sep, __VA_ARGS__)

#define __ARGS_FIELD13(my_typeof, sep, a13, ...)	\
	my_typeof(a13) sep(_filed13) __ARGS_FIELD12(my_typeof, sep, __VA_ARGS__)

#define __ARGS_FIELD14(my_typeof, sep, a14, ...)	\
	my_typeof(a14) sep(_filed14) __ARGS_FIELD13(my_typeof, sep, __VA_ARGS__)

#define __ARGS_FIELD15(my_typeof, sep, a15, ...)	\
	my_typeof(a15) sep(_filed15) __ARGS_FIELD14(my_typeof, sep, __VA_ARGS__)

#define __ARGS_FIELD16(my_typeof, sep, a16, ...)	\
	my_typeof(a16) sep(_filed16) __ARGS_FIELD15(my_typeof, sep, __VA_ARGS__)

#define __ARGS_FIELD(n, my_typeof, sep, ...)	__ARGS_FIELD##n(my_typeof, sep, __VA_ARGS__)

#define __ARGS_FUNC_ARGS_TYPEOF(n, ...)	__ARGS_FIELD(n, __ARGS_TYPEOF_OF, __ARGS_COMMA_SEP_NONE, __VA_ARGS__)
#define __ARGS_FUNC_ARGS_TYPEOF_H(n, ...)	__ARGS_FUNC_ARGS_TYPEOF(n, ##__VA_ARGS__)
#define __ARGS_CALL_FUNC_PTR(fn_ptr, ret_type, ...)	\
	((ret_type (*)(__ARGS_FUNC_ARGS_TYPEOF_H(COUNT_ARG(__VA_ARGS__), ##__VA_ARGS__)))((uintptr_t)fn_ptr))(__VA_ARGS__)

#define __ARGS_STRUCT_FIELD(n, ...)	__ARGS_FIELD(n, __ARGS_TYPEOF_P, __ARGS_SEMI_SEP, __VA_ARGS__)
#define __ARGS_STRUCT_VAL(n, ...)	__ARGS_FIELD(n, __ARGS_TYPEOF_P, __ARGS_COMMA_SEP_NONE, __VA_ARGS__)

#define __ARGS_FUNC_PARAM(n, ...)	__ARGS_FIELD(n, __ARGS_TYPEOF_P, __ARGS_COMMA_SEP, __VA_ARGS__)
#define __ARGS_FUNC_PVAL(n, ...)	__ARGS_FIELD(n, __ARGS_TYPEOF_NONE, __ARGS_COMMA_SEP, __VA_ARGS__)

#define __ARGS_USE0(args)
#define __ARGS_USE1(args)	(args)->_filed1
#define __ARGS_USE2(args)	(args)->_filed2, __ARGS_USE1(args)
#define __ARGS_USE3(args)	(args)->_filed3, __ARGS_USE2(args)
#define __ARGS_USE4(args)	(args)->_filed4, __ARGS_USE3(args)
#define __ARGS_USE5(args)	(args)->_filed5, __ARGS_USE4(args)
#define __ARGS_USE6(args)	(args)->_filed6, __ARGS_USE5(args)
#define __ARGS_USE7(args)	(args)->_filed7, __ARGS_USE6(args)
#define __ARGS_USE8(args)	(args)->_filed8, __ARGS_USE7(args)
#define __ARGS_USE9(args)	(args)->_filed9, __ARGS_USE8(args)
#define __ARGS_USE10(args)	(args)->_filed10, __ARGS_USE9(args)
#define __ARGS_USE11(args)	(args)->_filed11, __ARGS_USE10(args)
#define __ARGS_USE12(args)	(args)->_filed12, __ARGS_USE11(args)
#define __ARGS_USE13(args)	(args)->_filed13, __ARGS_USE12(args)
#define __ARGS_USE14(args)	(args)->_filed14, __ARGS_USE13(args)
#define __ARGS_USE15(args)	(args)->_filed15, __ARGS_USE14(args)
#define __ARGS_USE16(args)	(args)->_filed16, __ARGS_USE15(args)
#define __ARGS_USE(n, args)	__ARGS_USE##n(args)

#endif /* DEVHOST_PLUGIN_HELPER_H */
