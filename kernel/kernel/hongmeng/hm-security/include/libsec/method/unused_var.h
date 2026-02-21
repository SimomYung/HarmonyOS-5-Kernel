/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Unused variable macro
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 03 19:42:23 2019
 */
#ifndef ULIBS_LIBSEC_METHOD_UNUSED_VAR_H
#define ULIBS_LIBSEC_METHOD_UNUSED_VAR_H

#undef ULIBS_LIBSEC_METHOD_UNUSED_VAR_UNDEF_H

#include "map.h"

#define __UNUSED_VAR_PREFIX_N
#define __UNUSED_VAR_PREFIX_Y
#define __UNUSED_VAR_SUFFIX_N
#define __UNUSED_VAR_SUFFIX_Y
#define __UNUSED_VAR(T, V) ((void)(V));
#define __UNUSED_VAR0()
#define __UNUSED_VAR1(T, V) ((void)(V))
#define UNUSED_VAR(_ARG) __MAP(__UNUSED_VAR, _ARG)

#endif
