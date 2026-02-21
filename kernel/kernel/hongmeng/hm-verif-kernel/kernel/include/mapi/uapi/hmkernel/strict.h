/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Uapi strict.h
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 01 12:06:46 2020
 */
#ifndef UAPI_HMKERNEL_STRICT_H
#define UAPI_HMKERNEL_STRICT_H

#include <hmkernel/types.h>

#define ____IS(x) (((x) != ((typeof(x))0)) ? __true : __false)
#define ____NOT(x) (!____IS(x))

#endif
