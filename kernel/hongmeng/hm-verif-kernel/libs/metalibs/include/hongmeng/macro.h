/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Implement macro for generating functions with parameters
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 26 11:38:36 2019
 */
#ifndef ULIBS_HONGMENG_MACRO_H
#define ULIBS_HONGMENG_MACRO_H

#define __SC_MAP0(n, m, ...)
#define __SC_MAP1(n, m, t, a, ...) m(n - 1UL, t, a)
#define __SC_MAP2(n, m, t, a, ...) m(n - 2UL, t, a), __SC_MAP1(n, m, __VA_ARGS__)
#define __SC_MAP3(n, m, t, a, ...) m(n - 3UL, t, a), __SC_MAP2(n, m, __VA_ARGS__)
#define __SC_MAP4(n, m, t, a, ...) m(n - 4UL, t, a), __SC_MAP3(n, m, __VA_ARGS__)
#define __SC_MAP5(n, m, t, a, ...) m(n - 5UL, t, a), __SC_MAP4(n, m, __VA_ARGS__)
#define __SC_MAP6(n, m, t, a, ...) m(n - 6UL, t, a), __SC_MAP5(n, m, __VA_ARGS__)
#define __SC_MAP7(n, m, t, a, ...) m(n - 7UL, t, a), __SC_MAP6(n, m, __VA_ARGS__)
#define __SC_MAP8(n, m, t, a, ...) m(n - 8UL, t, a), __SC_MAP7(n, m, __VA_ARGS__)
#define __SC_MAP9(n, m, t, a, ...) m(n - 9UL, t, a), __SC_MAP8(n, m, __VA_ARGS__)
#define __SC_MAP10(n, m, t, a, ...) m(n - 10UL, t, a), __SC_MAP9(n, m, __VA_ARGS__)
#define __SC_MAP11(n, m, t, a, ...) m(n - 11UL, t, a), __SC_MAP10(n, m, __VA_ARGS__)
#define __SC_MAP12(n, m, t, a, ...) m(n - 12UL, t, a), __SC_MAP11(n, m, __VA_ARGS__)
#define __SC_MAP13(n, m, t, a, ...) m(n - 13UL, t, a), __SC_MAP12(n, m, __VA_ARGS__)
#define __SC_MAP14(n, m, t, a, ...) m(n - 14UL, t, a), __SC_MAP13(n, m, __VA_ARGS__)
#define __SC_MAP15(n, m, t, a, ...) m(n - 15UL, t, a), __SC_MAP14(n, m, __VA_ARGS__)
#define __SC_MAP(n, ...) __SC_MAP##n(n, __VA_ARGS__)

#define __SC_MAPSTAT0(n, m, ...)
#define __SC_MAPSTAT1(n, m, t, a, ...) m(n##ul - 1UL, t, a)
#define __SC_MAPSTAT2(n, m, t, a, ...) m(n##ul - 2UL, t, a); __SC_MAPSTAT1(n, m, __VA_ARGS__)
#define __SC_MAPSTAT3(n, m, t, a, ...) m(n##ul - 3UL, t, a); __SC_MAPSTAT2(n, m, __VA_ARGS__)
#define __SC_MAPSTAT4(n, m, t, a, ...) m(n##ul - 4UL, t, a); __SC_MAPSTAT3(n, m, __VA_ARGS__)
#define __SC_MAPSTAT5(n, m, t, a, ...) m(n##ul - 5UL, t, a); __SC_MAPSTAT4(n, m, __VA_ARGS__)
#define __SC_MAPSTAT6(n, m, t, a, ...) m(n##ul - 6UL, t, a); __SC_MAPSTAT5(n, m, __VA_ARGS__)
#define __SC_MAPSTAT7(n, m, t, a, ...) m(n##ul - 7UL, t, a); __SC_MAPSTAT6(n, m, __VA_ARGS__)
#define __SC_MAPSTAT8(n, m, t, a, ...) m(n##ul - 8UL, t, a); __SC_MAPSTAT7(n, m, __VA_ARGS__)
#define __SC_MAPSTAT9(n, m, t, a, ...) m(n##ul - 9UL, t, a); __SC_MAPSTAT8(n, m, __VA_ARGS__)
#define __SC_MAPSTAT10(n, m, t, a, ...) m(n##ul - 10UL, t, a); __SC_MAPSTAT9(n, m, __VA_ARGS__)
#define __SC_MAPSTAT11(n, m, t, a, ...) m(n##ul - 11UL, t, a); __SC_MAPSTAT10(n, m, __VA_ARGS__)
#define __SC_MAPSTAT12(n, m, t, a, ...) m(n##ul - 12UL, t, a); __SC_MAPSTAT11(n, m, __VA_ARGS__)
#define __SC_MAPSTAT13(n, m, t, a, ...) m(n##ul - 13UL, t, a); __SC_MAPSTAT12(n, m, __VA_ARGS__)
#define __SC_MAPSTAT14(n, m, t, a, ...) m(n##ul - 14UL, t, a); __SC_MAPSTAT13(n, m, __VA_ARGS__)
#define __SC_MAPSTAT15(n, m, t, a, ...) m(n##ul - 15UL, t, a); __SC_MAPSTAT14(n, m, __VA_ARGS__)
#define __SC_MAPSTAT(n, ...) __SC_MAPSTAT##n(n, __VA_ARGS__)

#define __SC_DECL(n, t, a)	t a
#define __SC_USE(n, t, a)	a

/*
 * To make sure VA_ARGS_NUM() returns to 0, use ##__VA_ARGS__ to remove ',' when
 * __VA_ARGS__ is empty. The first 'x' is placeholder to make '##' works.
 */
#define __VA_ARGS_NUM(_x, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, n, ...) n
#define VA_ARGS_NUM(...) __VA_ARGS_NUM(x, ##__VA_ARGS__, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#endif
