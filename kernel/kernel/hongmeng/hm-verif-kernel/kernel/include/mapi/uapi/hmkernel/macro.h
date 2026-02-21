/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Define macros for uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 22 13:07:43 2024
 */

#ifndef MAPI_UAPI_HMKERNEL_MACRO_H
#define MAPI_UAPI_HMKERNEL_MACRO_H

#define __hmkernel_macro_args(...)	__VA_ARGS__

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
#define __hmkernel_macro_exarg1_map13(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map12(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map14(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map13(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map15(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map14(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map16(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map15(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map17(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map16(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map18(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map17(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map19(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map18(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map20(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map19(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map21(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map20(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map22(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map21(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map23(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map22(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map24(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map23(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map25(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map24(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map26(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map25(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map27(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map26(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map28(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map27(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map29(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map28(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map30(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map29(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map31(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map30(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map32(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map31(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map33(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map32(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map34(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map33(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map35(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map34(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map36(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map35(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map37(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map36(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map38(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map37(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map39(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map38(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map40(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map39(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map41(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map40(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map42(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map41(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map43(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map42(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map44(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map43(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map45(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map44(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map46(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map45(m, x, __VA_ARGS__)
#define __hmkernel_macro_exarg1_map47(m, x, v, ...) m(x, v) __hmkernel_macro_exarg1_map46(m, x, __VA_ARGS__)
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

/* Macro to detect if argument is void */
#define ____hmkernel_macro_is_not_void_void
#define __hmkernel_macro_is_not_void(x)	__hmkernel_macro_is_not_nul(____hmkernel_macro_is_not_void_##x)
#define ______hmkernel_macro_is_void_y	n
#define ______hmkernel_macro_is_void_n	y
#define ______hmkernel_macro_is_void(r) ______hmkernel_macro_is_void_##r
#define ____hmkernel_macro_is_void(r)	______hmkernel_macro_is_void(r)
#define __hmkernel_macro_is_void(x)	____hmkernel_macro_is_void(__hmkernel_macro_is_not_void(x))

/*
 * mapc: c for continuation. Macro m receives the first element in list and the tail part of
 * the list, it is allowed to process first and second element. Use __hmkernel_macro_args
 * to wrap all exargs so we can deliver things like __hmkernel_macro_args(void *a, void *b)
 * through macro argument list.
 */
#define __hmkernel_macro_exarg3_mapc0(m, x1, x2, x3, ...)
#define __hmkernel_macro_exarg3_mapc1(m, x1, x2, x3, v, ...) \
	m(pr, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__) \
	__hmkernel_macro_exarg3_mapc0(m, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), \
					 __hmkernel_macro_args(x3), __VA_ARGS__) \
	m(po, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__)

#define __hmkernel_macro_exarg3_mapc2(m, x1, x2, x3, v, ...) \
	m(pr, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__) \
	__hmkernel_macro_exarg3_mapc1(m, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), \
					 __hmkernel_macro_args(x3), __VA_ARGS__) \
	m(po, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__)

#define __hmkernel_macro_exarg3_mapc3(m, x1, x2, x3, v, ...) \
	m(pr, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__) \
	__hmkernel_macro_exarg3_mapc2(m, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), \
					 __hmkernel_macro_args(x3), __VA_ARGS__) \
	m(po, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__)

#define __hmkernel_macro_exarg3_mapc4(m, x1, x2, x3, v, ...) \
	m(pr, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__) \
	__hmkernel_macro_exarg3_mapc3(m, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), \
					 __hmkernel_macro_args(x3), __VA_ARGS__) \
	m(po, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__)

#define __hmkernel_macro_exarg3_mapc5(m, x1, x2, x3, v, ...) \
	m(pr, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__) \
	__hmkernel_macro_exarg3_mapc4(m, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), \
					 __hmkernel_macro_args(x3), __VA_ARGS__) \
	m(po, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__)

#define __hmkernel_macro_exarg3_mapc6(m, x1, x2, x3, v, ...) \
	m(pr, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__) \
	__hmkernel_macro_exarg3_mapc5(m, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), \
					 __hmkernel_macro_args(x3), __VA_ARGS__) \
	m(po, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), __hmkernel_macro_args(x3), v, __VA_ARGS__)

#define __hmkernel_macro_exarg3_mapc(n, m, x1, x2, x3, ...) \
	__hmkernel_macro_exarg3_mapc##n(m, __hmkernel_macro_args(x1), \
					   __hmkernel_macro_args(x2), \
					   __hmkernel_macro_args(x3), __VA_ARGS__)

#define ____hmkernel_macro_exarg3_mapc_list(n, m, x1, x2, x3, ...) \
	__hmkernel_macro_exarg3_mapc(n, m, __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), \
					   __hmkernel_macro_args(x3), ##__VA_ARGS__)
#define __hmkernel_macro_exarg3_mapc_list(m, x1, x2, x3, list)	\
	____hmkernel_macro_exarg3_mapc_list(__hmkernel_macro_nr_vars(list), m, \
					    __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), \
					    __hmkernel_macro_args(x3), list)
