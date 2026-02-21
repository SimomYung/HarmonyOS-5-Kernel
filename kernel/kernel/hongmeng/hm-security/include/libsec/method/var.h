/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Variable macro define head file
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 08 11:20:33 2019
 */
#ifndef ULIBS_LIBSEC_METHOD_VAR_H
#define ULIBS_LIBSEC_METHOD_VAR_H

#undef ULIBS_LIBSEC_METHOD_VAR_UNDEF_H

#include "map.h"

#define __VAR_PREFIX_N ,
#define __VAR_PREFIX_Y
#define __VAR_SUFFIX_N
#define __VAR_SUFFIX_Y
#define __VAR(T, V) (V),
#define __VAR0()
#define __VAR1(T, V) V
#define VAR(_ARG) __MAP(__VAR, _ARG)

#define __VAR_VOID_PREFIX_N
#define __VAR_VOID_PREFIX_Y
#define __VAR_VOID_SUFFIX_N
#define __VAR_VOID_SUFFIX_Y
#define __VAR_VOID(T, V) (V),
#define __VAR_VOID0()
#define __VAR_VOID1(T, V) V
#define VAR_VOID(_ARG) __MAP(__VAR_VOID, _ARG)

#endif
