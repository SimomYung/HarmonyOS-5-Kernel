/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Macro for modules
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 23 09:52:49 2021
 */
#ifndef MAPI_HMKERNEL_MACRO_H
#define MAPI_HMKERNEL_MACRO_H

#include <uapi/hmkernel/macro.h>

#define __MAP0(n, m, ...)
#define __MAP1(n, m, t, a, ...) m(n - 1, t, a)
#define __MAP2(n, m, t, a, ...) m(n - 2, t, a), __MAP1(n, m, __VA_ARGS__)
#define __MAP3(n, m, t, a, ...) m(n - 3, t, a), __MAP2(n, m, __VA_ARGS__)
#define __MAP4(n, m, t, a, ...) m(n - 4, t, a), __MAP3(n, m, __VA_ARGS__)
#define __MAP5(n, m, t, a, ...) m(n - 5, t, a), __MAP4(n, m, __VA_ARGS__)
#define __MAP6(n, m, t, a, ...) m(n - 6, t, a), __MAP5(n, m, __VA_ARGS__)
#define __MAP7(n, m, t, a, ...) m(n - 7, t, a), __MAP6(n, m, __VA_ARGS__)
#define __MAP8(n, m, t, a, ...) m(n - 8, t, a), __MAP7(n, m, __VA_ARGS__)
#define __MAP9(n, m, t, a, ...) m(n - 9, t, a), __MAP8(n, m, __VA_ARGS__)
#define __MAP10(n, m, t, a, ...) m(n - 10, t, a), __MAP9(n, m, __VA_ARGS__)
#define __MAP11(n, m, t, a, ...) m(n - 11, t, a), __MAP10(n, m, __VA_ARGS__)
#define __MAP12(n, m, t, a, ...) m(n - 12, t, a), __MAP11(n, m, __VA_ARGS__)
#define __MAP13(n, m, t, a, ...) m(n - 13, t, a), __MAP12(n, m, __VA_ARGS__)
#define __MAP(n, ...) __MAP##n(n, __VA_ARGS__)

#define __PROTO_MAP0(n, m, ...)	void
#define __PROTO_MAP1(n, m, t, a, ...) m(n - 1, t, a)
#define __PROTO_MAP2(n, m, t, a, ...) m(n - 2, t, a), __PROTO_MAP1(n, m, __VA_ARGS__)
#define __PROTO_MAP3(n, m, t, a, ...) m(n - 3, t, a), __PROTO_MAP2(n, m, __VA_ARGS__)
#define __PROTO_MAP4(n, m, t, a, ...) m(n - 4, t, a), __PROTO_MAP3(n, m, __VA_ARGS__)
#define __PROTO_MAP5(n, m, t, a, ...) m(n - 5, t, a), __PROTO_MAP4(n, m, __VA_ARGS__)
#define __PROTO_MAP6(n, m, t, a, ...) m(n - 6, t, a), __PROTO_MAP5(n, m, __VA_ARGS__)
#define __PROTO_MAP7(n, m, t, a, ...) m(n - 7, t, a), __PROTO_MAP6(n, m, __VA_ARGS__)
#define __PROTO_MAP8(n, m, t, a, ...) m(n - 8, t, a), __PROTO_MAP7(n, m, __VA_ARGS__)
#define __PROTO_MAP9(n, m, t, a, ...) m(n - 9, t, a), __PROTO_MAP8(n, m, __VA_ARGS__)
#define __PROTO_MAP10(n, m, t, a, ...) m(n - 10, t, a), __PROTO_MAP9(n, m, __VA_ARGS__)
#define __PROTO_MAP11(n, m, t, a, ...) m(n - 11, t, a), __PROTO_MAP10(n, m, __VA_ARGS__)
#define __PROTO_MAP12(n, m, t, a, ...) m(n - 12, t, a), __PROTO_MAP11(n, m, __VA_ARGS__)
#define __PROTO_MAP13(n, m, t, a, ...) m(n - 13, t, a), __PROTO_MAP12(n, m, __VA_ARGS__)
#define __PROTO_MAP(n, ...) __PROTO_MAP##n(n, __VA_ARGS__)

