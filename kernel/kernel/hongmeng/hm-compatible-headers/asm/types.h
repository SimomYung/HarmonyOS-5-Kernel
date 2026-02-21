/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add asm/types.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 19 16:30:55 2021
 */

#ifndef LINUX_ASM_TYPES_H
#define LINUX_ASM_TYPES_H

#include <asm-generic/types.h>

typedef unsigned long __uptr_t;
#define __ptr_to_u64(ptr) ((__u64)(__uptr_t)(ptr))

#define __PRIx64	"llx"
#define __PRId64	"lld"
#define __PRIu64	"llu"
#define __PRIx32	"x"
#define __PRId32	"d"
#define __PRIu32	"u"

#ifdef __SIZE_TYPE__
typedef __SIZE_TYPE__ __kernel_size_t;
#else
typedef unsigned long __kernel_size_t;
#endif

#endif
