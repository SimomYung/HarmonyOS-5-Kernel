/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: lsyscall map
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 22 15:19:38 2023
 */

#ifndef ULIBS_HONGMENG_LSYSCALL_H
#define ULIBS_HONGMENG_LSYSCALL_H

#define __LSC_MAP0(m, ...) m##0(__VA_ARGS__)
#define __LSC_MAP1(m, t, a, ...) m##_R1(t, a)
#define __LSC_MAP2(m, t, a, ...) m(t, a), __LSC_MAP1(m, __VA_ARGS__)
#define __LSC_MAP3(m, t, a, ...) m(t, a), __LSC_MAP2(m, __VA_ARGS__)
#define __LSC_MAP4(m, t, a, ...) m(t, a), __LSC_MAP3(m, __VA_ARGS__)
#define __LSC_MAP5(m, t, a, ...) m(t, a), __LSC_MAP4(m, __VA_ARGS__)
#define __LSC_MAP6(m, t, a, ...) m(t, a), __LSC_MAP5(m, __VA_ARGS__)
#define __LSC_MAP7(m, t, a, ...) m(t, a), __LSC_MAP6(m, __VA_ARGS__)
#define __LSC_MAP(n, ...) __LSC_MAP##n(__VA_ARGS__)

#define __LSC_LONG(t, a)	long a
#define __LSC_DECL(t, a)	t a
#define __LSC_ARGS_T(t, a)	a
#define __LSC_ARGS_F(t, a)	a
#define __LSC_CAST(t, a)	(t)(unsigned long)a

#define __LSC_LONG0(...)	void
#define __LSC_DECL0(...)	void
#define __LSC_ARGS_T0(...)
#define __LSC_ARGS_F0(...)
#define __LSC_CAST0(...)

#define __LSC_LONG_R1	__LSC_LONG
#define __LSC_DECL_R1	__LSC_DECL
#define __LSC_CAST_R1	__LSC_CAST

/*
 * To make sure LSC_ARGS_NUM() returns to 0, use ##__VA_ARGS__ to remove ',' when
 * __VA_ARGS__ is empty. The first 'x' is placeholder to make '##' works.
 */
#define __LSC_ARGS_NUM(_x, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, n, ...) n
#define LSC_ARGS_NUM(...) __LSC_ARGS_NUM(x, ##__VA_ARGS__, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0)

#endif