#define __MAPSTAT_DEFVAR0(n, m, ...)
#define __MAPSTAT_DEFVAR1(n, m, t, a, ...) m(n##UL - 1UL, t, a)
#define __MAPSTAT_DEFVAR2(n, m, t, a, ...) m(n##UL - 2UL, t, a); __MAPSTAT_DEFVAR1(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR3(n, m, t, a, ...) m(n##UL - 3UL, t, a); __MAPSTAT_DEFVAR2(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR4(n, m, t, a, ...) m(n##UL - 4UL, t, a); __MAPSTAT_DEFVAR3(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR5(n, m, t, a, ...) m(n##UL - 5UL, t, a); __MAPSTAT_DEFVAR4(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR6(n, m, t, a, ...) m(n##UL - 6UL, t, a); __MAPSTAT_DEFVAR5(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR7(n, m, t, a, ...) m(n##UL - 7UL, t, a); __MAPSTAT_DEFVAR6(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR8(n, m, t, a, ...) m(n##UL - 8UL, t, a); __MAPSTAT_DEFVAR7(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR9(n, m, t, a, ...) m(n##UL - 9UL, t, a); __MAPSTAT_DEFVAR8(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR10(n, m, t, a, ...) m(n##UL - 10UL, t, a); __MAPSTAT_DEFVAR9(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR11(n, m, t, a, ...) m(n##UL - 11UL, t, a); __MAPSTAT_DEFVAR10(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR12(n, m, t, a, ...) m(n##UL - 12UL, t, a); __MAPSTAT_DEFVAR11(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR13(n, m, t, a, ...) m(n##UL - 13UL, t, a); __MAPSTAT_DEFVAR12(n, m, __VA_ARGS__)
#define __MAPSTAT_DEFVAR14(n, m, t, a, ...) m(n##UL - 14UL, t, a); __MAPSTAT_DEFVAR13(n, m, __VA_ARGS__)


#define __MAPSTAT_DEFVAR(n, ...) __MAPSTAT_DEFVAR##n(n, __VA_ARGS__)

/* e must be initialized to 0 */
#define __MAPSTAT_OP0(n, m, e, ...)
#define __MAPSTAT_OP1(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 1UL, t, a);} } while(0)
#define __MAPSTAT_OP2(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 2UL, t, a);} __MAPSTAT_OP1(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP3(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 3UL, t, a);} __MAPSTAT_OP2(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP4(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 4UL, t, a);} __MAPSTAT_OP3(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP5(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 5UL, t, a);} __MAPSTAT_OP4(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP6(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 6UL, t, a);} __MAPSTAT_OP5(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP7(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 7UL, t, a);} __MAPSTAT_OP6(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP8(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 8UL, t, a);} __MAPSTAT_OP7(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP9(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 9UL, t, a);} __MAPSTAT_OP8(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP10(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 10UL, t, a);} __MAPSTAT_OP9(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP11(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 11UL, t, a);} __MAPSTAT_OP10(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP12(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 12UL, t, a);} __MAPSTAT_OP11(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP13(n,m,e,t,a,...) \
	do { if (e == 0) {e = m(n##UL - 13UL, t, a);} __MAPSTAT_OP12(n, m, e, __VA_ARGS__); } while(0)
#define __MAPSTAT_OP(n, ...) __MAPSTAT_OP##n(n, __VA_ARGS__)

#define __SC_DEF_VAR(n, t, a)		t a = (t)0u
#define __SC_DEF(n, t, a)		t a
#define __SC_CALL(n, t, a)		a

/* The following are macros for static CONFIG detection, and CONFIG-controlled code inclusion */

/* Some low level macro tricks for static test and code isolation control */
#define ___concat(a, b) a ## b
#define __concat(a, b) ___concat(a, b)
#define ___get_arg2(arg1, arg2, ...) arg2
#define __get_arg2(...) ___get_arg2(__VA_ARGS__)

#define STATIC_TEST_CONFIG_1 dummy,
#define calc_config_01(_conf) __get_arg2(__concat(STATIC_TEST_CONFIG_, _conf) 1, 0)
#define config_gen_ops(_conf, _ops) __concat(_ops, calc_config_01(_conf))

#define __emit_code1(_code1, _)  _code1
#define __emit_code0(_, _code0)  _code0
#define __if_def(_conf, _code1, _code0) config_gen_ops(_conf, __emit_code)(_code1, _code0)

/*
 * Static-control1: statically test whether a config is enabled, and optimize branch compilation.
 *
 * example:
 *   if (is_config_enabled(CONFIG_ABC)) {
 *	  code_abc;
 *   } else {
 * 	  code_xyz;
 *   }
 *
 * if CONFIG_ABC is defined, `else` will be never reached, thus remove by compiler.
 */
#define is_config_enabled(_conf) (calc_config_01(_conf) == 1)

/*
 * Static-control2:. macro for code isolation control.
 * If `_conf` enabled, `_code` will be included, otherwise `...` will be included.
 *
 *  example1 : _if_def(CONFIG_ABC, code_abc)
 *       code_abc will be included only if CONFIG_ABC is defined.
 *  example2 :
 *       _if_def(CONFIG_ABC, code_abc, code_xyz), or reading-friendly format:
 *       _if_def(CONFIG_ABC,
 *               code_abc
 *       __ELSE,
 *               code_xyz)
 *
 *       code_abc will be included only if CONFIG_ABC is defined, other wise code_xyz will
 *       be include.
 */
#define _if_def(_conf, _code, ...) __if_def(_conf, _code, __VA_ARGS__)
#define __ELSE

#endif
