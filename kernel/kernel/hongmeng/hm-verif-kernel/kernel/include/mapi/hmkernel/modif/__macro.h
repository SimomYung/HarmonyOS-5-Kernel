/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Common macro of modif
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 21 10:31:25 2020
 */
#ifndef MAPI_HMKERNEL_MODIF___MACRO_H
#define MAPI_HMKERNEL_MODIF___MACRO_H

#include <hmkernel/modif/__kernel.h>

#define __MODIF_MAP0(n, m, ...)
#define __MODIF_MAP1(n, m, t, a, ...) m(n - 1, t, a)
#define __MODIF_MAP2(n, m, t, a, ...) m(n - 2, t, a), __MODIF_MAP1(n, m, __VA_ARGS__)
#define __MODIF_MAP3(n, m, t, a, ...) m(n - 3, t, a), __MODIF_MAP2(n, m, __VA_ARGS__)
#define __MODIF_MAP4(n, m, t, a, ...) m(n - 4, t, a), __MODIF_MAP3(n, m, __VA_ARGS__)
#define __MODIF_MAP5(n, m, t, a, ...) m(n - 5, t, a), __MODIF_MAP4(n, m, __VA_ARGS__)
#define __MODIF_MAP6(n, m, t, a, ...) m(n - 6, t, a), __MODIF_MAP5(n, m, __VA_ARGS__)
#define __MODIF_MAP(n, ...) __MODIF_MAP##n(n, __VA_ARGS__)

#define __MODIF_DEF_VAR(n, t, a)		t a = (t)0u
#define __MODIF_DEF(n, t, a)		t a
#define __MODIF_CALL(n, t, a)		a

#define __MODIF_MAPSTAT_DEFVAR0(n, m, ...)
#define __MODIF_MAPSTAT_DEFVAR1(n, m, t, a, ...) m(n##ul - 1UL, t, a)
#define __MODIF_MAPSTAT_DEFVAR2(n, m, t, a, ...) m(n##ul - 2UL, t, a); __MODIF_MAPSTAT_DEFVAR1(n, m, __VA_ARGS__)
#define __MODIF_MAPSTAT_DEFVAR3(n, m, t, a, ...) m(n##ul - 3UL, t, a); __MODIF_MAPSTAT_DEFVAR2(n, m, __VA_ARGS__)
#define __MODIF_MAPSTAT_DEFVAR4(n, m, t, a, ...) m(n##ul - 4UL, t, a); __MODIF_MAPSTAT_DEFVAR3(n, m, __VA_ARGS__)
#define __MODIF_MAPSTAT_DEFVAR5(n, m, t, a, ...) m(n##ul - 5UL, t, a); __MODIF_MAPSTAT_DEFVAR4(n, m, __VA_ARGS__)
#define __MODIF_MAPSTAT_DEFVAR6(n, m, t, a, ...) m(n##ul - 6UL, t, a); __MODIF_MAPSTAT_DEFVAR5(n, m, __VA_ARGS__)
#define __MODIF_MAPSTAT_DEFVAR7(n, m, t, a, ...) m(n##ul - 7UL, t, a); __MODIF_MAPSTAT_DEFVAR6(n, m, __VA_ARGS__)
#define __MODIF_MAPSTAT_DEFVAR(n, ...) __MODIF_MAPSTAT_DEFVAR##n(n, __VA_ARGS__)

