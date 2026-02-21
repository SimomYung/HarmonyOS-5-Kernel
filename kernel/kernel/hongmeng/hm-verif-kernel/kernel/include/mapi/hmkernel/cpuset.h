/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of cpuset functions for modules
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 28 18:07:14 CST 2019
 */
#ifndef MAPI_HMKERNEL_CPUSET_H
#define MAPI_HMKERNEL_CPUSET_H

#include <hmkernel/types.h>
#include <uapi/hmkernel/cpuset.h>
#include <hmkernel/bitops/bitops.h>

typedef __cpuset_t cpuset_t;
#define cpuset_one(n)	__cpuset_one(n)
#define cpuset_empty	__cpuset_empty
#define cpuset_full	__cpuset_full
#define cpuset_all	(__cpuset_t){ .__bits = (__u32)((1UL << CONFIG_NR_CPUS) - 1UL) }

static inline cpuset_t
cpuset_add(const cpuset_t set, unsigned int cpu)
{
	return __cpuset_add(set, cpu);
}

static inline cpuset_t
cpuset_or(const cpuset_t a, const cpuset_t b)
{
	return __cpuset_or(a, b);
}

static inline cpuset_t
cpuset_del(const cpuset_t set, unsigned int cpu)
{
	return __cpuset_del(set, cpu);
}

static inline bool
cpuset_has(cpuset_t set, unsigned int cpu)
{
	return (__cpuset_has(set, cpu) != (int)0) ? true : false;
}

static inline cpuset_t
cpuset_and(const cpuset_t a, const cpuset_t b)
{
	return __cpuset_and(a, b);
}

static inline bool
cpuset_equal(const cpuset_t a, const cpuset_t b)
{
	return (__cpuset_equal(a, b) != (int)0) ? true : false;
}

static inline bool
cpuset_contain(const cpuset_t a, const cpuset_t b)
{
	cpuset_t c = cpuset_or(a, b);
	return cpuset_equal(a, c);
}

static inline bool
cpuset_is_empty(cpuset_t set)
{
	return set.__bits == 0u ? true : false;
}

static inline unsigned int
cpuset_first_cpu(cpuset_t set)
{
	return __cpuset_first_cpu(set);
}

static inline int
cpuset_last_cpu(cpuset_t set)
{
	return __cpuset_last_cpu(set);
}

static inline cpuset_t
cpuset_cast(cpuset_t a)
{
	return __cpuset_and(a, cpuset_all);
}

/* performance is good when a has fewer 1s. */
static inline unsigned int
cpuset_weight_light(cpuset_t cpus)
{
	unsigned int c = 0u;
	cpuset_t a = cpus;

	a = cpuset_cast(a);
	while (a.__bits != 0u) {
		a.__bits &= a.__bits - 1u;
		c++;
	}
	return c;
}

static inline cpuset_t
cpuset_clr(const cpuset_t a, const cpuset_t b)
{
	return __cpuset_clr(a, b);
}

#define cpuset_for_each_cpu(cpu, cpuset)               \
	for (u32 cpubits = (cpuset).__bits;                     \
		(cpu) = ((u32)__builtin_ffs((int)cpubits) - 1),   \
		(u32)(cpu) < U(CONFIG_NR_CPUS);                 \
		cpubits &= ~(1U << (cpu)))

#endif
