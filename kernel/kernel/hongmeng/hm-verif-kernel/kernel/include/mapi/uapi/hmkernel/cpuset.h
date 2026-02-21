/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Cpuset
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 14 13:58:42 2018
 */
#ifndef UAPI_HMKERNEL_CPUSET_H
#define UAPI_HMKERNEL_CPUSET_H

#include <hmasm/types.h>
#include <hmkernel/compiler.h>

#define __BITS_PER_BYTE		8U
/* support CPUS less than 32 */
typedef struct { __u32 __bits; } __cpuset_t;

#define __cpuset_empty	(__cpuset_t){ .__bits = (__u32)(0) }
#define __cpuset_full	(__cpuset_t){ .__bits = (__u32)(-1) }

static inline __cpuset_t
__cpuset_add(const __cpuset_t set, unsigned int cpu)
{
	__cpuset_t res = set;

	if (cpu < 32u) {
		res.__bits |= ((__u32)1u << cpu);
	}

	return res;
}

static inline __cpuset_t
__cpuset_del(const __cpuset_t set, unsigned int cpu)
{
	__cpuset_t res = set;

	if (cpu < 32u) {
		res.__bits &= ~((__u32)1u << cpu);
	}

	return res;
}

static inline int
__cpuset_has(const __cpuset_t set, unsigned int cpu)
{
	__u32 __cpu = (__u32)1u << cpu;
	return ((set.__bits & __cpu) == __cpu) ? 1 : 0;
}

static inline __cpuset_t
__cpuset_and(const __cpuset_t a, const __cpuset_t b)
{
	__cpuset_t res;

	res.__bits = a.__bits & b.__bits;
	return res;
}

static inline __cpuset_t
__cpuset_neg(const __cpuset_t a)
{
	__cpuset_t res;
	res.__bits = ~a.__bits;
	return res;
}

static inline __cpuset_t
__cpuset_or(const __cpuset_t a, const __cpuset_t b)
{
	__cpuset_t res;

	res.__bits = a.__bits | b.__bits;
	return res;
}

static inline int
__cpuset_equal(const __cpuset_t a, const __cpuset_t b)
{
	return a.__bits == b.__bits ? 1 : 0;
}

static inline __cpuset_t
__cpuset_clr(const __cpuset_t a, const __cpuset_t b)
{
	__cpuset_t res;

	res.__bits = a.__bits & ~b.__bits;
	return res;
}

static inline int
__cpuset_is_empty(__cpuset_t set)
{
	return set.__bits == 0u ? 1 : 0;
}

static inline int
__cpuset_contain(const __cpuset_t a, const __cpuset_t b)
{
	__cpuset_t c = __cpuset_or(a, b);
	return __cpuset_equal(a, c);
}

static inline int
__cpuset_first_cpu(__cpuset_t set)
{
	int err = 0;

	if (__cpuset_is_empty(set) == 1) {
		err = -1;
	}

	if (err == 0) {
		err = (int)(__builtin_ffs((int)set.__bits) - 1);
	}

	return err;
}

static inline int
__cpuset_last_cpu(__cpuset_t set)
{
	int err = 0;

	if (__cpuset_is_empty(set)) {
		err = -1;
	}

	if (err == 0) {
		err = (int)(__BITS_PER_BYTE * sizeof(set.__bits)) - __builtin_clz(set.__bits) - 1;
	}

	return err;
}

#define __cpuset_one(n)	__cpuset_add(__cpuset_empty, n)

static inline unsigned int
__cpuset_weight_local(const __cpuset_t a)
{
	unsigned int cnt = 0;
	unsigned long _bit = (unsigned long)(unsigned int)a.__bits;
	do {
		cnt += _bit & 1;
	} while (_bit >>= 1);

	return cnt;
}

static inline unsigned int
__cpuset_weight(const __cpuset_t a)
{
	return __cpuset_weight_local(a);
}

#ifndef __KERNEL__
/*
 * return the lowest CPU number of a CPU set.
 * Should not used in kernel code because we can't control the
 * instructions of __builtin functions.
 */
static inline __pure unsigned int
__cpuset_lowest(const __cpuset_t a)
{
	/* Currently __bits is __u32 so it is okay now */
	unsigned long _bit = (unsigned long)(unsigned int)a.__bits;

	if (_bit == 0) {
		return 0;
	}
	return (unsigned int)__builtin_ctzl(_bit);
}
#endif

#endif
