/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_HIMALLOCSCLASS_H
#define VOS_HIMALLOCSCLASS_H

#include "vos_himallocassert.h"
#include "vos_himallocbase.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/* tiny slice对齐长度的对数 */
#define VOS_HIMALLOC_LG_TINY_ALIGN_SIZE 4U

/* tiny slice的对齐长度 */
#define VOS_HIMALLOC_TINY_ALIGN_SIZE (1U << VOS_HIMALLOC_LG_TINY_ALIGN_SIZE)

/* small slice对齐长度的对数 */
#define VOS_HIMALLOC_LG_SMALL_ALIGN_SIZE 9U

/* small slice的对齐长度 */
#define VOS_HIMALLOC_SMALL_ALIGN_SIZE (1U << VOS_HIMALLOC_LG_SMALL_ALIGN_SIZE)

/* tiny slice 的最大内存 */
#define VOS_HIMALLOC_MAX_TINY_SIZE 1024UL

/* small slice 的最大内存 */
#define VOS_HIMALLOC_MAX_SMALL_SIZE 2048UL

/* VOS_HIMALLOC_MAX_SMALL_SIZE 对应的index */
#define VOS_HIMALLOC_MAX_SMALL_INDEX 14U

/* 将VOS_HIMALLOC_MAX_SMALL_INDEX按照3的幂次方对齐 */
#define VOS_HIMALLOC_MAX_SMALL_INDEX_LG_ALIGN (VOS_HIMALLOC_LG_ALIGN(VOS_HIMALLOC_MAX_SMALL_INDEX, 3))

/* 根据index获取size */
extern HIDDEN const size_t g_auvHimallocIndex2size[VOS_HIMALLOC_MAX_SMALL_INDEX];

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