/* e must be initialized to 0 */
#define __MODIF_MAPSTAT_OP0(n, m, e, t, a, ...)
#define __MODIF_MAPSTAT_OP1(n, m, e, t, a, ...) \
	if (e == 0) {e = m(n##ul - 1UL, t, a);}
#define __MODIF_MAPSTAT_OP2(n, m, e, t, a, ...) \
	if (e == 0) {e = m(n##ul - 2UL, t, a);} __MODIF_MAPSTAT_OP1(n, m, e, __VA_ARGS__)
#define __MODIF_MAPSTAT_OP3(n, m, e, t, a, ...) \
	if (e == 0) {e = m(n##ul - 3UL, t, a);} __MODIF_MAPSTAT_OP2(n, m, e, __VA_ARGS__)
#define __MODIF_MAPSTAT_OP4(n, m, e, t, a, ...) \
	if (e == 0) {e = m(n##ul - 4UL, t, a);} __MODIF_MAPSTAT_OP3(n, m, e, __VA_ARGS__)
#define __MODIF_MAPSTAT_OP5(n, m, e, t, a, ...) \
	if (e == 0) {e = m(n##ul - 5UL, t, a);} __MODIF_MAPSTAT_OP4(n, m, e, __VA_ARGS__)
#define __MODIF_MAPSTAT_OP6(n, m, e, t, a, ...) \
	if (e == 0) {e = m(n##ul - 6UL, t, a);} __MODIF_MAPSTAT_OP5(n, m, e, __VA_ARGS__)
#define __MODIF_MAPSTAT_OP7(n, m, e, t, a, ...) \
	if (e == 0) {e = m(n##ul - 7UL, t, a);} __MODIF_MAPSTAT_OP6(n, m, e, __VA_ARGS__)
#define __MODIF_MAPSTAT_OP(n, ...) __MODIF_MAPSTAT_OP##n(n, __VA_ARGS__)

#define __MODIF_FETCH_OP(n, t, a) ({				\
	int ___err;						\
	struct arg_type_info ___arg_type_info = {		\
		.is_signed = __is_signed(t),			\
		.is_fixed64 = __is_fixed64(t),			\
		.argsz = sizeof(t)				\
	};							\
	___err = modif_sysarg((void *)(&(a)),			\
			      caller, (unsigned long)(n),	\
			      ___arg_type_info);		\
	___err;							\
})

#define __MODIF_ARGSx(x, e, ...)			\
	__MODIF_MAPSTAT_DEFVAR(x, __MODIF_DEF_VAR, __VA_ARGS__);	\
	__MODIF_MAPSTAT_OP(x, __MODIF_FETCH_OP, e, __VA_ARGS__)

#define MODIF_HANDLER_DEFINEx(x, handler, ioctl, ...)			\
static long								\
handler(struct tcb_s *caller)						\
{									\
	int __err = 0;							\
									\
	__MODIF_ARGSx(x, __err, __VA_ARGS__);				\
									\
	if (__err == 0) {						\
		__err = ioctl(NULL, __MODIF_MAP(x, __MODIF_CALL, __VA_ARGS__));\
	}								\
									\
	return (long)__err;							\
}

#define MODIF_HANDLER_DEFINE1(handler, ioctl, ...)	MODIF_HANDLER_DEFINEx(1, handler, ioctl, __VA_ARGS__)
#define MODIF_HANDLER_DEFINE2(handler, ioctl, ...)	MODIF_HANDLER_DEFINEx(2, handler, ioctl, __VA_ARGS__)
#define MODIF_HANDLER_DEFINE3(handler, ioctl, ...)	MODIF_HANDLER_DEFINEx(3, handler, ioctl, __VA_ARGS__)
#define MODIF_HANDLER_DEFINE4(handler, ioctl, ...)	MODIF_HANDLER_DEFINEx(4, handler, ioctl, __VA_ARGS__)
#define MODIF_HANDLER_DEFINE5(handler, ioctl, ...)	MODIF_HANDLER_DEFINEx(5, handler, ioctl, __VA_ARGS__)
#define MODIF_HANDLER_DEFINE6(handler, ioctl, ...)	MODIF_HANDLER_DEFINEx(6, handler, ioctl, __VA_ARGS__)
#define MODIF_HANDLER_DEFINE7(handler, ioctl, ...)	MODIF_HANDLER_DEFINEx(7, handler, ioctl, __VA_ARGS__)

#endif
