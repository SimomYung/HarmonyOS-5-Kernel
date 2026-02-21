/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2025. All rights reserved.
 * Description: dmd_kernel_map.h
 * Create: 2019-10-29
 */

#ifndef DMD_KERNEL_MAP_H
#define DMD_KERNEL_MAP_H

#define RUNTIME_VAR_BASE                     0x20140
#define RUNTIME_TMP_CUR_ADDR(n)              (RUNTIME_VAR_BASE + 0x68 + n) /* 6*1Byte */

#endif /* end of dmd_kernel_map.h */
