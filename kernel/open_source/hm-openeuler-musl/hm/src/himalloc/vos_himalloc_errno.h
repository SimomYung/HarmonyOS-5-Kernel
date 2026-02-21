/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_HIMALLOC_ERRNO_H
#define VOS_HIMALLOC_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup himalloc
 * 0x1 操作权限受限。
 */
#define HIMALLOC_ERRNO_PERM 1 /* Operation not permitted */

/**
 * @ingroup himalloc
 * 0x2 操作权限受限。
 */
#define HIMALLOC_ERRNO_OVERFLOW 2 /* Value overflow */

/**
 * @ingroup himalloc
 * 0x3 没有足够的block块
 */
#define HIMALLOC_ERRNO_NOBLOCK 3 /* Not enough block */

/**
 * @ingroup himalloc
 * 0xC 内存不足
 */
#define HIMALLOC_ERRNO_NOMEM 12 /* Out of memory */

/**
 * @ingroup himalloc
 * 0x16 参数错误。
 */
#define HIMALLOC_ERRNO_INVAL 22 /* Invalid argument */

#ifdef __cplusplus
}
#endif

#endif
