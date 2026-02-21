/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Cpuid for modules
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 1 17:58:31 2021
 */
#ifndef MAPI_HMKERNEL_CPUID_H
#define MAPI_HMKERNEL_CPUID_H

#include <hmkernel/types.h>

extern unsigned int
cpuid_register_num(void);
extern bool cpuid_is_registered(unsigned int logic_cpuid);

#endif
