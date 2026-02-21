/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/compiler_types.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 18 10:14:42 2021
 */

#ifndef LINUX_COMPILER_TYPES_H
#define LINUX_COMPILER_TYPES_H

#ifdef __CHECKER__
#define __user		__attribute__((noderef, address_space(__user)))
#define __force		__attribute__((force))
#else

#ifdef STRUCTLEAK_PLUGIN
#define __user		__attribute__((user))
#else
#define __user
#endif

#define __force
#endif

#endif
