/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Wrapper for actvhdlr marco
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb  4 16:55:01 2020
 */
#ifndef __DEVHOST_UTILS_ACTV_H__
#define __DEVHOST_UTILS_ACTV_H__

#include <libsysif/utils.h>

static inline void __actvhdlrals_noop_hook(void) {}

#define __DEFINE_ACTVHDLR_HOOK_ALS(name, pre_hook, post_hook, m, t, n, ...)	\
static inline t __actvhdlrals_hook_##name(__SC_MAP(n, __SC_DECL, __VA_ARGS__)); \
m(name, __VA_ARGS__)								\
{										\
	t ret;									\
	pre_hook();								\
	ret = __actvhdlrals_hook_##name(__SC_MAP(n, __SC_USE, __VA_ARGS__));	\
	post_hook();								\
	return ret;								\
}										\
static inline t __actvhdlrals_hook_##name(__SC_MAP(n, __SC_DECL, __VA_ARGS__))

#define DEFINE_ACTVHDLR_HOOK_ALS(name, pre_hook, post_hook, ...)		\
	__DEFINE_ACTVHDLR_HOOK_ALS(name, pre_hook, post_hook,			\
				   DEFINE_ACTVHDLR_ALS, int,			\
				   __HDLR_ARG_NR(__VA_ARGS__), __VA_ARGS__)

#define DEFINE_ACTVHDLR_PREHOOK_ALS(name, pre_hook, ...)			\
	__DEFINE_ACTVHDLR_HOOK_ALS(name, pre_hook, __actvhdlrals_noop_hook,	\
				   DEFINE_ACTVHDLR_ALS, int,			\
				   __HDLR_ARG_NR(__VA_ARGS__), __VA_ARGS__)

#define DEFINE_ACTVHDLR_POSTHOOK_ALS(name, post_hook, ...)			\
	__DEFINE_ACTVHDLR_HOOK_ALS(name, __actvhdlrals_noop_hook, post_hook,	\
				   DEFINE_ACTVHDLR_ALS, int,			\
				   __HDLR_ARG_NR(__VA_ARGS__), __VA_ARGS__)

#define DEFINE_ACTVHDLR_HOOK_LONG_ALS(name, pre_hook, post_hook, ...)		\
	__DEFINE_ACTVHDLR_HOOK_ALS(name, pre_hook, post_hook,			\
				   DEFINE_ACTVHDLR_LONG_ALS, long,		\
				   __HDLR_ARG_NR(__VA_ARGS__), __VA_ARGS__)

#endif /* __DEVHOST_UTILS_ACTV_H__ */
