/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: add macro for static and shared libs' build
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 30 11:18:29 2022
 */

#ifndef MACRO_H
#define MACRO_H

#define ENABLE_UCFI 1

#ifdef __hmbuild_target_is_shared__
#define UCFI_SHARED 1
#endif

#endif
