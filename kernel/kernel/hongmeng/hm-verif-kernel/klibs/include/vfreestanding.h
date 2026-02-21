/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Description: libvsync freestanding wrapper
 * Create: Jul 28 14:34 2021
 */

#ifndef KLIBS_VSYNC_VFREESTANDING_H
#define KLIBS_VSYNC_VFREESTANDING_H

#include <hmkernel/types.h>
typedef u8		vuint8_t;
typedef u16		vuint16_t;
typedef u32		vuint32_t;
typedef u64		vuint64_t;
typedef s8		vint8_t;
typedef s16		vint16_t;
typedef s32		vint32_t;
typedef s64		vint64_t;
typedef bool		vbool_t;
typedef size_t		vsize_t;
typedef uptr_t		vuintptr_t;

// Ideally ptr_t should be used here,
// but currently it is not defined for some tests.
// This is a workaround.
#if 8 == __SIZEOF_POINTER__
typedef vint64_t vintptr_t;
#else
typedef vint32_t vintptr_t;
#endif /* 8 == __SIZEOF_POINTER__ */

#ifndef UINT32_MAX
#define UINT32_MAX ((vuint32_t) ~(vuint32_t)0)
#endif

#endif /* !KLIBS_VSYNC_VFREESTANDING_H */