#define __hmkernel_macro_exarg3_mapc_vaargs(m, x1, x2, x3, ...)	\
	____hmkernel_macro_exarg3_mapc_list(__hmkernel_macro_nr_vars(__VA_ARGS__), m, \
					    __hmkernel_macro_args(x1), __hmkernel_macro_args(x2), \
					    __hmkernel_macro_args(x3), ##__VA_ARGS__)


#define __hmkernel_macro_proto_mkargs0(...)		void
#define __hmkernel_macro_proto_mkargs1(x)		void
#define __hmkernel_macro_proto_mkargs2(t, v)		t v
#define __hmkernel_macro_proto_mkargs4(t, v, ...)	t v, __hmkernel_macro_proto_mkargs2(__VA_ARGS__)
#define __hmkernel_macro_proto_mkargs6(t, v, ...)	t v, __hmkernel_macro_proto_mkargs4(__VA_ARGS__)
#define __hmkernel_macro_proto_mkargs8(t, v, ...)	t v, __hmkernel_macro_proto_mkargs6(__VA_ARGS__)
#define __hmkernel_macro_proto_mkargs10(t, v, ...)	t v, __hmkernel_macro_proto_mkargs8(__VA_ARGS__)
#define __hmkernel_macro_proto_mkargs12(t, v, ...)	t v, __hmkernel_macro_proto_mkargs10(__VA_ARGS__)
#define __hmkernel_macro_proto_mkargs14(t, v, ...)	t v, __hmkernel_macro_proto_mkargs12(__VA_ARGS__)
#define __hmkernel_macro_proto_mkargs16(t, v, ...)	t v, __hmkernel_macro_proto_mkargs14(__VA_ARGS__)
#define __hmkernel_macro_proto_mkargs18(t, v, ...)	t v, __hmkernel_macro_proto_mkargs16(__VA_ARGS__)
#define __hmkernel_macro_proto_mkargs20(t, v, ...)	t v, __hmkernel_macro_proto_mkargs18(__VA_ARGS__)

#define __hmkernel_macro_proto_mkprms0(...)
#define __hmkernel_macro_proto_mkprms1(...)
#define __hmkernel_macro_proto_mkprms2(t, v)		v
#define __hmkernel_macro_proto_mkprms4(t, v, ...)	v, __hmkernel_macro_proto_mkprms2(__VA_ARGS__)
#define __hmkernel_macro_proto_mkprms6(t, v, ...)	v, __hmkernel_macro_proto_mkprms4(__VA_ARGS__)
#define __hmkernel_macro_proto_mkprms8(t, v, ...)	v, __hmkernel_macro_proto_mkprms6(__VA_ARGS__)
#define __hmkernel_macro_proto_mkprms10(t, v, ...)	v, __hmkernel_macro_proto_mkprms8(__VA_ARGS__)
#define __hmkernel_macro_proto_mkprms12(t, v, ...)	v, __hmkernel_macro_proto_mkprms10(__VA_ARGS__)
#define __hmkernel_macro_proto_mkprms14(t, v, ...)	v, __hmkernel_macro_proto_mkprms12(__VA_ARGS__)
#define __hmkernel_macro_proto_mkprms16(t, v, ...)	v, __hmkernel_macro_proto_mkprms14(__VA_ARGS__)
#define __hmkernel_macro_proto_mkprms18(t, v, ...)	v, __hmkernel_macro_proto_mkprms16(__VA_ARGS__)
#define __hmkernel_macro_proto_mkprms20(t, v, ...)	v, __hmkernel_macro_proto_mkprms18(__VA_ARGS__)

#define ______hmkernel_macro_proto_mkargs(n, ...)	__hmkernel_macro_proto_mkargs##n(__VA_ARGS__)
#define ______hmkernel_macro_proto_mkprms(n, ...)	__hmkernel_macro_proto_mkprms##n(__VA_ARGS__)
#define ____hmkernel_macro_proto_mkargs(n, ...)	______hmkernel_macro_proto_mkargs(n, __VA_ARGS__)
#define ____hmkernel_macro_proto_mkprms(n, ...)	______hmkernel_macro_proto_mkprms(n, __VA_ARGS__)

#define __hmkernel_macro_proto_mkargs_list(protolist) \
	____hmkernel_macro_proto_mkargs(__hmkernel_macro_nr_vars(protolist), protolist)

#define __hmkernel_macro_proto_mkprms_list(protolist) \
	____hmkernel_macro_proto_mkprms(__hmkernel_macro_nr_vars(protolist), protolist)

#define __hmkernel_macro_proto_mkargs(...) \
	____hmkernel_macro_proto_mkargs(__hmkernel_macro_nr_vars(__VA_ARGS__), __VA_ARGS__)

#define __hmkernel_macro_proto_mkprms(...) \
	____hmkernel_macro_proto_mkprms(__hmkernel_macro_nr_vars(__VA_ARGS__), __VA_ARGS__)

#endif
