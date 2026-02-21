/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/byteorder/big_endian.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 18 10:44:56 2021
 */

#ifndef LINUX_BYTEORDER_BIG_ENDIAN_H
#define LINUX_BYTEORDER_BIG_ENDIAN_H

#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN 4321
#endif

#include <linux/types.h>
#include <linux/compiler_types.h>

#define __cpu_to_le32(x) ((__force __le32)__swab32((x)))
#define __le32_to_cpu(x) __swab32((__force __u32)(__le32)(x))
#define __cpu_to_le16(x) ((__force __le16)__swab16((x)))
#define __le16_to_cpu(x) __swab16((__force __u16)(__le16)(x))

#endif
