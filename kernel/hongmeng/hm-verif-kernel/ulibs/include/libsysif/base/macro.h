/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Macro
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 16 10:27:57 2018
 */
#ifndef LIBSYSIF_BASE_MACRO_H
#define LIBSYSIF_BASE_MACRO_H

#define __SYSIF_MAP2(n, m, t, a, ...) m(n - 2, t, a) __SYSIF_MAP1(n, m, __VA_ARGS__)
#define __SYSIF_MAP3(n, m, t, a, ...) m(n - 3, t, a) __SYSIF_MAP2(n, m, __VA_ARGS__)
#define __SYSIF_MAP4(n, m, t, a, ...) m(n - 4, t, a) __SYSIF_MAP3(n, m, __VA_ARGS__)
#define __SYSIF_MAP5(n, m, t, a, ...) m(n - 5, t, a) __SYSIF_MAP4(n, m, __VA_ARGS__)
#define __SYSIF_MAP6(n, m, t, a, ...) m(n - 6, t, a) __SYSIF_MAP5(n, m, __VA_ARGS__)
#define __SYSIF_MAP7(n, m, t, a, ...) m(n - 7, t, a) __SYSIF_MAP6(n, m, __VA_ARGS__)
#define __SYSIF_MAP8(n, m, t, a, ...) m(n - 8, t, a) __SYSIF_MAP7(n, m, __VA_ARGS__)
#define __SYSIF_MAP9(n, m, t, a, ...) m(n - 9, t, a) __SYSIF_MAP8(n, m, __VA_ARGS__)
#define __SYSIF_MAP10(n, m, t, a, ...) m(n - 10, t, a) __SYSIF_MAP9(n, m, __VA_ARGS__)
#define __SYSIF_MAP(n, ...) __SYSIF_MAP##n(n, __VA_ARGS__)

#define ___COMB(x, y)	x##_##y
#define __COMB(x, y)	___COMB(x, y)
#define ___DCOMB(x, y)	__##x##_##y
#define __DCOMB(x, y)	___DCOMB(x, y)

#define ____STR(x)	#x
#define __STR(x)	____STR(x)

#endif
