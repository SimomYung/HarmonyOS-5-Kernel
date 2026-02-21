/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Author: Huawei Dresden Research Center
 * Create: 2022-11-29
 */
#ifndef VATOMIC_X86_H
#define VATOMIC_X86_H

#if !defined(VATOMIC_DISABLE_POLITE_AWAIT)
#define vatomic_cpu_pause() __asm__ volatile("pause\n" ::: "memory")
#endif

#endif /* VATOMIC_X86_H */
