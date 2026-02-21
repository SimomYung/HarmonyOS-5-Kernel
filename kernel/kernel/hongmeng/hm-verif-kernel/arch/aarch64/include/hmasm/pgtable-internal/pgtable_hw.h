/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 29 21:34:30 2018
 */
#ifndef ASM_PGTABLE_HW_H
#define ASM_PGTABLE_HW_H

#include <uapi/hmasm/mm/pgtable.h>

/* A64 page table mair types */
#define A64_PAGE_DEVICE_nGnRnE	0
#define A64_PAGE_DEVICE_nGnRE	1
#define A64_PAGE_DEVICE_GRE	2
#define A64_PAGE_NORMAL_NC	3
#define A64_PAGE_NORMAL		4

#endif
