/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: header about kdp trampolines
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 25 06:35:02 2024
 */

#ifndef A64_ASM_KDP_H
#define A64_ASM_KDP_H

extern void kdp_init_wp(void);
extern void kdp_setwvr0(void);
extern void kdp_setwvr1(void);
extern void kdp_checkfail(void);

#endif
