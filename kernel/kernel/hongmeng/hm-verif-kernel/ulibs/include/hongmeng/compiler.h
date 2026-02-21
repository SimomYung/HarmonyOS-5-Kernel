/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Compiler
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 25 12:57:57 2018
 */
#ifndef ULIBS_HONGMENG_COMPILER_H
#define ULIBS_HONGMENG_COMPILER_H

#include <hmkernel/compiler.h>

#define __app

#ifndef __always_inline
#define __always_inline inline __attribute__((always_inline))
#endif

#ifndef likely
#define likely(x)	__likely(x)
#endif

#ifndef unlikely
#define unlikely(x)	__unlikely(x)
#endif

#define __raw_static_assert_name_comb(x, y)	x##y
#define raw_static_assert(condition, name) 	\
	extern int __raw_static_assert_name_comb(__STATIC_ASSERT_, name)[1 - 2 * ((condition) ? 0 : 1)]

#ifndef __hmbuild__
/* legacy */
#ifdef CONFIG_SHADOW_STACK
#define __nosstack	__attribute__((nosstack))
#else
#define __nosstack
#endif
#ifdef CONFIG_SFI
#define __nosfi	__attribute__((nosfi))
#else
#define __nosfi
#endif

#ifdef CONFIG_UCFI
#define __nocfi		__attribute__((nocfi))
#else
#define __nocfi
#endif

#else

/* morden */
#ifdef __hmfeature_shadowstack__
#define __nosstack	__attribute__((nosstack))
#else
#define __nosstack
#endif
#ifdef __hmfeature_sfi__
#define __nosfi	__attribute__((nosfi))
#else
#define __nosfi
#endif

#ifdef __hmfeature_cfi__
#define __nocfi		__attribute__((nocfi))
#else
#define __nocfi
#endif

#endif

#if defined(__clang__) || (defined(__GNUC__) && __GNUC__ >= 12)
#define __uninitialized __attribute__((uninitialized))
#else
#define __uninitialized
#endif

#ifdef __clang__
#define __no_sanitize_thread __attribute__((no_sanitize("thread")))
#elif defined(__GNUC__)
#define __no_sanitize_thread __attribute__((no_sanitize_thread))
#else
#error "unknown compiler"
#endif

#endif
