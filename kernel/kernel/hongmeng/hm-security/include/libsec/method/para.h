/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Define parameter
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:20:17 2019
 */
#ifndef ULIBS_LIBSEC_METHOD_PARA_H
#define ULIBS_LIBSEC_METHOD_PARA_H

#undef ULIBS_LIBSEC_METHOD_PARA_UNDEF_H

#include "map.h"

#define __PARA_PREFIX_N ,
#define __PARA_PREFIX_Y
#define __PARA_SUFFIX_N
#define __PARA_SUFFIX_Y
#define __PARA(T, V) T V,
#define __PARA0()
#define __PARA1(T, V) T V
#define PARA(_ARG) __MAP(__PARA, _ARG)

#define __PARA_VOID_PREFIX_N
#define __PARA_VOID_PREFIX_Y
#define __PARA_VOID_SUFFIX_N
#define __PARA_VOID_SUFFIX_Y
#define __PARA_VOID(T, V) T V,
#define __PARA_VOID0() void
#define __PARA_VOID1(T, V) T V
#define PARA_VOID(_ARG) __MAP(__PARA_VOID, _ARG)

#endif
