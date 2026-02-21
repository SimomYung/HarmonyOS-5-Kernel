/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 7 10:56:55 2018
 */
#ifndef AARCH64_UAPI_ASM_BARRIER_H
#define AARCH64_UAPI_ASM_BARRIER_H

#define isb()           asm volatile("isb" : : : "memory")
#define dmb(opt)        asm volatile("dmb " #opt : : : "memory")
#define dsb(opt)        asm volatile("dsb " #opt : : : "memory")

#define mb()            dsb(sy)
#define rmb()           dsb(ld)
#define wmb()           dsb(st)

#define __smp_mb()      dmb(ish)
#define __smp_rmb()     dmb(ishld)
#define __smp_wmb()     dmb(ishst)

#define smp_write_rel(p, v)		\
do {					\
	asm volatile ("stlr %1, %0"	\
			: "=Q" (p)	\
			: "r" (v)	\
			: "memory");	\
} while (0)

#endif
