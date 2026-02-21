/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: add micro for static and shared libs' build
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 06 13:49:29 2021
 */

#ifndef MACRO_H
#define MACRO_H

#define ENABLE_KASAN 1

#ifdef __hmbuild_target_is_shared__
#define KASAN_SHARED 1

#ifdef CONFIG_KASAN_RUNTIME
#define ADAPT_FOR_LIBC_KASAN 1
#endif

#endif

#endif
