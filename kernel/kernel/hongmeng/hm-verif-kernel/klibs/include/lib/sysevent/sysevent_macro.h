/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Sysevent header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 9 20:44:58 2024
 */
#ifndef KLIBS_SYSEVENT_MACRO_H
#define KLIBS_SYSEVENT_MACRO_H

#define MAX_DOMAIN_LENGTH	16
#define MAX_EVENT_NAME_LENGTH	32
#define MAX_PARAM_NAME_LENGTH	48
#define MAX_PARAM_NUMBER	128
#define PARAM_STR_MAX_LEN	1536

#ifndef __KERNEL__
#define BIT(nr) (1UL << (nr))
#endif /* __KERNEL__ */

#define TAG_BYTE_OFFSET		5
#define TAG_BYTE_BOUND		(BIT(TAG_BYTE_OFFSET))
#define TAG_BYTE_MASK		(TAG_BYTE_BOUND - 1)

#define NON_TAG_BYTE_OFFSET	7
#define NON_TAG_BYTE_BOUND	(BIT(NON_TAG_BYTE_OFFSET))
#define NON_TAG_BYTE_MASK	(NON_TAG_BYTE_BOUND - 1)

#define TIMEZONE_UTC_INDEX	14
#define SYSEVT_BUF_SIZE		2048

#define SYSEVT_TYPE_OF(n, key, value)							\
	__builtin_choose_expr(								\
		(__builtin_types_compatible_p(typeof(value), long long)			\
		 || __builtin_types_compatible_p(typeof(value), long)			\
		 || __builtin_types_compatible_p(typeof(value), int)			\
		 || __builtin_types_compatible_p(typeof(value), short)			\
		 || __builtin_types_compatible_p(typeof(value), unsigned long long)	\
		 || __builtin_types_compatible_p(typeof(value), unsigned long)		\
		 || __builtin_types_compatible_p(typeof(value), unsigned int)		\
		 || __builtin_types_compatible_p(typeof(value), unsigned short)),	\
		INT64,								\
		__builtin_choose_expr(							\
			(__builtin_types_compatible_p(typeof(value), char *)		\
			 || __builtin_types_compatible_p(typeof(value), char [])	\
			 || __builtin_types_compatible_p(typeof(value), const char *)),	\
			STRING,								\
			 /* void expression results in a compile-time error */		\
			(void)0)), (key),						\
	__builtin_choose_expr(								\
			(__builtin_types_compatible_p(typeof(value), char *)		\
			 || __builtin_types_compatible_p(typeof(value), char [])	\
			 || __builtin_types_compatible_p(typeof(value), const char *)),	\
			(value),							\
			(long long)(value))

#ifdef __KERNEL__
#define SYSEVT_KV_MAP0(n, ...)	(n)
#define SYSEVT_KV_MAP2(n, ...)	__MAP1(n, __VA_ARGS__)
#define SYSEVT_KV_MAP4(n, ...)	__MAP2(n, __VA_ARGS__)
#define SYSEVT_KV_MAP6(n, ...)	__MAP3(n, __VA_ARGS__)
#define SYSEVT_KV_MAP8(n, ...)	__MAP4(n, __VA_ARGS__)
#define SYSEVT_KV_MAP10(n, ...)	__MAP5(n, __VA_ARGS__)
#define SYSEVT_KV_MAP12(n, ...)	__MAP6(n, __VA_ARGS__)
#define SYSEVT_KV_MAP14(n, ...)	__MAP7(n, __VA_ARGS__)
#define SYSEVT_KV_MAP16(n, ...)	__MAP8(n, __VA_ARGS__)
#else
#define SYSEVT_KV_MAP0(n, ...)	(n)
#define SYSEVT_KV_MAP2(n, ...)	__SC_MAP1(n, __VA_ARGS__)
#define SYSEVT_KV_MAP4(n, ...)	__SC_MAP2(n, __VA_ARGS__)
#define SYSEVT_KV_MAP6(n, ...)	__SC_MAP3(n, __VA_ARGS__)
#define SYSEVT_KV_MAP8(n, ...)	__SC_MAP4(n, __VA_ARGS__)
#define SYSEVT_KV_MAP10(n, ...)	__SC_MAP5(n, __VA_ARGS__)
#define SYSEVT_KV_MAP12(n, ...)	__SC_MAP6(n, __VA_ARGS__)
#define SYSEVT_KV_MAP14(n, ...)	__SC_MAP7(n, __VA_ARGS__)
#define SYSEVT_KV_MAP16(n, ...)	__SC_MAP8(n, __VA_ARGS__)
#endif /* __KERNEL__ */

#define SYSEVT_KV_MAP(n, ...)	SYSEVT_KV_MAP##n(((n) / 2), __VA_ARGS__)
#define SYSEVT_KV_PAIRS(n, ...)	((n) / 2), SYSEVT_KV_MAP(n, SYSEVT_TYPE_OF, __VA_ARGS__)
#define hm_sysevent_report(domain, name, type, ...)				\
	hm_sysevent_report_raw(domain, name, type,					\
			       SYSEVT_KV_PAIRS(VA_ARGS_NUM(__VA_ARGS__), __VA_ARGS__))

#endif
