/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Define macros for uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 22 13:07:43 2024
 */

#ifndef MAPI_UAPI_HMKERNEL_MACRO_H
#define MAPI_UAPI_HMKERNEL_MACRO_H


# define ____hmkernel_macro_nr_vars(ignored, a, b, c, d, e, f, g, h, i, j, k, l, n, ...) n
# define __hmkernel_macro_nr_vars(...)  ____hmkernel_macro_nr_vars(ignored, ##__VA_ARGS__, \
								   12, 11, 10, 9, 8, 7, 6, 5, \
								   4, 3, 2, 1, 0)

#define __hmkernel_macro_simple_map0(m, ...)
#define __hmkernel_macro_simple_map1(m, v, ...) m(v) __hmkernel_macro_simple_map0(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map2(m, v, ...) m(v) __hmkernel_macro_simple_map1(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map3(m, v, ...) m(v) __hmkernel_macro_simple_map2(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map4(m, v, ...) m(v) __hmkernel_macro_simple_map3(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map5(m, v, ...) m(v) __hmkernel_macro_simple_map4(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map6(m, v, ...) m(v) __hmkernel_macro_simple_map5(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map7(m, v, ...) m(v) __hmkernel_macro_simple_map6(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map8(m, v, ...) m(v) __hmkernel_macro_simple_map7(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map9(m, v, ...) m(v) __hmkernel_macro_simple_map8(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map10(m, v, ...) m(v) __hmkernel_macro_simple_map9(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map11(m, v, ...) m(v) __hmkernel_macro_simple_map10(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map12(m, v, ...) m(v) __hmkernel_macro_simple_map11(m, __VA_ARGS__)
#define __hmkernel_macro_simple_map(n, m, ...) __hmkernel_macro_simple_map##n(m, __VA_ARGS__)

#define ____hmkernel_macro_simple_map_list(n, m, ...)	__hmkernel_macro_simple_map(n, m, ##__VA_ARGS__)
#define __hmkernel_macro_simple_map_list(m, list)	\
	____hmkernel_macro_simple_map_list(__hmkernel_macro_nr_vars(list), m, list)
#define __hmkernel_macro_simple_map_vaargs(m, ...)	\
	____hmkernel_macro_simple_map_list(__hmkernel_macro_nr_vars(__VA_ARGS__), m, ##__VA_ARGS__)

#define __hmkernel_macro_exarg1_map0(m, x, ...)
#define __hmkernel_macro_exarg1_map1(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map0(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map2(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map1(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map3(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map2(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map4(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map3(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map5(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map4(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map6(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map5(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map7(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map6(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map8(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map7(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map9(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map8(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map10(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map9(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map11(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map10(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map12(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map11(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map(n, m, x, ...) __hmkernel_macro_exarg1_map##n(m, x, __VA_ARGS__)

#define ____hmkernel_macro_exarg1_map_list(n, m, x, ...)	__hmkernel_macro_exarg1_map(n, m, x, ##__VA_ARGS__)
#define __hmkernel_macro_exarg1_map_list(m, x, list)	\
	____hmkernel_macro_exarg1_map_list(__hmkernel_macro_nr_vars(list), m, x, list)
#define __hmkernel_macro_exarg1_map_vaargs(m, x, ...)	\
	____hmkernel_macro_exarg1_map_list(__hmkernel_macro_nr_vars(__VA_ARGS__), m, x, ##__VA_ARGS__)

#define __hmkernel_macro_exarg2_map0(m, x, y, ...)
#define __hmkernel_macro_exarg2_map1(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map0(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map2(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map1(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map3(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map2(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map4(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map3(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map5(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map4(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map6(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map5(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map7(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map6(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map8(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map7(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map9(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map8(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map10(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map9(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map11(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map10(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map12(m, x, y, v, ...) m(x, y, v) __hmkernel_macro_exarg2_map11(m, x, y, __VA_ARGS__)
#define __hmkernel_macro_exarg2_map(n, m, x, y, ...) __hmkernel_macro_exarg2_map##n(m, x, y, __VA_ARGS__)

#define ____hmkernel_macro_exarg2_map_list(n, m, x, y, ...)	__hmkernel_macro_exarg2_map(n, m, x, y, ##__VA_ARGS__)
#define __hmkernel_macro_exarg2_map_list(m, x, y, list)	\
	____hmkernel_macro_exarg2_map_list(__hmkernel_macro_nr_vars(list), m, x, y, list)
#define __hmkernel_macro_exarg2_map_vaargs(m, x, y, ...)	\
	____hmkernel_macro_exarg2_map_list(__hmkernel_macro_nr_vars(__VA_ARGS__), m, x, y, ##__VA_ARGS__)

#define ____hmkernel_macro_is_not_nul_yn(ign, y1, y2, ...) y2
#define ____hmkernel_macro_is_not_nul(...) \
	____hmkernel_macro_is_not_nul_yn(ign, ##__VA_ARGS__, y, n)
#define __hmkernel_macro_is_not_nul(...) ____hmkernel_macro_is_not_nul(__VA_ARGS__)
#define ____hmkernel_macro_cat(a, b) a ## b
#define __hmkernel_macro_cat(a, b) ____hmkernel_macro_cat(a, b)

#endif
