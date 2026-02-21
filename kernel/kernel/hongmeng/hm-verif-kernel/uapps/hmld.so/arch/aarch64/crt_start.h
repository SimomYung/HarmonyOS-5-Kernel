/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 9:24:58 2019
 */
#ifndef HMLDSO_ARCH_AARCH64_CRT_START_H
#define HMLDSO_ARCH_AARCH64_CRT_START_H

__asm__(
".text \n"
".global " LDSO_START "\n"
".type " LDSO_START ",%function\n"
LDSO_START ":\n"
"	mov lr, #0\n"
"	mov x0, sp\n"
"	mov fp, #0\n"
".hidden _DYNAMIC\n"
".weak _DYNAMIC\n"
"	adrp x1, _DYNAMIC\n"
"	add x1, x1, #:lo12:_DYNAMIC\n"
"	mov x3, sp\n"
"	and sp, x3, #-16\n"
"	b " LDSO_START "_c\n"
);

#endif
