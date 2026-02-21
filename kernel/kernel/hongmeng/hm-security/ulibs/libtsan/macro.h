/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: add macro for static and shared tsanlibs' build
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 29 13:49:29 2022
 */

#ifndef TSAN_MACRO_H
#define TSAN_MACRO_H

#ifdef __hmbuild_target_is_shared__
#define TSAN_SHARED 1
#endif

#endif
