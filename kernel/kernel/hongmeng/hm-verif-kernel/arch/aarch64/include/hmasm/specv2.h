/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Interfaces of arm specv2
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 9 17:10:48 2020
 */
#ifndef AARCH64_ASM_SPECV2_H
#define AARCH64_ASM_SPECV2_H

#include <hmkernel/types.h>

bool arch_specv2_check_feature(unsigned long func_id);

void arch_specv2_set_enable(void);

void arch_specv2_set_smccc_call(bool is_smc_flag);

#endif
