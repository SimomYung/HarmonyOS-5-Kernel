/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 7 17:22:27 2018
 */

#ifndef AARCH64_ASM_REGISTERS_H
#define AARCH64_ASM_REGISTERS_H

#include <hmkernel/kernel.h>
#include <mapi/hmasm/registers.h>

#define A64_NORMAL_REG_NUM_64	30
#define A64_NORMAL_REG_NUM_32	13

/* sysproc boot args */
#define REG_SYSPROC_ARG0	__REG_SYSPROC_ARG0
#define REG_SYSPROC_ARG1	__REG_SYSPROC_ARG1
#define REG_SYSPROC_ARG2	__REG_SYSPROC_ARG2
#define REG_SYSPROC_ARG3	__REG_SYSPROC_ARG3
#define REG_SYSPROC_ARG4	__REG_SYSPROC_ARG4
#define REG_SYSPROC_ARG5	__REG_SYSPROC_ARG5
#define REG_SYSPROC_ARG6	__REG_SYSPROC_ARG6
#define REG_SYSPROC_ARG7	__REG_SYSPROC_ARG7
#define REG_SYSPROC_ARG8	__REG_SYSPROC_ARG8
#define REG_SYSPROC_ARG9	__REG_SYSPROC_ARG9
#define REG_SYSPROC_ARG10	__REG_SYSPROC_ARG10
#define REG_SYSPROC_ARG11	__REG_SYSPROC_ARG11

#define REG_RPCINFO		__REG_RPCINFO
#define REG_ACTV_INITATTR	__REG_ACTV_INITATTR
#define REG_ACTV_USERADDR	__REG_ACTV_USERADDR

#define __REGOFF(R)	((unsigned long)(&(R((struct arch_regs *)(0)))))

#define REGOFF_LR	__REGOFF(REG_LR)
#define REGOFF_SP	__REGOFF(REG_SP)
#define REGOFF_PC	__REGOFF(REG_PC)

#endif
