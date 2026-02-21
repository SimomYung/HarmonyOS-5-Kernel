/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: arch independent cpufeature implementation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 20 16:56:26 2024
 */

#ifndef H_SYSMGR_INCLUDE_CPUFEATURE_H
#define H_SYSMGR_INCLUDE_CPUFEATURE_H

void cpufeature_init(void);
__u64 cpufeature_elf_hwcap(void);

#endif
