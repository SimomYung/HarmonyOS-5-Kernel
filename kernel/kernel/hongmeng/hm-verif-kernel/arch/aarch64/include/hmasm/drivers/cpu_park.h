/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header file of cpu_park
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 20 10:29:16 2023
 */
#ifndef AARCH64_ASM_DRIVERS_CPU_PARK_H
#define AARCH64_ASM_DRIVERS_CPU_PARK_H

#define CPU_PARK_MAGIC 0x7061726b
#define OFFSET_OF_CPU_PARK_SECTION 8

#ifndef __ASSEMBLY__
extern void __cpu_park_loop(unsigned long text, unsigned long info);
extern unsigned long __cpu_parking_code_start[];
extern unsigned long __cpu_parking_code_end[];
#endif

#endif
