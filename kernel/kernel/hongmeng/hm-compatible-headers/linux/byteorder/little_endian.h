/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/byteorder/little_endian.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 18 10:42:45 2021
 */

#ifndef LINUX_BYTEORDER_LITTLE_ENDIAN_H
#define LINUX_BYTEORDER_LITTLE_ENDIAN_H

#include <linux/types.h>
#include <linux/compiler_types.h>

#define __cpu_to_le32(x) ((__force __le32)(__u32)(x))
#define __le32_to_cpu(x) ((__force __u32)(__le32)(x))
#define __cpu_to_le16(x) ((__force __le16)(__u16)(x))
#define __le16_to_cpu(x) ((__force __u16)(__le16)(x))

#endif
