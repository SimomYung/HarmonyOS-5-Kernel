/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef HIMALLOC_INNER_H
#define HIMALLOC_INNER_H

#ifdef _MUSL_SOURCE
#include <bits/reg.h>
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "vos_himalloc_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#ifdef MALLOC_HI_PERFORMANCE
#define HIMALLOC_TCACHE_POLICY_ARRAY    // 采用数组缓存策略，底噪开销较大，性能较高
#else
#define HIMALLOC_TCACHE_POLICY_LIST     // 采用链表缓存策略，底噪开销较小，性能比另一种策略稍微差点
#endif

/* 与DLM算法保持一致 */
typedef enum tagHimallocFreeTYPE {
    VOS_HIMALLOC_FREE_SUCCEED_E = 0, /**< 正常释放内存成功 */
    VOS_HIMALLOC_FREE_REF_DEC_E,     /**< 对应要释放的内存块引用计数正常减一 */
    VOS_HIMALLOC_FREE_REF_ZERO_E,    /**< 对应要释放的内存块引用计数为0，释放失败 */
    VOS_HIMALLOC_FREE_FAILED_E       /**< 其他情况释放失败返回值 */
} VOS_HIMALLOC_FREE_TYPE_E;

/* 与VOS_LOG保持一致 */
typedef enum tagVosHimallocLogLevel {
    VOS_HIMALLOC_LOG_LEVEL_INFO = 0,
    VOS_HIMALLOC_LOG_LEVEL_WARNING = 1,
    VOS_HIMALLOC_LOG_LEVEL_ERROR = 2,
    VOS_HIMALLOC_LOG_LEVEL_EXCEPT = 3,
    VOS_HIMALLOC_LOG_LEVEL_BUTT
} VOS_HIMALLOC_LOG_LEVEL_E;

/* 作为so独立发布时所需的模块初始化标志 */
typedef enum tagHimModuleInitState {
    HIM_MODULE_INIT_STATE_UNINIT = 0,
    HIM_MODULE_INIT_STATE_ININIT = 1,
    HIM_MODULE_INIT_STATE_INITED = 2,
    HIM_MODULE_INIT_STATE_INITED_BUTT
} VOS_HIM_MODULE_INIT_STATE_E;

#define WEAK_ALIAS(name, alias) \
    extern __typeof(name) (alias) __attribute__((__weak__, __alias__(#name)))

#define HIMALLOC_NULL_PTR NULL
#define HIMALLOC_NULL_DWORD 0xFFFFFFFFU
#define HIMALLOC_NULL_SIZE_T (~(size_t)0)

/* Common Error Code */
#define HIMALLOC_ERR 1 /* 出错 */
#define HIMALLOC_OK 0  /* 正确 */

/**
 * @ingroup himalloc
 * @brief 适配鸿蒙OS的double free dfx机制，开关
 */
extern bool g_himalloc_enable_mcheck;

#if defined(__GNUC__) && (__GNUC__ >= 4)
#define HIMALLOC_ALWAYS_INLINE inline __attribute__((always_inline))
#else
#define HIMALLOC_ALWAYS_INLINE inline
#endif

#ifndef HIMALLOC_LIKELY

#if defined(__GNUC__) && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ > 3))) /* above 3.4 */
/**
 * @ingroup himalloc
 * 预测分支可能走向，帮助编译器优化编译，减少代码的分支预测失败时的可能开销。
 */
#define HIMALLOC_LIKELY(x) ((bool)__builtin_expect(!!(x), 1))
/**
 * @ingroup himalloc
 * 预测分支不可能走向，帮助编译器优化编译，减少代码的分支预测失败时的可能开销。
 */
#define HIMALLOC_UNLIKELY(x) ((bool)__builtin_expect(!!(x), 0))

#else

#define HIMALLOC_LIKELY(x) (x)
#define HIMALLOC_UNLIKELY(x) (x)

#endif

#endif

#ifndef HIDDEN
#ifdef OS_HONGMENG
#define HIDDEN __attribute__((__visibility__("hidden")))
#else
#define HIDDEN
#endif
#endif

HIDDEN uint32_t VosHimallocModuleInit(void);

#define ptr_to_void(ptr)        ((void *)(uintptr_t)(ptr))
#define ptr_from_void(ptr, type)    ({void *__ptr = ptr; (type *)(uintptr_t)(__ptr);})
#define ptr_cast(ptr, type)        ptr_from_void(ptr_to_void(ptr), type)
#define ptr_to_ulong(ptr)        ((unsigned long)(uintptr_t)(ptr))
#define ulong_to_ptr(val, type)        ({type *__p; __p = (type *)(uintptr_t)(val); __p;})

#ifndef __ASSEMBLY__
/*
 * __hmattr_sideff_resolved(reason) is a semantically trivial function aiming to
 * tell the code reviewers that before an early return statement, all the
 * side-effects (including locks, temporarily allocated objects, etc.) are
 * carefully checked and resolved (e.g. released). Any invocation of this function
 * is expected to be erased by the compilers.
 *
 * The function has no return value.
 *
 * @param reason a constant string telling the reviewer why and how the side
                 effect is resolved, or "no relative side effect"
 */
static inline void __hmattr_sideff_resolved(const char *reason)
{
    const char *_reason = reason;
    (void)(_reason);
}
#endif
#define __pure    __attribute__((pure))


#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
