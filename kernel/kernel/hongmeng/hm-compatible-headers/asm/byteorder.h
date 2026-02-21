/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add asm/byteorder.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 16 11:00:28 2021
 */

#ifndef LINUX_ASM_BYTEORDER_H
#define LINUX_ASM_BYTEORDER_H

#ifdef __ARMEB__
#include <linux/byteorder/big_endian.h>
#else
#include <linux/byteorder/little_endian.h>
#endif

#endif
