/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Definition of registers for modules
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 23 10:56:26 2021
 */
#ifndef MAPI_AARCH64_ASM_REGISTERS_H
#define MAPI_AARCH64_ASM_REGISTERS_H

#include <uapi/hmasm/registers.h>

#define REG_LR		__REG_LR
#define REG_SPSR	__REG_SPSR
#define REG_PC		__REG_PC
#define REG_SP		__REG_SP
#define REG_TPID	__REG_TPID
#define REG_TLS		__REG_TLS
#define REG_TLS_K	__REG_TLS_K

#define REG_NORMAL	__REG_NORMAL
#define REG_FP		__REG_FP
#define REG_X0		__REG_X0
#define REG_ARG0	__REG_ARG0
#define REG_ARG1	__REG_ARG1
#define REG_ARG2	__REG_ARG2
#define REG_ARG3	__REG_ARG3
#define REG_ARG4	__REG_ARG4
#define REG_ARG5	__REG_ARG5
#define REG_ARG6	__REG_ARG6
#define REG_ARG7	__REG_ARG7

#endif
