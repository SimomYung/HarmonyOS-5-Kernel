/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Asm code of crt in hmld on arm
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 9:24:58 2019
 */

#ifndef HMLDSO_ARCH_ARM_CRT_START_H
#define HMLDSO_ARCH_ARM_CRT_START_H

__asm__(
".text \n"
".global " LDSO_START " \n"
".type " LDSO_START ",%function \n"
LDSO_START ": \n"
"	mov lr, #0 \n"
"	mov fp, #0 \n"
"	ldr a2, 1f \n"
"	add a2, pc, a2 \n"
"	mov a1, sp \n"
"2:	and ip, sp, #-16 \n"
"	mov sp, ip \n"
"	bl " LDSO_START "_c \n"
".hidden _DYNAMIC \n"
".weak _DYNAMIC \n"
".align 2 \n"
"1:	.word _DYNAMIC-2b \n"
);

#endif
