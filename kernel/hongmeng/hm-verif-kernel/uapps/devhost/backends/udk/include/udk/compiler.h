/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK wrappings for compiler features
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 15 09:25:53 2019
 */
#ifndef __UDK_COMPILER_H__
#define __UDK_COMPILER_H__

#include <hongmeng/compiler.h>

/* Additional compiler macros */
#define __maybe_unused		__attribute__((unused))

#define UDK_CONSTRUCTOR __used __attribute__((constructor))

#endif /* __UDK_COMPILER_H__ */
