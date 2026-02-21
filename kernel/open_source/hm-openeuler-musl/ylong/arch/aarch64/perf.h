/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: perf header, supply functions to get cnt
 */

#ifndef PERF_H
#define PERF_H

#include <stdint.h>

/**
 * This call is portable to any ARMv8 architecture, however, typically
 * cntvct_el0 runs at <= 100MHz and it may be imprecise for some tasks.
 */
static inline uint64_t read_cnt(void)
{
    uint64_t tsc;
    asm volatile("mrs %0, cntvct_el0" : "=r"(tsc));
    return tsc;
}

/**
 * This is an alternative method to enable rte_rdtsc() with high resolution PMU cycles counter.
 * The cycle counter runs at cpu frequency and this scheme uses ARMv8 PMU subsystem to get the cycle counter
 * at userspace. It is possible to enable cycle counter at user space access by configuring
 * the PMU from the privileged mode (kernel space).
 *
 * asm volatile("msr pmintenset_el1, %0" : : "r" ((u64)(0 << 31)));
 * asm volatile("msr pmcntenset_el0, %0" :: "r" BIT(31));
 * asm volatile("msr pmuserenr_el0, %0" : : "r"(BIT(0) | BIT(2)));
 * asm volatile("mrs %0, pmcr_el0" : "=r" (val));
 * val |= (BIT(0) | BIT(2));
 * isb();
 * asm volatile("msr pmcr_el0, %0" : : "r" (val));
 *
 */
static inline uint64_t read_tsc(void)
{
    uint64_t tsc;
    asm volatile("mrs %0, pmccntr_el0" : "=r"(tsc));
    return tsc;
}

static inline uint64_t read_tsc_precise(void)
{
    asm volatile("isb" : : : "memory");
    return read_tsc();
}

#endif
