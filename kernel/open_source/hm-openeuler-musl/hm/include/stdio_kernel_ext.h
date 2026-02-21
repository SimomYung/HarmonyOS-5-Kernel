/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Kernel stdio ext header
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 26 15:13:25 2022
 */

#ifndef _STDIO_KERNEL_EXT_H
#define _STDIO_KERNEL_EXT_H

#define __NEED_uintmax_t
#include <bits/alltypes.h>

void printf_set_addr_check(int (*fn)(uintmax_t));

#endif
